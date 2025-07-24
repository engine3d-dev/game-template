#include "ui_properties_panel.hpp"
#include <core/utilities/state.hpp>
#include <core/engine_logger.hpp>

namespace atlas {
    //change

ui_properties_panel::ui_properties_panel(atlas::strong_ref<flecs::world>& p_world,
                                         flecs::entity& p_selected_entity)
    : m_selected_entity(p_selected_entity) {
  m_world = p_world;

  // Initialize lambda map
  m_type_drawers[m_world->component<float>().id()] = [](const std::string& label, void* ptr) {
    atlas::ui::draw_float(label, *reinterpret_cast<float*>(ptr));
  };

  m_type_drawers[m_world->component<int>().id()] = [](const std::string& label, void* ptr) {
    int& val = *reinterpret_cast<int*>(ptr);
    ImGui::InputInt(label.c_str(), &val);
  };

  m_type_drawers[m_world->component<uint32_t>().id()] = [](const std::string& label, void* ptr) {
    uint32_t& val = *reinterpret_cast<uint32_t*>(ptr);
    ImGui::InputScalar(label.c_str(), ImGuiDataType_U32, &val);
  };

  m_type_drawers[m_world->component<bool>().id()] = [](const std::string& label, void* ptr) {
    bool& val = *reinterpret_cast<bool*>(ptr);
    ImGui::Checkbox(label.c_str(), &val);
  };

  m_type_drawers[m_world->component<glm::vec3>().id()] = [](const std::string& label, void* ptr) {
    atlas::ui::draw_vec3(label, *reinterpret_cast<glm::vec3*>(ptr));
  };

  m_type_drawers[m_world->component<glm::highp_vec3>().id()] = [](const std::string& label, void* ptr) {
    atlas::ui::draw_vec3(label, *reinterpret_cast<glm::highp_vec3*>(ptr));
  };

  m_type_drawers[m_world->component<glm::vec4>().id()] = [](const std::string& label, void* ptr) {
    atlas::ui::draw_vec4(label, *reinterpret_cast<glm::vec4*>(ptr));
  };

  m_type_drawers[m_world->component<glm::highp_vec4>().id()] = [](const std::string& label, void* ptr) {
    atlas::ui::draw_vec4(label, *reinterpret_cast<glm::highp_vec4*>(ptr));
  };

  atlas::register_ui(this, &ui_properties_panel::on_ui_update);
}

void ui_properties_panel::on_ui_update() {
  if (ImGui::Begin("Properties Panel")) {
    if (m_selected_entity.is_valid()) {
      m_selected_entity.each([&](flecs::id id) {
        if (id.is_pair()) return;

        flecs::entity component_type = id.entity();
        if (!component_type || !component_type.has<flecs::Struct>()) return;

        void* comp_ptr = m_selected_entity.get_mut(id);
        if (!comp_ptr) return;

        const std::string label = component_type.name().c_str();
        ImGui::PushID(id);
        atlas::ui::draw_panel_component<void>(label, [&]() {
          draw_component(component_type, comp_ptr);
        });
        ImGui::PopID();
      });
    }
  }
  ImGui::End();
}

void ui_properties_panel::draw_component(flecs::entity component_entity, void* component_ptr) {
  component_entity.children([&](flecs::entity member_entity) {
    if (member_entity.has<flecs::Member>()) {
      auto* member = member_entity.get<flecs::Member>();
      draw_member(member_entity, member, component_ptr);
    }
  });
}

void ui_properties_panel::draw_member(const flecs::entity& member_entity,
                                      const flecs::Member* member,
                                      void* base_ptr) {
  if (!member || !member->use_offset || !base_ptr) {
    ImGui::Text("%s: <invalid member>", member_entity.name().c_str());
    return;
  }

  void* member_ptr = static_cast<char*>(base_ptr) + member->offset;
  const std::string label = member_entity.name().c_str();
  flecs::id_t type_id = member->type;

  if (m_type_drawers.contains(type_id)) {
    m_type_drawers.at(type_id)(label, member_ptr);
  } else {
    ImGui::Text("%s: <unsupported type id=%llu>", label.c_str(),
                static_cast<unsigned long long>(type_id));
  }
}

} // namespace atlas