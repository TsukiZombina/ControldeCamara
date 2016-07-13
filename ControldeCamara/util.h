#pragma once

#include <iostream>
#include <fstream>
#include <GL/glew.h>

using std::cout;
using std::endl;

std::string loadShaderCode(const char* filename)
{
	std::string shaderCode;
	std::ifstream file(filename, std::ios::in);

	if (!file.good())
	{
		std::cout << "Can't read file " << filename << std::endl;

		exit(1);
	}

	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());

	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();

	return shaderCode;
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

	std::string shaderStr = loadShaderCode("vertex_shader.glsl");
	const GLchar* shaderCode = shaderStr.c_str();

	GLuint vertexShader = compileShaderObject(shaderCode, GL_VERTEX_SHADER);

	shaderStr = loadShaderCode("fragment_shader.glsl");
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

template<class T>
std::vector<T>& readVector(std::vector<T>& data, const char* filename) {
	std::ifstream ifs(filename);
	T element;
	if (ifs.is_open())
	{
		while (ifs >> element)
			data.push_back(element);
		ifs.close();
	}
	else std::cerr << "Unable to open file" << endl;
	return data;
}