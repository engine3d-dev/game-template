#pragma once
#include <core/scene/scene.hpp>
#include <assets/data/camera/camera_data.hpp>
#include <imgui.h>

class ui_hierarchy {
public:
  ui_hierarchy() = delete;
  ui_hierarchy(atlas::strong_ref<flecs::world>& p_registery, flecs::entity& p_selected_entity);
    // change
  ~ui_hierarchy();

  void on_ui_update();

private:
  void draw_entity_tree(flecs::entity p_entity);

  atlas::optional_ref<atlas::scene_scope> m_current_scene = nullptr;

  flecs::entity& m_selected_entity;
  atlas::optional_ref<flecs::world> m_registery = nullptr;
  flecs::query<> m_root_query;
  std::unordered_map<flecs::entity_t, ImVec2> m_entity_positions;
};