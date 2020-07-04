#include "IndexBuffer.h"
#include "GLUtils.h"

#include <GL/glew.h>


static_assert(sizeof(unsigned int) == sizeof(GLuint));

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : rendererID_{0}
    , indexCount_{count}
{
    GLCall(glGenBuffers(1, &rendererID_));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &rendererID_));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int IndexBuffer::Count() const { return indexCount_; }

