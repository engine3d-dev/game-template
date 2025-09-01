#pragma once
#include <core/core.hpp>
#include <core/scene/scene.hpp>
#include <core/scene/scene_object.hpp>
#include <physics/physics_3d/physics_engine.hpp>
#include <string>
#include <vector>
#include <core/serialize/serializer.hpp>

/**
 * @name main_scene
 * @brief Implementation of a custom scene that contains game objects
 * 
 * main_scene acts as the first scene created that is associated with the world.
 * 
 */
class main_scene : public atlas::scene_scope {
public:
    main_scene(const std::string& p_tag);

    ~main_scene()  = default;


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
    // TODO: Will implement scene management system to coordinate with physics system
    //      for starting and stopping the physics runtime
    void runtime_start();
    void runtime_stop();

    // All this does is resets all of the game objects back to their initial starting positions
    // NOTE: Typically re-serialization would occur in replacement of this
    void reset_objects();

private:
    atlas::serializer m_deserializer_test;
    atlas::optional_ref<atlas::scene_object> m_viking_room;
    atlas::optional_ref<atlas::scene_object> m_platform;
    atlas::optional_ref<atlas::scene_object> m_giant_platform;
    
    atlas::optional_ref<atlas::scene_object> m_sphere;
    atlas::optional_ref<atlas::scene_object> m_box;

    atlas::optional_ref<atlas::scene_object> m_camera;
    std::vector<std::vector<std::vector<atlas::strong_ref<atlas::scene_object>>>> m_grouped_spheres;
    atlas::optional_ref<atlas::scene_object> m_giant_cube1;
    atlas::optional_ref<atlas::scene_object> m_giant_cube2;

    // Pre-setup that should be handled by atlas::physics_system
    atlas::optional_ref<atlas::scene_object> m_physics_object_representation_of_settings;
    atlas::optional_ref<atlas::physics::physics_engine> m_physics_engine_handler;

    // Should be hidden from initializing the physics system entirely
    std::pmr::polymorphic_allocator<> m_physics_system_allocator;
    flecs::entity m_selected_entity;

    bool m_blink_text=false;
    glm::vec3 m_offset_from_camera;

    bool m_physics_is_runtime=false;
};