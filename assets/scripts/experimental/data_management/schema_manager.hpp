#pragma once
#include "schemas/schema.hpp"
#include <core/core.hpp>
#include <flecs.h>

/**
 * @brief Allows for each
 *
 */
class schema_manager {
public:
  schema_manager();
  schema_manager(const schema_manager &) = delete;
  schema_manager(schema_manager &&) = delete;
  schema_manager &operator=(const schema_manager &) = delete;
  schema_manager &operator=(schema_manager &&) = delete;

  const std::string &schema_to_json(const std::string &p_schema_name);
  const std::string &world_to_json();
  const std::string &from_json(const std::string &p_json);
  const std::string &schema_from_json(const std::string &p_json);

  void create(const std::string &p_schema_name);
  void add(const std::string &p_script);

  void set_world();
  void set_schema();

void flush();
void remove(const std::string &p_schema_name);

void destruct();

private:
  std::unordered_map<std::string, schema> schemas;
  ~schema_manager();
};