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
};


template<typename T>
struct identity { typedef T type; };

template<typename T>
class VertexBufferLayout
{
public:
    VertexBufferLayout() : m_stride(0) {}
    inline const std::vector<LayoutElement>& GetElements() const { return m_elements; }
    inline const unsigned int GetStride() const { return m_stride; }

public:
    template<typename TL>
    void Push(unsigned int count)
    {
        Push(count, identity<TL>());
    }


private:
    template<typename TL>
    void Push(unsigned int count, identity<TL>)
    {

    }

    void Push(unsigned int count, identity<float>)
    {
        m_stride += count;
        m_elements.push_back( { GL_FLOAT, count, GL_FALSE } );
    }

private:
    std::vector<LayoutElement> m_elements;
    unsigned int m_stride;
};

} // namespace crank
