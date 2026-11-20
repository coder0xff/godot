/**************************************************************************/
/*  test_physics_material.cpp                                             */
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

#include "tests/test_macros.h"

TEST_FORCE_LINK(test_physics_material)

#ifndef PHYSICS_3D_DISABLED

#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"
#include "core/object/callable_mp.h"
#include "scene/3d/physics/rigid_body_3d.h"
#include "scene/3d/physics/static_body_3d.h"
#include "scene/resources/physics_material.h"
#include "servers/physics_3d/physics_server_3d_dummy.h"
#include "servers/physics_3d/physics_server_3d_extension.h"
#include "tests/test_utils.h"

namespace TestPhysicsMaterial {

TEST_CASE("[Physics_material] Defaults") {
	Ref<PhysicsMaterial> physics_material;
	physics_material.instantiate();

	CHECK(physics_material->get_friction() == 1.);
	CHECK(physics_material->is_rough() == false);
	CHECK(physics_material->get_bounce() == 0.);
	CHECK(physics_material->is_absorbent() == false);
}

TEST_CASE("[Physics_material] Friction") {
	Ref<PhysicsMaterial> physics_material;
	physics_material.instantiate();

	real_t friction = 0.314;
	physics_material->set_friction(friction);
	CHECK(physics_material->get_friction() == friction);
}

TEST_CASE("[Physics_material] Rough") {
	Ref<PhysicsMaterial> physics_material;
	physics_material.instantiate();

	bool rough = true;
	physics_material->set_rough(rough);
	CHECK(physics_material->is_rough() == rough);

	real_t friction = 0.314;
	physics_material->set_friction(friction);
	CHECK(physics_material->computed_friction() == -friction);

	rough = false;
	physics_material->set_rough(rough);
	CHECK(physics_material->is_rough() == rough);

	CHECK(physics_material->computed_friction() == friction);
}

TEST_CASE("[Physics_material] Bounce") {
	Ref<PhysicsMaterial> physics_material;
	physics_material.instantiate();

	real_t bounce = 0.271;
	physics_material->set_bounce(bounce);
	CHECK(physics_material->get_bounce() == bounce);
}

TEST_CASE("[Physics_material] Absorbent") {
	Ref<PhysicsMaterial> physics_material;
	physics_material.instantiate();

	bool absorbent = true;
	physics_material->set_absorbent(absorbent);
	CHECK(physics_material->is_absorbent() == absorbent);

	real_t bounce = 0.271;
	physics_material->set_bounce(bounce);
	CHECK(physics_material->computed_bounce() == -bounce);

	absorbent = false;
	physics_material->set_absorbent(absorbent);
	CHECK(physics_material->is_absorbent() == absorbent);

	CHECK(physics_material->computed_bounce() == bounce);
}

// The dummy supplies unrelated virtuals. This opt-in double admits a snapshot or refuses atomically.
class MaterialServer : public PhysicsServer3DDummy {
	uint64_t next_body = 1;
	HashMap<RID, Ref<PhysicsMaterial>> accepted;

public:
	Error admission = OK;
	int submissions = 0;
	int coefficient_writes = 0;
	real_t last_friction = 1.0;
	real_t last_bounce = 0.0;

