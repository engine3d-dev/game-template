cmake_minimum_required(VERSION 3.27)
project(game-template CXX)

# build_application(
#     SOURCES
#     Application.cpp
#     game_world.cpp
#     main_scene.cpp

#     PACKAGES
#     spdlog
#     flecs
#     Jolt
#     atlas

#     LINK_PACKAGES
#     spdlog::spdlog
#     flecs::flecs_static
#     Jolt::Jolt
#     atlas::atlas
# )

add_executable(
    ${PROJECT_NAME}
    application.cpp
    game_world.cpp
    main_scene.cpp
)

find_package(spdlog REQUIRED)
find_package(flecs REQUIRED)
find_package(Jolt REQUIRED)
find_package(atlas REQUIRED)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    spdlog::spdlog
    flecs::flecs_static
    Jolt::Jolt
    atlas::atlas
)

generate_compile_commands()
