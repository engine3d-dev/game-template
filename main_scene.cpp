#include "main_scene.hpp"
#include <assets/data/modules/core/core_module.hpp>
#include <assets/modules/blaze_draco_editor/text_editor.hpp>
#include <core/event/event.hpp>
#include <core/filesystem/file_dialog.hpp>
#include <core/scene/components.hpp>
#include <core/ui/widgets.hpp>
#include <core/utilities/state.hpp>
#include <filesystem>
#include <physics/jolt-cpp/jolt_components.hpp>
#include <physics/physics_3d/physics.hpp>

main_scene::main_scene(const std::string &p_tag,
                       const atlas::strong_ref<text_editor> p_editor)
    : atlas::scene_scope(p_tag) {
  atlas::strong_ref<flecs::world> scene = *this;
  scene->import <core_components_module>();

  m_editor = p_editor;

  ecs_script_run_file(*scene, "assets/flecs_scripting/buildings.flecs");

  // test_schema =
  //     atlas::create_strong_ref<schema>(m_allocator_object, "test_schema");

  // Serializer is created and activated last.
  m_serializer = atlas::create_strong_ref<serializer>(m_allocator_object);
  /*
   * This checks if the serialization needs to create a new json file or if it
   * exsists than it should load that json file. As of right now there is no way
   * to keep track of where that path is. So for it is stored based on scene
   * tag.
   *
   */
  std::string location = "assets/scenes/" + this->get_tag() + ".json";
  std::ifstream file_check(location);
  if (file_check.good()) {
    file_check.close();

    m_serializer->deserialize_scene(location, *scene);

  } else {

    m_serializer->serialize_scene(location, *scene);
  }

  m_query = scene
                ->query_builder<atlas::transform, atlas::perspective_camera,
                                editor_controls>()
                .build();

  atlas::register_update(this, &main_scene::on_update);
  atlas::register_physics(this, &main_scene::on_physics_update);
  atlas::register_ui(this, &main_scene::on_ui_update);
  atlas::register_start(this, &main_scene::start);
}

void main_scene::start() {
  atlas::strong_ref<flecs::world> registry = *this;

  m_ui_hierarchy = atlas::create_strong_ref<ui_hierarchy>(
      m_allocator_object, registry, selected_entity);
  m_ui_properties = atlas::create_strong_ref<atlas::ui_properties_panel>(
      m_allocator_object, registry, selected_entity);
}

void main_scene::on_update() {
  float delta_time = atlas::application::delta_time();
  atlas::strong_ref<flecs::world> registry = *this;
  registry->defer_begin();

  m_query.each([&](flecs::entity p_entity, atlas::transform camera_transform,
                   atlas::perspective_camera camera_comp,
                   editor_controls p_controls) {
    if (!atlas::event::is_key_pressed(key_left_control)) {
      glm::quat orientation = glm::quat(
          camera_transform.quaternion.w, camera_transform.quaternion.x,
          camera_transform.quaternion.y, camera_transform.quaternion.z);

      glm::vec3 forward = glm::rotate(orientation, glm::vec3(0.f, 0.f, -1.f));
      glm::vec3 right = glm::rotate(orientation, glm::vec3(1.f, 0.f, 0.f));
      glm::vec3 world_up = glm::vec3(0.f, 1.f, 0.f);

      float velocity = p_controls.movement_speed * delta_time;

      if (atlas::event::is_key_pressed(p_controls.forward)) {
        camera_transform.position += forward * velocity;
      }
      if (atlas::event::is_key_pressed(p_controls.backward)) {
        camera_transform.position -= forward * velocity;
      }
      if (atlas::event::is_key_pressed(p_controls.left)) {
        camera_transform.position -= right * velocity;
      }
      if (atlas::event::is_key_pressed(p_controls.right)) {
        camera_transform.position += right * velocity;
      }
      if (atlas::event::is_key_pressed(p_controls.down)) {
        camera_transform.position -= world_up * velocity;
      }
      if (atlas::event::is_key_pressed(p_controls.up)) {
        camera_transform.position += world_up * velocity;
      }

      if (atlas::event::is_mouse_pressed(mouse_button_1)) {
        glm::vec2 cursor_pos = atlas::event::cursor_position();
        if (!p_controls.on_click_check) {
          p_controls.last_cursor_pos = cursor_pos;
          p_controls.on_click_check = true;
        }

        glm::vec2 offset = cursor_pos - p_controls.last_cursor_pos;
        glm::vec2 mouse_velocity = offset * p_controls.mouse_sensitivity;
        p_controls.last_cursor_pos = cursor_pos;

        p_controls.yaw -= mouse_velocity.x * (int)p_controls.x;

        if (p_controls.yaw <= 0) {
          p_controls.yaw += 360;
        }
        if (p_controls.yaw > 360.1) {
          p_controls.yaw -= 360;
        }

        p_controls.pitch -= mouse_velocity.y * (int)p_controls.y;

        p_controls.pitch = std::clamp(p_controls.pitch, -89.9f, 89.9f);
        glm::quat yaw_q = glm::angleAxis(glm::radians(p_controls.yaw),
                                         glm::vec3(0.f, 1.f, 0.f));
        glm::quat pitch_q = glm::angleAxis(glm::radians(p_controls.pitch),
                                           glm::vec3(1.f, 0.f, 0.f));

        glm::quat target_orientation = yaw_q * pitch_q;

        glm::quat current_orientation = glm::quat(
            camera_transform.quaternion.w, camera_transform.quaternion.x,
            camera_transform.quaternion.y, camera_transform.quaternion.z);

        glm::quat smoothed = glm::slerp(current_orientation, target_orientation,
                                        delta_time * p_controls.rotation_speed);
        smoothed = glm::normalize(smoothed);

        camera_transform.quaternion =
            glm::vec4(smoothed.x, smoothed.y, smoothed.z, smoothed.w);

      } else {
        p_controls.on_click_check = false;
      }

      p_entity.set<atlas::transform>(camera_transform);
      p_entity.set<atlas::perspective_camera>(camera_comp);
      p_entity.set<editor_controls>(p_controls);
    }
  });

  registry->defer_end();
}

