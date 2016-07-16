#pragma once
#include<iostream>
#include<stdexcept>
#include "vector3D.h"

#define row_s (row > 0 ? row : 1)
#define column_s (column > 0 ? column : 1)
template<class T, size_t row, size_t column>
class Matrix
{
	T elements[row_s][column_s];
public:
	Matrix(const T m[row_s][column_s] = nullptr);
	template<class T2>
	Matrix(const Matrix<T2, row, column>&);
	/*template<class T2>
	Matrix(const Vector3D<T2>&);*/
	~Matrix();
	const T* GetArray() const;
	Matrix<T, column, row> Transpose();
	void InitZero();
	void Identity();
	void MakeRotation(Vector3D<T>& axis, float angle);
	void set(const size_t& i, const size_t& j, T element);
	template<class T2>
	Matrix<T, row, column> operator=(const Matrix<T2, row, column>&);
	template<class T1, size_t row1, size_t column1, class T2, size_t row2, size_t column2>
	friend Matrix<T1, row1, column2> operator*(const Matrix<T1, row1, column1>&, const Matrix<T2, row2, column2>&);
	template<class T1, size_t row1, class T2>
	friend Vector3D<T2> operator*(const Matrix<T1, row1, 3>&, Vector3D<T2>&);
	template<class T, size_t row, size_t column>
	friend ostream& operator<<(ostream&, const Matrix<T, row, column>&);
	T operator()(const size_t&, const size_t&) const;
};

template<class T, size_t row, size_t column>
Matrix<T, row, column>::Matrix(const T m[row_s][column_s])
{
	if (m == nullptr) return;
	for (size_t i = 0; i < row_s; i++)
	{
		for (size_t j = 0; j < column_s; j++)
		{
			elements[i][j] = m[i][j];
		}
	}
}

template<class T, size_t row, size_t column>
template<class T2>
Matrix<T, row, column>::Matrix(const Matrix<T2, row, column>& m)
{
	for (size_t i = 0; i < row_s; i++)
	{
		for (size_t j = 0; j < column_s; j++)
		{
			elements[i][j] = m(i)(j);
		}
	}
}

template<class T, size_t row, size_t column>
const T * Matrix<T, row, column>::GetArray() const
{
	const T* ptr = (T*)elements;
	return ptr;
}

template<class T, size_t row, size_t column>
template<class T2>
Matrix<T, row, column> Matrix<T, row, column>::operator=(const Matrix<T2, row, column>& m)
{
	return *this(m);
}

template<class T, size_t row, size_t column>
Matrix<T, row, column>::~Matrix()
{
}

template<class T, size_t row, size_t column>
Matrix<T, column, row> Matrix<T, row, column>::Transpose()
{
	T arr[column_s][row_s];
	for (size_t i = 0; i < row_s; i++)
	{
		for (size_t j = 0; j < column_s; j++)
		{
			arr[j][i] = elements[i][j];
		}
	}
	return Matrix<T, column, row>(arr);
}

template<class T, size_t row, size_t column>
void Matrix<T, row, column>::InitZero()
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < column; j++)
			elements[i][j] = 0;
}

template<class T, size_t row, size_t column>
void Matrix<T, row, column>::Identity()
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < column; j++)
		{
			if (i == j)
				elements[i][j] = 1;
			else
				elements[i][j] = 0;
		}
}

template<class T, size_t row, size_t column>
void Matrix<T, row, column>::MakeRotation(Vector3D<T>& axis, float angle)
{
	//Convert angle to radians
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

template<class T, size_t row, size_t column>
void Matrix<T, row, column>::set(const size_t& i, const size_t& j, T element)
{
	if(i < row && j < column)
		elements[i][j] = element;
}

template<class T, size_t row, size_t column>
T Matrix<T, row, column>::operator()(const size_t& i, const size_t& j) const
{
	return elements[i][j];
}

template<class T1, size_t row1, size_t column1, class T2, size_t row2, size_t column2>
Matrix<T1, row1, column2> operator*(const Matrix<T1, row1, column1>& m1, const Matrix<T2, row2, column2>& m2)
{
	if (row2 != column1)
	{
		throw invalid_argument("Matrix size not match");
	}
	T1 ans[row1][column2] = { 0 };
	//Matrix<T1, row1, column2> ans;
	for (size_t i = 0; i < row1; i++)
	{
		for (size_t j = 0; j < column2; j++)
		{
			for (size_t a = 0; a < column1; a++)
			{
				ans[i][j] += (m1(i, a) * m2(a, j));
			}
		}
	}
	return Matrix<T1, row1, column2>(ans);
}

template<class T1, size_t row1, class T2>
Vector3D<T2> operator*(const Matrix<T1, row1, 3>& m1, Vector3D<T2>& v2)
{
	T2 ans[row1] = { 0 };
	//Vector<T1> ans;
	for (size_t i = 0; i < row1; i++)
		for (size_t j = 0; j < 3; j++)
			ans[i] += (m1(i, j) * v2(j));
	Vector3D<T2> answer(ans[0], ans[1], ans[2]);
	return answer;
}

template<size_t row1, size_t column1, class T2, size_t row2, size_t column2>
Matrix<T2, row1, column2> operator*(const Matrix<double, row1, column1>& m1, const Matrix<T2, row2, column2>& m2)
{
	if (row2 != column1)
	{
		throw invalid_argument("Matrix size not match");
	}
	T2 ans[row1][column2] = { 0 };
	//Matrix<T1, row1, column2> ans;
	for (size_t i = 0; i < row1; i++)
	{
		for (size_t j = 0; j < column2; j++)
		{
			for (size_t a = 0; a < column1; a++)
			{
				ans[i][j] += (m1(i, a) * m2(a, j));
			}
		}
	}
	return Matrix<T2, row1, column2>(ans);
}

template<class T, size_t row, size_t column>
ostream& operator<<(ostream& cout, const Matrix<T, row, column>& m)
{
	for (size_t i = 0; i < row; i++)
	{
		for (size_t j = 0; j < column; j++)
		{
			cout << m.elements[i][j] << " ";
		}
		cout << endl;
	}
	return cout;
}
