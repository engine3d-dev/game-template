#include <assets/data/camera/editor_controls.hpp>
#include <core/core.hpp>
#include <core/scene/components.hpp>
#include <flecs.h>
#include <glm/glm.hpp>

struct core_primitives_module {
  core_primitives_module(flecs::world &p_world) {
    p_world.component<int>("::int");
    p_world.component<bool>("::bool");

    auto opaque_serializer =
        p_world.component<std::string>()
            .opaque(flecs::String)
            .serialize([](const flecs::serializer *s, const std::string *data) {
              const char *str = data->c_str();
              return s->value(flecs::String, &str);
            })
            .assign_string(
                [](std::string *data, const char *value) { *data = value; });

    p_world.component<glm::vec2>("::glm_vec2")
        .member("x", &glm::vec2::x)
        .member("y", &glm::vec2::y);

    p_world.component<glm::vec3>("::glm_vec3")
        .member("x", &glm::vec3::x)
        .member("y", &glm::vec3::y)
        .member("z", &glm::vec3::z);

    p_world.component<glm::vec4>("::glm_vec4")
        .member("x", &glm::vec4::x)
        .member("y", &glm::vec4::y)
        .member("z", &glm::vec4::z)
        .member("w", &glm::vec4::w);

    p_world.component<glm::vec3>("::glm_highp_vec3")
        .member("x", &glm::highp_vec3::x)
        .member("y", &glm::highp_vec3::y)
        .member("z", &glm::highp_vec3::z);

    p_world.component<glm::vec4>("::glm_highp_vec4")
        .member("x", &glm::highp_vec4::x)
        .member("y", &glm::highp_vec4::y)
        .member("z", &glm::highp_vec4::z)
        .member("w", &glm::highp_vec4::w);
  }
};

struct core_components_module {

  core_components_module(flecs::world &p_world) {
    p_world.entity("::user");
    p_world.entity("::object");

    // Create dependecies inside modules like this
    p_world.import <core_primitives_module>();

    // These are components, that allow us to define structre for reflection and
    // entities in general.
    p_world.component<atlas::transform>("::transform")
        .member("position", &atlas::transform::position)
        .member("rotation", &atlas::transform::rotation)
        .member("scale", &atlas::transform::scale);

    p_world.component<atlas::perspective_camera>("::Perspective Camera")
        .member("planes", &atlas::perspective_camera::plane)
        .member("fov", &atlas::perspective_camera::field_of_view)
        .member("active", &atlas::perspective_camera::is_active)
        .member("target", &atlas::perspective_camera::target);

    p_world.component<editor_controls>("::Camera Controls")
        .member("down", &editor_controls::down)
        .member("up", &editor_controls::up)
        .member("left", &editor_controls::left)
        .member("right", &editor_controls::right)
        .member("forward", &editor_controls::forward)
        .member("backward", &editor_controls::backward)
        .member("player", &editor_controls::player)
        .member("sensitivity", &editor_controls::mouse_sensitivity)
        .member("pitch", &editor_controls::pitch)
        .member("yaw", &editor_controls::yaw)
        .member("speed", &editor_controls::movement_speed)
        .member("speed", &editor_controls::rotation_speed);

    p_world.component<atlas::material>("::material")
        .member("color", &atlas::material::color)
        .member<std::string>("model_path")
        .member<std::string>("texture_path");

    p_world.component<atlas::projection_view>("::projection_view");
  }
};