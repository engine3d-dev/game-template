#pragma once
// #define ATLAS_DEBUG_BUILD
#include "assets/data/camera/editor_controls.hpp"
#include <core/core.hpp>
#include <core/scene/scene.hpp>
#include <core/scene/scene_object.hpp>
#include <assets/modules/serialization/serializer.hpp>
#include <physics/physics_3d/physics_engine.hpp>
#include <assets/scripts/gui/ui_hierarchy.hpp>
#include <assets/scripts/gui/ui_properties_panel.hpp>
#include <assets/modules/blaze_draco_editor/text_editor.hpp>

/**
 * @name LevelScene
 * @note Implementation of our scene
 * @note Meaning this is just the first iteration of how we can look at these
 * scenes and how they can be defined
 * @note Each scene is a type of scene scope, this means that when a player
 * leaves the outer scope of a scene, the player transitions into a different
 * scene
 * @note Either the player transitions into a different scene or hits a
 * invisible wall preventing leaving the scope
 */

class main_scene : public atlas::scene_scope {
public:
    main_scene(const std::string& p_tag, const atlas::strong_ref<text_editor> p_editor);

    ~main_scene() override {
    }

    //! @note Remove these from being overridden
    //! TODO: We should have an indication on what functions are update phased
    //! functions
    void start();
    void init();
    void on_update();

    void on_ui_update();

    // Temporary
    void runtime_start();
    void runtime_stop();

    //! TODO: Remove this and integrate a few functions into LevelScene such as
    //! on_runtime_start/on_runtime_stop
    void on_physics_update();

private:

    struct text_filter {
        std::string name = "";
    };

    atlas::optional_ref<text_editor> m_editor;

    atlas::optional_ref<atlas::scene_object> m_test_object = nullptr;
    atlas::optional_ref<atlas::scene_object> m_platform = nullptr;
    atlas::optional_ref<atlas::scene_object> m_platform2 = nullptr;
    atlas::optional_ref<atlas::scene_object> m_camera = nullptr;
    atlas::optional_ref<atlas::scene_object> m_physics_settings = nullptr;

    atlas::optional_ref<atlas::ui_properties_panel> m_ui_properties = nullptr;

    atlas::optional_ref<ui_hierarchy> m_ui_hierarchy = nullptr;

    atlas::optional_ref<serializer> m_serializer = nullptr;

    std::pmr::polymorphic_allocator<> m_allocator_object;

    flecs::query<atlas::transform, atlas::perspective_camera, editor_controls>
      m_query;

    flecs::entity selected_entity = flecs::entity::null();

    std::string m_current_filename = "";

    bool is_runtime = false;
    bool test_bool = false;
};