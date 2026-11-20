/**************************************************************************/
/*  physics_material.cpp                                                  */
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

#include "physics_material.h"

#if !defined(PHYSICS_2D_DISABLED) || !defined(PHYSICS_3D_DISABLED)

#include "core/object/callable_mp.h"
#include "core/object/class_db.h"

void PhysicsMaterial::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_friction", "friction"), &PhysicsMaterial::set_friction);
	ClassDB::bind_method(D_METHOD("get_friction"), &PhysicsMaterial::get_friction);

	ClassDB::bind_method(D_METHOD("set_rough", "rough"), &PhysicsMaterial::set_rough);
	ClassDB::bind_method(D_METHOD("is_rough"), &PhysicsMaterial::is_rough);

	ClassDB::bind_method(D_METHOD("set_bounce", "bounce"), &PhysicsMaterial::set_bounce);
	ClassDB::bind_method(D_METHOD("get_bounce"), &PhysicsMaterial::get_bounce);

	ClassDB::bind_method(D_METHOD("set_absorbent", "absorbent"), &PhysicsMaterial::set_absorbent);
	ClassDB::bind_method(D_METHOD("is_absorbent"), &PhysicsMaterial::is_absorbent);

	ClassDB::bind_method(D_METHOD("set_density", "density"), &PhysicsMaterial::set_density);
	ClassDB::bind_method(D_METHOD("get_density"), &PhysicsMaterial::get_density);
	ClassDB::bind_method(D_METHOD("has_density"), &PhysicsMaterial::has_density);
	ClassDB::bind_method(D_METHOD("clear_density"), &PhysicsMaterial::clear_density);
	ClassDB::bind_method(D_METHOD("_set_density", "value"), &PhysicsMaterial::_set_density);
	ClassDB::bind_method(D_METHOD("set_specific_heat", "specific_heat"), &PhysicsMaterial::set_specific_heat);
	ClassDB::bind_method(D_METHOD("get_specific_heat"), &PhysicsMaterial::get_specific_heat);
	ClassDB::bind_method(D_METHOD("has_specific_heat"), &PhysicsMaterial::has_specific_heat);
	ClassDB::bind_method(D_METHOD("clear_specific_heat"), &PhysicsMaterial::clear_specific_heat);
	ClassDB::bind_method(D_METHOD("_set_specific_heat", "value"), &PhysicsMaterial::_set_specific_heat);
	ClassDB::bind_method(D_METHOD("set_thermal_conductivity", "thermal_conductivity"), &PhysicsMaterial::set_thermal_conductivity);
	ClassDB::bind_method(D_METHOD("get_thermal_conductivity"), &PhysicsMaterial::get_thermal_conductivity);
	ClassDB::bind_method(D_METHOD("has_thermal_conductivity"), &PhysicsMaterial::has_thermal_conductivity);
	ClassDB::bind_method(D_METHOD("clear_thermal_conductivity"), &PhysicsMaterial::clear_thermal_conductivity);
	ClassDB::bind_method(D_METHOD("_set_thermal_conductivity", "value"), &PhysicsMaterial::_set_thermal_conductivity);
	ClassDB::bind_method(D_METHOD("set_melting_temperature", "melting_temperature"), &PhysicsMaterial::set_melting_temperature);
	ClassDB::bind_method(D_METHOD("get_melting_temperature"), &PhysicsMaterial::get_melting_temperature);
	ClassDB::bind_method(D_METHOD("has_melting_temperature"), &PhysicsMaterial::has_melting_temperature);
	ClassDB::bind_method(D_METHOD("clear_melting_temperature"), &PhysicsMaterial::clear_melting_temperature);
	ClassDB::bind_method(D_METHOD("_set_melting_temperature", "value"), &PhysicsMaterial::_set_melting_temperature);
	ClassDB::bind_method(D_METHOD("set_latent_heat", "latent_heat"), &PhysicsMaterial::set_latent_heat);
	ClassDB::bind_method(D_METHOD("get_latent_heat"), &PhysicsMaterial::get_latent_heat);
	ClassDB::bind_method(D_METHOD("has_latent_heat"), &PhysicsMaterial::has_latent_heat);
	ClassDB::bind_method(D_METHOD("clear_latent_heat"), &PhysicsMaterial::clear_latent_heat);
	ClassDB::bind_method(D_METHOD("_set_latent_heat", "value"), &PhysicsMaterial::_set_latent_heat);
	ClassDB::bind_method(D_METHOD("set_liquid_specific_heat", "liquid_specific_heat"), &PhysicsMaterial::set_liquid_specific_heat);
	ClassDB::bind_method(D_METHOD("get_liquid_specific_heat"), &PhysicsMaterial::get_liquid_specific_heat);
	ClassDB::bind_method(D_METHOD("has_liquid_specific_heat"), &PhysicsMaterial::has_liquid_specific_heat);
	ClassDB::bind_method(D_METHOD("clear_liquid_specific_heat"), &PhysicsMaterial::clear_liquid_specific_heat);
	ClassDB::bind_method(D_METHOD("_set_liquid_specific_heat", "value"), &PhysicsMaterial::_set_liquid_specific_heat);
	ClassDB::bind_method(D_METHOD("set_resistivity", "resistivity"), &PhysicsMaterial::set_resistivity);
	ClassDB::bind_method(D_METHOD("get_resistivity"), &PhysicsMaterial::get_resistivity);
	ClassDB::bind_method(D_METHOD("has_resistivity"), &PhysicsMaterial::has_resistivity);
	ClassDB::bind_method(D_METHOD("clear_resistivity"), &PhysicsMaterial::clear_resistivity);
	ClassDB::bind_method(D_METHOD("_set_resistivity", "value"), &PhysicsMaterial::_set_resistivity);
	ClassDB::bind_method(D_METHOD("set_resistivity_temperature_coefficient", "resistivity_temperature_coefficient"), &PhysicsMaterial::set_resistivity_temperature_coefficient);
	ClassDB::bind_method(D_METHOD("get_resistivity_temperature_coefficient"), &PhysicsMaterial::get_resistivity_temperature_coefficient);
	ClassDB::bind_method(D_METHOD("has_resistivity_temperature_coefficient"), &PhysicsMaterial::has_resistivity_temperature_coefficient);
	ClassDB::bind_method(D_METHOD("clear_resistivity_temperature_coefficient"), &PhysicsMaterial::clear_resistivity_temperature_coefficient);
	ClassDB::bind_method(D_METHOD("_set_resistivity_temperature_coefficient", "value"), &PhysicsMaterial::_set_resistivity_temperature_coefficient);
	ClassDB::bind_method(D_METHOD("set_emissivity", "emissivity"), &PhysicsMaterial::set_emissivity);
	ClassDB::bind_method(D_METHOD("get_emissivity"), &PhysicsMaterial::get_emissivity);
	ClassDB::bind_method(D_METHOD("has_emissivity"), &PhysicsMaterial::has_emissivity);
	ClassDB::bind_method(D_METHOD("clear_emissivity"), &PhysicsMaterial::clear_emissivity);
	ClassDB::bind_method(D_METHOD("_set_emissivity", "value"), &PhysicsMaterial::_set_emissivity);
	ClassDB::bind_method(D_METHOD("set_yield_enabled", "yield_enabled"), &PhysicsMaterial::set_yield_enabled);
	ClassDB::bind_method(D_METHOD("is_yield_enabled"), &PhysicsMaterial::is_yield_enabled);
	ClassDB::bind_method(D_METHOD("has_yield_enabled"), &PhysicsMaterial::has_yield_enabled);
	ClassDB::bind_method(D_METHOD("clear_yield_enabled"), &PhysicsMaterial::clear_yield_enabled);
	ClassDB::bind_method(D_METHOD("_set_yield_enabled", "value"), &PhysicsMaterial::_set_yield_enabled);
	ClassDB::bind_method(D_METHOD("set_yield_stress", "yield_stress"), &PhysicsMaterial::set_yield_stress);
	ClassDB::bind_method(D_METHOD("get_yield_stress"), &PhysicsMaterial::get_yield_stress);
	ClassDB::bind_method(D_METHOD("has_yield_stress"), &PhysicsMaterial::has_yield_stress);
	ClassDB::bind_method(D_METHOD("clear_yield_stress"), &PhysicsMaterial::clear_yield_stress);
	ClassDB::bind_method(D_METHOD("_set_yield_stress", "value"), &PhysicsMaterial::_set_yield_stress);
	ClassDB::bind_method(D_METHOD("set_yield_curve", "yield_curve"), &PhysicsMaterial::set_yield_curve);
	ClassDB::bind_method(D_METHOD("get_yield_curve"), &PhysicsMaterial::get_yield_curve);
	ClassDB::bind_method(D_METHOD("has_yield_curve"), &PhysicsMaterial::has_yield_curve);
	ClassDB::bind_method(D_METHOD("clear_yield_curve"), &PhysicsMaterial::clear_yield_curve);
	ClassDB::bind_method(D_METHOD("_set_yield_curve", "value"), &PhysicsMaterial::_set_yield_curve);
	ClassDB::bind_method(D_METHOD("has_yield"), &PhysicsMaterial::has_yield);
	ClassDB::bind_method(D_METHOD("clear_yield"), &PhysicsMaterial::clear_yield);
	ClassDB::bind_method(D_METHOD("get_material_revision"), &PhysicsMaterial::get_material_revision);
	ClassDB::bind_method(D_METHOD("_set_fields_set_flags", "flags"), &PhysicsMaterial::_set_fields_set_flags);
	ClassDB::bind_method(D_METHOD("_get_fields_set_flags"), &PhysicsMaterial::_get_fields_set_flags);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "friction", PROPERTY_HINT_RANGE, "0,1,0.01,or_greater"), "set_friction", "get_friction");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "rough"), "set_rough", "is_rough");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "bounce", PROPERTY_HINT_RANGE, "0,1,0.01,or_greater"), "set_bounce", "get_bounce");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "absorbent"), "set_absorbent", "is_absorbent");
	ADD_GROUP("Fields Material", "");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "density", PROPERTY_HINT_RANGE, "0,100000,0.001,or_greater,suffix:kg/m³", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CHECKABLE), "_set_density", "get_density");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "specific_heat", PROPERTY_HINT_RANGE, "0,100000,0.001,or_greater,suffix:J/(kg K)", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CHECKABLE), "_set_specific_heat", "get_specific_heat");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "thermal_conductivity", PROPERTY_HINT_RANGE, "0,10000,0.001,or_greater,suffix:W/(m K)", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CHECKABLE), "_set_thermal_conductivity", "get_thermal_conductivity");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "melting_temperature", PROPERTY_HINT_RANGE, "0,10000,0.01,or_greater,suffix:K", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CHECKABLE), "_set_melting_temperature", "get_melting_temperature");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "latent_heat", PROPERTY_HINT_RANGE, "0,10000000,1,or_greater,suffix:J/kg", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CHECKABLE), "_set_latent_heat", "get_latent_heat");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "liquid_specific_heat", PROPERTY_HINT_RANGE, "0,100000,0.001,or_greater,suffix:J/(kg K)", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CHECKABLE), "_set_liquid_specific_heat", "get_liquid_specific_heat");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "resistivity", PROPERTY_HINT_RANGE, "0,10000000000000,0.000000000001,or_greater,suffix:Ω m", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CHECKABLE), "_set_resistivity", "get_resistivity");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "resistivity_temperature_coefficient", PROPERTY_HINT_RANGE, "-1,1,0.000001,or_less,or_greater,suffix:1/K", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CHECKABLE), "_set_resistivity_temperature_coefficient", "get_resistivity_temperature_coefficient");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "emissivity", PROPERTY_HINT_RANGE, "0,1,0.001", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CHECKABLE), "_set_emissivity", "get_emissivity");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "yield_enabled", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CHECKABLE), "_set_yield_enabled", "is_yield_enabled");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "yield_stress", PROPERTY_HINT_RANGE, "0,1000000000000,1,or_greater,suffix:Pa", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CHECKABLE), "_set_yield_stress", "get_yield_stress");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "yield_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CHECKABLE), "_set_yield_curve", "get_yield_curve");
	// Persist the explicit flags even when a set value is equal to its dormant zero/false/null.
	ADD_PROPERTY(PropertyInfo(Variant::INT, "_fields_set_flags", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_INTERNAL), "_set_fields_set_flags", "_get_fields_set_flags");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "material_revision", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_READ_ONLY), "", "get_material_revision");

}

