#include "Renderer.h"
#include <GL/glew.h>
#include <iostream>


void GLClearAllErrors() { while(glGetError() != GL_NO_ERROR){}; }

bool GLCheckErrors()
{
    GLenum error{GL_NO_ERROR};
    while((error = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "[OpenGL Error] (" << error << ")\n";
        return false;
    }

    std::cout.flush();
    return true;
}

bool contains(const std::string& str, const std::string_view fragment)
{
    return str.find(fragment) != std::string::npos;
}
