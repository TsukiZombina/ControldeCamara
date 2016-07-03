#pragma once
#ifndef CONTROL_H
#define CONTROL_H

#include <iostream>
#include <fstream>
#include <GL/glew.h>

#include "camera.h"
#include "math3d.h"

using std::cout;
using std::endl;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_W || key == GLFW_KEY_UP)
	{
		CameraInfo* camera = (CameraInfo*)glfwGetWindowUserPointer(window);
		camera->position = camera->position + SPEEDK * camera->target;
	}

	if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN)
	{
		CameraInfo* camera = (CameraInfo*)glfwGetWindowUserPointer(window);
		camera->position = camera->position - SPEEDK * camera->target;
	}

	if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
	{
		CameraInfo* camera = (CameraInfo*)glfwGetWindowUserPointer(window);

		Vector3 targetxup;
		Vector3CrossProduct(&camera->target, &camera->up, &targetxup);
		Vector3Normalize(&targetxup);

		camera->position = camera->position - SPEEDK * targetxup;
	}

	if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
	{
		CameraInfo* camera = (CameraInfo*)glfwGetWindowUserPointer(window);

		Vector3 targetxup;
		Vector3CrossProduct(&camera->target, &camera->up, &targetxup);
		Vector3Normalize(&targetxup);

		camera->position = camera->position + SPEEDK * targetxup;
	}
}

void cursor_pos_callback(GLFWwindow* window, double x, double y)
{
	CameraInfo* camera = (CameraInfo*)glfwGetWindowUserPointer(window);

	GLdouble offsetX = camera->mouseX - x;
	GLdouble offsetY = camera->mouseY - y;
	
	camera->mouseX = x;
	camera->mouseY = y;

	// Calcula los angulos horizontal y vertical en base al desplazamiento obtenido
	float horizontalAngle = offsetX / 5.0f;
	float verticalAngle = offsetY / 5.0f;

	// Rota el vector target con respecto al eje vertical de acuerdo al angulo horizontal
	Matrix3x3 rotation;
	Matrix3x3MakeRotationY(horizontalAngle, &rotation);

	Vector3 newTarget;
	Matrix3x3MultiplicationByVector(&rotation, &camera->target, &newTarget);

	Vector3Normalize(&newTarget);

	camera->target.x = newTarget.x;
	camera->target.y = newTarget.y;
	camera->target.z = newTarget.z;

	// Rota el vector target con respecto al eje horizontal de acuerdo al angulo vertical
	Vector3 horizontalAxis;
	Vector3CrossProduct(&camera->up, &newTarget, &horizontalAxis);
	Vector3Normalize(&horizontalAxis);

	Matrix3x3MakeRotation(&horizontalAxis, verticalAngle, &rotation);
	Matrix3x3MultiplicationByVector(&rotation, &camera->target, &newTarget);

	camera->target.x = newTarget.x;
	camera->target.y = newTarget.y;
	camera->target.z = newTarget.z;
}

#endif // CONTROL_H