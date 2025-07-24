#include "game_world.hpp"
#include <core/engine_logger.hpp>
#include <core/system/registry.hpp>

game_world::game_world() {
    console_log_fatal("Instantiate Default game_world!");
}

game_world::game_world(const std::string& p_tag) {
    // m_main_world = atlas::world_scope(p_tag);
    m_main_world = atlas::system_registry::create_world(p_tag);

    console_log_trace("m_main_world->get_tag() = {}", m_main_world->name());
    atlas::strong_ref<text_editor> editor = atlas::create_strong_ref<text_editor>(m_allocator);
    auto lang = text_editor::LanguageDefinition::CPlusPlus();
    editor->SetLanguageDefinition(lang);
    editor->GetDarkPalette();
    m_editor = editor;
    m_first_scene = atlas::create_ref<main_scene>("LevelScene", editor);
    m_main_world->add_scene(m_first_scene);
}