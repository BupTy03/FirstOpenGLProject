#include "VertexBuffer.h"
#include "GLUtils.h"

#include <GL/glew.h>


VertexBuffer::VertexBuffer(const void* data, unsigned int size)
    : rendererID_{0}
{
    GLCall(glGenBuffers(1, &rendererID_));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID_));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() { GLCall(glDeleteBuffers(1, &rendererID_)); }

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID_));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
