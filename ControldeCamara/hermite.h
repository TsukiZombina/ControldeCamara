#pragma once
#ifndef HERMITE_H
#define HERMITE_H

#include "math3d.h"
#include "matrix.h"
#include "vector3D.h"

float m[4][4] = { {1, 0, -3, 2},
				{0, 0, 3, -2},
				{0, 1, -2, 1},
				{0, 0, -1, 1} };

Matrix<float, 4, 4> Mh(m);

#endif // HERMITE_H
