cmake_minimum_required(VERSION 3.27)
project(game-template CXX)

set(DATA assets/data)
set(SCRIPTS assets/scripts)
set(MODULES assets/modules)

build_application(
    SOURCES
    Application.cpp
    game_world.cpp
    main_scene.cpp

    # Data
    ${DATA}/camera/editor_controls.hpp

    # Physics
    ${SCRIPTS}/physics/rigidbody.hpp
    ${SCRIPTS}/physics/rigidbody.cpp

    # Action Components
    assets/modules/action_components/requirements.cpp
    assets/modules/action_components/requirements.hpp

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