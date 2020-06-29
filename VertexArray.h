#pragma once


class VertexBuffer;
class VertexBufferLayout;


class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind();
    void Unbind();

private:
    bool bound_;
    unsigned int rendererID_;
};