	RID body_create() override { return RID::from_uint64(next_body++); }
	Error body_set_material(RID p_body, Object *p_material) override {
		submissions++;
		if (admission != OK) {
			return admission;
		}
		PhysicsMaterial *material = Object::cast_to<PhysicsMaterial>(p_material);
		if (material) {
			accepted[p_body] = material->duplicate(true);
		} else {
			accepted.erase(p_body);
		}
		return OK;
	}
	Object *body_get_material(RID p_body) const override {
		const Ref<PhysicsMaterial> *material = accepted.getptr(p_body);
		return material ? material->ptr() : nullptr;
	}
	void body_set_param(RID p_body, PS3DE::BodyParameter p_param, const Variant &p_value) override {
		if (p_param == PS3DE::BODY_PARAM_FRICTION) {
			coefficient_writes++;
			last_friction = p_value;
		}
		if (p_param == PS3DE::BODY_PARAM_BOUNCE) {
			coefficient_writes++;
			last_bounce = p_value;
		}
	}
};

TEST_CASE("[Physics_material] Fields start unset and explicit flags survive zero and clear") {
	Ref<PhysicsMaterial> material;
	material.instantiate();
	CHECK_FALSE(material->has_density());
	CHECK_FALSE(material->has_specific_heat());
	CHECK_FALSE(material->has_thermal_conductivity());
	CHECK_FALSE(material->has_melting_temperature());
	CHECK_FALSE(material->has_latent_heat());
	CHECK_FALSE(material->has_liquid_specific_heat());
	CHECK_FALSE(material->has_resistivity());
	CHECK_FALSE(material->has_resistivity_temperature_coefficient());
	CHECK_FALSE(material->has_emissivity());
	CHECK_FALSE(material->has_yield());
	int64_t revision = material->get_material_revision();
	material->set_density(7850.0);
	CHECK(material->has_density());
	CHECK(material->get_density() == 7850.0);
	CHECK(material->get_material_revision() == revision + 1);
	material->set_latent_heat(0.0);
	CHECK(material->has_latent_heat());
	material->clear_density();
	CHECK_FALSE(material->has_density());
	material->set("latent_heat", Variant()); // Inspector checkbox's nil protocol, not a numeric sentinel.
	CHECK_FALSE(material->has_latent_heat());
}

TEST_CASE("[Physics_material] Fields scalars map without display conversions") {
	Ref<PhysicsMaterial> material;
	material.instantiate();
	material->set_specific_heat(490.0);
	material->set_thermal_conductivity(45.0);
	material->set_melting_temperature(1700.0);
	material->set_liquid_specific_heat(820.0);
	material->set_resistivity(1.43e-7);
	material->set_resistivity_temperature_coefficient(0.0045);
	material->set_emissivity(0.8);
	CHECK(material->get_specific_heat() == 490.0);
	CHECK(material->get_thermal_conductivity() == 45.0);
	CHECK(material->get_melting_temperature() == 1700.0);
	CHECK(material->get_liquid_specific_heat() == 820.0);
	CHECK(material->get_resistivity() == 1.43e-7);
	CHECK(material->get_resistivity_temperature_coefficient() == 0.0045);
	CHECK(material->get_emissivity() == 0.8);
}

TEST_CASE("[Physics_material] Yield is one explicit group and nested changes advance revision") {
	Ref<PhysicsMaterial> material;
	material.instantiate();
	Ref<Curve> old_curve;
	old_curve.instantiate();
	Ref<Curve> curve;
	curve.instantiate();
	material->set_yield_curve(old_curve);
	CHECK(material->has_yield_enabled());
	CHECK(material->has_yield_stress());
	material->set_yield_curve(curve);
	int64_t revision = material->get_material_revision();
	old_curve->add_point(Vector2(0, 1));
	CHECK(material->get_material_revision() == revision);
	curve->add_point(Vector2(0, 1));
	CHECK(material->get_material_revision() == revision + 1);
	material->set_yield_stress(235000000.0);
	material->set_yield_enabled(true);
	CHECK(material->is_yield_enabled());
	CHECK(material->get_yield_stress() == 235000000.0);
	material->clear_yield_enabled();
	CHECK_FALSE(material->has_yield());
	CHECK_FALSE(material->has_yield_curve());
	revision = material->get_material_revision();
	curve->add_point(Vector2(1, 1));
	CHECK(material->get_material_revision() == revision);
}

TEST_CASE("[Physics_material] Serialization retains explicit false zero and unset fields") {
	Ref<PhysicsMaterial> material;
	material.instantiate();
	material->set_latent_heat(0.0);
	material->set_yield_enabled(false);
	String path = TestUtils::get_temp_path("fields_material.tres");
	REQUIRE(ResourceSaver::save(material, path) == OK);
	Ref<PhysicsMaterial> restored = ResourceLoader::load(path, "PhysicsMaterial", ResourceLoader::CACHE_MODE_IGNORE);
	REQUIRE(restored.is_valid());
	CHECK(restored->has_latent_heat());
	CHECK(restored->get_latent_heat() == 0.0);
	CHECK(restored->has_yield());
	CHECK_FALSE(restored->is_yield_enabled());
	CHECK_FALSE(restored->has_density());
}

TEST_CASE("[Physics_material] Native and optional extension servers keep the legacy default") {
	Ref<PhysicsMaterial> material;
	material.instantiate();
	{
		PhysicsServer3DDummy server;
		CHECK(server.body_set_material(RID(), material.ptr()) == ERR_UNAVAILABLE);
		CHECK(server.body_get_material(RID()) == nullptr);
	}
	{
		PhysicsServer3DExtension server;
		CHECK(server.body_set_material(RID(), material.ptr()) == ERR_UNAVAILABLE);
		CHECK(server.body_get_material(RID()) == nullptr);
	}
}

template <typename T>
void check_material_node() {
	MaterialServer *server = memnew(MaterialServer);
	T *body = memnew(T);
	Ref<PhysicsMaterial> first;
	first.instantiate();
	first->set_density(100.0);
	first->set_friction(0.4);
	first->set_bounce(0.2);
	body->set_physics_material_override(first);
	CHECK(body->get_physics_material_override() == first);
	CHECK(server->last_friction == doctest::Approx(0.4));
	CHECK(server->last_bounce == doctest::Approx(0.2));
	int writes = server->coefficient_writes;
	server->admission = ERR_INVALID_DATA;
	Ref<PhysicsMaterial> rejected;
	rejected.instantiate();
	body->set_physics_material_override(rejected);
	CHECK(body->get_physics_material_override() == first);
	CHECK(server->coefficient_writes == writes);
	first->set_density(-1.0);
	CHECK(body->get_physics_material_override() == first); // User's edited resource stays attached.
	PhysicsMaterial *simulated = Object::cast_to<PhysicsMaterial>(server->body_get_material(body->get_rid()));
	REQUIRE(simulated);
	CHECK(simulated->get_density() == 100.0);
	CHECK(server->coefficient_writes == writes);
	server->admission = OK;
	first->set_density(200.0);
	simulated = Object::cast_to<PhysicsMaterial>(server->body_get_material(body->get_rid()));
	REQUIRE(simulated);
	CHECK(simulated->get_density() == 200.0);
	body->set_physics_material_override(Ref<PhysicsMaterial>());
	CHECK(body->get_physics_material_override().is_null());
	CHECK(server->last_friction == 1.0);
	CHECK(server->last_bounce == 0.0);
	int submissions = server->submissions;
	first->set_density(300.0);
	CHECK(server->submissions == submissions); // Clearing disconnected the old resource.
	memdelete(body);
	memdelete(server);
}

TEST_CASE("[Physics_material] Rigid body set changed refusal and clear are transactional") {
	check_material_node<RigidBody3D>();
}

TEST_CASE("[Physics_material] Static body set changed refusal and clear are transactional") {
	check_material_node<StaticBody3D>();
}


static int changed_count = 0;
static PhysicsMaterial *observed_material = nullptr;
static int64_t observed_revision = 0;
static void record_material_changed() {
	changed_count++;
	observed_revision = observed_material->get_material_revision();
}

TEST_CASE("[Physics_material] One changed signal observes the new revision") {
	Ref<PhysicsMaterial> material;
	material.instantiate();
	observed_material = material.ptr();
	changed_count = 0;
	material->connect_changed(callable_mp_static(record_material_changed));
	int64_t before = material->get_material_revision();
	material->set_density(100.0);
	CHECK(changed_count == 1);
	CHECK(observed_revision == before + 1);
	material->set_friction(0.3);
	CHECK(changed_count == 2);
	CHECK(observed_revision == before + 2);
	Ref<Curve> curve;
	curve.instantiate();
	material->set_yield_curve(curve);
	CHECK(changed_count == 3);
	curve->add_point(Vector2(0, 1));
	CHECK(changed_count == 4);
	CHECK(observed_revision == before + 4);
	material->disconnect_changed(callable_mp_static(record_material_changed));
	observed_material = nullptr;
}

TEST_CASE("[Physics_material] Shared nested Curve refusal retains both authored resources") {
	MaterialServer *server = memnew(MaterialServer);
	RigidBody3D *rigid = memnew(RigidBody3D);
	StaticBody3D *stationary = memnew(StaticBody3D);
	Ref<PhysicsMaterial> material;
	material.instantiate();
	Ref<Curve> curve;
	curve.instantiate();
	curve->add_point(Vector2(0, 1));
	material->set_yield_curve(curve);
	rigid->set_physics_material_override(material);
	stationary->set_physics_material_override(material);
	server->admission = ERR_INVALID_DATA;
	curve->add_point(Vector2(1, 0.5));
	CHECK(rigid->get_physics_material_override() == material);
	CHECK(stationary->get_physics_material_override() == material);
	for (RID body : {rigid->get_rid(), stationary->get_rid()}) {
		PhysicsMaterial *snapshot = Object::cast_to<PhysicsMaterial>(server->body_get_material(body));
		REQUIRE(snapshot);
		CHECK(snapshot->get_yield_curve()->get_point_count() == 1);
	}
	server->admission = OK;
	curve->add_point(Vector2(0.5, 0.75));
	for (RID body : {rigid->get_rid(), stationary->get_rid()}) {
		PhysicsMaterial *snapshot = Object::cast_to<PhysicsMaterial>(server->body_get_material(body));
		REQUIRE(snapshot);
		CHECK(snapshot->get_yield_curve()->get_point_count() == 3);
	}
	memdelete(rigid);
	memdelete(stationary);
	memdelete(server);
}

} // namespace TestPhysicsMaterial

#endif // PHYSICS_3D_DISABLED
