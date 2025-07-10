#include "main_scene.hpp"
#include "assets/data/camera/editor_controls.hpp"
#include <core/event/event.hpp>
#include <core/scene/components.hpp>
#include <core/ui/widgets.hpp>
#include <core/utilities/state.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>
#include <physics/physics_3d/physics.hpp>

main_scene::main_scene(const std::string &p_tag) : atlas::scene_scope(p_tag) {

  m_physics_settings = this->create_object("Physics Settings");
  m_physics_settings->add<atlas::physics::jolt_settings>();
  m_physics_settings->add<atlas::physics::jolt_config>();

  m_spheres.reserve(10);
  for (int x = 0; x < 1; x++) {
    m_spheres.emplace_back(); 
    m_spheres[x].reserve(10);

    for (int y = 0; y < 3; y++) {
      m_spheres[x].emplace_back();
      m_spheres[x][y].reserve(10);

      for (int z = 0; z < 3; z++) {
        std::string name = "Ball " + std::to_string(x) + "_" +
                           std::to_string(y) + "_" + std::to_string(z);

        auto obj = this->create_object(name);
        m_spheres[x][y].emplace_back(obj); 

        obj->set<atlas::physics::physics_body>(
            {.restitution = 1.25f,
             .body_movement_type = atlas::physics::Dynamic});

        obj->set<atlas::physics::collider_body>(
            {.shape_type = atlas::physics::collider_shape::Sphere,
             .radius = 1.0f});

        glm::vec3 pos = {float(x*1.4), float(y * 1.4),
                         float(z * -3)};

        obj->set<atlas::transform_physics>(
            {.position = pos, .rotation = {.3f, 0.0f, 0.0f}});

        obj->set<atlas::transform>(
            {.Position = pos, .Rotation = {.3f, 0.0f, 0.0f}});

        obj->set<atlas::material>({.model_path = "assets/models/Ball OBJ.obj",
                                   .texture_path = "assets/models/clear.png"});
      }
    }
  }

  m_camera = create_object("camera");
  auto aspect_ratio = atlas::application::aspect_ratio();
  atlas::camera camera_comp = atlas::camera(aspect_ratio);
  camera_comp.Position = {-1.1f, 6.53f, 23.32f};
  camera_comp.IsMainCamera = true;

  m_camera->set<atlas::camera>(camera_comp);

  m_camera->set<editor_controls>({});
  //! @note Creating our objects from our scene
  m_viking_room = create_object("Viking Room Object");
  atlas::transform sphere_transform = {
      // .Position{0.f, 0.f, 1.60f},
      .Position = {-2.70f, 2.70, -8.30f},
      .Rotation = {2.30f, 95.90f, 91.80f},
      .Scale{1.f},
  };
  m_viking_room->set<atlas::transform>(sphere_transform);

  m_platform = create_object("Platform");

  m_platform->set<atlas::transform>(
      {.Position = {0.f, -20.00f, 0.4f}, .Rotation = {4.0, 0, 0}});
  auto plat_transform = *m_platform->get<atlas::transform>();
  plat_transform.Scale = {15.f, 15.0f, 15.0f};
  auto rot = glm::quat{plat_transform.Rotation};
  plat_transform.QuaternionRotation = {rot.x, rot.y, rot.z, rot.w};
  m_platform->set<atlas::transform>(plat_transform);

  m_platform->set<atlas::transform_physics>(
      {.position = {0.f, -20.00f, 0.4f}, .rotation = {4.0, 0, 0}});

  auto plat_transform_ph = *m_platform->get<atlas::transform_physics>();
  plat_transform_ph.scale = {15.f, 15.f, 15.0f};
  rot = glm::quat{plat_transform_ph.rotation};
  plat_transform_ph.quaterion_rotation = {rot.x, rot.y, rot.z, rot.w};
  m_platform->set<atlas::transform_physics>(plat_transform_ph);

  m_platform->set<atlas::physics::collider_body>(
      {.half_extents = {15.f, 15.0f, 15.0f}});

  m_platform->set<atlas::material>(
      {.color = {1.f, 1.f, 1.f, 1.f},
       .model_path = "assets/models/cube.obj",
       .texture_path = "assets/models/wallace_gromit_image.jpg"});

  m_platform2 = create_object("Platform 2");

  m_platform2->set<atlas::transform>(
      {.Position = {0.f, -20.00f, -50.4f}, .Rotation = {4.0, 0, 0}});
  plat_transform = *m_platform2->get<atlas::transform>();
  plat_transform.Scale = {15.f, 15.0f, 15.0f};
  rot = glm::quat{plat_transform.Rotation};
  plat_transform.QuaternionRotation = {rot.x, rot.y, rot.z, rot.w};
  m_platform2->set<atlas::transform>(plat_transform);

  m_platform2->set<atlas::transform_physics>(
      {.position = {0.f, -20.00f, -50.4f}, .rotation = {7.0, 0, 0}});

  plat_transform_ph = *m_platform2->get<atlas::transform_physics>();
  plat_transform_ph.scale = {15.f, 15.f, 15.0f};
  rot = glm::quat{plat_transform_ph.rotation};
  plat_transform_ph.quaterion_rotation = {rot.x, rot.y, rot.z, rot.w};
  m_platform2->set<atlas::transform_physics>(plat_transform_ph);

  m_platform2->set<atlas::physics::collider_body>(
      {.half_extents = {15.f, 15.0f, 15.0f}});

  m_platform2->set<atlas::material>(
      {.color = {1.f, 1.f, 1.f, 1.f},
       .model_path = "assets/models/cube.obj",
       .texture_path = "assets/models/wallace_gromit_image.jpg"});

  atlas::register_update(this, &main_scene::on_update);
  atlas::register_physics(this, &main_scene::on_physics_update);
  atlas::register_ui(this, &main_scene::on_ui_update);
  atlas::register_start(this, &main_scene::start);
}

