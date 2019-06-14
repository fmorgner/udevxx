from conans import ConanFile, CMake


class UdevxxConan(ConanFile):
    name = "udevxx"
    version = "1.0.0"
    license = "BSD-3-Clause"
    author = "Felix Morgner <felix.morgner@hsr.ch>"
    url = "https://github.com/fmorgner/udevxx"
    description = "A clean C++ interface for libudev"
    topics = (
        "device-interface",
        "udev",
        "wrapper",
    )
    settings = None
    options = None
    generators = "cmake"
    exports_sources = (
        "CMakeLists.txt",
        "include/*",
        "src/*",
    )

    def configure_build(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake

    def build(self):
        self.configure_build().build()

    def package(self):
        self.configure_build().install()

    def package_id(self):
        self.info.header_only()
        
    def package_info(self):
        self.cpp_info.libs = [
            "stdc++fs",
            "udev"
        ]