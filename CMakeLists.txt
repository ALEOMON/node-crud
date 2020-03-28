cmake_minimum_required(VERSION 3.17)

SET(CMAKE_TOOLCHAIN_FILE "/usr/share/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(OpenPokemonTCG VERSION 1.0.0)

# Global options.
set(CMAKE_CXX_STANDARD 17)
set(OpenGL_GL_PREFERENCE "GLVND")

# Global debug options.
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_MESSAGE_LOG_LEVEL "DEBUG")

# Load configuration cmake files from dependencies.
find_package(glfw3 3.3.2 CONFIG REQUIRED)
find_package(glm 0.9.9.8 CONFIG REQUIRED)
find_package(imgui CONFIG R