#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	//FILE READING
	char* vertexCode, * fragmentCode;
	std::ifstream vertexFile, fragmentFile;
	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//open files
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);
		std::stringstream vertexStream, fragmentStream;
		//load the contents of the files into the string
		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();
		//close file handlers
		vertexFile.close();
		fragmentFile.close();
		//convert streams into strings
		vertexCode = new char[std::strlen(vertexStream.str().c_str()) + 1];
		std::strcpy(vertexCode, vertexStream.str().c_str()); //_CRT_SECURE_NO_WARNINGS
		fragmentCode = new char[std::strlen(fragmentStream.str().c_str()) + 1];
		std::strcpy(fragmentCode, fragmentStream.str().c_str());
	} catch (std::ifstream::failure e) {
		std::cout << "Failed to read shader files. :(" << std::endl;
	}

	//COMPILATION
	unsigned int vertexShader, fragmentShader;
	int success;
	char infoLog[512];
	//Vertex Compilation
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex shader failed to compile! :(\n" << infoLog << std::endl;
		return;
	}
	//Fragment Compilation
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Fragment shader failed to compile! :(\n" << infoLog << std::endl;
		return;
	}
	//SHADER PROGRAM
	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "Shader program failed to compile! :(\n" << infoLog << std::endl;
		return;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() {
	glUseProgram(id);
}

void Shader::setBool(const std::string &name, const bool value) {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int) value);
}

void Shader::setUniform1i(const std::string &name, const int value) {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setUniform1f(const std::string &name, const float value) {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}


//THIS IS HOW WE'RE GOING TO USE OUR VECTOR/MATRIX CLASSES
//MATRIX CLASS NEED TO BE 1D ARRAY
//CHANGE EVERYTHING TO GLFLOAT FOR CONSISTENCY?
void Shader::setUniform2f(const std::string &name, const Vector& vector) {
	glUniform2f(glGetUniformLocation(id, name.c_str()),
				vector[Coordinates::X], vector[Coordinates::Y]);
}

void Shader::setUniform3f(const std::string &name, const Vector& vector) {
	glUniform3f(glGetUniformLocation(id, name.c_str()),
				vector[Coordinates::X], vector[Coordinates::Y],
				vector[Coordindates::Z]);
}

void Shader::setUniform4f(const std::string &name, const Vector& vector) {
	glUniform4f(glGetUniformLocation(id, name.c_str()),
				vector[Coordinates::X], vector[Coordinates::Y],
				vector[Coordinates::Z], vector[Coordinates::W]);
}

void Shader::setUniformMatrix(const std::string &name, const Matrix& matrix) {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, matrix.elements);
}
