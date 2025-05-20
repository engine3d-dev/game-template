#include <core/application.hpp>
#include "game_world.hpp"

class editor_application : public atlas::application {
public:
    editor_application(const atlas::application_settings& p_settings)
      : application(p_settings) {
        m_world = atlas::create_ref<game_world>("Editor World");
    }

private:
    atlas::ref<game_world> m_world;
};

namespace atlas {
    ref<application> initialize_application() {
        application_settings settings = {
            .Name = "game-template",
            .Width = 1200,
            .Height = 800,
        };

        return create_ref<editor_application>(settings);
    }

} // end of namespace atlas