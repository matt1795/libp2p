from conans import ConanFile, CMake, tools
import os


class Libp2pTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    requires = "gtest/1.8.0@bincrafters/stable", "libp2p/0.1@matt1795/testing"

    def configure(self):
        self.options["gtest"].build_gmock = False

    def build(self):
        cmake = CMake(self)
        cmake.definitions['WITH_GMOCK'] = self.options['gtest'].build_gmock
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy('*.so*', dst='bin', src='lib')

    def test(self):
        if not tools.cross_building(self.settings):
            os.chdir("bin")
            self.run(".%sLibp2pTest" % os.sep)
