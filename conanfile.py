from conans import ConanFile, CMake
import os

class GeodConan(ConanFile):
   name = "geod"
   version = "0.0.1"
   settings = "os", "compiler", "build_type", "arch"
   requires = "adcr/0.0.1@zdev/testing", "cppzmq/4.7.1"
   generators = "cmake", "gcc", "txt", "cmake_find_package"
   default_options = {}

   scm = {
      "type" : "git",
      "subfolder" : ".",
      "url" : "https://github.com/zuckerman-dev/geod.git",
      "revision" : "auto", 
      "submodule" : "recursive"
   }
   no_copy_source = True
   keep_imports = True
     
   def build(self):
      cmake = CMake(self)
      cmake.definitions["SYSTEMD_UNIT_PATH"] = os.path.join(self.package_folder,"lib/systemd/system")
      cmake.configure()
      cmake.build()   
      cmake.install()

   def imports(self):
      self.copy("*.dll", dst="bin", src="bin") # From bin to bin
      self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin
   