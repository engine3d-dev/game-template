cmake_minimum_required(VERSION 3.27)
project(game-template CXX)


build_application(
    SOURCES
    Application.cpp
    game_world.cpp
    main_scene.cpp

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

generate_compile_commands()