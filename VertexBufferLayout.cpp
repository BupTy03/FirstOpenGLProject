#include "VertexBufferLayout.h"


namespace VertexBufferLayoutImpl {

    unsigned int SizeOfGLType(unsigned int type) {
        switch (type) {
            case GL_FLOAT:
                return sizeof(float);
            case GL_UNSIGNED_INT:
                return sizeof(unsigned int);
            case GL_UNSIGNED_BYTE:
                return sizeof(unsigned char);
        }

        assert(false);
        return 0;
    }

}


/// VertexBufferElement class
VertexBufferElement::VertexBufferElement(
        unsigned int type,
        unsigned int count,
        bool normalized)
        : type_{type}
        , count_{count}
        , normalized_{normalized}
{
}

unsigned int VertexBufferElement::Count() const { return count_; }
unsigned int VertexBufferElement::Type() const { return type_; }
bool VertexBufferElement::Normalized() const { return normalized_;}


/// VertexBufferLayout class
VertexBufferLayout::VertexBufferLayout() : stride_{0}, elements_() { }

void VertexBufferLayout::PushInternal(
        unsigned int type,
        unsigned int count,
        bool normalized,
        unsigned int currentStride)
{
    elements_.emplace_back(type, count, normalized);
    stride_ += currentStride * count;
}

const std::vector<VertexBufferElement> &VertexBufferLayout::Elements() const { return elements_; }
unsigned int VertexBufferLayout::Stride() const { return stride_; }
