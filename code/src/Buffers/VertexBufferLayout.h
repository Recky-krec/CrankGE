#pragma once
#include <vector>
#include <GL/glew.h>

namespace crank
{


struct LayoutElement
{
    unsigned int Type;
    unsigned int Count;
    unsigned char Normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch(type)
        {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }

        ASSERT(false);
        return 0;
    }
};

template<typename T>
struct identity { typedef T type; };

template<typename T = void>
class VertexBufferLayout_
{
public:
    template<typename U>
    void Push(unsigned int count)
    {
        Push(count, identity<U>());
    }

private:
    template<typename U>
    void Push(unsigned int count, identity<U>)
    {}

    void Push(unsigned int count, identity<float>)
    {
        m_elements.push_back( { GL_FLOAT, count, GL_FALSE } );
        m_stride += count * sizeof(GLfloat);
    }

    void Push(unsigned int count, identity<unsigned int>)
    {
        m_elements.push_back( { GL_UNSIGNED_INT, count, GL_FALSE } );
        m_stride += count * sizeof(GLuint);
    }

    void Push(unsigned int count, identity<unsigned char>)
    {
        m_elements.push_back( { GL_BYTE, count, GL_TRUE } );
        m_stride += count * sizeof(GLbyte);
    }

public:
    VertexBufferLayout_() : m_stride(0) {}
    inline const std::vector<LayoutElement>& GetElements() const { return m_elements; }
    inline const unsigned int GetStride() const { return m_stride; }

private:
    std::vector<LayoutElement> m_elements;
    unsigned int m_stride;
};

using VertexBufferLayout = VertexBufferLayout_<void>;


} // namespace crank