void PhysicsMaterial::set_friction(real_t p_val) {
	friction = p_val;
	_material_changed();
}

void PhysicsMaterial::set_rough(bool p_val) {
	rough = p_val;
	_material_changed();
}

void PhysicsMaterial::set_bounce(real_t p_val) {
	bounce = p_val;
	_material_changed();
}

void PhysicsMaterial::set_absorbent(bool p_val) {
	absorbent = p_val;
	_material_changed();
}

void PhysicsMaterial::_material_changed(bool p_flags_changed) {
	ERR_FAIL_COND(material_revision == INT64_MAX);
	material_revision++;
	if (p_flags_changed) {
		notify_property_list_changed();
	}
	emit_changed();
}

void PhysicsMaterial::_set_fields_set_flags(int64_t p_flags) {
	ERR_FAIL_COND(p_flags < 0 || (uint64_t(p_flags) & ~uint64_t(FIELD_ALL)) != 0);
	fields_set_flags = p_flags;
	_material_changed(true);
}

void PhysicsMaterial::_validate_property(PropertyInfo &p_property) const {
	uint64_t flag = 0;
	if (p_property.name == "density") {
		flag = FIELD_DENSITY;
	}
	if (p_property.name == "specific_heat") {
		flag = FIELD_SPECIFIC_HEAT;
	}
	if (p_property.name == "thermal_conductivity") {
		flag = FIELD_THERMAL_CONDUCTIVITY;
	}
	if (p_property.name == "melting_temperature") {
		flag = FIELD_MELTING_TEMPERATURE;
	}
	if (p_property.name == "latent_heat") {
		flag = FIELD_LATENT_HEAT;
	}
	if (p_property.name == "liquid_specific_heat") {
		flag = FIELD_LIQUID_SPECIFIC_HEAT;
	}
	if (p_property.name == "resistivity") {
		flag = FIELD_RESISTIVITY;
	}
	if (p_property.name == "resistivity_temperature_coefficient") {
		flag = FIELD_RESISTIVITY_TEMPERATURE_COEFFICIENT;
	}
	if (p_property.name == "emissivity") {
		flag = FIELD_EMISSIVITY;
	}
	if (p_property.name == "yield_enabled" || p_property.name == "yield_stress" || p_property.name == "yield_curve") {
		flag = FIELD_YIELD;
	}
	if (flag) {
		if (fields_set_flags & flag) {
			p_property.usage |= PROPERTY_USAGE_CHECKED;
		} else {
			p_property.usage &= ~(PROPERTY_USAGE_CHECKED | PROPERTY_USAGE_STORAGE);
		}
	}
}

