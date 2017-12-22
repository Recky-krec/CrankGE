#pragma once
#include "GL/glew.h"
#include "Buffers/VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace crank
{

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind();
    void Unbind();

    void AddBuffer(const crank::VertexBuffer& vb, const crank::VertexBufferLayout<void>& layout);

private:
    unsigned int m_bufferId;

};

} // namespace crank
