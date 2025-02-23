from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, cmake_layout

class NSFWRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("opencv/4.10.0")
        self.requires("drogon/1.9.9")
        self.requires("onnxruntime/1.18.1")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        tc = CMakeToolchain(self)
        tc.generate()

    def layout(self):
        self.folders.build = "build"
        cmake_layout(self)
