#pragma once

#include <flecs.h>
#include <imgui.h>
#include <core/ui/widgets.hpp>
#include <core/core.hpp>

namespace atlas {
    // change
class ui_properties_panel {
public:
    ui_properties_panel() = delete;
    ui_properties_panel(atlas::strong_ref<flecs::world>& p_world, flecs::entity& p_selected_entity);

    void on_ui_update();

private:
    // Draw each reflected component (e.g. transform, physics_body, etc.)
    void draw_component(flecs::entity component_entity, void* component_data);

    // Draw individual reflected members using offset info from Flecs::Member
    void draw_member(const flecs::entity& member_name, const flecs::Member* member, void* member_ptr);

    optional_ref<flecs::world> m_world = nullptr;
    flecs::entity& m_selected_entity;

    std::unordered_map<flecs::id_t, std::function<void(const std::string&, void*)>> m_type_drawers;
};

} // namespace atlas