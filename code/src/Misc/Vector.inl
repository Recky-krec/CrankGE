#include "Vector.h"

namespace crank
{

template<typename T, typename A>
Vector<T, A>::Vector()
    : m_sz(0), m_space(0), m_elem(nullptr)
{}


template<typename T, typename A>
Vector<T, A>::Vector(unsigned int sz)
    : m_sz(sz), m_space(sz), m_elem(new T[sz])
{}


template<typename T, typename A>
Vector<T, A>::Vector(const Vector<T, A>& v)
    : m_sz(v.m_sz), m_space(v.m_sz), m_elem(new T[v.m_sz])
{
    std::copy(v.m_elem, v.m_elem + v.m_sz, m_elem);
}

template<typename T, typename A>
Vector<T, A>& Vector<T, A>::operator=(const Vector<T, A> &v)
{
    if(this == &v) return *this; // self assignment, no work needed

    if(v.m_sz == m_sz)
    { // enough space, no need for new allocation
        std::copy(v.m_elem, v.m_elem + v.m_sz, m_elem);
        m_sz = v.m_sz;
        return *this;
    }

    T* p = new T[v.m_sz];
    std::copy(v.m_elem, v.m_elem + v.m_sz, p);

    delete[] m_elem;
    m_space = m_sz = v.m_sz;
    m_elem = p;
    return *this;
}


template<typename T, typename A>
Vector<T, A>::Vector(Vector<T, A> &&v)
    : m_sz(v.m_sz), m_space(v.m_sz), m_elem(v.m_elem)
{
    v.m_sz = 0;
    v.m_elem = nullptr;
}


template<typename T, typename A>
Vector<T, A>& Vector<T, A>::operator=(Vector &&v)
{
    delete [] m_elem;
    m_elem = v.m_elem;
    m_space = m_sz = v.m_sz;
    v.m_elem = nullptr;
    v.m_sz = 0;
    return *this;
}


template<typename T, typename A>
Vector<T, A>::~Vector()
{
    delete[] m_elem;
}

template<typename T, typename A>
void Vector<T, A>::reserve(unsigned int newalloc)
{
    if(newalloc <= m_space) return;
    auto* p = new T[newalloc];
    for(int i = 0; i<m_sz; i++) p[i] = m_elem[i];
    delete[] m_elem;
    m_elem = p;
    m_space = newalloc;
}


template<typename T, typename A>
void Vector<T, A>::resize(unsigned int newsize, T def)
{
    reserve(newsize);
    for(int i = m_sz; i<newsize; i++) m_elem[i] = def;
    m_sz = newsize;
}

template<typename T, typename A>
void Vector<T, A>::push_back(T t)
{
    if(m_space == 0)
        reserve(8);

    else if(m_sz == m_space)
        reserve(m_space * 2);

    m_elem[m_sz] = t;
    ++m_sz;
}


} // namespace crank
