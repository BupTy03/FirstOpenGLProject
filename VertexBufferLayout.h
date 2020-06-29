#pragma once

#include <GL/glew.h>

#include <vector>
#include <cassert>
#include <type_traits>


class VertexBufferElement
{
public:
    explicit VertexBufferElement(unsigned int type, unsigned int count, bool normalized);

    [[nodiscard]] unsigned int Count() const;
    [[nodiscard]] unsigned int Type() const;
    [[nodiscard]] bool Normalized() const;

private:
    unsigned int type_;
    unsigned int count_;
    bool normalized_;
};


namespace VertexBufferLayoutImpl {

    template<typename T> struct GLTypeParams;

    template<> struct GLTypeParams<float> {
        static constexpr auto type_code = GL_FLOAT;
        static constexpr bool normalized = false;
    };

    template<> struct GLTypeParams<unsigned int> {
        static constexpr auto type_code = GL_UNSIGNED_INT;
        static constexpr bool normalized = false;
    };

    template<> struct GLTypeParams<unsigned char> {
        static constexpr auto type_code = GL_UNSIGNED_BYTE;
        static constexpr bool normalized = true;
    };


    unsigned int SizeOfGLType(unsigned int type);

}


class VertexBufferLayout
{
public:
    VertexBufferLayout();

    template<typename T>
    void Push(unsigned int count)
    {
        using TypeParams = VertexBufferLayoutImpl::GLTypeParams<T>;
        PushInternal(TypeParams::type_code, count, TypeParams::normalized, sizeof(T));
    }

    [[nodiscard]] const std::vector<VertexBufferElement>& Elements() const;
    [[nodiscard]] unsigned int Stride() const;

private:
    void PushInternal(
            unsigned int type,
            unsigned int count,
            bool normalized,
            unsigned int currentStride);

private:
    unsigned int stride_;
    std::vector<VertexBufferElement> elements_;
};
