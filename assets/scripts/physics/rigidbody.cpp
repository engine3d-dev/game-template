#include "rigidbody.hpp"
#include <core/utilities/state.hpp>

rigid_body::rigid_body(flecs::world &p_registery) : m_registery(p_registery) {

  m_query = p_registery.query_builder<atlas::transform>()
                .with<atlas::requirements::runs>()
                .second<rigid_body_action>()
                .build();

  m_requirements.add<flecs::pair<atlas::requirements::runs, rigid_body_action>>(p_registery);

  // More data requirements go here:
  // EX: m_requirements.add<atlas::transform>(p_registery);

  atlas::register_update(this, &rigid_body::on_update);

  // Add more functions here:
}

void rigid_body::attach_entity(flecs::entity &p_entity, flecs::world &p_registery) {
  if (!m_requirements.does_exist(p_entity, p_registery,
                                 p_registery.id<rigid_body_action>())) {
    m_requirements.apply(p_entity);
  }
}

void rigid_body::on_update() {
  m_query.each([](flecs::entity p_entity, atlas::transform &p_transform) {
    // Actions With Data, Go Here...

    /* EX:(Moves the x position 1 unit for each object attached to this action)
    *  glm::vec3 position = p_transform.position;
    *  position.x += 1;
    *  transform.position = position;
    */
  });
}