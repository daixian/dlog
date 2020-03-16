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
    version = "2.5.0"
    license = "WTFPL???"
    author = "daixian<amano_tooko@qq.com>"
    url = "https://github.com/daixian/dlog"
    description = "dx's log lib"
    topics = ("log", "C++", "C#")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "build_test": [
        True, False], "fmt.fPIC": [True, False]}
    default_options = {"shared": True,
                       "build_test": True,
                       "fmt.fPIC": False,
                       "*:shared": False}
    generators = "cmake"
    exports_sources = "src/*"

    def requirements(self):
        pass

    def build_requirements(self):
        self.build_requires("spdlog/1.5.0")
        self.build_requires("poco/1.9.4")
        self.build_requires("rapidjson/1.1.0")
        self.build_requires("gtest/1.8.1@bincrafters/stable")

    def _configure_cmake(self):
        '''
        转换python的设置到CMake
        '''
        cmake = CMake(self)
        # 在windows平台目前只支持动态库
        if self.settings.os == "Windows":
            cmake.definitions["DLOG_BUILD_SHARED"] = True
        else:
            cmake.definitions["DLOG_BUILD_SHARED"] = self.options.shared
        cmake.definitions["DLOG_BUILD_TESTS"] = self.options.build_test
        cmake.definitions["LLVM_ENABLE_PIC"] = False
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
        # 拷贝如果不带*那么不会搜索到下一级文件夹
        self.copy("*dlog.h", dst="include", src="src")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["dlog"]
