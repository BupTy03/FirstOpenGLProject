#pragma once


class VertexBuffer
{
public:
    struct do_not_bind{};

    explicit VertexBuffer(const void* data, unsigned int size);
    explicit VertexBuffer(const void* data, unsigned int size, do_not_bind);
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&&) = delete;
    VertexBuffer& operator=(VertexBuffer&&) = delete;

    void Bind();
    void Unbind();

private:
    unsigned int rendererID_;
};
