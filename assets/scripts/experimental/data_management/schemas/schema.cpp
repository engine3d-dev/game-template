#include "schema.hpp"
#include <core/engine_logger.hpp>

/*
 * This gives us the ability to hide the underlying data used to get schema
 * built. Including how to get flecs set up and everything.
 *
 * Only one of these need to exsist per state
 */
schema::schema(std::string name) : m_impl(std::make_unique<schema_data>()) {
  m_impl->name = std::move(name);
}

// Define ways to allow for this to move to other states.
schema::~schema() = default;
schema::schema(schema &&) noexcept = default;
schema &schema::operator=(schema &&) noexcept = default;

const std::string &schema::get_name() { return m_impl->name; }

const std::string &schema::from_script(const std::string &p_script) {

  schema_parser::parse(p_script, m_impl->parsing_commands);

  return m_impl->last_json_result;
}

const std::string &schema::from_json(const std::string &p_json) {
  // TODO: Implement JSON parsing (e.g., using nlohmann/json)
  // This would read a JSON structure and populate the internal definition
  // vectors.
  m_impl->last_json_result = "{\"status\":\"parsed from json\"}";
  return m_impl->last_json_result;
}

const std::string &schema::to_json() {
  // TODO: Implement serialization to JSON.
  // This would iterate over the internal definition vectors and build a JSON
  // string.
  m_impl->last_json_result =
      "{\"name\":\"" + m_impl->name + "\", \"components\":[]}";
  return m_impl->last_json_result;
}

void schema::apply_to_world(flecs::world &p_world) {
  console_log_info("Applying schema {} to world.", m_impl->name.c_str());
  for (const auto &def : m_impl->parsing_commands.component_definitions) {
    const std::string &component_name = def.name;
    if (p_world.lookup(component_name.c_str()).is_valid()) {
      console_log_warn("Component '{}' already exists. Skipping.",
                       component_name);
      continue;
    }

    flecs::untyped_component component =
        p_world.component(component_name.c_str());
    size_t i = 0;
    const auto &tokens = def.tokens;

    while (i + 1 < tokens.size()) {
      if (tokens[i].type != token_type::IDENTIFIER) {
        console_log_error("Expected type identifier at position {}", i);
        component.destruct();
        return;
      }

      std::string type_name = tokens[i].value;
      i++;
      if (i >= tokens.size() || tokens[i].type != token_type::IDENTIFIER) {
        console_log_error("Expected field name after type '{}'", type_name);
        component.destruct();
        return;
      }

      std::string field_name = tokens[i].value;
      i++;

      flecs::entity type_entity = p_world.lookup(type_name.c_str());
      if (!type_entity.is_valid()) {
        std::string meta_fallback = "flecs::meta::" + type_name;
        type_entity = p_world.lookup(meta_fallback.c_str());

        if (!type_entity.is_valid()) {
          console_log_error("Unknown type '{}' (or '{}') for field '{}'",
                            type_name, meta_fallback, field_name);
          component.destruct();
          return;
        }
      }

      component.member(type_entity, field_name.c_str(), 1);

      if (i >= tokens.size() || tokens[i].type != token_type::SEMICOLON) {
        console_log_error("Expected ';' after field '{}'", field_name);
        break;
      }

      ++i;
    }
  }
  console_log_info("Getting here!");
  for (const auto &def : m_impl->parsing_commands.entity_definitions) {
    const std::string &entity_name = def.name;

    console_log_info("Getting here! {}", def.name.c_str());

    if (p_world.lookup(entity_name.c_str()).is_valid()) {
      console_log_warn("Entity '{}' already exists. Skipping.", entity_name);
      continue;
    }

    flecs::entity new_entity = p_world.entity(entity_name.c_str());
    new_entity.add(p_world.lookup("user"), p_world.lookup("object"));
    const auto &tokens = def.tokens;
    size_t i = 0;

    while (i < tokens.size()) {
      if (tokens[i].type != token_type::IDENTIFIER) {
        console_log_error("Expected component name at token {}, {}", i, tokens[i].value);
        break;
      }

      std::string component_name = tokens[i++].value;
      console_log_info("Getting here! {}", component_name.c_str());
      flecs::entity component_type = p_world.lookup(component_name.c_str());
      if (!component_type.is_valid()) {
        console_log_error("Component type '{}' not found.", component_name);
        break;
      }

      if (i >= tokens.size() || tokens[i].type != token_type::OPEN_BRACE) {
        console_log_error("Expected '{{' after component '{}'", component_name);
        break;
      }

      i++;

      console_log_info("Getting here!2");
      if (!(new_entity.lookup("component_name").is_valid())) {
        new_entity.add(component_type);
      }
      void *component_data = new_entity.get_mut(component_type);
      flecs::cursor cursor(p_world.c_ptr(), component_type.id(),
                           component_data);
      cursor.push();
      console_log_info("Getting here!3");
      while (i < tokens.size() && tokens[i].type == token_type::DOT) {
        i++;

        if (tokens[i].type != token_type::IDENTIFIER) {
          console_log_error("Expected field name after '.'");
          break;
        }

        std::string field_name = tokens[i++].value;

        if (tokens[i].type != token_type::EQUALS) {
          console_log_error("Expected '=' after field '{}'", field_name);
          break;
        }
        i++;
        cursor.member(field_name.c_str());

        if (i < tokens.size() && tokens[i].type == token_type::OPEN_BRACE) {
          i++;
          cursor.push();

          while (i + 2 < tokens.size() && tokens[i].type == token_type::DOT) {
            i++;
            std::string member_name = tokens[i++].value;
            console_log_info("Getting here! {}", member_name);
            i++;
            float val = std::stof(tokens[i++].value);
            console_log_info("Getting here! {}", val);
            cursor.member(member_name.c_str());
            *static_cast<float *>(cursor.get_ptr()) = val;
            if (tokens[i].type == token_type::COMMA) {
              i++;
            } else {
              console_log_info("Getting here! no comma");
              break;
            }
          }
          if (tokens[i].type == token_type::CLOSE_BRACE)
            i++;
          cursor.pop();
        } else if (tokens[i].type == token_type::NUMBER) {
          std::string_view val_str = tokens[i++].value;

          flecs::entity member_type = cursor.get_type();
          flecs::string_view type_name = member_type.name();

          void *ptr = cursor.get_ptr();

          if (type_name == "float" || type_name == "f32") {
            try {
              float val = std::stof(std::string(val_str));
              *static_cast<float *>(ptr) = val;
            } catch (...) {
              console_log_error("Invalid float value: {}", val_str);
            }
          } else if (type_name == "int" || type_name == "i32") {
            try {
              int val = std::stoi(std::string(val_str));
              *static_cast<int *>(ptr) = val;
            } catch (...) {
              console_log_error("Invalid int value: {}", val_str);
            }
          } else if (type_name == "bool") {
            bool b = (val_str == "true" || val_str == "1");
            *static_cast<bool *>(ptr) = b;
          } else {
            console_log_error("Unsupported primitive type '{}' for assignment.",
                              type_name.c_str());
          }
        }

        if (tokens[i].type == token_type::SEMICOLON) {
          i++;
        }
      }

      if (tokens[i].type != token_type::CLOSE_BRACE) {
        console_log_error("Expected closing brace for component '{}'",
                          component_name);
      } else {
        i++;
      }

      cursor.pop();

      if(tokens[i].type == token_type::CLOSE_BRACE) {
        i++;
      }
    }
  }
}