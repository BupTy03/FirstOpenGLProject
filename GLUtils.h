#pragma once

#include <cassert>
#include <string_view>


#define GLCall(x) \
    GLClearAllErrors(); \
    x; \
    assert(GLCheckErrors())


void GLClearAllErrors();
bool GLCheckErrors();

bool contains(std::string_view str, std::string_view fragment);
