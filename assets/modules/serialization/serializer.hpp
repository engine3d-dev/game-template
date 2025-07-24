#pragma once
#include <string>
#include <core/core.hpp>
#include <flecs.h>

class serializer {
    public:
    void serialize_scene(const std::string &p_output_path, flecs::world& p_world);
    void deserialize_scene(const std::string &p_input_path, flecs::world& p_world);
};