void main_scene::on_ui_update() {
  atlas::transform *transform = m_platform->get_mut<atlas::transform>();
  atlas::material *material = m_platform->get_mut<atlas::material>();

  //   atlas::ui::draw_panel_component<atlas::material>("platform", [&]() {
  //     atlas::ui::draw_vec3("pos", transform->Position);
  //     atlas::ui::draw_vec3("rotation", transform->Rotation);
  //     atlas::ui::draw_vec3("scale", transform->Scale);
  //     atlas::ui::draw_vec4("color", material->color);
  //   });

  atlas::ui::draw_panel_component<atlas::material>("ball", [&]() {
    atlas::ui::draw_vec3("pos", transform->Position);
    atlas::ui::draw_vec3("rotation", transform->Rotation);
    atlas::ui::draw_vec3("scale", transform->Scale);
    atlas::ui::draw_vec4("color", material->color);
  });
}

void main_scene::on_update() {
  atlas::camera *camera_comp = m_camera->get_mut<atlas::camera>();
  editor_controls *p_controls = m_camera->get_mut<editor_controls>();
  float delta_time = atlas::application::delta_time();

  if (atlas::event::is_key_pressed(key_f12)) {
    atlas::application::get_window().close();
  }

  if (atlas::event::is_key_pressed(p_controls->forward)) {
    camera_comp->process_keyboard(atlas::forward, delta_time);
  }
  if (atlas::event::is_key_pressed(p_controls->backward)) {
    camera_comp->process_keyboard(atlas::backward, delta_time);
  }
  if (atlas::event::is_key_pressed(p_controls->left)) {
    camera_comp->process_keyboard(atlas::left, delta_time);
  }
  if (atlas::event::is_key_pressed(p_controls->right)) {
    camera_comp->process_keyboard(atlas::right, delta_time);
  }
  if (atlas::event::is_key_pressed(p_controls->down)) {
    camera_comp->process_keyboard(atlas::up, delta_time);
  }
  if (atlas::event::is_key_pressed(p_controls->up)) {
    camera_comp->process_keyboard(atlas::down, delta_time);
  }
  if (atlas::event::is_mouse_pressed(mouse_button_1)) {
    glm::vec2 cursor_pos = atlas::event::cursor_position();
    //! @note On right click make sure change starts as 0
    if (!p_controls->on_click_check) {
      p_controls->last_cursor_pos = cursor_pos;
      p_controls->on_click_check = true;
    }

    //! @note offset is now delta_x and delta_y
    //! @note the difference between mouse old and new positions
    glm::vec2 offset;
    offset = cursor_pos - p_controls->last_cursor_pos;

    glm::vec2 velocity;
    velocity = offset * (delta_time * 4500);

    camera_comp->process_mouse_movement(velocity.x * -1, 0.0f);

    camera_comp->process_mouse_movement(0.0f, velocity.y * -1);

    p_controls->last_cursor_pos = cursor_pos;
  } else {
    p_controls->on_click_check = false;
  }
  camera_comp->MovementSpeed = 5;

  camera_comp->update_proj_view();
  camera_comp->IsMainCamera = true;
}

