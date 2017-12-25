#pragma once

#pragma once

#include <GL/glew.h>

namespace crank
{

class RenderBuffer
{
public:
    RenderBuffer();

    void Bind() const;
    void Unbind() const;
    void Storage(unsigned int width, unsigned int height);
    inline unsigned int Id() const { return m_bufferId; }

public:
    unsigned int m_bufferId;
};

} // namespace crank

/* render buffer objects to be use within the framebuffer
 * advantages: already in opengl internal format, avoiding to texture conversions
 * thus making them faster to be written on or to have its data copied to other buffers
 * cons: write only, we can use glReadPixels() to read from the currently bound frame buffer but
 * not from the attachment itself
 * Since renderbuffer objects are generally write-only they are often used as depth and stencil attachments,
 * since most of the time we don't really need to read values from the depth and stencil buffers but still care about depth and stencil testing.
 * We need the depth and stencil values for testing,
 * but don't need to sample these values so a renderbuffer object suits this perfectly.
 * When we're not sampling from these buffers, a renderbuffer object is generally preferred since it's more optimized. */