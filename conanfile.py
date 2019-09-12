from conans import ConanFile, CMake, tools


class Libp2pConan(ConanFile):
    name = "libp2p"
    version = "0.1"
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of Libp2p here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "with_plaintext": [True, False]
    }
    default_options = "shared=False", "with_plaintext=True"
    generators = "cmake", "cmake_find_package"
    requires = "protobuf/3.9.1@bincrafters/stable", "openssl/1.1.1c"
    build_requires = "protoc_installer/3.9.1@bincrafters/stable"
    exports_sources = "*"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=self.source_folder)
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="hello")
        self.copy("*hello.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.cppflags = ["-std=c++17"]

