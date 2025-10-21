#pragma once
#include "main_scene.hpp"
#include <core/scene/world.hpp>
#include <core/event/event_bus.hpp>

class game_world {
public:
    game_world();
    game_world(const std::string& p_tag);

private:
    atlas::ref<atlas::world_scope> m_main_world;
    atlas::ref<main_scene> m_first_scene;
    atlas::event::event_bus m_bus;
};