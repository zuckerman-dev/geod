from conans import ConanFile, CMake

class GeodConan(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   requires = "cppzmq/4.7.1", "spdlog/[>=1.4.1]", "CLI11/1.9.1@cliutils/stable"
   generators = "cmake", "gcc", "txt", "cmake_find_package"
   default_options = {}
   
   def imports(self):
      self.copy("*.dll", dst="bin", src="bin") # From bin to bin
      self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin
   
   def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()   