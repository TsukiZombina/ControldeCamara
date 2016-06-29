#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "math3d.h"

struct CameraInfo
{
	Vector3 position = { 0.0f, 0.0f, 0.0f };
	Vector3 target = { 0.0f, 0.0f ,0.0f };
	Vector3 up = { 0.0f, 0.0f, 0.0f };

	double mouseX = 0.0;
	double mouseY = 0.0;

	GLfloat speed = 0.2f;
};

#endif // CAMERA_H