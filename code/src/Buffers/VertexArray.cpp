#include "VertexArray.h"

namespace crank
{

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_bufferId);
    glBindVertexArray(m_bufferId);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_bufferId);
}

void VertexArray::Bind()
{
    glBindVertexArray(m_bufferId);
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}

void VertexArray::AddBuffer(const crank::VertexBuffer& vb, const crank::VertexBufferLayout<void>& layout)
{
    vb.Bind();
    auto& elements = layout.GetElements();
    unsigned int offset = 0;

    for(int i = 0; i < elements.size() - 1; i++)
    {
        glVertexAttribPointer(i, elements[i].Count, elements[i].Type, /*elements[i].Normalized*/GL_FALSE, layout.GetStride(), (const void*)(offset));
        glEnableVertexAttribArray(i);
        offset += elements[i].Count * 4;//sizeof(elements[i].Type);
    }
}

} // namespace crank
