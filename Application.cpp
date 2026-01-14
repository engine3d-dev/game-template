import atlas.application;
import atlas.common;

import game_world;
import atlas.core.scene.system_registry;

class editor_application : public atlas::application {
public:
    editor_application(const atlas::application_settings& p_settings) : application(p_settings) {
        m_world = atlas::create_ref<game_world>("Editor World", renderer_instance());
    }

private:
    atlas::ref<game_world> m_world;
};

atlas::ref<atlas::application> initialize_application() {
    atlas::application_settings settings = {
        .name = "game-template",
        .width = 1200,
        .height = 800,
        .background_color = {0.f, 0.7f, 1.f, 1.f}
    };

    return atlas::create_ref<editor_application>(settings);
}