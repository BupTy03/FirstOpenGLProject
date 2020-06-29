#pragma once


class IndexBuffer
{
public:
    struct do_not_bind{};

    explicit IndexBuffer(const unsigned int* data, unsigned int count);
    explicit IndexBuffer(const unsigned int* data, unsigned int count, do_not_bind);
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    IndexBuffer(IndexBuffer&&) = delete;
    IndexBuffer& operator=(IndexBuffer&&) = delete;

    void Bind();
    void Unbind();

    [[nodiscard]]
    unsigned int Count() const;

private:
    bool bound_;
    unsigned int rendererID_;
    unsigned int indexCount_;
};
