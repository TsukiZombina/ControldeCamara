#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "math3d.h"

const GLfloat SPEED = 0.1f;

struct CameraInfo
{
	Vector3 position = { 0.0f, 0.0f, 0.0f };
	Vector3 target = { 0.0f, 0.0f ,0.0f };
	Vector3 up = { 0.0f, 0.0f, 0.0f };

	double mouseX = 0.0;
	double mouseY = 0.0;
};

#endif // CAMERA_H