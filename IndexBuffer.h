#pragma once


class IndexBuffer
{
public:
    explicit IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    IndexBuffer(IndexBuffer&&) = delete;
    IndexBuffer& operator=(IndexBuffer&&) = delete;

    void Bind() const;
    void Unbind() const;

    [[nodiscard]]
    unsigned int Count() const;

private:
    unsigned int rendererID_;
    unsigned int indexCount_;
};
