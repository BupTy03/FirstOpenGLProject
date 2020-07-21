#pragma once

#include "glm/glm.hpp"

#include <cassert>
#include <string_view>


#define GLCall(x) \
    GLClearAllErrors(); \
    x; \
    assert(GLCheckErrors())


void GLClearAllErrors();
bool GLCheckErrors();

bool contains(std::string_view str, std::string_view fragment);

namespace std {
    template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q = glm::defaultp>
    auto data(const glm::mat<C, R, T, Q>& m) { return &m[0][0]; }

    template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q = glm::defaultp>
    auto data(glm::mat<C, R, T, Q>& m) { return &m[0][0]; }
}

