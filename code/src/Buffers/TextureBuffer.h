#pragma once

#include <GL/glew.h>
#include "Utils/Debug.h"

namespace crank
{

class TextureBuffer
{
public:
    enum Format
    {
        COLOR,
        DEPTH,
        STENCIL,
        DEPTH24_STENCIL8
    };

public:
    TextureBuffer(Format format = COLOR);

    void Storage(unsigned int width, unsigned int height);
    void Bind() const;
    void Unbind() const;
    unsigned int GetAttachmentType() const;

    inline unsigned int Id() const { return m_bufferId; }

private:
    unsigned int GetInternalFormat(Format format) const;
    unsigned int GetFormat(Format format) const;
    unsigned int GetType(Format format) const;


private:
    unsigned int m_bufferId;
    Format m_format;
};

} // namespace crank
