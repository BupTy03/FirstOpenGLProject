#pragma once


class VertexBuffer
{
public:
    explicit VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&&) = delete;
    VertexBuffer& operator=(VertexBuffer&&) = delete;

    void Bind() const;
    void Unbind() const;

private:
    unsigned int rendererID_;
};
