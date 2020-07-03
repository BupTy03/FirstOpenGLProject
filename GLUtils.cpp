#include <GL/glew.h>
#include <iostream>


bool contains(std::string_view str, const std::string_view fragment)
{
    return str.find(fragment) != std::string_view::npos;
}

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