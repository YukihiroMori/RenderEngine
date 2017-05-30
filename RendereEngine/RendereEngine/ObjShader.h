#pragma once

#include "Main.h"
#include "Shader.h"
#include "Utils.h"

using namespace utils;

class ObjShader : Shader {
	bool isresisterd;

public:

	ObjShader(const char* vert, const char* frag);

	virtual void use() const {
		Shader::use();

		//glUniform4fv(pl, 1, value_ptr(l.pos));
		//glUniform4fv(lamb, 1, value_ptr(l.amb));
		//glUniform4fv(ldiff, 1, value_ptr(l.diff));
		//glUniform4fv(lspec, 1, value_ptr(l.spec));

		glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, value_ptr(m.ProjectionMatrix));
		glUniformMatrix4fv(uniform_view, 1, GL_FALSE, value_ptr(m.ViewMatrix));
		glUniformMatrix4fv(uniform_model, 1, GL_FALSE, value_ptr(m.ModelMatrix));
		glUniformMatrix4fv(uniform_light, 1, GL_FALSE, value_ptr(m.LightMatrix));
	}

	void setMaterial(vec4 amb, vec4 diff, vec4 spec, float shi) {
		if (!isresisterd) {
			glUniform4fv(kamb, 1, value_ptr(amb));
			glUniform4fv(kdiff, 1, value_ptr(diff));
			glUniform4fv(kspec, 1, value_ptr(spec));
			glUniform1f(kshi, shi);
			isresisterd = true;
		}
	}

	void setProjectionMatrix(mat4 projection) {
		m.ProjectionMatrix = projection;
	}

	void setViewMatrix(mat4 view) {
		m.ViewMatrix = view;
	}

	void setModelMatrix(mat4 model) {
		m.ModelMatrix = model;
	}

	void setLightMatrix(mat4 light) {
		m.LightMatrix = light;
	}

	virtual void unuse() const {
		Shader::unuse();
	}

	GLuint get() const {
		return Shader::get();
	}

	class light {
	public:
		vec4 pos;
		vec4 amb;
		vec4 diff;
		vec4 spec;
	};

	class Matrix {
	public:
		mat4 ProjectionMatrix;
		mat4 ViewMatrix;
		mat4 ModelMatrix;
		mat4 LightMatrix;
	};

	light l;
	Matrix m;

	GLint pl;
	GLint lamb;
	GLint ldiff;
	GLint lspec;
	GLint kamb;
	GLint kdiff;
	GLint kspec;
	GLint kshi;

	GLint uniform_projection;
	GLint uniform_view;
	GLint uniform_model;
	GLint uniform_light;
};