cmake_minimum_required(VERSION 3.27)
project(game-template CXX)

set(DATA assets/data)
set(SCRIPTS assets/scripts)

build_application(
    SOURCES
    Application.cpp
    game_world.cpp
    main_scene.cpp

    # Data
    ${DATA}/camera/editor_controls.hpp

    ${SCRIPTS}/experimental/data_management/schema_manager.cpp
    ${SCRIPTS}/experimental/data_management/schema_manager.hpp

    ${SCRIPTS}/experimental/data_management/schemas/schema.cpp
    ${SCRIPTS}/experimental/data_management/schemas/schema.hpp

    PACKAGES
    spdlog
    flecs
    Jolt
    atlas

    LINK_PACKAGES
    spdlog::spdlog
    flecs::flecs_static
    Jolt::Jolt
    atlas::atlas
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

generate_compile_commands()