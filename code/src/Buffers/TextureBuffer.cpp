#include "TextureBuffer.h"

namespace crank
{

TextureBuffer::TextureBuffer(Format format)
    : m_format(format)
{
    glGenTextures(1, &m_bufferId);
    glBindTexture(GL_TEXTURE_2D, m_bufferId);

}

void TextureBuffer::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_bufferId);
}

void TextureBuffer::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int TextureBuffer::GetInternalFormat(TextureBuffer::Format format) const
{
    switch(format)
    {
        case COLOR:             return GL_RGB;
        case DEPTH:             return GL_DEPTH_COMPONENT;
        case STENCIL:           return GL_STENCIL_INDEX;
        case DEPTH24_STENCIL8:  return GL_DEPTH24_STENCIL8;
    }

    ASSERT(false);
    return 0;
}

unsigned int TextureBuffer::GetFormat(TextureBuffer::Format format) const
{
    switch(format)
    {
        case COLOR:             return GL_RGB;
        case DEPTH:             return GL_DEPTH_COMPONENT;
        case STENCIL:           return GL_STENCIL_INDEX;
        case DEPTH24_STENCIL8:  return GL_DEPTH_STENCIL;
    }

    ASSERT(false);
    return 0;
}

unsigned int TextureBuffer::GetType(TextureBuffer::Format format) const
{
    switch(format)
    {
        case COLOR:             return GL_UNSIGNED_BYTE;
        case DEPTH:             return GL_UNSIGNED_INT;
        case STENCIL:           return GL_UNSIGNED_BYTE;
        case DEPTH24_STENCIL8:  return GL_UNSIGNED_INT_24_8;
    }

    ASSERT(false);
    return 0;
}

void TextureBuffer::Storage(unsigned int width, unsigned int height)
{
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GetInternalFormat(m_format), width, height, 0, GetFormat(m_format), GetType(m_format), nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

unsigned int TextureBuffer::GetAttachmentType() const
{
    switch(m_format)
    {
        case COLOR:            return GL_COLOR_ATTACHMENT0;
        case DEPTH:            return GL_DEPTH_ATTACHMENT;
        case STENCIL:          return GL_STENCIL_ATTACHMENT;
        case DEPTH24_STENCIL8: return GL_DEPTH_STENCIL_ATTACHMENT;
    }

    ASSERT(false);
    return 0;
}

} // namespace crank
