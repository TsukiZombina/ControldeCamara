#pragma once
#include<iostream>
#include"Matrix.h"
#include"vector3D.h"
#define size_s (order > 0 ? order : 1)
using namespace std;
template<class T, size_t order>
class SquareMatrix :public Matrix<T, order, order>
{
	T elements[size_s][size_s];
public:
	SquareMatrix(T m[size_s][size_s] = { 0 });
	template<class T2>
	SquareMatrix(const SquareMatrix<T2, order>&);
	~SquareMatrix();
	T Cofactor(const size_t&, const size_t&) const;
	T Det() const;
	void Identity();
	void MakeRotation(Vector3D<T>& axis, float angle);
	SquareMatrix<double, order> Inverse() const;
	template<class T1, class T2>
	friend Vector3D<T2> operator*(const SquareMatrix<T1, 3>&, Vector3D<T2>&);
};

template<class T, size_t order>
SquareMatrix<T, order>::SquareMatrix(T m[size_s][size_s]) :Matrix(m)
{

}

template<class T, size_t order>
template<class T2>
SquareMatrix<T, order>::SquareMatrix(const SquareMatrix<T2, order>& m) :matrix(m)
{
}

template<class T, size_t order>
SquareMatrix<T, order>::~SquareMatrix()
{

}

template<class T, size_t order>
T SquareMatrix<T, order>::Cofactor(const size_t& a, const size_t& b) const
{
	const size_t minusone = size_s > 1 ? (size_s - 1) : 1;
	T m[minusone][minusone];
	for (size_t i = 0, x = 0; i < order; i++)
	{
		if (i == a) continue;
		for (size_t j = 0, y = 0; j < order; j++)
		{
			if (j == b) continue;
			m[x][y] = elements[i][j];
			y++;
		}
		x++;
	}
	return ((a + b) % 2 ? -1 : 1)*SquareMatrix<T, minusone>(m).Det();
}

template<class T, size_t order>
T SquareMatrix<T, order>::Det() const
{
	T ans = 0;
	if (order > 2)
	{
		for (size_t i = 0; i < order; i++)
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

template<class T, size_t order>
void SquareMatrix<T, order>::Identity()
{
	for (int i = 0; i < order; i++)
		for (int j = 0; j < order; j++)
			elements[i][j] = (i == j ? 1 : 0);
}

template<class T, size_t order>
void SquareMatrix<T, order>::MakeRotation(Vector3D<T>& axis, float angle)
{
	// Convert angle to radians
	angle = angle * 3.1415926536f / 180;

	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);

	elements[0][0] = cosAngle + (1 - cosAngle) * axis(0) * axis(0);
	elements[0][1] = (1 - cosAngle) * axis(0) * axis(1) - sinAngle * axis(2);
	elements[0][2] = (1 - cosAngle) * axis(0) * axis(2) + sinAngle * axis(1);

	elements[1][0] = (1 - cosAngle) * axis(0) * axis(0) + sinAngle * axis(2);
	elements[1][1] = cosAngle + (1 - cosAngle) * axis(1) * axis(1);
	elements[1][2] = (1 - cosAngle) * axis(1) * axis(2) - sinAngle * axis(0);

	elements[2][0] = (1 - cosAngle) * axis(0) * axis(2) - sinAngle * axis(1);
	elements[2][1] = (1 - cosAngle) * axis(1) * axis(2) + sinAngle * axis(0);
	elements[2][2] = cosAngle + (1 - cosAngle) * axis(2) * axis(2);
}

template<class T, size_t order>
SquareMatrix<double, order> SquareMatrix<T, order>::Inverse() const
{
	double temp[order][order];
	double delta = Det();
	for (size_t i = 0; i < order; i++)
	{
		for (size_t j = 0; j < order; j++)
		{
			temp[i][j] = Cofactor(j, i) / delta;
		}
	}
	return SquareMatrix<double, order>(temp);
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
