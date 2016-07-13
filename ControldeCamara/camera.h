#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "vector3D.h"

const GLfloat SPEEDK = 0.1f, SPEEDC = 0.025f;

struct CameraInfo
{
	Vector3D<GLfloat> position;
	Vector3D<GLfloat> target;
	Vector3D<GLfloat> up;

	float mouseX = 0.0;
	float mouseY = 0.0;
};

#endif // CAMERA_H