#include "Renderer.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "GLUtils.h"

#include <GL/glew.h>


void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    va.Bind();
    ib.Bind();
    shader.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.Count(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
