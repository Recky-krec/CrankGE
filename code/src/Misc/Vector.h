#pragma once

#include <memory>
#include <algorithm>

namespace crank
{

struct out_of_range {};

template<typename T, typename A = std::allocator<T>> // requires Element<T>()
class Vector
{
public:
    Vector();
    explicit Vector(unsigned int sz);
    ~Vector();

    Vector(const Vector& v); // copy constructor
    Vector& operator=(const Vector& v); // copy assignment
    Vector(Vector&& v); // move constructor
    Vector& operator=(Vector&& v); // move assignment

    inline T& operator[](int n) { return m_elem[n]; }
    inline const T& operator[](int n) const { return m_elem[n]; }

    T& at(int n);
    const T& at(int n) const;

    void reserve(unsigned int newalloc);
    void resize (unsigned int newsize, T def = T());
    void push_back(T t);

    inline unsigned int size() const { return m_sz; }
    inline unsigned int capacity() const { return m_space; }

private:
    A m_alloc;
    T* m_elem;
    unsigned int m_sz;
    unsigned int m_space;
};

} // namespace crank

#include "Vector.inl"