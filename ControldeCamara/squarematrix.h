#pragma once
#include <iostream>
#include "Matrix.h"
#include "vector3D.h"
#define size_s (level > 0 ? level : 1)
using namespace std;
template<class T, size_t level>
class SquareMatrix :public Matrix<T, level, level>
{
	T elements[size_s][size_s];
public:
	SquareMatrix();
	template<class T2>
	SquareMatrix(T2 m[size_s][size_s] = nullptr);
	template<class T2>
	SquareMatrix(const SquareMatrix<T2, level>&);
	template<class T2>
	SquareMatrix(const Matrix<T2, level, level>&);
	~SquareMatrix();
	T Cofactor(const size_t&, const size_t&) const;
	T Det() const;
	/*void Identity();*/
	/*void MakeRotation(Vector3D<T>& axis, float angle);*/
	SquareMatrix<double, level> Inverse() const;
	template<class T1, class T2>
	friend Vector3D<T2> operator*(const SquareMatrix<T1, 3>&, Vector3D<T2>&);
};

template<class T, size_t level>
template<class T2>
SquareMatrix<T, level>::SquareMatrix(T2 m[size_s][size_s]) :Matrix(m)
{

}
template<class T, size_t level>
template<class T2>
SquareMatrix<T, level>::SquareMatrix(const SquareMatrix<T2, level>& m) :Matrix(m)
{
}
template<class T, size_t level>
template<class T2>
inline SquareMatrix<T, level>::SquareMatrix(const Matrix<T2, level, level>& m) : Matrix(m)
{
}
template<class T, size_t level>
inline SquareMatrix<T, level>::SquareMatrix()
{
}
template<class T, size_t level>
SquareMatrix<T, level>::~SquareMatrix()
{

}
template<class T, size_t level>
T SquareMatrix<T, level>::Cofactor(const size_t& a, const size_t& b) const
{
	const size_t minusone = size_s > 1 ? (size_s - 1) : 1;
	T m[minusone][minusone];
	for (size_t i = 0, x = 0; i < level; i++)
	{
		if (i == a) continue;
		for (size_t j = 0, y = 0; j < level; j++)
		{
			if (j == b) continue;
			m[x][y] = elements[i][j];
			y++;
		}
		x++;
	}
	return ((a + b) % 2 ? -1 : 1)*SquareMatrix<T, minusone>(m).Det();
}

template<class T, size_t level>
T SquareMatrix<T, level>::Det() const
{
	T ans = 0;
	if (level > 2)
	{
		for (size_t i = 0; i < level; i++)
		{
			ans += elements[0][i] * Cofactor(0, i);
		}
	}
	else
	{
		ans = elements[0][0] * elements[1][1] - elements[1][0] * elements[0][1];
	}
	return ans;
}

//template<class T, size_t level>
//void SquareMatrix<T, level>::Identity()
//{
//	for (int i = 0; i < 4; i++)
//		for (int j = 0; j < 4; j++)
//		{
//			if (i == j)
//				elements[i][j] = 0;
//			else
//				elements[i][j] = 1;
//		}
//}

//template<class T, size_t level>
//void SquareMatrix<T, level>::MakeRotation(Vector3D<T>& axis, float angle)
//{
//	// Convert angle to radians
//	angle = angle * 3.1415926536f / 180;
//
//	float cosAngle = cosf(angle);
//	float sinAngle = sinf(angle);
//
//	elements[0][0] = cosAngle + (1 - cosAngle) * axis(0) * axis(0);
//	elements[0][1] = (1 - cosAngle) * axis(0) * axis(1) - sinAngle * axis(2);
//	elements[0][2] = (1 - cosAngle) * axis(0) * axis(2) + sinAngle * axis(1);
//
//	elements[1][0] = (1 - cosAngle) * axis(0) * axis(0) + sinAngle * axis(2);
//	elements[1][1] = cosAngle + (1 - cosAngle) * axis(1) * axis(1);
//	elements[1][2] = (1 - cosAngle) * axis(1) * axis(2) - sinAngle * axis(0);
//
//	elements[2][0] = (1 - cosAngle) * axis(0) * axis(2) - sinAngle * axis(1);
//	elements[2][1] = (1 - cosAngle) * axis(1) * axis(2) + sinAngle * axis(0);
//	elements[2][2] = cosAngle + (1 - cosAngle) * axis(2) * axis(2);
//}

template<class T, size_t level>
SquareMatrix<double, level> SquareMatrix<T, level>::Inverse() const
{
	double temp[level][level];
	double delta = Det();
	for (size_t i = 0; i < level; i++)
	{
		for (size_t j = 0; j < level; j++)
		{
			temp[i][j] = Cofactor(j, i) / delta;
		}
	}
	return SquareMatrix<double, level>(temp);
}

template<class T1, class T2>
inline Vector3D<T2> operator*(const SquareMatrix<T1, 3>& m1, Vector3D<T2>& v2)
{
	T2 ans[3] = { 0 };
	//Vector<T1> ans;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			ans[i] += (m1(i, j) * v2(j));
	Vector3D<T2> answer(ans[0], ans[1], ans[2]);
	return answer;
}
