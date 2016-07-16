#pragma once
#include<iostream>
#include<cmath>

using namespace std;

template<class T>
class Vector3D
{
	T elements[4];
	T &x = elements[0], &y = elements[1], &z = elements[2], &w = elements[3];
public:
	Vector3D(const T& a = 0, const T& b = 0, const T& c = 0);
	void set(const T&, const T&, const T&);
	Vector3D(const Vector3D<T>&);
	template<class T2>
	Vector3D(const Vector3D<T2>&);
	~Vector3D();
	T Length() const;
	Vector3D<T> Normalize() const;
	const T* GetArray() const;
	T operator()(const int&) const;
	Vector3D<T> operator=(const Vector3D<T>&);
	template<class T1, class T2>
	friend Vector3D<T1> operator+(Vector3D<T1>, const Vector3D<T2>&);
	template<class T1, class T2>
	friend Vector3D<T1> operator+=(Vector3D<T1>&, const Vector3D<T2>&);
	template<class T1, class T2>
	friend Vector3D<T1> operator-(const Vector3D<T1>&, const Vector3D<T2>&);
	template<class T1, class T2>
	friend Vector3D<T1> operator*(Vector3D<T1>, const T2&);
	template<class T1, class T2>
	friend Vector3D<T1> operator*(const T2&, Vector3D<T1>);
	template<class T1, class T2>
	friend Vector3D<T1> operator/(Vector3D<T1>, const T2&);
	template<class T>
	friend istream& operator >> (istream& cin, Vector3D<T>& v);
	template<class T>
	friend ostream& operator << (ostream& cout, const Vector3D<T>& v);
	template<class T1, class T2>
	friend T1 Dot(const Vector3D<T1>&, const Vector3D<T2>&);
	template<class T1, class T2>
	friend Vector3D<T1> Cross(const Vector3D<T1>&, const Vector3D<T2>&);
};
template<class T1, class T2>
Vector3D<T1> CrossNormalize(const Vector3D<T1>&, const Vector3D<T2>&);


template<class T>
Vector3D<T>::Vector3D(const T& a, const T& b, const T& c)
{
	x = a;
	y = b;
	z = c;
}

template<class T>
void Vector3D<T>::set(const T & a, const T & b, const T & c)
{
	x = a;
	y = b;
	z = c;
}

template<class T>
Vector3D<T>::Vector3D(const Vector3D<T>& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

template<class T>
template<class T2>
Vector3D<T>::Vector3D(const Vector3D<T2>& v)
{
	const T2* arr = v.GetArray();
	x = arr[0];
	y = arr[1];
	z = arr[2];
}

template<class T>
Vector3D<T>::~Vector3D()
{
}

template<class T>
T Vector3D<T>::Length() const
{
	return sqrt(x*x + y*y + z*z);
}

template<class T>
Vector3D<T> Vector3D<T>::Normalize() const
{
	if (Length() > 0)
	{
		return *this / Length();
	}
	return *this;
}

template<class T>
const T* Vector3D<T>::GetArray() const
{
	return &elements[0];
}

template<class T>
T Vector3D<T>::operator()(const int& i)const
{
	return elements[i];
}

template<class T>
Vector3D<T> Vector3D<T>::operator=(const Vector3D<T>& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

template<class T1, class T2>
Vector3D<T1> operator+(Vector3D<T1> v1, const Vector3D<T2>& v2)
{
	return v1 += v2;
}

template<class T1, class T2>
Vector3D<T1> operator+=(Vector3D<T1>& v1, const Vector3D<T2>& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;
}

template<class T1, class T2>
Vector3D<T1> operator-(const Vector3D<T1>& v1, const Vector3D<T2>& v2)
{
	return Vector3D<T1>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

template<class T1, class T2>
Vector3D<T1> operator*(Vector3D<T1> v, const T2& a)
{
	v.x *= a;
	v.y *= a;
	v.z *= a;
	return v;
}

template<class T1, class T2>
Vector3D<T1> operator*(const T2& a, Vector3D<T1> v)
{
	return v*a;
}

template<class T1, class T2>
Vector3D<T1> operator/(Vector3D<T1> v, const T2& a)
{
	v.x /= a;
	v.y /= a;
	v.z /= a;
	return v;
}

template<class T>
istream& operator >> (istream& cin, Vector3D<T>& v)
{
	return cin >> v.x >> v.y >> v.z;
}

template<class T>
ostream& operator<<(ostream& cout, const Vector3D<T>& v)
{
	return cout << v.x << " " << v.y << " " << v.z;
}

template<class T1, class T2>
T1 Dot(const Vector3D<T1>& v1, const Vector3D<T2>& v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

template<class T1, class T2>
Vector3D<T1> Cross(const Vector3D<T1>& v1, const Vector3D<T2>& v2)
{
	return Vector3D<T1>(v1.y*v2.z - v2.y*v1.z, v1.z*v2.x - v2.z*v1.x, v1.x*v2.y - v2.x*v1.y);
}

template<class T1, class T2>
Vector3D<T1> CrossNormalize(const Vector3D<T1>& v1, const Vector3D<T2>& v2)
{
	Vector3D<T1> ans = Cross(v1, v2);
	if (ans.Length() > 0)
	{
		return ans / ans.Length();
	}
	else
	{
		return ans;
	}
}