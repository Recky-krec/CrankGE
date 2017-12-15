#pragma once

#include "GL/glew.h"
#include "Utils/GL_DebugUtils.h"

class VertexBuffer
{
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

private:
    unsigned int m_BufferID;

};
