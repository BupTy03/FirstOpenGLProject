#include "VertexArray.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "GLUtils.h"


VertexArray::VertexArray()
    : bound_{false}
    , rendererID_{0}
{
    GLCall(glGenVertexArrays(1, &rendererID_));
}

VertexArray::~VertexArray() { GLCall(glDeleteVertexArrays(1, &rendererID_)); }

static auto ToGlBool(bool value) { return (value) ? GL_TRUE : GL_FALSE; }

void VertexArray::AddBuffer(VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();

    const auto& elems = layout.Elements();
    unsigned int offset = 0;
    for(std::size_t i = 0; i < elems.size(); ++i)
    {
        const auto& elem = elems.at(i);
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(
                i,
                elem.Count(),
                elem.Type(),
                ToGlBool(elem.Normalized()),
                layout.Stride(),
                reinterpret_cast<const void*>(offset)));

        offset += elem.Count() * VertexBufferLayoutImpl::SizeOfGLType(elem.Type());
    }
}

void VertexArray::Bind()
{
    //if(bound_) return;
    GLCall(glBindVertexArray(rendererID_));
    bound_ = true;
}

void VertexArray::Unbind()
{
    //if(!bound_) return;
    GLCall(glBindVertexArray(0));
    bound_ = false;
}
