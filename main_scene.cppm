module;

#include <string>
#include <flecs.h>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <string>

export module main_scene;

// import atlas.logger;
import atlas.core.utilities;
import atlas.application;
import atlas.core.scene;
import atlas.core.scene.game_object;
// import atlas.core.event.bus;
// import atlas.core.event.keys;
// import atlas.core.event.mouse_codes;
// import atlas.core.event.types;
import atlas.core.event;
import atlas.core.scene.components;
import atlas.core.utilities.state;
import atlas.core.math;
import atlas.core.editor.dockspace;
import atlas.application;
import atlas.core.ui.widgets;
import atlas.core.editor.menu_item;
import atlas.core.serialize;
import atlas.physics.engine;

/**
 * @name main_scene
 * @brief Implementation of a custom scene that contains game objects
 * 
 * main_scene acts as the first scene created that is associated with the world.
 * 
 */
export class main_scene : public atlas::scene {
public:
    main_scene(const std::string& p_tag, atlas::event::bus& p_bus) : atlas::scene(p_tag, p_bus) {
        auto editor_camera = entity("Editor Camera");
        editor_camera.add<flecs::pair<atlas::tag::editor, atlas::projection_view>>();
        editor_camera.set<atlas::transform>({
            .position = { 3.50f, 4.90f, 36.40f },
            .scale{ 1.f },
        });
        editor_camera.set<atlas::perspective_camera>({
            .plane = { 0.1f, 5000.f },
            .is_active = true,
            .field_of_view = 45.f,
        });

        atlas::game_object bob_object = entity("Bob");
        bob_object.add<atlas::point_light>();

        // @brief For now adding this in
        // because we do not have a way to handle empty scenes
        // so we are adding this in just to have a specific mesh associated with the renderer for the time being, just to make sure it works.
        atlas::game_object viking_room = entity("Viking Room");
        viking_room.add<atlas::tag::serialize>();
        viking_room.set<atlas::transform>({
            .position = { -2.70f, 2.70, -8.30f },
            .rotation = { 2.30f, 95.90f, 91.80f },
            .scale{ 1.f },
        });

        viking_room.set<atlas::sphere_collider>({
            .radius = 1.0f,
        });

        viking_room.set<atlas::physics_body>({
            .friction = 15.f,
            .restitution = 0.3f,
            .body_movement_type = atlas::dynamic,
        });

        atlas::game_object cube = entity("Aircraft");

        cube.set<atlas::transform>({
            .position = { 0.f, 2.10f, -7.30f },
            .scale = { 0.9f, 0.9f, 0.9f },
        });

        cube.set<atlas::mesh_source>({
        .color = { 1.f, 1.f, 1.f, 1.f },
            // .model_path = "assets/models/E 45 Aircraft_obj.obj",
            .model_path = "assets/backpack/backpack.obj",
            .diffuse = "assets/backpack/diffuse.jpg",
            .specular = "assets/backpack/specular.jpg"
            //   .diffuse = "assets/models/E-45-steel detail_2_col.jpg",
        });

        atlas::game_object robot_model = entity("Cube");
        robot_model.add<atlas::tag::serialize>();
        // robot_model.add<atlas::tag::serialize>();
        robot_model.set<atlas::transform>({
            .position = { -2.70, 3.50f, 4.10f },
            .scale = { 1.f, 1.f, 1.f },
        });

        robot_model.set<atlas::mesh_source>({
            .color = { 1.f, 1.f, 1.f, 1.f },
            .model_path = "assets/models/cube.obj",
            .diffuse = "assets/models/container_diffuse.png",
            .specular = "assets/models/container_specular.png",
        });

        robot_model.set<atlas::box_collider>({
            .half_extent = { 1.f, 1.f, 1.f },
        });
        robot_model.set<atlas::physics_body>({
            //   .restitution = 1.f,
            .body_movement_type = atlas::dynamic,
        });

        atlas::game_object platform = entity("Platform");

        platform.set<atlas::transform>({
            .scale = { 15.f, 0.30f, 10.0f },
        });
        platform.set<atlas::mesh_source>({
            .model_path = "assets/models/cube.obj",
            .diffuse = "assets/models/wood.png",
        });
        platform.set<atlas::physics_body>({
            .body_movement_type = atlas::fixed,
        });
        platform.set<atlas::box_collider>({
            .half_extent = { 15.f, 0.30f, 10.0f },
        });

        atlas::game_object point_light = entity("Point Light 1");
        point_light.set<atlas::transform>({
            .position = { 0.f, 2.10f, -7.30f },
            .scale = { 0.9f, 0.9f, 0.9f },
        });
        point_light.add<atlas::tag::serialize>();

        // benchmark

        // auto start = std::chrono::high_resolution_clock::now();
        // TEMP Code
        // [[maybe_unused]] atlas::game_object point_light_test = entity("Point
        // Light 1"); auto end = std::chrono::high_resolution_clock::now(); auto
        // duration = (end - start);

        // auto seconds =
        // std::chrono::duration_cast<std::chrono::seconds>(duration).count(); auto
        // nanoseconds =
        // std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        // auto microseconds =
        // std::chrono::duration_cast<std::chrono::microseconds>(duration).count();

        // console_log_fatal("Seconds = {:.1f}", static_cast<float>(seconds));
        // console_log_fatal("Nanoseconds = {:.1f}",
        // static_cast<float>(nanoseconds)); console_log_fatal("Microseconds =
        // {:.1f}", static_cast<float>(microseconds));

        // for(size_t i = 0; i < 26; i++) {
        // 	auto obj = entity(std::format("Object #{}", i));
        // 	obj->set<atlas::physics_body>({
        // 		.restitution = 1.25f,
        // 		.body_movement_type = atlas::dynamic,
        // 	});

        // 	obj->set<atlas::sphere_collider>(
        // 		{
        // 		.radius = 1.0f,
        // 	});

        // 	glm::vec3 pos = {float(0*1.4),float(0 * 1.4),float(0 * -3) };

        // 	obj->set<atlas::transform>({
        // 		.position = pos,
        // 		.rotation = {.3f, 0.0f, 0.0f},
        // 	});

        // 	obj->set<atlas::mesh_source>({
        // 		.model_path = "assets/models/Ball OBJ.obj",
        // 		.diffuse = "assets/models/clear.png",
        // 	});
        // }

        atlas::game_object gerald = entity("Gerald");
        gerald.add<atlas::point_light>();

        m_deserializer_test = atlas::serializer();

        subscribe<atlas::event::collision_enter>(this,
                                                &main_scene::collision_enter);

        atlas::register_start(this, &main_scene::start_game);
        atlas::register_physics(this, &main_scene::physics_update);
        atlas::register_update(this, &main_scene::on_update);
        atlas::register_ui(this, &main_scene::on_ui_update);
    }

