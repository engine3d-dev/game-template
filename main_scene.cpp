#include "main_scene.hpp"
#include <core/common.hpp>
#include <core/math/utilities.hpp>
#include <core/application.hpp>
#include <core/event/event.hpp>
#include <drivers/jolt-cpp/jolt_components.hpp>
#include <any>

main_scene::main_scene(const std::string& p_tag, atlas::event::event_bus& p_bus)
  : atlas::scene_scope(p_tag, p_bus) {

    m_camera = create_object("camera");

    // Editor camera creation
    m_camera->add<flecs::pair<atlas::tag::editor, atlas::projection_view>>();
    m_camera->set<atlas::transform>({
        .position = { 2.46f, 4.90f, -34.92f },
        .rotation = {0.f, 3.08f, 0.f},
        .scale{ 1.f },
    });
    m_camera->set<atlas::perspective_camera>({
      .plane = { 0.1f, 5000.f },
      .is_active = true,
      .field_of_view = 45.f,
    });

    //! @note Creating our objects from our scene
    m_viking_room = create_object("Viking Room Object");
    m_viking_room->set<atlas::transform>({
        .position = {7.20f, 2.70f, -1.70f},
        .rotation = {0.10f, 1.55f, 7.96f},
        .scale{6.f},
    });
    m_viking_room->set<atlas::material>({
        .color = {1.f, 1.f, 1.f, 1.f},
        .model_path = "assets/models/viking_room.obj",
        .texture_path = "assets/models/viking_room.png"
    });

    m_viking_room->set<atlas::box_collider>({
        .half_extent = {6.f, 6.f, 6.f}
    });
    m_viking_room->set<atlas::physics_body>({
        // .body_type = atlas::dynamic
        .body_movement_type = atlas::fixed
    });

    m_platform = create_object("Platform");

    m_platform->set<atlas::transform>({
        .scale = { 15.f, 0.30f, 10.0f },
    });

    m_platform->set<atlas::material>({
        .color = {1.f, 1.f, 1.f, 1.f},
        .model_path = "assets/models/cube.obj",
        .texture_path = "assets/models/wood.png"
    });
    
    m_platform->set<atlas::physics_body>({
      .body_movement_type = atlas::fixed,
    });
    m_platform->set<atlas::box_collider>({
      .half_extent = { 15.f, 0.30f, 10.0f },
    });

    m_sphere = create_object("Sphere");
    glm::vec3 sphere_scale{1.f};
    m_sphere->set<atlas::transform>({
        .position = {-2.70f, 2.70, -8.30f},
        .rotation = {2.30f, 95.90f, 91.80f},
        .scale = sphere_scale,
    });
    m_sphere->set<atlas::material>({
        .color{1.f},
        .model_path = "assets/models/sphere.obj",
        // .texture_path = "assets/models/wood.png",
        // .texture_path = "assets/models/clear.png"
        .texture_path = "assets/models/wall.jpg"
    });

    // Adding physics body
    m_sphere->set<atlas::physics_body>({
        .body_movement_type = atlas::dynamic
    });
    m_sphere->set<atlas::sphere_collider>({
        .radius = glm::compMax(sphere_scale) * 2.5f,
    });
    m_sphere->set<atlas::transform>({
        .position = {-2.70f, 2.70, -8.30f},
        .scale{1.f}
    });

    atlas::strong_ref<atlas::scene_object> cube = create_object("Experiement Cube");
    cube->set<atlas::transform>({
        .position = {-2.70f, 3.f, -8.30f},
        .rotation = {4.0, 0, 0},
        .scale = {15.f, 15.f, 15.f},
    });    
    cube->set<atlas::material>({
        .color = {1.f, 1.f, 1.f, 1.f},
        .model_path = "assets/models/cube.obj",
        .texture_path = "assets/models/wall.jpg"
    });


    // subscription example
    subscribe<atlas::event::collision_enter>(this, &main_scene::collision_enter);

    // game state behavior
    // registration update callbacks for offloading your own game logic
    atlas::register_start(this, &main_scene::start_game);
    atlas::register_update(this, &main_scene::on_update);
    atlas::register_physics(this, &main_scene::on_physics_update);
    atlas::register_ui(this, &main_scene::on_ui_update);
}

void main_scene::reset_objects() {
    m_viking_room->set<atlas::transform>({
        .position = {7.20f, 2.70f, -1.70f},
        .rotation = {0.10f, 1.55f, 7.96f},
        .scale{6.f},
    });

    m_sphere->set<atlas::transform>({
        .position = {-2.70f, 2.70, -8.30f},
        .scale{1.f}
    });
    glm::vec3 sphere_scale{1.f};
    m_sphere->set<atlas::transform>({
        .position = {-2.70f, 2.70, -8.30f},
        .rotation = {2.30f, 95.90f, 91.80f},
        .scale = sphere_scale,
    });


}

