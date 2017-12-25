#include "VertexArray.h"
#include "VertexBufferLayout.h"

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

void VertexArray::Bind() const
{
    glBindVertexArray(m_bufferId);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddBuffer(const crank::VertexBuffer& vb, const crank::VertexBufferLayout& layout)
{
    vb.Bind();
    auto& elements = layout.GetElements();
    unsigned int offset = 0;

    for(int index = 0; index <= elements.size() - 1; index++)
    {
        auto& element = elements[index];

        glVertexAttribPointer(index, element.Count, element.Type, element.Normalized, layout.GetStride(), (const void*)(offset));
        glEnableVertexAttribArray(index);

        offset += element.Count * LayoutElement::GetSizeOfType(element.Type);
    }
}

} // namespace crank