void PhysicsMaterial::set_density(double p_value) {
	bool flags_changed = !has_density();
	density = p_value;
	fields_set_flags |= FIELD_DENSITY;
	_material_changed(flags_changed);
}

void PhysicsMaterial::clear_density() {
	if (!has_density()) {
		return;
	}
	fields_set_flags &= ~uint64_t(FIELD_DENSITY);
	density = 0.0;
	_material_changed(true);
}

void PhysicsMaterial::_set_density(const Variant &p_value) {
	if (p_value.get_type() == Variant::NIL) {
		clear_density();
		return;
	}
	ERR_FAIL_COND(p_value.get_type() != Variant::FLOAT && p_value.get_type() != Variant::INT);
	set_density(p_value);
}

void PhysicsMaterial::set_specific_heat(double p_value) {
	bool flags_changed = !has_specific_heat();
	specific_heat = p_value;
	fields_set_flags |= FIELD_SPECIFIC_HEAT;
	_material_changed(flags_changed);
}

void PhysicsMaterial::clear_specific_heat() {
	if (!has_specific_heat()) {
		return;
	}
	fields_set_flags &= ~uint64_t(FIELD_SPECIFIC_HEAT);
	specific_heat = 0.0;
	_material_changed(true);
}

void PhysicsMaterial::_set_specific_heat(const Variant &p_value) {
	if (p_value.get_type() == Variant::NIL) {
		clear_specific_heat();
		return;
	}
	ERR_FAIL_COND(p_value.get_type() != Variant::FLOAT && p_value.get_type() != Variant::INT);
	set_specific_heat(p_value);
}

