#pragma once
#include <iostream>

template <class T>
class vec2
{
public:
	vec2(T vx, T vy);
	vec2(const vec2<T>& vector);

private:
	template <typename U>
	friend std::ostream& operator<<(std::ostream& os, const vec2<U>& vector);
	template <typename U>
	friend vec2<U> operator+(const vec2<U>& left, const vec2<U>& right);
	template <typename U>
	friend vec2<U> operator-(const vec2<U>& left, const vec2<U>& right);
	template <typename U>
	friend vec2<U> operator*(const vec2<U>& left, T right);
	template <typename U>
	friend vec2<U> operator*(T left, const vec2<U>& right);
	template <typename U>
	friend vec2<U> operator/(const vec2<U>& left, T right);
	template <typename U>
	friend bool operator==(const vec2<U>& left, const vec2<U>& right);
	template <typename U>
	friend bool operator!=(const vec2<U>& left, const vec2<U>& right);

public:
	T x, y;
};

template <class T>
vec2<T>::vec2(T vx, T vy)
	: x(vx), y(vy)
{
}

template <class T>
vec2<T>::vec2(const vec2<T>& vector)
	: x(vector.x), y(vector.y)
{
}

template <class T>
vec2<T> operator+(const vec2<T>& left, const vec2<T>& right)
{
	vec2<T> result(left.x + right.x, left.y + right.y);
	return result;
}

template <class T>
vec2<T> operator-(const vec2<T>& left, const vec2<T>& right)
{
	vec2<T> result(left.x - right.x, left.y - right.y);
	return result;
}

template <class T>
vec2<T> operator*(const vec2<T>& left, T right)
{
	vec2<T> result(left.x * right, left.y * right);
	return result;
}

template <class T>
vec2<T> operator*(T left, const vec2<T>& right)
{
	vec2<T> result(left * right.x, left * right.y);
	return result;
}

template <class T>
vec2<T> operator/(const vec2<T>& left, T right)
{
	vec2<T> result(left.x / right, left.y / right);
	return result;
}

template <class T>
bool operator==(const vec2<T>& left, const vec2<T>& right)
{
	return (left.x == right.x) && (left.y == right.y);
}

template <class T>
bool operator!=(const vec2<T>& left, const vec2<T>& right)
{
	return !(left == right);
}

template <class T>
std::ostream& operator<<(std::ostream& os, const vec2<T>& vector)
{
	os << vector.x << ", " << vector.y;
	return os;
}