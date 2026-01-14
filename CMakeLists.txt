cmake_minimum_required(VERSION 3.27)
project(game-template CXX)

# build_application(
#     SOURCES
#     application.cpp
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

add_executable(${PROJECT_NAME} application.cpp)

set_packages(
    ENABLE_TESTS OFF

    PACKAGES
    glfw3
    Vulkan
    glm
    spdlog
    imguidocking
    Jolt
    yaml-cpp
    stb
    flecs

    tinyobjloader
    ${SHADERC_PACKAGE}
    vulkan-cpp
    nfd
    watcher
    atlas

    LINK_PACKAGES
    glfw
    Vulkan::Vulkan
    glm::glm
    spdlog::spdlog
    imguidocking::imguidocking

    Jolt::Jolt
    yaml-cpp
    stb::stb
    flecs::flecs_static
    tinyobjloader::tinyobjloader
    ${SHADERC_LINK_PACKAGE}
    vulkan-cpp
    nfd::nfd
    watcher::watcher
    atlas
)

target_compile_options(
    ${PROJECT_NAME}
    PUBLIC
    -g -Wall -Wextra -Wno-missing-field-initializers -Wshadow -msse4.1 -mavx
)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET CXX_MODULES
    TYPE CXX_MODULES
    FILES
    game_world.cppm
    main_scene.cppm
)

# generate_compile_commands()
