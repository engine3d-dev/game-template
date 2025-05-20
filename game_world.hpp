#pragma once
#include "main_scene.hpp"

class game_world {
public:
    game_world();
    game_world(const std::string& p_tag);

private:
    atlas::ref<atlas::world_scope> m_main_world;
    atlas::ref<main_scene> m_first_scene;
};