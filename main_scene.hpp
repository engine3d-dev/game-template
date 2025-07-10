#pragma once
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


    void start_game();

    //! @note Remove these from being overridden
    //! TODO: We should have an indication on what functions are update phased
    //! functions
    void on_update();

    void on_ui_update();

    //! TODO: Remove this and integrate a few functions into LevelScene such as
    //! on_runtime_start/on_runtime_stop
    void on_physics_update();


private:
    // TEMPORARY Functions for handling runtime
    void runtime_start();
    void runtime_stop();

private:
    atlas::optional_ref<atlas::scene_object> m_viking_room;
    atlas::optional_ref<atlas::scene_object> m_platform;
    
    atlas::optional_ref<atlas::scene_object> m_sphere;
    atlas::optional_ref<atlas::scene_object> m_box;

    atlas::optional_ref<atlas::scene_object> m_camera;
    atlas::optional_ref<atlas::scene_object> m_physics_object_representation_of_settings;
    atlas::optional_ref<atlas::physics::physics_engine> m_physics_engine_handler;
    std::pmr::polymorphic_allocator<> m_allocator;
    bool m_physics_is_runtime=false;
};