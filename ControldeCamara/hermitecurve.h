#pragma once
#include "Curve.h"
#include "Vector3D.h"
class HermiteCurve :public Curve
{
	Matrix<double, 4, 1> T;
public:
	HermiteCurve(const Vector3D<double>& P1 = { 0,0,0 }, const Vector3D<double>&P2 = { 0,0,0 }, const Vector3D<double>&T1 = { 0,0,0 }, const Vector3D<double>&T2 = { 0,0,0 });
	~HermiteCurve();
	Vector3D<double> operator()(const double& t);
};