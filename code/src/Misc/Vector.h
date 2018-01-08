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
    using iterator = T*; // random access iterator
    using const_iterator = const T*;
    using size_type = unsigned long;
    using value_type = T;

    /*template<typename C>
    using Value_type = typename C::value_type;

    template<typename C>
    using Iterator = typename C::iterator;*/

public:
    inline iterator begin() { return m_elem; }
    inline iterator end() { return m_elem + m_sz; }
    inline const_iterator begin() const { return m_elem; }
    inline const_iterator end() const { return m_elem + m_sz; }

public:
    Vector();
    explicit Vector(unsigned int sz);
    Vector(std::initializer_list<T> il);
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
    iterator insert(iterator p, const T& val);
    iterator erase(iterator p);

    inline size_type size() const { return m_sz; }
    inline size_type capacity() const { return m_space; }

private:
    A m_alloc;
    unsigned int m_sz;
    unsigned int m_space;
    T* m_elem;
};

} // namespace crank

#include "Vector.inl"