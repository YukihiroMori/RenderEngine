#pragma once

#include "Main.h"

class Mesh
{
	GLuint vao;

public:
	Mesh() {
		glGenVertexArrays(1, &vao);
	}

	~Mesh() {
		glDeleteVertexArrays(1, &vao);
	}

	void use() const
	{
		glBindVertexArray(vao);
	}

	void unuse() const
	{
		glBindVertexArray(0);
	}

	GLuint get() const {
		return vao;
	}
};