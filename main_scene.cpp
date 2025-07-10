#include "main_scene.hpp"
#include <core/ui/widgets.hpp>
#include <core/event/event.hpp>
#include <core/utilities/state.hpp>
#include <core/scene/components.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>
#include <physics/jolt-cpp/jolt_collision.hpp>
#include <physics/physics_3d/physics.hpp>

main_scene::main_scene(const std::string& p_tag)
  : atlas::scene_scope(p_tag) {

    m_camera = create_object("camera");
    auto aspect_ratio = atlas::application::aspect_ratio();
    atlas::camera camera_comp = atlas::camera(aspect_ratio);
    camera_comp.Position = {-1.1f, 6.53f, 23.32f};
    camera_comp.IsMainCamera = true;

    m_camera->set<atlas::camera>(camera_comp);


    //! @note Creating our objects from our scene
    m_viking_room = create_object("Viking Room Object");
    atlas::transform sphere_transform = {
        // .Position{0.f, 0.f, 1.60f},
        .Position = {-2.70f, 2.70, -8.30f},
        .Rotation = {2.30f, 95.90f, 91.80f},
        .Scale{1.f},
    };
    m_viking_room->set<atlas::transform>(sphere_transform);
    m_viking_room->set<atlas::material>({
        .color = {1.f, 1.f, 1.f, 1.f},
        .model_path = "assets/models/viking_room.obj",
        .texture_path = "assets/models/viking_room.png"
    });

    m_viking_room->set<atlas::transform_physics>({
        .position = sphere_transform.Position,
        .scale = sphere_transform.Scale,
    });

    m_viking_room->set<atlas::physics::collider_body>({
        // .half_extents = {15.f, 0.30f, 10.0f},
        .half_extents = sphere_transform.Scale
    });
    m_viking_room->set<atlas::physics::physics_body>({
        .body_type = atlas::physics::Dynamic
    });
    m_viking_room->set<atlas::physics::collider_body>({
        .shape_type = atlas::physics::collider_shape::Box,
        .radius = 1.f,
    });

    m_platform = create_object("Platform");

    m_platform->set<atlas::transform>({
        .Position = { 0.f, -10.f, 0.4f },
        .Scale = {15.f, -0.30f, 10.0f}
    });

    m_platform->set<atlas::material>({
        .color = {1.f, 1.f, 1.f, 1.f},
        .model_path = "assets/models/cube.obj",
        .texture_path = "assets/models/wallace_gromit_image.jpg"
    });
    
    m_platform->set<atlas::transform_physics>({
        .position = { 0.f, -10.f, 0.4f },
        .scale = {15.f, 0.30f, 10.0f},
    });

    m_platform->set<atlas::physics::collider_body>({
        .half_extents = {15.f, 0.30f, 10.0f},
    });

    m_sphere = create_object("Sphere");
    m_sphere->set<atlas::transform>({
        .Position = {-2.70f, 2.70, -8.30f},
        .Rotation = {2.30f, 95.90f, 91.80f},
        .Scale{1.f},
    });
    m_sphere->set<atlas::material>({
        .color{1.f},
        .model_path = "assets/models/sphere.obj",
        .texture_path = "assets/models/wood.png",
    });

    // Adding physics body
    // m_sphere->add<atlas::physics::physics_body>();
    m_sphere->set<atlas::physics::physics_body>({
        .body_type = atlas::physics::Dynamic
    });
    m_sphere->set<atlas::physics::collider_body>({
        .shape_type = atlas::physics::collider_shape::Sphere,
        .radius = 1.f,
    });
    m_sphere->set<atlas::transform_physics>({
        // .position = {-2.70f, 2.70, -8.30f},
        .position = {-2.70f, 2.70, -8.30f},
        // .position{0.f},
        .scale{1.f}
    });



    // Setting up physics stuff
    m_physics_object_representation_of_settings = create_object("Physics Settings");
    // settings is for pre-runtime before runtime gets invoked
    m_physics_object_representation_of_settings->set<atlas::physics::jolt_settings>({});

    // config is for runtime
    m_physics_object_representation_of_settings->set<atlas::physics::jolt_config>({});

    atlas::register_update(this, &main_scene::on_update);
    atlas::register_start(this, &main_scene::start_game);
    atlas::register_physics(this, &main_scene::on_physics_update);
    atlas::register_ui(this, &main_scene::on_ui_update);
}

void main_scene::start_game() {
    // Now we instantiate the physics engine itself
    flecs::world handler = *this;
    m_physics_engine_handler = atlas::physics::initialize_engine(m_allocator, m_physics_object_representation_of_settings, handler);

    // atlas::physics::jolt_collision(((flecs::entity)*m_sphere).id());
}

