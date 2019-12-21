# coding: utf-8
from conans import ConanFile, CMake, tools
from conans.errors import ConanInvalidConfiguration

import os
import sys
import io
# sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='gbk')
# os.system("chcp 65001")


class DlogConan(ConanFile):
    name = "dlog"
    version = "2.4.0"
    license = "WTFPL???"
    author = "daixian<amano_tooko@qq.com>"
    url = "https://github.com/daixian/dlog"
    description = "dx's log lib"
    topics = ("log", "C++", "C#")
    settings = "os", "compiler", "build_type", "arch"
    requires = ("spdlog/1.4.2@bincrafters/stable",
                "boost/1.71.0@conan/stable",
                "rapidjson/1.1.0@bincrafters/stable",
                "gtest/1.8.1@bincrafters/stable")
    options = {"shared": [True, False], "build_test": [True, False]}
    default_options = {"shared": True,
                       "build_test": True,
                       "*:shared": False,
                       "boost:without_test": True}  # 这个"boost:without_test"的写法可能不被支持
    generators = "cmake"
    exports_sources = "src/*"

    def _configure_cmake(self):
        '''
        转换python的设置到CMake
        '''
        cmake = CMake(self)
        cmake.definitions["DLOG_BUILD_SHARED"] = self.options.shared
        cmake.definitions["DLOG_BUILD_TESTS"] = self.options.build_test
        return cmake

    def build(self):
        print("进入了build...")
        cmake = self._configure_cmake()
        cmake.configure(source_folder="src")
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("dlog.h", dst="include", src="src")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["dlog"]
