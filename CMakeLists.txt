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
                 glm/0.9.9.5@g-truc/stable
        BUILD missing)
conan_define_targets()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
set(SOURCE_FILES
        main.cpp
        Renderer.cpp Renderer.h
        VertexBuffer.cpp VertexBuffer.h
        IndexBuffer.cpp IndexBuffer.h
        VertexArray.cpp VertexArray.h
        VertexBufferLayout.cpp VertexBufferLayout.h
        Shader.cpp Shader.h
        GLUtils.cpp GLUtils.h
        stb/stb_image.cpp stb/stb_image.h
        Texture.cpp Texture.h
        Size2.h
        ScopeExit.h)

set(TESTS
        tests/Test.h
        tests/TestMenu.cpp tests/TestMenu.h
        tests/TestClearColor.cpp tests/TestClearColor.h)

set(IMGUI_FILES
        imgui/imconfig.h
        imgui/imgui.cpp imgui/imgui.h
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_impl_glfw_gl3.cpp imgui/imgui_impl_glfw_gl3.cpp
        imgui/imgui_internal.h
        imgui/stb_rect_pack.h
        imgui/stb_textedit.h
        imgui/stb_truetype.h)

add_executable(FirstOpenGLProject ${SOURCE_FILES} ${IMGUI_FILES} ${TESTS})
target_include_directories(FirstOpenGLProject PRIVATE ".")
target_link_libraries(FirstOpenGLProject CONAN_PKG::glfw CONAN_PKG::glew CONAN_PKG::glm)
