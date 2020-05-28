#!/usr/bin/env python3
# coding=utf-8
import os

from conans import ConanFile, CMake, tools


class DLogTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    default_options = {"dlog:shared": False}

    def requirements(self):
        self.requires("spdlog/1.5.0")
        self.requires("poco/[>=1.10.1]@daixian/stable")
        self.requires("rapidjson/1.1.0")

    def build_requirements(self):
        self.build_requires("gtest/1.8.1@bincrafters/stable")

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
        if not tools.cross_building(self.settings):
            os.chdir("bin")
            self.run('.%spackage_test --gtest_output="xml:gtest_report.xml"' % os.sep)
