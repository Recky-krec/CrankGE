#pragma once

#include <GL/glew.h>
#include "Buffers/RenderBuffer.h"
#include "Buffers/TextureBuffer.h"

namespace crank
{

class FrameBuffer
{
public:
    enum Mode
    {
        READ,
        DRAW,
        READ_DRAW
    };

public:
    FrameBuffer(Mode mode = READ_DRAW);

    void Bind() const;
    void Unbind() const;

    void Attach(const RenderBuffer& rb);
    void Attach(const TextureBuffer& tb);

    inline bool Complete() const { return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE; }

private:
    unsigned int GetMode(Mode mode) const;

private:
    unsigned int m_bufferId;
    Mode m_mode;

};

} // namespace crank