void main_scene::collision_enter(atlas::event::collision_enter& p_event) {
    console_log_warn("Collision Enter happened!!! Executed from main_scene::collision_enter");

    flecs::world registry = *this;

    flecs::entity e1 = registry.entity(p_event.entity1);
    flecs::entity e2 = registry.entity(p_event.entity2);

    console_log_warn("Entity1 = {}", e1.name().c_str());
    console_log_warn("Entity2 = {}", e2.name().c_str());
}

void main_scene::start_game() {
    // Was only used for testing purpose. Can remove if you want
    // m_deserializer_test = atlas::serializer();
    // if(!m_deserializer_test.load("LevelScene", *this)) {
    //     console_log_error("Cannot load LevelScene!!!");
    // }

    atlas::physics::jolt_settings settings = {};
    flecs::world registry = *this;
    m_physics_engine_handler = atlas::physics::physics_engine(settings, registry, *event_handle());
}

void main_scene::runtime_start() {
    // runs the physics simulation
    m_physics_is_runtime = true;

    m_physics_engine_handler.start();
}

void main_scene::runtime_stop() {
    // stops the physics simulation
    // also does post-cleanup
    m_physics_is_runtime = false;

    m_physics_engine_handler.stop();

    reset_objects();
}

void
main_scene::on_ui_update() {
    // atlas::transform* t = m_viking_room->get_mut<atlas::transform>();
    // atlas::transform* t = m_camera->get_mut<atlas::transform>();
    atlas::transform* t = m_platform->get_mut<atlas::transform>();


    atlas::ui::draw_vec3("Rotation", t->rotation);
    atlas::ui::draw_vec3("Scale", t->scale);
    atlas::ui::draw_vec3("Position", t->position);



}

void
main_scene::on_update() {
    float smooth_speed = 0.1f;
    atlas::transform* camera_transform = m_camera->get_mut<atlas::transform>();
    atlas::transform* sphere_transform = m_sphere->get_mut<atlas::transform>();

    float dt = atlas::application::delta_time();
    float movement_speed = 10.f;
    float rotation_speed = 1.f;
    float velocity = movement_speed * dt;
    float rotation_velocity = rotation_speed * dt;
    glm::quat quaternion = atlas::to_quat(camera_transform->quaternion);
    glm::vec3 up = glm::rotate(quaternion, glm::vec3(0.f, 1.f, 0.f));
    glm::vec3 forward = glm::rotate(quaternion, glm::vec3(0.f, 0.f, -1.f));
    glm::vec3 right = glm::rotate(quaternion, glm::vec3(1.0f, 0.0f, 0.0f));

    if (atlas::event::is_key_pressed(key_left_shift)) {
        if (atlas::event::is_mouse_pressed(mouse_button_middle)) {
            camera_transform->position += up * velocity;
        }

        if (atlas::event::is_mouse_pressed(mouse_button_right)) {
            camera_transform->position -= up * velocity;
        }
    }

    if (atlas::event::is_key_pressed(key_w)) {
        camera_transform->position += forward * velocity;
    }
    if (atlas::event::is_key_pressed(key_s)) {
        camera_transform->position -= forward * velocity;
    }

    if (atlas::event::is_key_pressed(key_d)) {
        camera_transform->position += right * velocity;
    }
    if (atlas::event::is_key_pressed(key_a)) {
        camera_transform->position -= right * velocity;
    }

    if (atlas::event::is_key_pressed(key_q)) {
        camera_transform->rotation.y += rotation_velocity;
    }
    if (atlas::event::is_key_pressed(key_e)) {
        camera_transform->rotation.y -= rotation_velocity;
    }

    camera_transform->set_rotation(camera_transform->rotation);
}

void
main_scene::on_physics_update() {
    float dt = atlas::application::delta_time();
    atlas::physics_body* sphere_body = m_sphere->get_mut<atlas::physics_body>();

    if (atlas::event::is_key_pressed(key_r) and !m_physics_is_runtime) {
        runtime_start();
    }

    if(m_physics_is_runtime) {
        m_physics_engine_handler.update(dt);
    }

    if (atlas::event::is_key_pressed(key_l) and m_physics_is_runtime) {
        runtime_stop();
    }

    // U = +up
    // J = -up
    // H = +left
    // L = -Left
    if(atlas::event::is_key_pressed(key_u)) {
        glm::vec3 angular_vel = {0.f, 1.f, 0.f};
        sphere_body->angular_velocity = angular_vel;
    }

    if(atlas::event::is_key_pressed(key_j)) {
        glm::vec3 angular_vel = {0.f, -1.f, 0.f};
        sphere_body->angular_velocity = angular_vel;
    }

    if(atlas::event::is_key_pressed(key_h)) {
        glm::vec3 angular_vel = {1.f, 0.f, 0.f};
        sphere_body->angular_velocity = angular_vel;
    }

    if(atlas::event::is_key_pressed(key_l)) {
        glm::vec3 angular_vel = {-1.f, 0.f, 0.f};
        sphere_body->angular_velocity = angular_vel;
    }
}
