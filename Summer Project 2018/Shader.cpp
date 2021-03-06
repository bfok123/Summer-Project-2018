#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	//FILE READING
	// char* vertexCode, * fragmentCode;
	std::string vertexCode, fragmentCode; // unfiltered code
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
		// vertexCode = new char[std::strlen(vertexStream.str().c_str()) + 1];
		// std::strcpy(vertexCode, vertexStream.str().c_str()); //_CRT_SECURE_NO_WARNINGS
		// fragmentCode = new char[std::strlen(fragmentStream.str().c_str()) + 1];
		// std::strcpy(fragmentCode, fragmentStream.str().c_str());
		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();

		std::cout << vertexCode << std::endl;
		std::cout << fragmentCode << std::endl;
	} catch (std::ifstream::failure e) {
		std::cout << "Failed to read shader files. :(" << std::endl;
	}

	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragmentShaderCode = fragmentCode.c_str();

	//COMPILATION
	unsigned int vertexShader, fragmentShader;
	int success;
	char infoLog[512];
	//Vertex Compilation
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);// &vertexCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex shader failed to compile! :(\n" << infoLog << std::endl;
		return;
	}
	//Fragment Compilation
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);// &fragmentCode, NULL);
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
				vector.elements[Coordinates::X], vector.elements[Coordinates::Y]);
}

void Shader::setUniform3f(const std::string &name, const Vector& vector) {
	glUniform3f(glGetUniformLocation(id, name.c_str()),
				vector.elements[Coordinates::X], vector.elements[Coordinates::Y],
				vector.elements[Coordinates::Z]);
}

void Shader::setUniform4f(const std::string &name, const Vector& vector) {
	glUniform4f(glGetUniformLocation(id, name.c_str()),
				vector.elements[Coordinates::X], vector.elements[Coordinates::Y],
				vector.elements[Coordinates::Z], vector.elements[Coordinates::W]);
}

void Shader::setUniformMatrix4(const std::string &name, const Matrix& matrix) {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, matrix.elements);
}
