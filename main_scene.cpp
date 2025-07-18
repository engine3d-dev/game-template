#include "main_scene.hpp"
#include <core/event/event.hpp>
#include <core/scene/components.hpp>
#include <core/ui/widgets.hpp>
#include <core/utilities/state.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>
#include <physics/physics_3d/physics.hpp>
#include <assets/scripts/experimental/data_management/schema_manager.hpp>

main_scene::main_scene(const std::string &p_tag) : atlas::scene_scope(p_tag) {

  m_physics_settings = this->create_object("Physics Settings");
  m_physics_settings->add<atlas::physics::jolt_settings>();
  m_physics_settings->add<atlas::physics::jolt_config>();

  atlas::register_update(this, &main_scene::on_update);
  atlas::register_physics(this, &main_scene::on_physics_update);
  atlas::register_ui(this, &main_scene::on_ui_update);
  atlas::register_start(this, &main_scene::start);
}

void main_scene::start() {

}

void main_scene::on_update() {
  
}

void main_scene::on_ui_update() {

}

void main_scene::runtime_start() {

}

void main_scene::runtime_stop() {
  
}

void main_scene::on_physics_update() {

}
