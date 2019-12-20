# coding: utf-8
from conans import ConanFile, CMake, tools

import os
import sys
import io
# sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='gbk')

os.system(" chcp 65001 ")


class DlogConan(ConanFile):
    name = "dlog"
    version = "2.4"
    license = "<Put the package license here>"
    author = "daixian,amano_tooko@qq.com"
    url = "https://github.com/daixian/dlog"
    description = "dx's log lib"
    topics = ("log", "C++", "C#")
    settings = "os", "compiler", "build_type", "arch"
    requires = ("spdlog/1.4.2@bincrafters/stable",
                "boost/1.71.0@conan/stable",
                "rapidjson/1.1.0@bincrafters/stable",
                "gtest/1.8.1@bincrafters/stable")
    options = {"shared": [True, False]}
    default_options = {"shared": False, "boost:without_test": True}
    generators = "cmake"
    exports_sources = "src/*"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.h", dst="include", src="src")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["dlog"]
