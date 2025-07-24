#include "schema_parser.hpp"
#include <cctype>
#include <core/engine_logger.hpp>

void schema_parser::parse(const std::string& p_script, commands& out_commands) {
    out_commands.clear();
    size_t current_pos = 0;
    while (current_pos < p_script.length()) {
        size_t block_start = p_script.find('{', current_pos);
        if (block_start == std::string::npos) {
            break;
        }

        size_t name_end_pos = p_script.find_last_not_of(" \t\n\r", block_start - 1);
        if (name_end_pos == std::string::npos) { out_commands.clear(); return; } 


        size_t name_start_pos = p_script.find_last_of(" \t\n\r", name_end_pos) + 1;
        std::string name = p_script.substr(name_start_pos, name_end_pos - name_start_pos + 1);


        size_t type_end_pos = p_script.find_last_not_of(" \t\n\r", name_start_pos - 1);
        if (type_end_pos == std::string::npos) { out_commands.clear(); return; } 


        size_t type_start_pos = p_script.find_last_of(" \t\n\r", type_end_pos);
        type_start_pos = (type_start_pos == std::string::npos) ? 0 : type_start_pos + 1;
        std::string type = p_script.substr(type_start_pos, type_end_pos - type_start_pos + 1);


        int brace_level = 1;
        size_t block_end = block_start + 1;
        while (block_end < p_script.length() && brace_level > 0) {
            if (p_script[block_end] == '{') brace_level++;
            if (p_script[block_end] == '}') brace_level--;
            block_end++;
        }
        if (brace_level > 0) { out_commands.clear(); return; }

        std::string content_str = p_script.substr(block_start + 1, block_end - (block_start + 1));
        std::vector<token> tokens = tokenize(content_str);


        if (type == "component") {
            out_commands.component_definitions.push_back({name, tokens});
        } else if (type == "entity") {
            out_commands.entity_definitions.push_back({name, tokens});
        } else if (type == "system") {
            out_commands.system_definitions.push_back({name, tokens});
        } else {
            out_commands.clear(); return; 
        }

        current_pos = block_end + 1;
    }
}


std::vector<token> schema_parser::tokenize(const std::string& source) {
    std::vector<token> tokens;
    size_t i = 0;
    while (i < source.length()) {
        char c = source[i];

        if (isspace(c)) { i++; continue; }

        if (isalpha(c) || c == '_') {
            std::string value;
            while (i < source.length() && (isalnum(source[i]) || source[i] == '_')) {
                value += source[i++];
            }
            tokens.push_back({token_type::IDENTIFIER, value});
            continue;
        }

        if (isdigit(c) || (c == '.' && i + 1 < source.length() && isdigit(source[i+1]))) {
            std::string value;
            while (i < source.length() && (isdigit(source[i]) || source[i] == '.')) {
                value += source[i++];
            }
            tokens.push_back({token_type::NUMBER, value});
            continue;
        }

        switch (c) {
            case '=': tokens.push_back({token_type::EQUALS, "="}); break;
            case ':': tokens.push_back({token_type::COLON, ":"}); break;
            case ';': tokens.push_back({token_type::SEMICOLON, ";"}); break;
            case '{': tokens.push_back({token_type::OPEN_BRACE, "{"}); break;
            case '}': tokens.push_back({token_type::CLOSE_BRACE, "}"}); break;
            case '.': tokens.push_back({token_type::DOT, "."}); break;
            case '+': tokens.push_back({token_type::PLUS, "+"}); break;
            case '-': tokens.push_back({token_type::MINUS, "-"}); break;
            case '*': tokens.push_back({token_type::TIMES, "*"}); break;
            case '/': tokens.push_back({token_type::DIVIDE, "/"}); break;
            case '%': tokens.push_back({token_type::MOD, "%"}); break;
            case ',': tokens.push_back({token_type::COMMA, ","}); break;
            default: tokens.push_back({token_type::UNKNOWN, std::string(1, c)}); break;
        }
        i++;
    }
    return tokens;
}