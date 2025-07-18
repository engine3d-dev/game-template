#include "schema.hpp"
#include <vector>
#include <core/engine_logger.hpp>


/*
 * This gives us the ability to hide the underlying data used to get schema
 * built. Including how to get flecs set up and everything.
 *
 * Only one of these need to exsist per state
 */

struct schema::schema_impl {
  std::string name;
  std::string last_json_result = "{}";

  struct ComponentDef {
    std::string name;
    size_t size;
    size_t alignment;
  };
  std::vector<ComponentDef> component_definitions;

  struct SystemDef {
    std::string name;
    std::string query_expr;
  };
  std::vector<SystemDef> system_definitions;
};

schema::schema(std::string name) : m_impl(std::make_unique<schema_impl>()) {
  m_impl->name = std::move(name);
}

// Define ways to allow for this to move to other states.
schema::~schema() = default;
schema::schema(schema &&) noexcept = default;
schema &schema::operator=(schema &&) noexcept = default;

const std::string &schema::get_name() const { return m_impl->name; }

const std::string &schema::from_script(const std::string &p_script) const {
  // TODO: Implement your script parsing logic here.
  // This logic would parse a text file (like Lua, or a custom format)
  // and populate the m_impl->component_definitions and m_impl->system_definitions
  // vectors. For example: if (line_is_component(p_script)) {
  //     m_impl->component_definitions.push_back({ ... });
  // }

  // For now, it's a placeholder.
  m_impl->last_json_result = "{\"status\":\"parsed from script\"}";
  return m_impl->last_json_result;
}

const std::string &schema::from_json(const std::string &p_json) const {
  // TODO: Implement JSON parsing (e.g., using nlohmann/json)
  // This would read a JSON structure and populate the internal definition
  // vectors.
  m_impl->last_json_result = "{\"status\":\"parsed from json\"}";
  return m_impl->last_json_result;
}

const std::string &schema::to_json() const {
  // TODO: Implement serialization to JSON.
  // This would iterate over the internal definition vectors and build a JSON
  // string.
  m_impl->last_json_result =
      "{\"name\":\"" + m_impl->name + "\", \"components\":[]}";
  return m_impl->last_json_result;
}

void schema::apply_to_world(const atlas::optional_ref<flecs::world>& p_world) const {
  // This is where the schema blueprint is applied to a live world.
  std::cout << "Applying schema '" << m_impl->name << "' to world." << std::endl;

  for (const auto &comp_def : m_impl->component_definitions) {
    // This is a placeholder registration.
    // In a real implementation, you would need a way to map these
    // definitions back to actual C++ types or handle them generically.
    std::cout << "  - Registering component: " << comp_def.name << std::endl;
    p_world->component(comp_def.name.c_str())
    .member("",);
  }

  for (const auto &sys_def : m_impl->system_definitions) {
    // NOTE: This part is complex. To make this work, you need a way
    // to map the system name back to a C++ function pointer (flecs::iter_cb).
    // This typically requires a central registry where C++ functions are
    // registered with string names at startup.
    std::cout << "  - Registering system: " << sys_def.name
              << " (callback not attached)" << std::endl;
  }
}