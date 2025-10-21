#include "game_world.hpp"
#include <core/engine_logger.hpp>
#include <core/system/registry.hpp>
#include <core/event/types.hpp>

game_world::game_world() {
    console_log_fatal("Instantiate Default game_world!");
}

game_world::game_world(const std::string& p_tag) {
    m_main_world = atlas::system_registry::create_world(p_tag);

    m_bus.create_listener<atlas::event::collision_enter>();
    m_bus.create_listener<atlas::event::collision_persisted>();
    m_bus.create_listener<atlas::event::collision_exit>();

    m_first_scene = atlas::create_ref<main_scene>("LevelScene", m_bus);
    m_main_world->add_scene(m_first_scene);
}