void main_scene::runtime_start() {
    // runs the physics simulation
    m_physics_is_runtime = true;

    // Setting up sphere transform and convert from euler to quaternions
    auto physics_sphere_transforms = m_sphere->get<atlas::transform_physics>();
    auto physics_sphere_rotation = physics_sphere_transforms->rotation;
    glm::quat quaternion = glm::quat({physics_sphere_rotation.x, physics_sphere_rotation.y, physics_sphere_rotation.z});

    m_sphere->set<atlas::transform>({
        .Position = glm::highp_vec4(quaternion.x, quaternion.y, quaternion.z, quaternion.w),
        .Rotation = physics_sphere_transforms->rotation,
        .Scale = physics_sphere_transforms->scale
    });


    // viking room
    auto viking_room_physics_sphere_transforms = m_viking_room->get<atlas::transform_physics>();
    auto viking_room_physics_sphere_rotation = viking_room_physics_sphere_transforms->rotation;
    glm::quat viking_room_quaternion = glm::quat({viking_room_physics_sphere_rotation.x, viking_room_physics_sphere_rotation.y, viking_room_physics_sphere_rotation.z});

    m_viking_room->set<atlas::transform>({
        .Position = glm::highp_vec4(viking_room_quaternion.x, viking_room_quaternion.y, viking_room_quaternion.z, viking_room_quaternion.w),
        .Rotation = viking_room_physics_sphere_transforms->rotation,
        .Scale = viking_room_physics_sphere_transforms->scale
    });

    m_physics_engine_handler->start_runtime();
}

void main_scene::runtime_stop() {
    // stops the physics simulation
    // also does post-cleanup
    m_physics_is_runtime = false;
    // Setting up sphere transform and convert from euler to quaternions
    auto physics_sphere_transforms = m_sphere->get<atlas::transform>();
    auto physics_sphere_rotation = physics_sphere_transforms->Rotation;
    glm::quat quaternion = glm::quat({physics_sphere_rotation.x, physics_sphere_rotation.y, physics_sphere_rotation.z});

    m_sphere->set<atlas::transform_physics>({
        .position = glm::highp_vec4(quaternion.x, quaternion.y, quaternion.z, quaternion.w),
        .rotation = physics_sphere_transforms->Rotation,
        .scale = physics_sphere_transforms->Scale
    });


    // viking room
    auto viking_physics_sphere_transforms = m_viking_room->get<atlas::transform>();
    auto viking_physics_sphere_rotation = viking_physics_sphere_transforms->Rotation;
    glm::quat viking_quaternion = glm::quat({viking_physics_sphere_rotation.x, viking_physics_sphere_rotation.y, viking_physics_sphere_rotation.z});

    m_viking_room->set<atlas::transform_physics>({
        .position = glm::highp_vec4(viking_quaternion.x, viking_quaternion.y, viking_quaternion.z, viking_quaternion.w),
        .rotation = viking_physics_sphere_transforms->Rotation,
        .scale = viking_physics_sphere_transforms->Scale
    });

    m_physics_engine_handler->stop_runtime();
}

void
main_scene::on_ui_update() {
    atlas::transform* t = m_sphere->get_mut<atlas::transform>();

    atlas::ui::draw_vec3("Position", t->Position);
}

void
main_scene::on_update() {
    atlas::camera* camera = m_camera->get_mut<atlas::camera>();
    float delta_time = atlas::application::delta_time();

    // if (atlas::event::is_key_pressed(key_escape)) {
    //     atlas::application::get_window().close();
    // }

    if (atlas::event::is_key_pressed(key_w)) {
        // console_log_trace(""
        camera->process_keyboard(atlas::forward, delta_time);
    }
    if (atlas::event::is_key_pressed(key_s)) {
        camera->process_keyboard(atlas::backward, delta_time);
    }
    if (atlas::event::is_key_pressed(key_a)) {
        camera->process_keyboard(atlas::left, delta_time);
    }
    if (atlas::event::is_key_pressed(key_d)) {
        camera->process_keyboard(atlas::right, delta_time);
    }
    if (atlas::event::is_key_pressed(key_q)) {
        camera->process_keyboard(atlas::up, delta_time);
    }
    if (atlas::event::is_key_pressed(key_e)) {
        camera->process_keyboard(atlas::down, delta_time);
    }

    //! @note Press shift key to move using the mouse to rotate around
    if (atlas::event::is_key_pressed(key_left_shift)) {
        if (atlas::event::is_mouse_pressed(mouse_button_right)) {
            glm::vec2 cursor_pos = atlas::event::cursor_position();

            float x_offset = cursor_pos.x;
            float velocity = x_offset * delta_time;
            camera->process_mouse_movement(-velocity, 0.f);
        }

        if (atlas::event::is_mouse_pressed(mouse_button_left)) {
            glm::vec2 cursor_pos = atlas::event::cursor_position();

            float x_offset = cursor_pos.x;
            float velocity = x_offset * delta_time;
            camera->process_mouse_movement(velocity, 0.f);
        }

        if (atlas::event::is_mouse_pressed(mouse_button_middle)) {
            glm::vec2 cursor_pos = atlas::event::cursor_position();

            float velocity = cursor_pos.y * delta_time;
            camera->process_mouse_movement(0.f, velocity);
        }

        if (atlas::event::is_key_pressed(key_space)) {
            glm::vec2 cursor_pos = atlas::event::cursor_position();
            float velocity = cursor_pos.y * delta_time;
            camera->process_mouse_movement(0.f, -velocity);
        }
    }
    camera->update_proj_view();
}

void
main_scene::on_physics_update() {

    // TEMPORARY -- Since this needs to be integrated still
    if (atlas::event::is_key_pressed(key_r) and !m_physics_is_runtime) {
        runtime_start();
    }

    if(m_physics_is_runtime) {
        m_physics_engine_handler->physics_step();
        m_physics_engine_handler->run_contact_add();
    }

    if (atlas::event::is_key_pressed(key_l) and m_physics_is_runtime) {
        runtime_stop();
    }

}
