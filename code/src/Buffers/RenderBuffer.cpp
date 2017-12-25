#include "RenderBuffer.h"

namespace crank
{

RenderBuffer::RenderBuffer()
{
    glGenRenderbuffers(1, &m_bufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_bufferId);
}


void RenderBuffer::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_bufferId);
}

void RenderBuffer::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderBuffer::Storage(unsigned int width, unsigned int height)
{
    Bind();
    // TODO: make the format configurable
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}


} // namespace crank
