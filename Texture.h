#pragma once

#include "Size2.h"
#include <string>


class Texture
{
public:
    explicit Texture(std::string path);
    ~Texture();

    void Bind(unsigned int slot) const;
    void Unbind() const;

    [[nodiscard]]
    Size2<int> Size() const;

private:
    unsigned char* localBuffer_;
    int bytesPerPixel_;
    unsigned int rendererID_;
    Size2<int> size_;
    std::string filePath_;
};
