#include "Curve.h"
Curve::Curve(const Vector3D<double>& P1, const Vector3D<double>& P2, const Vector3D<double>& T1, const Vector3D<double>& T2)
{
	Vector3D<double> in[1][4] = { P1,P2,T1,T2 };
	G = in;
}

Curve::~Curve()
{
}