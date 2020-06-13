cmake_minimum_required(VERSION 3.15)

project(FirstOpenGLProject)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(
        REQUIRES glfw/3.3.2@bincrafters/stable
                 glew/2.1.0@bincrafters/stable
        BUILD missing)
conan_define_targets()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
set(SOURCE_FILES main.cpp)

add_executable(FirstOpenGLProject ${SOURCE_FILES})
target_link_libraries(FirstOpenGLProject CONAN_PKG::glfw CONAN_PKG::glew)
