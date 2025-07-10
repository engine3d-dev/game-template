#pragma once
// #define ATLAS_DEBUG_BUILD
#include <core/core.hpp>
#include <core/scene/scene.hpp>
#include <core/scene/scene_object.hpp>
#include <physics/physics_3d/physics_engine.hpp>

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
    main_scene(const std::string& p_tag);

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
    atlas::optional_ref<atlas::scene_object> m_viking_room;
    atlas::optional_ref<atlas::scene_object> m_platform;
    atlas::optional_ref<atlas::scene_object> m_platform2;
    atlas::optional_ref<atlas::scene_object> m_camera;
    atlas::optional_ref<atlas::scene_object> m_physics_settings;
    std::vector<std::vector<std::vector<atlas::strong_ref<atlas::scene_object>>>> m_spheres;

    atlas::optional_ref<atlas::physics::physics_engine> engine;

    std::pmr::polymorphic_allocator<> m_allocator_object;

    bool is_runtime = false;
    bool test_bool = false;
};