void main_scene::on_ui_update() {
  if (ImGui::Begin("ViewPort")) {

    glm::vec2 viewport_panel_size =
        glm::vec2{atlas::application::get_window().width(),
                  atlas::application::get_window().height()};

    if (ImGui::IsWindowFocused()) {
      if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_S)) {
        std::string location = "assets/scenes/" + this->get_tag() + ".json";
        atlas::strong_ref<flecs::world> scene = *this;
        m_serializer->serialize_scene(location, *scene);
      }
    }
    ImGui::End();
  }

  if (ImGui::Begin("Text Editor")) {
    float header_bar_height = ImGui::GetFrameHeightWithSpacing() + 4.0f;
    ImGui::BeginChild("HeaderBar", ImVec2(0, header_bar_height), true,
                      ImGuiWindowFlags_NoScrollbar);

    if (ImGui::Button("New")) {
      m_editor->SetText("");
      m_current_filename = "n/a";
    }

    ImGui::SameLine();

    if (ImGui::Button("Open")) {

      std::string selected_path =
          atlas::filesystem::load_from_file_dialog("chai;flecs");

      if (!selected_path.empty()) {
        std::ifstream file_stream(selected_path);

        if (!file_stream.is_open()) {
          console_log_error("Could not open file!");
          return;
        }

        std::stringstream buffer;
        buffer << file_stream.rdbuf();

        m_editor->SetText(buffer.str());
        m_current_filename = selected_path;
      }
    }

    ImGui::SameLine();

    ImGui::Text("File: %s", m_current_filename.c_str());

    ImGui::EndChild();

    auto editor_size = ImGui::GetContentRegionAvail();
    m_editor->Render("TextEditorWidget", editor_size);

    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) {
      if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_S)) {
        if (m_current_filename == "n/a") {

          const char *filter = "chai;flecs";

          std::string result = atlas::filesystem::save_to_file(filter);

          if (result == "") {
            return;
          }
          m_current_filename = result;
        }

        if (m_current_filename != "" && m_current_filename != "n/a") {
          std::string editor_content = m_editor->GetText();
          {
            std::ofstream file_stream(m_current_filename);
            if (file_stream.is_open()) {
              file_stream << editor_content;
            }
          }
          atlas::strong_ref<flecs::world> scene = *this;
          ecs_script_run_file(*scene, m_current_filename.c_str());
        }
      }
    }
    ImGui::End();
  }
}

void main_scene::runtime_start() {}

void main_scene::runtime_stop() {}

void main_scene::on_physics_update() {}
