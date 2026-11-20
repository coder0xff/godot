/**************************************************************************/
/*  physics_material.h                                                    */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#if !defined(PHYSICS_2D_DISABLED) || !defined(PHYSICS_3D_DISABLED)
#include "core/io/resource.h"
#include "scene/resources/curve.h"

class PhysicsMaterial : public Resource {
	GDCLASS(PhysicsMaterial, Resource);
	OBJ_SAVE_TYPE(PhysicsMaterial);
	RES_BASE_EXTENSION("phymat");

	real_t friction = 1.0;
	bool rough = false;
	real_t bounce = 0.0;
	bool absorbent = false;

	uint64_t fields_set_flags = 0;
	int64_t material_revision = 1;
	double density = 0.0;
	double specific_heat = 0.0;
	double thermal_conductivity = 0.0;
	double melting_temperature = 0.0;
	double latent_heat = 0.0;
	double liquid_specific_heat = 0.0;
	double resistivity = 0.0;
	double resistivity_temperature_coefficient = 0.0;
	double emissivity = 0.0;
	bool yield_enabled = false;
	double yield_stress = 0.0;
	Ref<Curve> yield_curve;

	void _material_changed(bool p_flags_changed = false);
	void _yield_curve_changed();
	void _set_fields_set_flags(int64_t p_flags);
	int64_t _get_fields_set_flags() const { return fields_set_flags; }
	void _set_density(const Variant &p_value);
	void _set_specific_heat(const Variant &p_value);
	void _set_thermal_conductivity(const Variant &p_value);
	void _set_melting_temperature(const Variant &p_value);
	void _set_latent_heat(const Variant &p_value);
	void _set_liquid_specific_heat(const Variant &p_value);
	void _set_resistivity(const Variant &p_value);
	void _set_resistivity_temperature_coefficient(const Variant &p_value);
	void _set_emissivity(const Variant &p_value);
	void _set_yield_enabled(const Variant &p_value);
	void _set_yield_stress(const Variant &p_value);
	void _set_yield_curve(const Variant &p_value);

protected:
	static void _bind_methods();
	void _validate_property(PropertyInfo &p_property) const;

public:
	enum Field {
		FIELD_DENSITY = 1 << 0,
		FIELD_SPECIFIC_HEAT = 1 << 1,
		FIELD_THERMAL_CONDUCTIVITY = 1 << 2,
		FIELD_MELTING_TEMPERATURE = 1 << 3,
		FIELD_LATENT_HEAT = 1 << 4,
		FIELD_LIQUID_SPECIFIC_HEAT = 1 << 5,
		FIELD_RESISTIVITY = 1 << 6,
		FIELD_RESISTIVITY_TEMPERATURE_COEFFICIENT = 1 << 7,
		FIELD_EMISSIVITY = 1 << 8,
		FIELD_YIELD = 1 << 9,
		FIELD_ALL = (1 << 10) - 1,
	};

	int64_t get_material_revision() const { return material_revision; }

	void set_density(double p_value);
	double get_density() const { return density; }
	bool has_density() const { return fields_set_flags & FIELD_DENSITY; }
	void clear_density();

	void set_specific_heat(double p_value);
	double get_specific_heat() const { return specific_heat; }
	bool has_specific_heat() const { return fields_set_flags & FIELD_SPECIFIC_HEAT; }
	void clear_specific_heat();

	void set_thermal_conductivity(double p_value);
	double get_thermal_conductivity() const { return thermal_conductivity; }
	bool has_thermal_conductivity() const { return fields_set_flags & FIELD_THERMAL_CONDUCTIVITY; }
	void clear_thermal_conductivity();

	void set_melting_temperature(double p_value);
	double get_melting_temperature() const { return melting_temperature; }
	bool has_melting_temperature() const { return fields_set_flags & FIELD_MELTING_TEMPERATURE; }
	void clear_melting_temperature();

	void set_latent_heat(double p_value);
	double get_latent_heat() const { return latent_heat; }
	bool has_latent_heat() const { return fields_set_flags & FIELD_LATENT_HEAT; }
	void clear_latent_heat();

	void set_liquid_specific_heat(double p_value);
	double get_liquid_specific_heat() const { return liquid_specific_heat; }
	bool has_liquid_specific_heat() const { return fields_set_flags & FIELD_LIQUID_SPECIFIC_HEAT; }
	void clear_liquid_specific_heat();

	void set_resistivity(double p_value);
	double get_resistivity() const { return resistivity; }
	bool has_resistivity() const { return fields_set_flags & FIELD_RESISTIVITY; }
	void clear_resistivity();

	void set_resistivity_temperature_coefficient(double p_value);
	double get_resistivity_temperature_coefficient() const { return resistivity_temperature_coefficient; }
	bool has_resistivity_temperature_coefficient() const { return fields_set_flags & FIELD_RESISTIVITY_TEMPERATURE_COEFFICIENT; }
	void clear_resistivity_temperature_coefficient();

	void set_emissivity(double p_value);
	double get_emissivity() const { return emissivity; }
	bool has_emissivity() const { return fields_set_flags & FIELD_EMISSIVITY; }
	void clear_emissivity();

	void set_yield_enabled(bool p_enabled);
	bool is_yield_enabled() const { return yield_enabled; }
	void set_yield_stress(double p_stress);
	double get_yield_stress() const { return yield_stress; }
	void set_yield_curve(const Ref<Curve> &p_curve);
	Ref<Curve> get_yield_curve() const { return yield_curve; }
	bool has_yield() const { return fields_set_flags & FIELD_YIELD; }
	bool has_yield_enabled() const { return has_yield(); }
	bool has_yield_stress() const { return has_yield(); }
	bool has_yield_curve() const { return has_yield(); }
	void clear_yield();
	void clear_yield_enabled() { clear_yield(); }
	void clear_yield_stress() { clear_yield(); }
	void clear_yield_curve() { clear_yield(); }

	void set_friction(real_t p_val);
	_FORCE_INLINE_ real_t get_friction() const { return friction; }

	void set_rough(bool p_val);
	_FORCE_INLINE_ bool is_rough() const { return rough; }

	_FORCE_INLINE_ real_t computed_friction() const {
		return rough ? -friction : friction;
	}

	void set_bounce(real_t p_val);
	_FORCE_INLINE_ real_t get_bounce() const { return bounce; }

	void set_absorbent(bool p_val);
	_FORCE_INLINE_ bool is_absorbent() const { return absorbent; }

	_FORCE_INLINE_ real_t computed_bounce() const {
		return absorbent ? -bounce : bounce;
	}
};
#endif // !defined(PHYSICS_2D_DISABLED) || !defined(PHYSICS_3D_DISABLED)
