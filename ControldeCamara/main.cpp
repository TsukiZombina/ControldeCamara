#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>

#include "util.h"
#include "math3d.h"
#include "camera.h"
#include "control.h"

using std::cout;
using std::endl;

const int WIDTH = 512, HEIGHT = 512;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL App",
		nullptr, nullptr);

	if (window == nullptr)
	{
		cout << "Error creando la ventana de GLFW" << endl;

		return 1;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;

	GLenum result = glewInit();

	if (result != GLEW_OK)
	{
		cout << "Error inicializando GLEW" << endl;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	GLuint shaderProgram = linkShaderProgram();

	std::vector<GLfloat> vertices;
	readVector(vertices, "vertices.txt");

	CameraInfo camera;
	camera.position = { 0.0f, 0.0f,  3.0f };
	camera.target = { 0.0f, 0.0f, -1.0f };
	camera.up = { 0.0f, 1.0f,  0.0f };
	camera.mouseX = WIDTH / HEIGHT;
	camera.mouseY = WIDTH / HEIGHT;

	glfwSetWindowUserPointer(window, (GLvoid*)&camera);

	Matrix4x4 model;
	Matrix4x4MakeIdentity(&model);

	Matrix4x4 view;

	Matrix4x4 projection;
	Matrix4x4MakePerspective(45.0f, WIDTH / HEIGHT, 0.1f, 100.0f, &projection);

	Matrix4x4 modelViewProjection;

	GLuint vao, vbo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,	7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Matrix4x4MakeView(&camera.position, &camera.target, &camera.up, &view);
		Matrix4x4 modelViewProjection, temp;
		Matrix4x4Multiplication(&view, &model, &temp);
		Matrix4x4Multiplication(&projection, &temp, &modelViewProjection);

		// Dibuja
		glUseProgram(shaderProgram);

		GLint location = glGetUniformLocation(shaderProgram, "modelViewProjection");
		glUniformMatrix4fv(location, 1, GL_TRUE, (GLfloat*)modelViewProjection.m);

		glBindVertexArray(vao);

		glDrawArrays(GL_LINE_STRIP, 0, 12);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();

	return 0;
}