void main_scene::start() {
  flecs::world world = *this;
  engine = atlas::physics::initialize_engine(m_allocator_object,
                                             m_physics_settings, world);
}

// Temporary

void main_scene::runtime_start() {
  is_runtime = true;

  for (int number = 0; number < 1; number++) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        atlas::transform_physics physics_sphere_transform;
        physics_sphere_transform =
            *m_spheres[number][i][j]->get<atlas::transform_physics>();

        auto quaternion = glm::quat({physics_sphere_transform.rotation.x,
                                     physics_sphere_transform.rotation.y,
                                     physics_sphere_transform.rotation.z});

        m_spheres[number][i][j]->set<atlas::transform>(
            {.Position = physics_sphere_transform.position,
             .QuaternionRotation = glm::highp_vec4(quaternion.x, quaternion.y,
                                                   quaternion.z, quaternion.w),
             .Rotation = physics_sphere_transform.rotation,
             .Scale = physics_sphere_transform.scale});
      }
    }
  }
  engine->start_runtime();
}

void main_scene::runtime_stop() {
  is_runtime = false;

  for (int number = 0; number < 1; number++) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {

        atlas::transform_physics physics_sphere_transform;
        physics_sphere_transform =
            *m_spheres[number][i][j]->get<atlas::transform_physics>();

        auto quaternion = glm::quat({physics_sphere_transform.rotation.x,
                                     physics_sphere_transform.rotation.y,
                                     physics_sphere_transform.rotation.z});

        m_spheres[number][i][j]->set<atlas::transform>(
            {.Position = physics_sphere_transform.position,
             .QuaternionRotation = glm::highp_vec4(quaternion.x, quaternion.y,
                                                   quaternion.z, quaternion.w),
             .Rotation = physics_sphere_transform.rotation,
             .Scale = physics_sphere_transform.scale});
      }
    }
  }

  // Starts the engine
  engine->stop_runtime();
}

void main_scene::on_physics_update() {
  if (atlas::event::is_key_pressed(key_r) && !is_runtime) {
    runtime_start();
  }

  /**
   * @warning  All physics based actions should be run here. Including all
   * actions that run on this event. There will be some collision defining
   * physics_update() in different classes right now. But can't fix that until
   * we have a function for:
   *  init,
   *  start_runtime,
   *  physics_run,
   *  end_runtime.
   */

  // Physics time step
  if (is_runtime) {
    // Calls the physics
    engine->physics_step();

    // Calls the collider handler
    engine->run_contact_add();
  }

  // Phsyics runtime toggle off
  if (atlas::event::is_key_pressed(key_q) && is_runtime) {
    runtime_stop();
  }
}
