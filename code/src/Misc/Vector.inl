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
Vector<T, A>::Vector(std::initializer_list<T> il)
    : m_sz(), m_space(0), m_elem(nullptr)
{
    for(auto elem : il)
        push_back(elem);
};


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
    std::unique_ptr<T[]> p(m_alloc.allocate(newalloc));

    for(int i = 0; i<m_sz; i++) m_alloc.construct(&p[i], m_elem[i]);
    for(int i = 0; i<m_sz; i++) m_alloc.destroy(&m_elem[i]);

    m_alloc.deallocate(m_elem, m_space);
    m_elem = p.release();
    m_space = newalloc;
}


template<typename T, typename A>
void Vector<T, A>::resize(unsigned int newsize, T def)
{
    reserve(newsize);
    for(int i = m_sz; i<newsize; i++) m_alloc.construct(&m_elem[i], def);
    for(int i = newsize; i<m_sz; i++) m_alloc.destroy(&m_elem[i]); // make memory uninitialized in case of resizing to lower
    m_sz = newsize;
}

template<typename T, typename A>
void Vector<T, A>::push_back(T t)
{
    if(m_space == 0) reserve(8);
    else if(m_sz == m_space) reserve(m_space * 2);
    m_alloc.construct(&m_elem[m_sz], t);
    ++m_sz;
}

template<typename T, typename A>
T& Vector<T, A>::at(int n)
{
    if(n<0 || m_sz<=n) throw out_of_range();
    return m_elem[n];
}

template<typename T, typename A>
const T& Vector<T, A>::at(int n) const
{
    if(n<0 || m_sz<=n) throw out_of_range();
    return m_elem[n];
}

template<typename T, typename A>
typename Vector<T, A>::iterator Vector<T, A>::insert(Vector::iterator p, const T &val)
{
    int index = p - begin();

    if(size() == capacity())
        reserve(size()==0 ? 8: size()*2);

    m_alloc.construct(end(), *(end()-1) ); // copy last element into unitialized space
    ++m_sz;

    iterator pp = begin() + index;

    for(auto pos = end()-1; pos!=pp; --pos)
        *pos = *(pos-1);

    *pp = val;

    return pp;
}

template<typename T, typename A>
typename Vector<T, A>::iterator Vector<T, A>::erase(Vector::iterator p)
{
    if(p==end()) return p;
    for(auto it = p+1; it!=end(); ++it)
        *(it-1) = *it;  // copy elements one position to the left
    m_alloc.destroy( end()-1 );

    --m_sz;
    return p;
}


} // namespace crank
