#pragma once
#ifndef CONTROL_H
#define CONTROL_H

#include <iostream>
#include <fstream>
#include <GL/glew.h>

#include "camera.h"
#include "vector3D.h"
#include "squarematrix.h"

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

		Vector3D<float> targetxup;
		targetxup = CrossNormalize(camera->target, camera->up);
		camera->position = camera->position - SPEEDK * targetxup;
	}

	if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
	{
		CameraInfo* camera = (CameraInfo*)glfwGetWindowUserPointer(window);

		Vector3D<float> targetxup;
		targetxup = CrossNormalize(camera->target, camera->up);
		camera->position = camera->position + SPEEDK * targetxup;
	}
}

void cursor_pos_callback(GLFWwindow* window, double x, double y)
{
	SquareMatrix<float, 3> rotation;
	Vector3D<float> target;
	Vector3D<float> horizontalAxis;
	Vector3D<float> normalizedVertical(0.0f, 1.0f, 0.0f);

	CameraInfo* camera = (CameraInfo*)glfwGetWindowUserPointer(window);

	GLfloat offsetX = (camera->mouseX - x) * SPEEDC;
	GLfloat offsetY = (camera->mouseY - y) * SPEEDC;
	
	camera->mouseX = x;
	camera->mouseY = y;

	// First rotation: Horizontal angle respect to Y axis
	rotation.MakeRotation(normalizedVertical, offsetX);
	target = rotation * camera->target;
	target = target.Normalize();

	// Second rotation: vertical angle respect to horizontal axis
	horizontalAxis = CrossNormalize(camera->up, target);
	rotation.MakeRotation(horizontalAxis, offsetY);
	target = rotation * target;

	camera->target = target;
}

#endif // CONTROL_H