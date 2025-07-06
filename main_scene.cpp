#include "main_scene.hpp"
#include <core/ui/widgets.hpp>
#include <core/event/event.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <core/scene/components.hpp>

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

    m_platform = create_object("Platform");

    m_platform->set<atlas::transform>({
        // .Position = { 0.f, 1.40f, -7.4f },
        // .Scale = { 2.80f, -0.08f, 3.50f },
        .Scale = {15.f, -0.30f, 10.0f}
    });

    m_platform->set<atlas::material>({
        .color = {0.f, 1.f, 0.f, 1.f},
        .model_path = "assets/models/cube.obj",
        // .texture_path = "assets/models/wallace_gromit_image.jpg"
    });


    sync(this, &main_scene::on_update);
    sync_physics(this, &main_scene::on_physics_update);
    attach(this, &main_scene::on_ui_update);
}

void
main_scene::on_ui_update() {

    if (ImGui::Begin("Settings")) {
        ImGui::End();
    }

}

void
main_scene::on_update() {
    atlas::camera* camera = m_camera->get_mut<atlas::camera>();
    float delta_time = atlas::application::delta_time();

    if (atlas::event::is_key_pressed(key_escape)) {
        atlas::application::get_window().close();
    }

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
    camera->IsMainCamera = true;
}

void
main_scene::on_physics_update() {

}
