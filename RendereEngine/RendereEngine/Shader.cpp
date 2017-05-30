#include "Shader.h"

void Shader::use() const {
	glUseProgram(program);
}

void Shader::unuse() const {
	glUseProgram(0);
}

GLuint Shader::get() const {
	return program;
}