void PhysicsMaterial::set_thermal_conductivity(double p_value) {
	bool flags_changed = !has_thermal_conductivity();
	thermal_conductivity = p_value;
	fields_set_flags |= FIELD_THERMAL_CONDUCTIVITY;
	_material_changed(flags_changed);
}

void PhysicsMaterial::clear_thermal_conductivity() {
	if (!has_thermal_conductivity()) {
		return;
	}
	fields_set_flags &= ~uint64_t(FIELD_THERMAL_CONDUCTIVITY);
	thermal_conductivity = 0.0;
	_material_changed(true);
}

void PhysicsMaterial::_set_thermal_conductivity(const Variant &p_value) {
	if (p_value.get_type() == Variant::NIL) {
		clear_thermal_conductivity();
		return;
	}
	ERR_FAIL_COND(p_value.get_type() != Variant::FLOAT && p_value.get_type() != Variant::INT);
	set_thermal_conductivity(p_value);
}

void PhysicsMaterial::set_melting_temperature(double p_value) {
	bool flags_changed = !has_melting_temperature();
	melting_temperature = p_value;
	fields_set_flags |= FIELD_MELTING_TEMPERATURE;
	_material_changed(flags_changed);
}

void PhysicsMaterial::clear_melting_temperature() {
	if (!has_melting_temperature()) {
		return;
	}
	fields_set_flags &= ~uint64_t(FIELD_MELTING_TEMPERATURE);
	melting_temperature = 0.0;
	_material_changed(true);
}

