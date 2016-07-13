#pragma once
#include"Vector3D.h"
#include"Matrix.h"
#include"SquareMatrix.h"
class Curve
{
protected:
	Matrix<Vector3D<double>, 1, 4> G;
	SquareMatrix<double, 4> M;
public:
	Curve(const Vector3D<double>&, const Vector3D<double>&, const Vector3D<double>&, const Vector3D<double>&);
	~Curve();
	virtual Vector3D<double> operator()(const double& t) = 0;
};