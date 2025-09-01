#include "main_scene.hpp"
#include <core/common.hpp>
#include <core/math/utilities.hpp>
#include <core/application.hpp>
#include <core/event/event.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>
#include <physics/jolt-cpp/jolt_collision.hpp>
#include <physics/physics_3d/physics.hpp>

main_scene::main_scene(const std::string& p_tag)
  : atlas::scene_scope(p_tag) {

    m_camera = create_object("camera");

    m_camera->add<flecs::pair<atlas::tag::editor, atlas::projection_view>>();
    m_camera->set<atlas::transform>({
        .position = { 74.54f, 0.12f, -15.99f },
        .rotation = {0.f, 1.54f, 0.f}
    });
    m_camera->set<atlas::perspective_camera>({
      .plane = { 0.1f, 5000.f },
      .is_active = true,
      .field_of_view = 45.f,
    });

    //! @note Creating our objects from our scene
    m_viking_room = create_object("Viking Room Object");
    atlas::transform sphere_transform = {
        .position = {3.30f, 2.70, -8.30f},
        .rotation = {2.30f, 95.90f, 91.80f},
        .scale{1.f},
    };
    m_viking_room->set<atlas::transform>(sphere_transform);
    m_viking_room->set<atlas::material>({
        .color = {1.f, 1.f, 1.f, 1.f},
        .model_path = "assets/models/viking_room.obj",
        .texture_path = "assets/models/viking_room.png"
    });

    m_viking_room->set<atlas::transform>({
        .position = sphere_transform.position,
        .scale = sphere_transform.scale,
    });

    m_viking_room->set<atlas::physics::collider_body>({
        .half_extents = sphere_transform.scale
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
        .position = {-1.f, -16.80f, -25.40f},
        .scale = {5.70f, 0.30f, 4.70f}
    });

    m_platform->set<atlas::material>({
        .color = {1.f, 1.f, 1.f, 1.f},
        .model_path = "assets/models/cube.obj",
        .texture_path = "assets/models/wood.png"
    });
    
    m_platform->set<atlas::physics::collider_body>({
        .half_extents = {5.70f, 0.30f, 4.70f}
    });

    m_giant_platform = create_object("Second Platform");
    m_giant_platform->set<atlas::transform>({
        .position = {-1.f, -16.80f, -25.40f},
        .scale = {5.70f, 0.30f, 4.70f}
    });
    m_giant_platform->set<atlas::material>({
        .color = {1.f, 1.f, 1.f, 1.f},
        .model_path = "assets/models/cube.obj",
        .texture_path = "assets/models/E-45-steel detail_2_col.jpg"
    });
    
    m_giant_platform->set<atlas::physics::collider_body>({
        .half_extents = {5.70f, 0.30f, 4.70f}
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
    m_sphere->set<atlas::physics::physics_body>({
        .body_type = atlas::physics::Dynamic
    });
    m_sphere->set<atlas::physics::collider_body>({
        .shape_type = atlas::physics::collider_shape::Sphere,
        .half_extents = sphere_scale,
        .radius = glm::compMax(sphere_scale) * 2.5f,
    });
    m_sphere->set<atlas::transform>({
        .position = {-2.70f, 2.70, -8.30f},
        .scale{1.f}
    });

    // Setup our spheres
    m_grouped_spheres.reserve(10);
    for (int x = 0; x < 1; x++) {
        m_grouped_spheres.emplace_back();
        m_grouped_spheres[x].reserve(10);

        for (int y = 0; y < 3; y++) {
            m_grouped_spheres[x].emplace_back();
            m_grouped_spheres[x][y].reserve(10);

            for (int z = 0; z < 3; z++) {
                std::string name = "Ball " + std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z);

                auto obj = create_object(name);
                obj->add<atlas::tag::serialize>();
                obj->set<atlas::physics::physics_body>({
                    .restitution = 1.25f,
                    .body_movement_type = atlas::physics::Dynamic,
                });

                obj->set<atlas::physics::collider_body>(
                    {.shape_type = atlas::physics::collider_shape::Sphere,
                    .radius = 1.0f,
                });

                glm::vec3 pos = {float(x*1.4),float(y * 1.4),float(z * -3) };

                obj->set<atlas::transform>({
                    .position = pos,
                    .rotation = {.3f, 0.0f, 0.0f},
                });

                obj->set<atlas::material>({
                    .model_path = "assets/models/Ball OBJ.obj",
                    .texture_path = "assets/models/clear.png",
                });
                m_grouped_spheres[x][y].emplace_back(obj); 
            }
        }
    }


    // setup giant cube 1
    m_giant_cube1 = create_object("Cube 1");
    m_giant_cube1->add<atlas::tag::serialize>();
    m_giant_cube1->set<atlas::transform>(
      {
        .position = {0.f, -20.00f, 0.4f},
        .rotation = {4.0, 0, 0},
        .scale = {15.f, 15.0f, 15.0f},
    });

    auto cube1_transform = *m_giant_cube1->get<atlas::transform>();
    auto rot1 = glm::quat{cube1_transform.rotation};
    cube1_transform.quaternion = {rot1.x, rot1.y, rot1.z, rot1.w};
    m_giant_cube1->set<atlas::transform>(cube1_transform);
    m_giant_cube1->set<atlas::physics::collider_body>({.half_extents = {15.f, 15.0f, 15.0f}});
    m_giant_cube1->set<atlas::material>({
            .color = {1.f, 1.f, 1.f, 1.f},
            .model_path = "assets/models/cube.obj",
            .texture_path = "assets/models/wallace_gromit_image.jpg"
    });

    // setup giant cube 2
    m_giant_cube2 = create_object("Cube 2");
    m_giant_cube2->add<atlas::tag::serialize>();
    m_giant_cube2->set<atlas::transform>({
        .position = {0.f, -20.00f, -50.4f},
        .rotation = {4.0, 0, 0},
        .scale = {15.f, 15.f, 15.f},
    });

    auto cube2_transform = *m_giant_cube2->get<atlas::transform>();
    auto rot2 = glm::quat{cube2_transform.rotation};
    cube2_transform.quaternion = {rot2.x, rot2.y, rot2.z, rot2.w};
    m_giant_cube2->set<atlas::transform>(cube2_transform);
    m_giant_cube2->set<atlas::physics::collider_body>({.half_extents = {15.f, 15.0f, 15.0f}});
    m_giant_cube2->set<atlas::material>(
      {
            .color = {1.f, 1.f, 1.f, 1.f},
            .model_path = "assets/models/cube.obj",
            .texture_path = "assets/models/wallace_gromit_image.jpg"
        }
    );

    // Initiating physics system
    // Note: Each flecs::world would contain their own respective data of atlas::physics::settings
    // NOTE: atlas::physics::jolt_settings should not be directly from the application
    // atlas::physics::jolt_settings is essentially the parameters to initialize JoltPhysics directly.
    // Consider: This might be considered in a developers settings or so.
    m_physics_object_representation_of_settings = create_object("Physics Settings");
    // settings is for pre-runtime before runtime gets invoked
    m_physics_object_representation_of_settings->set<atlas::physics::jolt_settings>({});

    // config is for runtime
    // change atlas::physics::jolt_config to atlas::physics::environment_settings
    // atlas::physics::jolt_config is the global configuration that gets applied when physics runtime initiates and executes
    m_physics_object_representation_of_settings->set<atlas::physics::jolt_config>({});

    atlas::register_start(this, &main_scene::start_game);
    atlas::register_update(this, &main_scene::on_update);
    atlas::register_physics(this, &main_scene::on_physics_update);
    atlas::register_ui(this, &main_scene::on_ui_update);
}

void main_scene::reset_objects() {
    m_viking_room->set<atlas::transform>({
        .position = {3.30f, 2.70, -8.30f},
        .rotation = {2.30f, 95.90f, 91.80f},
        .scale{1.f},
    });

    m_sphere->set<atlas::transform>({
        .position = {-2.70f, 2.70, -8.30f},
        .rotation = {2.30f, 95.90f, 91.80f},
        .scale{1.f},
    });

    for (int x = 0; x < 1; x++) {

        for (int y = 0; y < 3; y++) {

            for (int z = 0; z < 3; z++) {

                m_grouped_spheres[x][y][z]->set<atlas::physics::physics_body>(
                    {.restitution = 1.25f,
                    .body_movement_type = atlas::physics::Dynamic});

                m_grouped_spheres[x][y][z]->set<atlas::physics::collider_body>(
                    {.shape_type = atlas::physics::collider_shape::Sphere,
                    .radius = 1.0f});

                glm::vec3 pos = {float(x*1.4), float(y * 1.4),
                                float(z * -3)};

                m_grouped_spheres[x][y][z]->set<atlas::transform>(
                    {.position = pos, .rotation = {.3f, 0.0f, 0.0f}});
            }
        }
    }
    m_giant_cube1->set<atlas::transform>({
        .position = {0.f, -20.00f, 0.4f},
        .rotation = {4.0, 0, 0},
        .scale = {15.f, 15.0f, 15.0f},
    });

    auto cube1_transform = *m_giant_cube1->get<atlas::transform>();
    auto rot1 = glm::quat{cube1_transform.rotation};
    cube1_transform.quaternion = {rot1.x, rot1.y, rot1.z, rot1.w};
    m_giant_cube1->set<atlas::transform>(cube1_transform);

    m_giant_cube2->set<atlas::transform>({
        .position = {0.f, -20.00f, -50.4f},
        .rotation = {4.0, 0, 0},
        .scale = {15.f, 15.f, 15.f},
    });

    auto cube2_transform = *m_giant_cube2->get<atlas::transform>();
    auto rot2 = glm::quat{cube2_transform.rotation};
    cube2_transform.quaternion = {rot2.x, rot2.y, rot2.z, rot2.w};
    m_giant_cube2->set<atlas::transform>(cube2_transform);
}

void main_scene::start_game() {
    // Quick deserialization
    m_deserializer_test = atlas::serializer();
    if(!m_deserializer_test.load("LevelScene", *this)) {
        console_log_error("Cannot load LevelScene!!!");
    }
    // Now we instantiate the physics engine itself
    m_physics_engine_handler = atlas::physics::initialize_engine(m_physics_system_allocator, m_physics_object_representation_of_settings, *this);
}

void main_scene::runtime_start() {
    // runs the physics simulation
    m_physics_is_runtime = true;

    m_physics_engine_handler->start_runtime();
}

void main_scene::runtime_stop() {
    // stops the physics simulation
    // also does post-cleanup
    m_physics_is_runtime = false;

    m_physics_engine_handler->stop_runtime();

    reset_objects();
}

void draw_transforms(atlas::transform* p_transform) {
    atlas::ui::draw_panel_component<atlas::transform>("Transform", [&]() {
        atlas::ui::draw_vec3("position", p_transform->position);
        atlas::ui::draw_vec3("scale", p_transform->scale);
        atlas::ui::draw_vec3("rotation", p_transform->rotation);
    });
}

void draw_entity_heirarchy(flecs::entity& p_entity) {
    if(p_entity.has<atlas::transform>()) {
        draw_transforms(p_entity.get_mut<atlas::transform>());
    }
}

void
main_scene::on_ui_update() {
    flecs::world current_registry = *this;
    atlas::transform* viking_transform = m_viking_room->get_mut<atlas::transform>();
    atlas::transform* t = m_sphere->get_mut<atlas::transform>();
    atlas::transform* platform_transform = m_platform->get_mut<atlas::transform>();
    atlas::transform* camera_transform = m_camera->get_mut<atlas::transform>();
    atlas::perspective_camera* persp_cam = m_camera->get_mut<atlas::perspective_camera>();

    if(ImGui::Begin("Scene Heirarchy Panel")){

        current_registry.each([&](flecs::entity p_entity){
            ImGui::Button(p_entity.name().c_str());
            if(ImGui::IsItemClicked() and !ImGui::IsItemToggledOpen()) {
                m_selected_entity = p_entity;
            }
        });

        ImGui::End();
    }


    atlas::ui::draw_vec3("position", t->position);

    if (ImGui::Begin("Properties Panel")) {
        atlas::ui::draw_panel_component<atlas::transform>("Sphere", [&]() {
            atlas::ui::draw_vec3("position", viking_transform->position);
            atlas::ui::draw_vec3("scale", viking_transform->scale);
            atlas::ui::draw_vec3("rotation", viking_transform->rotation);

            atlas::ui::draw_float("fov", persp_cam->field_of_view);
        });

        atlas::ui::draw_panel_component<atlas::perspective_camera>("Camera", [&](){
            atlas::ui::draw_vec3("position", camera_transform->position);
            atlas::ui::draw_vec3("scale", camera_transform->scale);
            atlas::ui::draw_vec3("rotation", camera_transform->rotation);
        });

        atlas::ui::draw_panel_component<atlas::physics::physics_body>("Platform", [&](){
            atlas::ui::draw_vec3("position", platform_transform->position);
            atlas::ui::draw_vec3("scale", platform_transform->scale);
            atlas::ui::draw_vec3("rotation", platform_transform->rotation);
        });

        if(m_selected_entity.is_alive()) {
            draw_entity_heirarchy(m_selected_entity);
        }

        ImGui::End();
    }
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

    // glm::quat to_quaternion = glm::quat({ camera_transform->quaternion.w,
    //                                       camera_transform->quaternion.x,
    //                                       camera_transform->quaternion.y,
    //                                       camera_transform->quaternion.z });
    glm::quat quaternion = atlas::to_quat(camera_transform->quaternion);
    // glm::quat some_quaternion = atlas::physics::t
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

    // camera_transform->quaternion =
    //   glm::vec4({ quaternion.x, quaternion.y, quaternion.z, quaternion.w });
    camera_transform->set_rotation(camera_transform->rotation);
}

void
main_scene::on_physics_update() {
    atlas::physics::physics_body* sphere_body = m_sphere->get_mut<atlas::physics::physics_body>();

    // TODO: atlas::scene_manager will be used to coordinate between physics runtime and the manager itself.
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

    // U = +up
    // J = -up
    // H = +left
    // L = -Left
    if(atlas::event::is_key_pressed(key_u)) {
        glm::vec3 angular_vel = {0.f, 1.f, 0.f};
        // atlas::physics::add_angular_velocity(angular_vel, *sphere_body);
        atlas::physics::add_velocity(angular_vel, *sphere_body);
    }

    if(atlas::event::is_key_pressed(key_j)) {
        glm::vec3 angular_vel = {0.f, -1.f, 0.f};
        atlas::physics::add_velocity(angular_vel, *sphere_body);
    }

    if(atlas::event::is_key_pressed(key_h)) {
        glm::vec3 angular_vel = {1.f, 0.f, 0.f};
        // atlas::physics::add_angular_velocity(angular_vel, *sphere_body);
        atlas::physics::add_velocity(angular_vel, *sphere_body);
    }

    if(atlas::event::is_key_pressed(key_l)) {
        glm::vec3 angular_vel = {-1.f, 0.f, 0.f};
        atlas::physics::add_velocity(angular_vel, *sphere_body);
    }
}
