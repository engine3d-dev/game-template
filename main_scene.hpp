#pragma once
#include <core/core.hpp>
#include <core/scene/scene.hpp>
#include <core/scene/scene_object.hpp>
#include <string>
#include <vector>
#include <core/serialize/serializer.hpp>
#include <physics/physics_engine.hpp>
#include <core/event/event_bus.hpp>
#include <core/event/types.hpp>

/**
 * @name main_scene
 * @brief Implementation of a custom scene that contains game objects
 * 
 * main_scene acts as the first scene created that is associated with the world.
 * 
 */
class main_scene : public atlas::scene_scope {
public:
    main_scene(const std::string& p_tag, atlas::event::event_bus& p_bus);

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


    void collision_enter(atlas::event::collision_enter& p_event);


private:
    // TODO: Will implement scene management system to coordinate with physics system
    //      for starting and stopping the physics runtime
    void runtime_start();
    void runtime_stop();

    // All this does is resets all of the game objects back to their initial starting positions
    // NOTE: Typically re-serialization would occur in replacement of this
    void reset_objects();

private:
    atlas::optional_ref<atlas::scene_object> m_viking_room;
    atlas::optional_ref<atlas::scene_object> m_platform;
    
    atlas::optional_ref<atlas::scene_object> m_sphere;
    atlas::optional_ref<atlas::scene_object> m_box;

    atlas::optional_ref<atlas::scene_object> m_camera;
    atlas::physics::physics_engine m_physics_engine_handler;

    bool m_blink_text=false;
    glm::vec3 m_offset_from_camera;

    bool m_physics_is_runtime=false;
};