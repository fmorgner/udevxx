import re

from conans import ConanFile, CMake
from conans.tools import valid_min_cppstd, load


def get_version():
    try:
        content = load("CMakeLists.txt")
        version = re.search("project\(\"udevxx\"\s*VERSION \"(.*)\"", content).group(1)
        return version.strip()
    except:
        return None


class UdevxxConan(ConanFile):
    name = "udevxx"
    version = get_version()
    license = "BSD-3-Clause"
    url = "https://github.com/fmorgner/udevxx"
    author = "Felix Morgner <felix.morgner@hsr.ch>"
    description = "A clean C++ interface for libudev"
    topics = (
        "device-interface",
        "udev",
        "wrapper",
    )
    scm = {
        "type": "git",
        "url": "https://github.com/fmorgner/udevxx.git",
        "revision": "auto",
    }
    options = {
        "examples": [False, True],
    }
    default_options = {
        "examples": False
    }
    settings = ("compiler", "build_type", "os", "arch")

    def configure(self):
        if not self.options.examples:
            del self.settings.build_type
            del self.settings.os
            del self.settings.arch
        if not valid_min_cppstd(self, "17"):
            self.settings.compiler.cppstd = "17"


    def _configure_build(self):
        cmake = CMake(self)
        cmake.definitions['BUILD_EXAMPLES'] = self.options.examples
        cmake.configure()
        return cmake

    def build(self):
        self._configure_build().build()

    def package(self):
        self._configure_build().install()

    def package_id(self):
        if not self.options.examples:
            self.info.header_only()
        
    def package_info(self):
        self.cpp_info.libs = [
            "stdc++fs",
            "udev",
            "pthread"
        ]