void PhysicsMaterial::_set_melting_temperature(const Variant &p_value) {
	if (p_value.get_type() == Variant::NIL) {
		clear_melting_temperature();
		return;
	}
	ERR_FAIL_COND(p_value.get_type() != Variant::FLOAT && p_value.get_type() != Variant::INT);
	set_melting_temperature(p_value);
}

void PhysicsMaterial::set_latent_heat(double p_value) {
	bool flags_changed = !has_latent_heat();
	latent_heat = p_value;
	fields_set_flags |= FIELD_LATENT_HEAT;
	_material_changed(flags_changed);
}

void PhysicsMaterial::clear_latent_heat() {
	if (!has_latent_heat()) {
		return;
	}
	fields_set_flags &= ~uint64_t(FIELD_LATENT_HEAT);
	latent_heat = 0.0;
	_material_changed(true);
}

void PhysicsMaterial::_set_latent_heat(const Variant &p_value) {
	if (p_value.get_type() == Variant::NIL) {
		clear_latent_heat();
		return;
	}
	ERR_FAIL_COND(p_value.get_type() != Variant::FLOAT && p_value.get_type() != Variant::INT);
	set_latent_heat(p_value);
}

void PhysicsMaterial::set_liquid_specific_heat(double p_value) {
	bool flags_changed = !has_liquid_specific_heat();
	liquid_specific_heat = p_value;
	fields_set_flags |= FIELD_LIQUID_SPECIFIC_HEAT;
	_material_changed(flags_changed);
}

void PhysicsMaterial::clear_liquid_specific_heat() {
	if (!has_liquid_specific_heat()) {
		return;
	}
	fields_set_flags &= ~uint64_t(FIELD_LIQUID_SPECIFIC_HEAT);
	liquid_specific_heat = 0.0;
	_material_changed(true);
}

void PhysicsMaterial::_set_liquid_specific_heat(const Variant &p_value) {
	if (p_value.get_type() == Variant::NIL) {
		clear_liquid_specific_heat();
		return;
	}
	ERR_FAIL_COND(p_value.get_type() != Variant::FLOAT && p_value.get_type() != Variant::INT);
	set_liquid_specific_heat(p_value);
}

void PhysicsMaterial::set_resistivity(double p_value) {
	bool flags_changed = !has_resistivity();
	resistivity = p_value;
	fields_set_flags |= FIELD_RESISTIVITY;
	_material_changed(flags_changed);
}

void PhysicsMaterial::clear_resistivity() {
	if (!has_resistivity()) {
		return;
	}
	fields_set_flags &= ~uint64_t(FIELD_RESISTIVITY);
	resistivity = 0.0;
	_material_changed(true);
}

void PhysicsMaterial::_set_resistivity(const Variant &p_value) {
	if (p_value.get_type() == Variant::NIL) {
		clear_resistivity();
		return;
	}
	ERR_FAIL_COND(p_value.get_type() != Variant::FLOAT && p_value.get_type() != Variant::INT);
	set_resistivity(p_value);
}

void PhysicsMaterial::set_resistivity_temperature_coefficient(double p_value) {
	bool flags_changed = !has_resistivity_temperature_coefficient();
	resistivity_temperature_coefficient = p_value;
	fields_set_flags |= FIELD_RESISTIVITY_TEMPERATURE_COEFFICIENT;
	_material_changed(flags_changed);
}

