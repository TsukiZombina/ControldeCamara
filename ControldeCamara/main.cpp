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

	GLfloat vertex;
	std::vector<GLfloat> vertices;
	std::ifstream ifsv("vertices.txt");
	if (ifsv.is_open())
	{
		while (ifsv >> vertex)
		{
			vertices.push_back(vertex);
		}
		ifsv.close();
	}
	else std::cerr << "Unable to open file" << endl;

	std::vector<GLuint> indices;
	std::ifstream ifsi("indices.txt");
	GLuint index;
	if (ifsi.is_open())
	{
		while (ifsi >> index)
		{
			indices.push_back(index);
		}
		ifsi.close();
	}
	else std::cerr << "Unable to open file" << endl;

	/*for (auto& verte : vertices)
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
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

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

		Matrix4x4MakeView(&camera.position, &camera.target, &camera.up, &view);
		Matrix4x4 modelViewProjection, temp;
		Matrix4x4Multiplication(&view, &model, &temp);
		Matrix4x4Multiplication(&projection, &temp, &modelViewProjection);

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