cmake_minimum_required(VERSION 3.27)
project(game-template CXX)

set(DATA assets/data)
set(SCRIPTS assets/scripts)
set(Modules assets/modules)

add_definitions(-DFLECS_SCRIPT)

build_application(
    SOURCES
    Application.cpp
    game_world.cpp
    main_scene.cpp

    ${Modules}/serialization/serializer.cpp
    ${Modules}/blaze_draco_editor/text_editor.cpp

    ${SCRIPTS}/gui/ui_hierarchy.cpp
    ${SCRIPTS}/gui/ui_hierarchy.hpp
    ${SCRIPTS}/gui/ui_properties_panel.cpp
    ${SCRIPTS}/gui/ui_properties_panel.hpp

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