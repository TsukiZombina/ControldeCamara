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
		camera->position = camera->position + SPEED * camera->target;
	}

	if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN)
	{
		CameraInfo* camera = (CameraInfo*)glfwGetWindowUserPointer(window);
		camera->position = camera->position - SPEED * camera->target;
	}

	if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
	{
		CameraInfo* camera = (CameraInfo*)glfwGetWindowUserPointer(window);

		Vector3 targetxup;
		Vector3CrossProduct(&camera->target, &camera->up, &targetxup);
		Vector3Normalize(&targetxup);

		camera->position = camera->position - SPEED * targetxup;
	}

	if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
	{
		CameraInfo* camera = (CameraInfo*)glfwGetWindowUserPointer(window);

		Vector3 targetxup;
		Vector3CrossProduct(&camera->target, &camera->up, &targetxup);
		Vector3Normalize(&targetxup);

		camera->position = camera->position + SPEED * targetxup;
	}
}

void cursor_pos_callback(GLFWwindow* window, double x, double y)
{
	CameraInfo* cameraInfo = (CameraInfo*)glfwGetWindowUserPointer(window);

	// Calcula el desplazamiento del mouse
	GLdouble offsetX = cameraInfo->mouseX - x;
	GLdouble offsetY = cameraInfo->mouseY - y;

	cameraInfo->mouseX = x;
	cameraInfo->mouseY = y;

	// Calcula los angulos horizontal y vertical en base al desplazamiento obtenido
	float horizontalAngle = offsetX / 5.0f;
	float verticalAngle = offsetY / 5.0f;

	// Rota el vector target con respecto al eje vertical de acuerdo al angulo horizontal
	Matrix3x3 rotation;
	Matrix3x3MakeRotationY(horizontalAngle, &rotation);

	Vector3 newTarget;
	Matrix3x3MultiplicationByVector(&rotation, &cameraInfo->target, &newTarget);

	Vector3Normalize(&newTarget);

	cameraInfo->target.x = newTarget.x;
	cameraInfo->target.y = newTarget.y;
	cameraInfo->target.z = newTarget.z;

	// Rota el vector target con respecto al eje horizontal de acuerdo al angulo vertical
	Vector3 horizontalAxis;
	Vector3CrossProduct(&cameraInfo->up, &newTarget, &horizontalAxis);
	Vector3Normalize(&horizontalAxis);

	Matrix3x3MakeRotation(&horizontalAxis, verticalAngle, &rotation);
	Matrix3x3MultiplicationByVector(&rotation, &cameraInfo->target, &newTarget);

	cameraInfo->target.x = newTarget.x;
	cameraInfo->target.y = newTarget.y;
	cameraInfo->target.z = newTarget.z;
}

#endif // CONTROL_H