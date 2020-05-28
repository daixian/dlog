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
    version = "2.5.2"
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
        if self.options.shared:
            pass
        else:
            self.requires("spdlog/1.5.0")
            self.requires("poco/[>=1.10.1]@daixian/stable")
            self.requires("rapidjson/1.1.0")

    def build_requirements(self):
        if self.options.shared:
            self.build_requires("spdlog/1.5.0")
            self.build_requires("poco/[>=1.10.1]@daixian/stable")
            self.build_requires("rapidjson/1.1.0")
        if self.options.build_test:
            self.build_requires("gtest/1.8.1@bincrafters/stable")

    def _configure_cmake(self):
        """转换python的设置到CMake"""
        print("_configure_cmake():package_folder="+self.package_folder)
        cmake = CMake(self)
        # 在windows平台目前只支持动态库
        # if self.settings.os == "Windows":
        #     cmake.definitions["DLOG_BUILD_SHARED"] = True
        # else:
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
        self.copy_archive()

    def package_info(self):
        self.cpp_info.libs = ["dlog"]
        self.cpp_info.defines.append("DLOG_UTF8")

        if self.settings.os == "Windows":
            if self.options.shared:
                self.cpp_info.defines.append("DLOG_DLL")
            else:
                self.cpp_info.defines.append("DLOG_STATIC")

    def copy_archive(self):
        """把安装文件拷贝到当前源文件目录来,方便在CI中上传"""
        print("copy_archive():package_folder="+self.package_folder)
        # if not os.environ["CONAN_ARCHIVE_PATH"] is None:
        #     dest = os.environ["CONAN_ARCHIVE_PATH"]+os.sep
        #     dest += self.settings["compiler"]+"_" + \
        #         self.setting["compiler.version"]+"_"+self.settings["arch"]
        #     if(self.settings["compiler"] is "Visual Studio"):
        #         dest += "_"+self.settings["compiler.runtime"]
        #     dest += os.sep
        #     print("copy_archive():归档目录="+dest)
        #     self.copy("*dlog.h", dst=dest+"include", src="src")
        #     self.copy("*.lib", dst=dest+"lib", keep_path=False)
        #     self.copy("*.dll", dst=dest+"bin", keep_path=False)
        #     self.copy("*.dylib*", dst=dest+"lib", keep_path=False)
        #     self.copy("*.so", dst=dest+"lib", keep_path=False)
        #     self.copy("*.a", dst=dest+"lib", keep_path=False)
        # else:
        #     print("copy_archive():执行目录CONAN_ARCHIVE_PATH为空，不执行archive拷贝")
