#include "HermiteCurve.h"
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

Vector3D<double> HermiteCurve::operator()(const double& t)
{
	double tarr[4][1] = { 1,t,t*t,t*t*t };
	T = tarr;
	return (G*M*T)(0, 0);
}