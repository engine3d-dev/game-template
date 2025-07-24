#include "schema_manager.hpp"
#include <core/engine_logger.hpp>

schema_manager::schema_manager() {
  console_log_info("Schema Manager created.");
}

schema_manager::~schema_manager() {
  console_log_info("Schema Manager is being destroyed.");
}

void schema_manager::create(const std::string &p_schema_name) {
  if (schemas.count(p_schema_name)) {
    console_log_error("Schema with name '{}' already exists.",
                      p_schema_name.c_str());
    return;
  }
  schemas.emplace(p_schema_name, schema(p_schema_name));
  console_log_info("Created schema page: {}", p_schema_name.c_str());
}

void schema_manager::remove(const std::string &p_schema_name) {
  if (!schemas.count(p_schema_name)) {
    console_log_error("Cannot remove non-existent schema '%s'.",
                      p_schema_name.c_str());
    return;
  }
  schemas.erase(p_schema_name);
  console_log_info("Removed schema page: %s", p_schema_name.c_str());
}

const std::string &
schema_manager::schema_to_json(const std::string &p_schema_name) {
  // TODO: Implement this logic.
  // This should find the schema and call its to_json() method.
  // For now, it's a placeholder.
  static std::string result = "{}"; // Static to ensure reference is valid.
  auto it = schemas.find(p_schema_name);
  if (it != schemas.end()) {
    result = it->second.to_json();
  }
  return result;
}

const std::string &schema_manager::world_to_json() {
  // TODO: Implement flecs world serialization.
  // This would require a flecs::world pointer to be stored in the manager.
  static std::string result = "{\"error\":\"world not set\"}";
  return result;
}

const std::string &schema_manager::from_json(const std::string &p_json) {
  // TODO: Implement deserialization for the entire manager state.
  static std::string result = "{\"status\":\"ok\"}";
  return result;
}

const std::string &schema_manager::schema_from_json(const std::string &p_json) {
  // TODO: Implement deserialization for a single schema from JSON.
  static std::string result = "{\"status\":\"ok\"}";
  return result;
}

void schema_manager::add(const std::string &p_script) {
  // TODO: This is ambiguous. You need to define what "add" means.
  // Does it add to the current schema? Does it create a new one?
  // This requires a "current schema" context to be set.
  console_log_info("Adding from script... (logic undefined)");
}

void schema_manager::set_world() {
  // TODO: This should take a flecs::world* parameter and store it.
  console_log_info("Setting world... (logic undefined)");
}

void schema_manager::set_schema() {
  // TODO: This should take a schema name and set it as the "active" one for
  // commands like `add`.
  console_log_info("Setting active schema... (logic undefined)");
}

void schema_manager::flush() {
  // TODO: Define what "flush" means. Does it apply all schemas to the world?
  // Does it clear unsaved changes?
  console_log_info("Flushing... (logic undefined)");
}

void schema_manager::destruct() {
  // This public method is the only way to destroy an instance of this class.
  delete this;
}
