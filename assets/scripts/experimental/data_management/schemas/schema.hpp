#pragma once

#include <assets/scripts/experimental/data_management/parsing/schema_parser.hpp>
#include <core/core.hpp>
#include <flecs.h>
#include <memory>

/**
 * @brief Defines how flecs components and queries should be injected into
 * a flecs::world. This class holds the blueprint, for the database like
 * intereted language.
 */
class schema {
public:
  schema() = default;
  schema(std::string name);
  ~schema();

  schema(const schema &) = delete;
  schema &operator=(const schema &) = delete;
  schema(schema &&) noexcept;
  schema &operator=(schema &&) noexcept;

  const std::string &get_name();

  const std::string &from_script(const std::string &p_script);
  const std::string &from_json(const std::string &p_json);
  const std::string &to_json();

  void apply_to_world(flecs::world &p_world);

private:
  struct schema_data {
    std::string name;
    std::string last_json_result = "{}";

    commands parsing_commands;
  };

  std::unique_ptr<schema_data> m_impl;
};
