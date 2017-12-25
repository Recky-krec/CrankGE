#include "FrameBuffer.h"

namespace crank
{

FrameBuffer::FrameBuffer(Mode mode)
    : m_mode(mode)
{
    glGenFramebuffers(1, &m_bufferId);
    glBindFramebuffer(GetMode(mode), m_bufferId);
}


void FrameBuffer::Bind() const
{
    glBindFramebuffer(GetMode(m_mode), m_bufferId);
}

void FrameBuffer::Unbind() const
{
    glBindFramebuffer(GetMode(m_mode), 0);
}

void FrameBuffer::Attach(const RenderBuffer& rb)
{
    Bind();
    glFramebufferRenderbuffer(GetMode(m_mode), GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb.Id());
}

void FrameBuffer::Attach(const TextureBuffer& tb)
{
    Bind();
    glFramebufferTexture2D(GetMode(m_mode), tb.GetAttachmentType(), GL_TEXTURE_2D, tb.Id(), 0);
}

unsigned int FrameBuffer::GetMode(Mode mode) const
{
    switch(mode)
    {
        case READ:      return GL_READ_FRAMEBUFFER;
        case DRAW:      return GL_DRAW_FRAMEBUFFER;
        case READ_DRAW: return GL_FRAMEBUFFER;
    }

    ASSERT(false);
    return 0;
}


} // namespace crank