    ~main_scene()  = default;


    void start_game() {
        if (!m_deserializer_test.load("LevelScene", *this)) {
            console_log_error("Could not load yaml file LevelScene!!!");
        }

        flecs::world registry = *this;
        m_physics_engine = atlas::physics::engine(registry, *event_handle());
    }

    //! @note Remove these from being overridden
    //! TODO: We should have an indication on what functions are update phased
    //! functions
    void on_update() {}

    void on_ui_update() {}

    //! TODO: Remove this and integrate a few functions into LevelScene such as
    //! on_runtime_start/on_runtime_stop
    void physics_update() {
        float dt = atlas::application::delta_time();
        if (atlas::event::is_key_pressed(key_r) and !m_physics_runtime) {
            runtime_start();
        }

        auto viking_room = entity("Viking Room");

        atlas::physics_body* sphere_body =
        viking_room.get_mut<atlas::physics_body>();
        // U = +up
        // J = -up
        // H = +left
        // L = -Left
        if (atlas::event::is_key_pressed(key_space)) {
            glm::vec3 linear_velocity = { 0.f, 10.0f, 0.f };
            sphere_body->linear_velocity = linear_velocity;
            sphere_body->impulse = linear_velocity;
        }

        if (atlas::event::is_key_pressed(key_j)) {
            glm::vec3 angular_vel = { -10.f, 0.f, 0.f };
            sphere_body->angular_velocity = angular_vel;
        }

        if (atlas::event::is_key_pressed(key_h)) {
            glm::vec3 angular_vel = { 10.f, 0.f, 0.f };
            sphere_body->angular_velocity = angular_vel;
        }

        if (atlas::event::is_key_pressed(key_l)) {
            glm::vec3 angular_vel = { -0.1f, 0.f, 0.f };
            sphere_body->angular_velocity = angular_vel;
        }

        if (m_physics_runtime) {
            m_physics_engine.update(dt);
        }

        if (atlas::event::is_key_pressed(key_l) and m_physics_runtime) {
            runtime_stop();
        }
    }


    void collision_enter(atlas::event::collision_enter& p_event) {
        console_log_warn("collision_enter event!!!");
        atlas::game_object e1 = entity(p_event.entity1);
        atlas::game_object e2 = entity(p_event.entity2);

        console_log_warn("Entity1 = {}", e1.name().c_str());
        console_log_warn("Entity2 = {}", e2.name().c_str());
    }


private:
    // TODO: Will implement scene management system to coordinate with physics system
    //      for starting and stopping the physics runtime
    void runtime_start() {
        m_physics_runtime = true;
        m_physics_engine.start();
    }

    void runtime_stop() {
        m_physics_runtime = false;
        m_physics_engine.stop();
        reset_objects();
    }

    // All this does is resets all of the game objects back to their initial starting positions
    // NOTE: Typically re-serialization would occur in replacement of this
    void reset_objects() {
        if (!m_deserializer_test.load("LevelScene", *this)) {
            console_log_error("Could not load yaml file LevelScene!!!");
        }
    }

private:
    // atlas::optional_ref<atlas::scene_object> m_viking_room;
    // atlas::optional_ref<atlas::scene_object> m_platform;
    
    // atlas::optional_ref<atlas::scene_object> m_sphere;
    // atlas::optional_ref<atlas::scene_object> m_box;

    // atlas::optional_ref<atlas::scene_object> m_camera;
    // atlas::physics::physics_engine m_physics_engine_handler;

    // bool m_blink_text=false;
    // glm::vec3 m_offset_from_camera;

    bool m_physics_runtime = false;
    atlas::serializer m_deserializer_test;
    // flecs::entity m_selected_entity;

    // atlas::game_object_optional m_current_entity;
    float m_movement_speed = 10.f;

    // Setting physics system
    // TODO -- when refactoring this would be at atlas::world level
    atlas::physics::engine m_physics_engine;
};