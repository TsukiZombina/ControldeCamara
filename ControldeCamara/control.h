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
	Matrix3x3 rotation;
	Vector3 target;
	Vector3 horizontalAxis;
	Vector3 normalizedVertical = {0.0f, 1.0f, 0.0f};

	CameraInfo* camera = (CameraInfo*)glfwGetWindowUserPointer(window);

	GLdouble offsetX = (camera->mouseX - x) * SPEEDC;
	GLdouble offsetY = (camera->mouseY - y) * SPEEDC;
	
	camera->mouseX = x;
	camera->mouseY = y;

	// First rotation: Horizontal angle respect to Y axis
	Matrix3x3MakeRotation(&normalizedVertical, offsetX, &rotation);
	Matrix3x3MultiplicationByVector(&rotation, &camera->target, &target);
	Vector3Normalize(&target);

	// Second rotation: vertical angle respect to horizontal axis
	Vector3CrossProduct(&camera->up, &target, &horizontalAxis);
	Vector3Normalize(&horizontalAxis);
	Matrix3x3MakeRotation(&horizontalAxis, offsetY, &rotation);
	Matrix3x3MultiplicationByVector(&rotation, &target, &target);
	camera->target = target;
}

#endif // CONTROL_H