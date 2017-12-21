#pragma once

#include "GL/glew.h"
#include "Utils/Debug.h"

namespace crank
{

class VertexBuffer
{
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

private:
    unsigned int m_bufferId;

};

} // namespace crank
