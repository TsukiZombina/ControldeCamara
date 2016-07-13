#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>

#include "util.h"
#include "camera.h"
#include "control.h"
#include "hermitecurve.h"
#include "perspective.h"

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

	Vector3D<float> P1(-0.5, -0.5, 0), P2(0.5, 0.5, 0), T1(1.0,-1.0, 0.0), T2(1.0, -1.0, 0.0);
	HermiteCurve H;
	H = HermiteCurve(P1, P2, T1, T2);
	float vertices[707];
	for (int i = 0; i <= 100; i++) {
		Vector3D<double> fill = H(0.01 * i);
		vertices[7 * i] = fill(0);
		//cout << fill(0) << " ";
		vertices[7 * i + 1] = fill(1);
		//cout << fill(1) << " ";
		vertices[7 * i + 2] = fill(2);
		//cout << fill(2) << endl;
		vertices[7 * i + 3] = 1.0;
		vertices[7 * i + 4] = 0.0;
		vertices[7 * i + 5] = 0.0;
		vertices[7 * i + 6] = 1.0;
	}

	CameraInfo camera;
	camera.position.set(0.0f, 0.0f,  3.0f);
	camera.target.set(0.0f, 0.0f, -1.0f);
	camera.up.set(0.0f, 1.0f,  0.0f);
	camera.mouseX = WIDTH / HEIGHT;
	camera.mouseY = WIDTH / HEIGHT;

	glfwSetWindowUserPointer(window, (GLvoid*)&camera);

	SquareMatrix<GLfloat, 4> model;
	SquareMatrix<GLfloat, 4> view;
	SquareMatrix<GLfloat, 4> projection;
	SquareMatrix<GLfloat, 4> modelView;
	model.Identity();
	
	MakePerspective(45.0f, WIDTH / HEIGHT, 0.1f, 100.0f, projection);
	
	GLfloat mvp[4][4];

	GLuint vao, vbo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MakeView(camera.position, camera.target, camera.up, view);
		
		Matrix<GLfloat, 4, 4> modelViewProjection;
		modelViewProjection = projection * (model * view);
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				cout << modelViewProjection(i, j) << " ";
			}
			cout << endl;
		}

		// Dibuja
		glUseProgram(shaderProgram);

		GLint location = glGetUniformLocation(shaderProgram, "modelViewProjection");
		glUniformMatrix4fv(location, 1, GL_TRUE, modelViewProjection.GetArray());
	
		glBindVertexArray(vao);

		glDrawArrays(GL_LINE_STRIP, 0, 101);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();

	return 0;
}