#pragma once
#ifndef CURVES_H
#define CURVES_H

#include "math3d.h"

class HermiteCurve :public Curve
{
	Matrix<double, 4, 1> T;
public:
	HermiteCurve(const Vector3D<double>& P1 = { 0,0,0 }, const Vector3D<double>&P2 = { 0,0,0 }, const Vector3D<double>&T1 = { 0,0,0 }, const Vector3D<double>&T2 = { 0,0,0 });
	~HermiteCurve();
	Vector3D<double> operator()(const double& t);
};

HermiteCurve::HermiteCurve(const Vector3D<double>& P1, const Vector3D<double>& P2, const Vector3D<double>& T1, const Vector3D<double>& T2) :Curve(P1, P2, T1.Normalize(), T2.Normalize())
{
	double Marr[4][4] =
	{ { 1,0,-3,2 },
	{ 0,0,3,-2 },
	{ 0,1,-2,1 },
	{ 0,0,-1,1 } };
	M = Marr;
}

HermiteCurve::~HermiteCurve()
{
}

Vector3<double> HermiteCurve::operator()(const double& t)
{
	double tarr[4][1] = { 1,t,t*t,t*t*t };
	T = tarr;
	return (G*M*T)(0, 0);
}

}

#endif // CURVES_H