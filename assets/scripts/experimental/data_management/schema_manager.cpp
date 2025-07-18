#include "schema_manager.hpp"
#include <iostream>
#include <stdexcept>

// --- schema_manager Implementation ---

schema_manager::schema_manager() {
    std::cout << "Schema Manager created." << std::endl;
}

// The private destructor means this class must be heap-allocated
// and explicitly destroyed via the public `destruct()` method.
schema_manager::~schema_manager() {
    std::cout << "Schema Manager is being destroyed." << std::endl;
    // The schemas map will automatically clean up its contents.
}

void schema_manager::create(const std::string& p_schema_name) {
    if (schemas.count(p_schema_name)) {
        throw std::runtime_error("Schema with name '" + p_schema_name + "' already exists.");
    }
    schemas.emplace(p_schema_name, schema(p_schema_name));
    std::cout << "Created schema page: " << p_schema_name << std::endl;
}

void schema_manager::remove(const std::string& p_schema_name) {
    if (!schemas.count(p_schema_name)) {
        // It's often better to just warn and return than to throw.
        std::cerr << "Warning: Cannot remove non-existent schema '" << p_schema_name << "'." << std::endl;
        return;
    }
    schemas.erase(p_schema_name);
    std::cout << "Removed schema page: " << p_schema_name << std::endl;
}

const std::string& schema_manager::schema_to_json(const std::string& p_schema_name) {
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

const std::string& schema_manager::world_to_json() {
    // TODO: Implement flecs world serialization.
    // This would require a flecs::world pointer to be stored in the manager.
    static std::string result = "{\"error\":\"world not set\"}";
    return result;
}

const std::string& schema_manager::from_json(const std::string& p_json) {
    // TODO: Implement deserialization for the entire manager state.
    static std::string result = "{\"status\":\"ok\"}";
    return result;
}

const std::string& schema_manager::schema_from_json(const std::string& p_json) {
    // TODO: Implement deserialization for a single schema from JSON.
    static std::string result = "{\"status\":\"ok\"}";
    return result;
}

void schema_manager::add(const std::string& p_script) {
    // TODO: This is ambiguous. You need to define what "add" means.
    // Does it add to the current schema? Does it create a new one?
    // This requires a "current schema" context to be set.
    std::cout << "Adding from script... (logic undefined)" << std::endl;
}

void schema_manager::set_world() {
    // TODO: This should take a flecs::world* parameter and store it.
    std::cout << "Setting world... (logic undefined)" << std::endl;
}

void schema_manager::set_schema() {
    // TODO: This should take a schema name and set it as the "active" one for commands like `add`.
    std::cout << "Setting active schema... (logic undefined)" << std::endl;
}

void schema_manager::flush() {
    // TODO: Define what "flush" means. Does it apply all schemas to the world?
    // Does it clear unsaved changes?
    std::cout << "Flushing... (logic undefined)" << std::endl;
}

void schema_manager::destruct() {
    // This public method is the only way to destroy an instance of this class.
    delete this;
}
