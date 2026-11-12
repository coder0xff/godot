#[compute]

#version 450

#VERSION_DEFINES

// The capture pass's cull (RenderForwardClustered::_capture_cull): each element of the opaque list against each layer's
// frustum, the pairs that pass written compactly by draw, then each draw's indirect instance count.

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct Element {
	vec4 center; // world bounds' centre
	vec4 half_extent; // world bounds' half size
	uvec4 info; // x: the draw, y: the element's index in the draw's run
};

struct Draw {
	uint item_offset;
	uint pair_count;
	uint instances;
	// Bit 0: point sizes emulated (six procedural vertices an instance). Bit 1: ordered (a transparent list): every
	// pair keeps its slot, element by element, a culled one marked by its layer CULLED, so a run of equal elements
	// still blends in its sorted order; pair_count is then every pair's.
	uint flags;
};

#define CULLED 0xFFFFFFFFu

layout(set = 0, binding = 0, std430) restrict readonly buffer Elements {
	Element data[];
}
elements;

// Six planes a layer, world space: a point with dot(normal, p) - d > 0 is outside.
layout(set = 0, binding = 1, std430) restrict readonly buffer Planes {
	vec4 data[];
}
planes;

layout(set = 0, binding = 2, std430) restrict buffer Draws {
	Draw data[];
}
draws;

layout(set = 0, binding = 3, std430) restrict writeonly buffer Items {
	uvec2 data[];
}
items;

// The indirect commands, five uints a draw; the second is the instance count (with emulated points, the first is).
layout(set = 0, binding = 4, std430) restrict buffer Commands {
	uint data[];
}
commands;

layout(push_constant, std430) uniform Params {
	uint element_count;
	uint layer_count;
	uint draw_count;
	uint pad;
}
params;

void main() {
	uint id = gl_GlobalInvocationID.x;
#ifdef MODE_CULL
	if (id >= params.element_count * params.layer_count) {
		return;
	}
	uint e = id / params.layer_count;
	uint l = id % params.layer_count;
	Element element = elements.data[e];
	bool inside = true;
	for (uint p = 0; p < 6; p++) {
		vec4 plane = planes.data[l * 6 + p];
		// The box's nearest point along the plane's outward normal: outside if even that is.
		if (dot(plane.xyz, element.center.xyz) - plane.w - dot(abs(plane.xyz), element.half_extent.xyz) > 0.0) {
			inside = false;
			break;
		}
	}
	uint d = element.info.x;
	if ((draws.data[d].flags & 2u) != 0u) {
		items.data[draws.data[d].item_offset + element.info.y * params.layer_count + l] = uvec2(element.info.y, inside ? l : CULLED);
	} else if (inside) {
		uint slot = atomicAdd(draws.data[d].pair_count, 1u);
		items.data[draws.data[d].item_offset + slot] = uvec2(element.info.y, l);
	}
#endif
#ifdef MODE_COUNT
	if (id >= params.draw_count) {
		return;
	}
	if ((draws.data[id].flags & 1u) != 0u) {
		// A quad (six vertices) a drawn instance; the mesh's points are the instances (filled on the CPU).
		commands.data[id * 5] = draws.data[id].pair_count * draws.data[id].instances * 6;
	} else {
		commands.data[id * 5 + 1] = draws.data[id].pair_count * draws.data[id].instances;
	}
#endif
}
