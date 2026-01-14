import os
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.system.package_manager import Apt, Yum, PacMan, Zypper
from conan.tools.scm import Git
from conan.tools.files import copy
import os

class StarterConanRecipe(ConanFile):
    name = "example"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    export_source = "CMakeLists.txt", "application.cpp"

    # Putting all of your build-related dependencies here
    def build_requirements(self):
        self.tool_requires("cmake/4.1.1")
        self.tool_requires("ninja/1.13.1")
        self.tool_requires("engine3d-cmake-utils/4.0")

    # Putting all of your packages here
    def requirements(self):
        self.requires("joltphysics/5.2.0")
        self.requires("imguidocking/3.0")
        self.requires("flecs/4.0.4")
        self.requires("glfw/3.4")
        self.requires("spdlog/1.16.0")
        self.requires("glm/1.0.1")
        self.requires("yaml-cpp/0.8.0")

        # Vulkan-related headers and includes packages
        self.requires("vulkan-cpp/4.0")
        self.requires("tinyobjloader/2.0.0-rc10")
        self.requires("stb/cci.20230920")

        self.requires("nfd/2.0")
        self.requires("watcher/0.12.0")
        self.requires("boost-ext-ut/2.3.1")
        self.requires("atlas/0.5")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generator = "Ninja"
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
    
    def layout(self):
        cmake_layout(self)
