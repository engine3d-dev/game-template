#include "serializer.hpp"
#include <core/engine_logger.hpp>

void serializer::serialize_scene(const std::string &p_output_path,
                                 flecs::world &p_world) {
  flecs::entity user = p_world.lookup("user");
  if (!user.is_valid()) {
    console_log_error("Cannot serialize: 'user' tag is not a valid entity.");
    return;
  }

  std::ofstream output_file(p_output_path);
  if (!output_file.is_open()) {
    console_log_error("Failed to open output file for writing: {}",
                      p_output_path.c_str());
    return;
  }

  flecs::query<> query = p_world.query_builder().with(user, flecs::Wildcard).build();

  flecs::iter_to_json_desc_t desc;
  desc.serialize_entity_ids = true;
  desc.serialize_builtin = false;
  desc.serialize_doc = false;
  desc.serialize_full_paths = true;
  desc.serialize_table = true;
  desc.serialize_type_info = false;
  desc.serialize_field_info = false;
  desc.serialize_query_info = false;
  desc.serialize_query_plan = false;
  desc.serialize_query_profile = false;
  desc.dont_serialize_results = false;
  desc.serialize_alerts = false;
  desc.serialize_refs = false;
  desc.serialize_matches = false;
  desc.serialize_values = true;

  flecs::string json_output = query.to_json(&desc);

  if (json_output) {
    output_file << json_output.c_str();
  } else {
    console_log_error("Failed to serialize query to JSON.");
  }

  if (output_file.fail()) {
    console_log_error("Failed to write scene data to file: %s",
                      p_output_path.c_str());
  } else {
    console_log_info("Scene saved successfully to {}", p_output_path.c_str());
  }
}

void serializer::deserialize_scene(const std::string &p_input_path,
                                   flecs::world &p_world) {

  flecs::entity user_handle = p_world.lookup("user");
  if (!user_handle) {
    console_log_error("Cannot clear scene: user tag not found!");
    return;
  }

  auto query_to_delete =
      p_world.query_builder().with(user_handle, flecs::Wildcard).build();

  p_world.defer_begin();
  std::vector<flecs::entity> entities_to_delete;
  query_to_delete.each(
      [&](flecs::entity e) { entities_to_delete.push_back(e); });

  for (const auto &e : entities_to_delete) {
    e.destruct();
  }

  p_world.defer_end();

  std::ifstream file(p_input_path);
  if (!file.is_open()) {
    console_log_error("Failed to open input file for reading: {}",
                      p_input_path.c_str());
    return;
  }
  std::string json_content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
  if (json_content.empty()) {
    console_log_warn("Scene file is empty, nothing to load.");
    return;
  }

  const char *result = p_world.from_json(json_content.c_str());

  if (result && *result != '\0') {
    size_t offset = result - json_content.c_str();
    std::string context =
        json_content.substr(std::max(0L, (long)offset - 20), 40);
    console_log_error("Failed to deserialize scene from JSON.");
    console_log_error("Error occurred at offset {}:\n...{}...", offset,
                      context.c_str());
  } else {
    console_log_info("Scene loaded successfully!");
  }
}
