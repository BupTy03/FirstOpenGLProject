#include "Texture.h"

#include "ScopeExit.h"
#include "GLUtils.h"

#include "stb/stb_image.h"
#include <GL/glew.h>


Texture::Texture(std::string path)
    : localBuffer_{nullptr}
    , bytesPerPixel_{0}
    , rendererID_{0}
    , size_()
    , filePath_(std::move(path))
{
    stbi_set_flip_vertically_on_load(1);

    int width{0};
    int height{0};
    localBuffer_ = stbi_load(filePath_.c_str(), &width, &height, &bytesPerPixel_, 4);
    SCOPE_EXIT
    {
        if(localBuffer_ != nullptr)
            stbi_image_free(localBuffer_);
    };

    size_ = Size2<int>(width, height);

    GLCall(glGenTextures(1, &rendererID_));
    GLCall(glBindTexture(GL_TEXTURE_2D, rendererID_));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
            size_.width(), size_.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer_));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &rendererID_));
}

void Texture::Bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, rendererID_));
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Size2<int> Texture::Size() const
{
    return size_;
}
