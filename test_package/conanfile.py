import os

from conans import ConanFile, CMake, tools


class UdevxxTestConan(ConanFile):
    settings = "compiler",
    generators = "cmake"

    def configure(self):
        self.settings.compiler.cppstd = "17"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        if not tools.cross_building(self.settings):
            os.chdir("bin")
            self.run(".%sPackageTest" % os.sep)
