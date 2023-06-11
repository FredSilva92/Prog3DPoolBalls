#pragma region importações

#include <iostream>
#include <fstream>

#define GLEW_STATIC
#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_inverse.hpp>

#include "Shaders.h"

#pragma endregion


#pragma region funções

static const GLchar* readShader(const char* filename) {

	std::ifstream ficheiro(filename, std::ifstream::ate | std::ifstream::binary);

	if (!ficheiro.is_open()) {
		std::cerr << "Erro ao abrir o ficheiro '" << filename << "'" << std::endl;
		return nullptr;
	}

	std::streampos fileLengthInBytes = ficheiro.tellg();

	ficheiro.seekg(0, std::ios::beg);

	GLchar* source = new GLchar[int(fileLengthInBytes) + 1];

	ficheiro.read(source, fileLengthInBytes);

	source[fileLengthInBytes] = 0;

	ficheiro.close();

	return const_cast<const GLchar*>(source);
}

GLuint loadShaders(ShaderInfo* shaders) {

	if (shaders == nullptr) {
		return -1;
	}


	GLuint program = glCreateProgram();


	for (GLint i = 0; shaders[i].type != GL_NONE; i++) {
		shaders[i].shader = glCreateShader(shaders[i].type);

		const GLchar* source = readShader(shaders[i].filename);

		if (source == nullptr) {
			for (int j = 0; shaders[j].type != GL_NONE; j++) {
				if (shaders[j].shader != 0) {
					glDeleteShader(shaders[j].shader);
				}

				shaders[j].shader = 0;
			}

			return -1;
		}

		glShaderSource(shaders[i].shader, 1, &source, nullptr);
		delete[] source;

		glCompileShader(shaders[i].shader);

		GLint compiled;
		glGetShaderiv(shaders[i].shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled) {
			GLsizei length;
			glGetShaderiv(shaders[i].shader, GL_INFO_LOG_LENGTH, &length);

			GLchar* log = new GLchar[length + 1];
			glGetShaderInfoLog(shaders[i].shader, length, &length, log);
			std::cerr << "Erro ao compilar shaders: " << log << std::endl;
			delete[] log;

			for (int j = 0; shaders[j].type != GL_NONE; j++) {
				
				if (shaders[j].shader != 0) {
					glDeleteShader(shaders[j].shader);
				}

				shaders[j].shader = 0;
			}

			return -1;
		}

		glAttachShader(program, shaders[i].shader);
	}

	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (!linked) {
		GLsizei length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		GLchar* log = new GLchar[length + 1];
		glGetProgramInfoLog(program, length, &length, log);
		std::cerr << "Erro ao linkar shaders: " << log << std::endl;
		delete[] log;

		for (int j = 0; shaders[j].type != GL_NONE; j++) {
			if (shaders[j].shader != 0) {
				glDeleteShader(shaders[j].shader);
			}

			shaders[j].shader = 0;
		}

		return -1;
	}

	return program;
}

#pragma endregion