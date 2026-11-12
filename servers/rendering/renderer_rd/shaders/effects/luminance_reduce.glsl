#[compute]

#version 450

#VERSION_DEFINES

#define BLOCK_SIZE 8

layout(local_size_x = BLOCK_SIZE, local_size_y = BLOCK_SIZE, local_size_z = 1) in;

// .x the metered luminance, .y the plain mean's (for the clipped mean's anchor).
shared vec2 tmp_data[BLOCK_SIZE * BLOCK_SIZE];

#ifdef READ_TEXTURE

//use for main texture
layout(set = 0, binding = 0) uniform sampler2D source_texture;

#else

//use for intermediate textures
layout(rg32f, set = 0, binding = 0) uniform restrict readonly image2D source_luminance;

#endif

layout(rg32f, set = 1, binding = 0) uniform restrict writeonly image2D dest_luminance;

#if defined(WRITE_LUMINANCE) || defined(READ_TEXTURE)
// The eye's current adaptation: the last frame's luminance (the clipped mean's first pass reads it too).
layout(set = 2, binding = 0) uniform sampler2D prev_luminance;
#endif

layout(push_constant, std430) uniform Params {
	ivec2 source_size;
	float max_luminance;
	float min_luminance;
	float exposure_adjust;
	// Bit 0: meter the log-average (geometric mean) luminance instead of the mean, and adapt in log space.
	// Bit 1: this is the last reduction, which writes the frame's luminance (exp of the log-average).
	// Bit 2: clip each pixel's luminance at clip_factor times the eye's current adaptation (the clipped mean).
	// Bit 3: the last reduction keeps the clipped mean at least anchor_factor times the plain mean (its anchor).
	uint flags;
	float clip_factor;
	float anchor_factor;
}
params;

#define FLAG_LOG_AVERAGE 1u
#define FLAG_LAST 2u
#define FLAG_CLIP 4u
#define FLAG_ANCHOR 8u
// The darkest luminance the log-average counts, so that a black pixel has a finite log.
#define LOG_FLOOR 1e-6

void main() {
	uint t = gl_LocalInvocationID.y * BLOCK_SIZE + gl_LocalInvocationID.x;
	ivec2 pos = ivec2(gl_GlobalInvocationID.xy);

	if (any(lessThan(pos, params.source_size))) {
#ifdef READ_TEXTURE
		vec3 v = texelFetch(source_texture, pos, 0).rgb;
		float l = max(v.r, max(v.g, v.b));
		float plain = l;
		if (bool(params.flags & FLAG_CLIP)) {
			// A meter reading its own exposed image saturates: a pixel counts at most clip_factor times the
			// luminance the eye is adapted to.
			l = min(l, texelFetch(prev_luminance, ivec2(0, 0), 0).r * params.clip_factor);
		}
		tmp_data[t] = vec2(bool(params.flags & FLAG_LOG_AVERAGE) ? log(max(l, LOG_FLOOR)) : l, plain);
#else
		tmp_data[t] = imageLoad(source_luminance, pos).rg;
#endif
	} else {
		tmp_data[t] = vec2(0.0);
	}

	groupMemoryBarrier();
	barrier();

	uint size = (BLOCK_SIZE * BLOCK_SIZE) >> 1;

	do {
		if (t < size) {
			tmp_data[t] += tmp_data[t + size];
		}
		groupMemoryBarrier();
		barrier();

		size >>= 1;
	} while (size >= 1);

	if (t == 0) {
		//compute rect size
		ivec2 rect_size = min(params.source_size - pos, ivec2(BLOCK_SIZE));
		vec2 avg2 = tmp_data[0] / float(rect_size.x * rect_size.y);
		float avg = avg2.x;
		pos /= ivec2(BLOCK_SIZE);
		bool log_average = bool(params.flags & FLAG_LOG_AVERAGE);
		if (log_average && bool(params.flags & FLAG_LAST)) {
			avg = exp(avg);
		}
		if (bool(params.flags & FLAG_ANCHOR)) {
			// The clipped mean never reads more than the anchor below the plain mean, so a frame that is nearly all
			// black does not run the eye down to its floor.
			avg = max(avg, avg2.y * params.anchor_factor);
		}
#ifdef WRITE_LUMINANCE
		float prev_lum = texelFetch(prev_luminance, ivec2(0, 0), 0).r; //1 pixel previous exposure
		if (log_average) {
			// Adapt by the same fraction of the step in stops, up or down.
			float prev_log = log(max(prev_lum, LOG_FLOOR));
			avg = exp(prev_log + (log(avg) - prev_log) * params.exposure_adjust);
		} else {
			avg = prev_lum + (avg - prev_lum) * params.exposure_adjust;
		}
		avg = clamp(avg, params.min_luminance, params.max_luminance);
#endif
		imageStore(dest_luminance, pos, vec4(avg, avg2.y, 0.0, 0.0));
	}
}
