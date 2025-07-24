#pragma once
#include <string>
#include <vector>

enum class token_type {
  IDENTIFIER,
  NUMBER,
  EQUALS,
  COLON,
  SEMICOLON,
  OPEN_BRACE,
  CLOSE_BRACE,
  COMMA,
  DOT,
  PLUS,
  MINUS,
  TIMES,
  DIVIDE,
  MOD,
  UNKNOWN
};

struct token {
  token_type type;
  std::string value;
};

struct definition {
  std::string name;
  std::vector<token> tokens;
};

struct commands {
  std::vector<definition> component_definitions;
  std::vector<definition> system_definitions;
  std::vector<definition> entity_definitions;

  void clear() {
    component_definitions.clear();
    system_definitions.clear();
    entity_definitions.clear();
  }
};

class schema_parser {
public:
  schema_parser();

  static void parse(const std::string &p_script, commands &out_commands);

  struct primitive_type_info {
    size_t size;
    void (*assign)(void *dst, std::string_view value);
  };

  inline const primitive_type_info *
  get_primitive_info(std::string_view type_name) {
    for (auto &[name, info] : primitive_map_array) {
      if (name == type_name)
        return &info;
    }
    return nullptr;
  }

private:
  static std::vector<token> tokenize(const std::string &source);

  static constexpr std::pair<std::string_view, primitive_type_info>
      primitive_map_array[] = {
          {"float",
           {sizeof(float),
            [](void *dst, std::string_view val) {
              *static_cast<float *>(dst) = std::stof(std::string(val));
            }}},
          {"int",
           {sizeof(int),
            [](void *dst, std::string_view val) {
              *static_cast<int *>(dst) = std::stoi(std::string(val));
            }}},
          {"bool", {sizeof(bool), [](void *dst, std::string_view val) {
                      *static_cast<bool *>(dst) = (val == "true" || val == "1");
                    }}}};
};