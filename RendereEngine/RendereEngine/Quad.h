#pragma once

#include "Main.h"
#include "Mesh.h"

class Quad
	: public Mesh
{
public:
	Transform transform;

	~Quad() {
		use();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &vbo);

		unuse();
	};

	Quad(vector<vec3> pos, GLenum mode = GL_POINTS, GLenum usage = GL_STATIC_DRAW)
		: Mesh(),
		position(pos),
		mode(mode)
	{
		glGenBuffers(1, &vbo);
		loadPosition(pos, usage);
	};

	void draw(int first = 0, int count = 0) {
		use();

		glDrawArrays(mode, first, count > 0 ? count : pnum() - first);

		unuse();
	}


private:
	GLuint vbo;
	vector<vec3> position;
	GLenum mode;

	GLenum getMode() {
		return this->mode;
	}

	void setMode(GLenum mode) {
		this->mode = mode;
	}

	int pnum() {
		return position.size();
	}

	void loadPosition(vector<vec3> pos, GLenum usage) {
		use();

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * pos.size(), &pos[0], usage);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		unuse();
	};
};