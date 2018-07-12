#ifndef SHADER
#define SHADER

#include "GL\glew.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "MathStuff.h"

class Shader {
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	unsigned int id; //for using shader program
	void use();
	void setBool(const std::string &name, const bool value);
	void setUniform1i(const std::string &name, const int value);
	void setUniform1f(const std::string &name, const float value);
	void setUniform2f(const std::string &name, const Vector& vector);
	void setUniform3f(const std::string &name, const Vector& vector);
	void setUniform4f(const std::string &name, const Vector& vector);
	void setUniformMatrix4(const std::string &name, const Matrix& vector);
};

#endif