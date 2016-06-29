#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "util.h"
#include "math3d.h"
#include "camera.h"

using std::cout;
using std::endl;

const int WIDTH = 512, HEIGHT = 512;

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

void cursor_pos_callback(GLFWwindow* window, double xPos, double yPos)
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

GLuint compileShaderObject(const GLchar* shaderCode, GLenum shaderType)
{
	GLuint shaderObject = glCreateShader(shaderType);

	if (shaderObject == 0)
	{
		cout << "Error creando shader del tipo: " << shaderType << endl;

		exit(1);
	}

	glShaderSource(shaderObject, 1, &shaderCode, NULL);
	glCompileShader(shaderObject);

	GLint success;
	GLchar infoLog[512];

	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderObject, 512, NULL, infoLog);

		cout << "Error compilando shader del tipo: " << shaderType << endl;
		cout << infoLog << endl;

		exit(1);
	}

	return shaderObject;
}

GLuint linkShaderProgram()
{
	GLuint shaderProgram = glCreateProgram();

	std::string shaderStr = loadShaderCode("../ControlDeCamara/vertex_shader.glsl");
	const GLchar* shaderCode = shaderStr.c_str();

	GLuint vertexShader = compileShaderObject(shaderCode, GL_VERTEX_SHADER);

	shaderStr = loadShaderCode("../ControlDeCamara/fragment_shader.glsl");
	shaderCode = shaderStr.c_str();

	GLuint fragmentShader = compileShaderObject(shaderCode, GL_FRAGMENT_SHADER);

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint success;
	GLchar infoLog[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

		cout << "Error de enlazado: " << infoLog << endl;

		exit(1);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
		7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
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