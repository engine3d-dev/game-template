#include "main_scene.hpp"
#include <core/ui/widgets.hpp>
#include <core/event/event.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <core/scene/components.hpp>

main_scene::main_scene(const std::string& p_tag)
  : atlas::scene_scope(p_tag) {
    console_log_info("scene_scope::scene_scope with Tag = {} called!", p_tag);

    //! @note Creating our objects from our scene
    m_sphere = this->create_new_object("sphere");

    m_sphere->set<atlas::rendertarget3d>(
      atlas::rendertarget3d("assets/models/colored_cube.obj"));

    //! @note Sets the sphere to it's default size
    m_sphere->set<atlas::transform>({ .Position = { 0.f, 2.10f, -7.30f },
                                      .Scale = { .20f, .20f, .20f },
                                      .Color = { 1.0f, 1.f, 1.f, 1.f } });

    m_camera = this->create_new_object("camera");
    m_camera->add<atlas::camera>();


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
        camera->process_keyboard(atlas::Forward, delta_time);
    }
    if (atlas::event::is_key_pressed(key_s)) {
        camera->process_keyboard(atlas::Backward, delta_time);
    }
    if (atlas::event::is_key_pressed(key_a)) {
        camera->process_keyboard(atlas::Left, delta_time);
    }
    if (atlas::event::is_key_pressed(key_d)) {
        camera->process_keyboard(atlas::Right, delta_time);
    }
    if (atlas::event::is_key_pressed(key_q)) {
        camera->process_keyboard(atlas::Up, delta_time);
    }
    if (atlas::event::is_key_pressed(key_e)) {
        camera->process_keyboard(atlas::Down, delta_time);
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
