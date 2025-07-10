#pragma once
#include <flecs.h>
#include <core/scene/components.hpp>
#include <assets/modules/action_components/requirements.hpp>

class rigid_body {
public:
  rigid_body() = delete;
  rigid_body(flecs::world &p_registery);

  void attach_entity(flecs::entity &p_entity, flecs::world &p_registery);

  void on_update();

private:
  atlas::requirements m_requirements;
  struct rigid_body_action {};

  flecs::world m_registery;
  flecs::query<atlas::transform> m_query;
};