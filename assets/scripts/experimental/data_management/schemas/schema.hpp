#pragma once

#include <core/core.hpp>
#include <flecs.h>
#include <memory> 

/**
 * @brief Defines how flecs components and queries should be injected into
 * a flecs::world. This class holds the blueprint.
 */
class schema {
public:
    schema() = default;
    schema(std::string name);
    ~schema();

    schema(const schema&) = delete;
    schema& operator=(const schema&) = delete;
    schema(schema&&) noexcept;
    schema& operator=(schema&&) noexcept;

    const std::string& get_name() const;

    const std::string& from_script(const std::string& p_script) const;
    const std::string& from_json(const std::string& p_json) const;
    const std::string& to_json() const;

    void apply_to_world(const atlas::optional_ref<flecs::world>& p_world) const;

private:
    struct schema_impl;
    std::unique_ptr<schema_impl> m_impl;
};
