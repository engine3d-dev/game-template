module;

#include <string>

export module game_world;

// import atlas.common;
// import atlas.logger;
import atlas.core.utilities;
import atlas.core.scene.world;
// import atlas.core.event.bus;
// import atlas.core.event.types;
import atlas.core.event;
import atlas.core.scene.system_registry;
import atlas.drivers.renderer_system;
import main_scene;

export class game_world {
public:
    game_world(const std::string& p_tag, atlas::ref<atlas::renderer_system> p_renderer_instance) : m_renderer(p_renderer_instance) {
        m_main_world = atlas::system_registry::create_world(p_tag);

        m_bus.create_listener<atlas::event::collision_enter>();
        m_bus.create_listener<atlas::event::collision_persisted>();
        m_bus.create_listener<atlas::event::collision_exit>();

        atlas::ref<main_scene> first_scene = atlas::create_ref<main_scene>("LevelScene", m_bus);
        m_renderer->current_scene_context(first_scene);
        m_main_world->add_scene(first_scene);

        console_log_error("editor_world initialized successfully!~!!");
    }

private:
    atlas::ref<atlas::world> m_main_world;

    atlas::event::bus m_bus;
    atlas::ref<atlas::renderer_system> m_renderer;
};