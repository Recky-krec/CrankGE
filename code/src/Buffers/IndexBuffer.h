#pragma once

#include "GL/glew.h"

namespace crank
{

class IndexBuffer
{
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int getCount() const { return m_count; }

private:
    unsigned int m_bufferId;
    unsigned int m_count;
};

} // namespace crank
