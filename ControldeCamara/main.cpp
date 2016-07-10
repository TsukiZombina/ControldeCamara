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

	GLfloat vertices[] =
	{
		// Posicion				// Color

		-0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f,
		0.0f,  0.5f,  0.0f,   1.0f, 0.0f, 1.0f, 1.0f
	};

	GLuint indices[] =
	{
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		1, 2, 0
	};

	/*std::vector<GLfloat> vertices;
	std::ifstream ifsv("vertices.txt");
	GLfloat vertex;
	while (ifsv >> vertex)
	{
	vertices.push_back(vertex);
	}
	ifsv.close();*/

	/*std::vector<GLfloat> indices;
	std::ifstream ifsi("indices.txt");
	GLfloat index;
	while (ifsi >> index)
	{
		indices.push_back(index);
	}
	ifsi.close();

	for (auto& verte : vertices)
		cout << verte << " ";
	for (auto& indi : indices)
		cout << indi << " ";*/

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

	GLuint vao, vbo, ebo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
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

		Vector3 newTarget = camera.position + camera.target;
		Matrix4x4MakeView(&camera.position, &newTarget, &camera.up, &view);
		modelViewProjection = projection * view * model;

		// Dibuja
		glUseProgram(shaderProgram);

		GLint location = glGetUniformLocation(shaderProgram, "modelViewProjection");
		glUniformMatrix4fv(location, 1, GL_TRUE, (GLfloat*)modelViewProjection.m);

		glBindVertexArray(vao);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();

	return 0;
}