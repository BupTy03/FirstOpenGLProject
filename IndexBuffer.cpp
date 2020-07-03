#include "IndexBuffer.h"
#include "GLUtils.h"

#include <GL/glew.h>


static_assert(sizeof(unsigned int) == sizeof(GLuint));

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : bound_{true}
    , rendererID_{0}
    , indexCount_{0}
{
    GLCall(glGenBuffers(1, &rendererID_));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count, IndexBuffer::do_not_bind)
    : IndexBuffer(data, count)
{
    Unbind();
}

IndexBuffer::~IndexBuffer() { GLCall(glDeleteBuffers(1, &rendererID_)); }

void IndexBuffer::Bind()
{
    //if(bound_) return;
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID_));
    bound_ = true;
}

void IndexBuffer::Unbind()
{
    //if(!bound_) return;
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    bound_ = false;
}

unsigned int IndexBuffer::Count() const { return indexCount_; }

