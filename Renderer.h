#pragma once
#include <string>
#include <string_view>
#include <cassert>


#define GLCall(x) \
    GLClearAllErrors(); \
    x; \
    assert(GLCheckErrors())


void GLClearAllErrors();
bool GLCheckErrors();

bool contains(const std::string& str, std::string_view fragment);


class Renderer
{

};
