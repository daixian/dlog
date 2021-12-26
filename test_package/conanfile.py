#!/usr/bin/env python3
# coding=utf-8
import os

from conans import ConanFile, CMake, tools


class DLogTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    default_options = {"dlog:shared": False,
                       "poco:enable_data_sqlite": False,
                       "poco:enable_data_mysql": False,
                       "poco:enable_apacheconnector": False,
                       "poco:enable_data_postgresql": False,
                       "poco:enable_mongodb": False,
                       "poco:enable_redis": False,
                       "spdlog:header_only": True,
                       "fmt:header_only": True}

    def requirements(self):
        self.requires("spdlog/1.8.5")
        self.requires("poco/1.10.1")
        self.requires("rapidjson/1.1.0")

    def build_requirements(self):
        self.build_requires("gtest/1.8.1")

    def build(self):
        cmake = CMake(self)
        # Current dir is "test_package/build/<build_id>" and CMakeLists.txt is
        # in "test_package"
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy('*.so*', dst='bin', src='lib')

    def test(self):
        # windows平台的x86其实也可以跑测试
        if self.settings.os == "Windows":
            os.chdir("bin")
            self.run(
                '.%spackage_test --gtest_output="xml:gtest_report.xml"' % os.sep)
        elif not tools.cross_building(self.settings):
            os.chdir("bin")
            self.run(
                '.%spackage_test --gtest_output="xml:gtest_report.xml"' % os.sep)
