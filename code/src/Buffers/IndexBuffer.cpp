#include "IndexBuffer.h"

namespace crank
{

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_count(count)
{
    glGenBuffers(1, &m_bufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_bufferId);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);

}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace crank