void PhysicsMaterial::clear_resistivity_temperature_coefficient() {
	if (!has_resistivity_temperature_coefficient()) {
		return;
	}
	fields_set_flags &= ~uint64_t(FIELD_RESISTIVITY_TEMPERATURE_COEFFICIENT);
	resistivity_temperature_coefficient = 0.0;
	_material_changed(true);
}

void PhysicsMaterial::_set_resistivity_temperature_coefficient(const Variant &p_value) {
	if (p_value.get_type() == Variant::NIL) {
		clear_resistivity_temperature_coefficient();
		return;
	}
	ERR_FAIL_COND(p_value.get_type() != Variant::FLOAT && p_value.get_type() != Variant::INT);
	set_resistivity_temperature_coefficient(p_value);
}

void PhysicsMaterial::set_emissivity(double p_value) {
	bool flags_changed = !has_emissivity();
	emissivity = p_value;
	fields_set_flags |= FIELD_EMISSIVITY;
	_material_changed(flags_changed);
}

void PhysicsMaterial::clear_emissivity() {
	if (!has_emissivity()) {
		return;
	}
	fields_set_flags &= ~uint64_t(FIELD_EMISSIVITY);
	emissivity = 0.0;
	_material_changed(true);
}

void PhysicsMaterial::_set_emissivity(const Variant &p_value) {
	if (p_value.get_type() == Variant::NIL) {
		clear_emissivity();
		return;
	}
	ERR_FAIL_COND(p_value.get_type() != Variant::FLOAT && p_value.get_type() != Variant::INT);
	set_emissivity(p_value);
}

void PhysicsMaterial::set_yield_enabled(bool p_enabled) {
	bool flags_changed = !has_yield();
	yield_enabled = p_enabled;
	fields_set_flags |= FIELD_YIELD;
	_material_changed(flags_changed);
}

void PhysicsMaterial::set_yield_stress(double p_stress) {
	bool flags_changed = !has_yield();
	yield_stress = p_stress;
	fields_set_flags |= FIELD_YIELD;
	_material_changed(flags_changed);
}

void PhysicsMaterial::set_yield_curve(const Ref<Curve> &p_curve) {
	bool flags_changed = !has_yield();
	if (yield_curve.is_valid()) {
		yield_curve->disconnect_changed(callable_mp(this, &PhysicsMaterial::_yield_curve_changed));
	}
	yield_curve = p_curve;
	if (yield_curve.is_valid()) {
		yield_curve->connect_changed(callable_mp(this, &PhysicsMaterial::_yield_curve_changed));
	}
	fields_set_flags |= FIELD_YIELD;
	_material_changed(flags_changed);
}

void PhysicsMaterial::_yield_curve_changed() {
	_material_changed();
}

void PhysicsMaterial::clear_yield() {
	if (!has_yield()) {
		return;
	}
	if (yield_curve.is_valid()) {
		yield_curve->disconnect_changed(callable_mp(this, &PhysicsMaterial::_yield_curve_changed));
	}
	yield_curve.unref();
	yield_enabled = false;
	yield_stress = 0.0;
	fields_set_flags &= ~uint64_t(FIELD_YIELD);
	_material_changed(true);
}

void PhysicsMaterial::_set_yield_enabled(const Variant &p_value) {
	if (p_value.get_type() == Variant::NIL) {
		clear_yield();
		return;
	}
	ERR_FAIL_COND(p_value.get_type() != Variant::BOOL);
	set_yield_enabled(p_value);
}

void PhysicsMaterial::_set_yield_stress(const Variant &p_value) {
	if (p_value.get_type() == Variant::NIL) {
		clear_yield();
		return;
	}
	ERR_FAIL_COND(p_value.get_type() != Variant::FLOAT && p_value.get_type() != Variant::INT);
	set_yield_stress(p_value);
}

void PhysicsMaterial::_set_yield_curve(const Variant &p_value) {
	if (p_value.get_type() == Variant::NIL) {
		clear_yield();
		return;
	}
	Ref<Curve> curve = p_value;
	ERR_FAIL_COND(curve.is_null() && p_value.get_type() != Variant::OBJECT);
	set_yield_curve(curve);
}
#endif // !defined(PHYSICS_2D_DISABLED) || !defined(PHYSICS_3D_DISABLED)
