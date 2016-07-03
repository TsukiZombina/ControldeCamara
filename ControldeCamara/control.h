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

	if (key == GLFW_KEY_W)
	{
		CameraInfo* camera = (CameraInfo*)glfwGetWindowUserPointer(window);
		camera->position = camera->position + camera->speed * camera->target;
	}

	if (key == GLFW_KEY_S)
	{
		CameraInfo* camera = (CameraInfo*)glfwGetWindowUserPointer(window);
		camera->position = camera->position - camera->speed * camera->target;
	}

	if (key == GLFW_KEY_A)
	{
		CameraInfo* camera = (CameraInfo*)glfwGetWindowUserPointer(window);

		Vector3 exu;
		Vector3CrossProduct(&camera->target, &camera->up, &exu);
		Vector3Normalize(&exu);

		camera->position = camera->position - camera->speed * exu;
	}

	if (key == GLFW_KEY_D)
	{
		CameraInfo* camera = (CameraInfo*)glfwGetWindowUserPointer(window);

		Vector3 exu;
		Vector3CrossProduct(&camera->target, &camera->up, &exu);
		Vector3Normalize(&exu);

		camera->position = camera->position + camera->speed * exu;
	}
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	CameraInfo* cameraInfo = (CameraInfo*)glfwGetWindowUserPointer(window);

	// Calcula el desplazamiento del mouse
	GLfloat mouseOffsetX = (float)(cameraInfo->mouseX - xPos);
	GLfloat mouseOffsetY = (float)(cameraInfo->mouseY - yPos);

	cameraInfo->mouseX = xPos;
	cameraInfo->mouseY = yPos;

	// Calcula los angulos horizontal y vertical en base al desplazamiento obtenido
	float horizontalAngle = mouseOffsetX / 5.0f;
	float verticalAngle = mouseOffsetY / 5.0f;

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