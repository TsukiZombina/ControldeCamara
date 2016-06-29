#pragma once

#include <iostream>
#include <fstream>
#include <GL/glew.h>

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