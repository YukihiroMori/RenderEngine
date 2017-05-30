#pragma once

#include "Main.h"
#include "Utils.h"

using namespace utils;

class Shader {
public:
	virtual ~Shader() {
		if (program != 0) {
			glDeleteProgram(program);
			program = 0;
		}
	}

	Shader() : program(0) {}

	Shader(const char *vert, const char *frag = 0) : program(LoadShader(vert, frag)) {
	}

	virtual void use(void) const;

	virtual void unuse(void) const;

	virtual GLuint get() const;

	bool UpdateUniform(int programId, int location, const mat4& m)
	{
		if (location == -1) return false;
		glProgramUniformMatrix4fv(programId, location, 1, GL_FALSE, value_ptr(m));
		return location > -1;
	}

	bool UpdateUniform(const std::string& name, const mat4& m)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1) return false;
		return UpdateUniform(program, location, m);
	}

	bool UpdateUniform(const std::string& name, const vec2& v)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1) return false;
		glProgramUniform2fv(program, location, 1, value_ptr(v));
		return location > -1;
	}

	bool UpdateUniform(const std::string& name, const vec3& v)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1) return false;
		glProgramUniform3fv(program, location, 1, value_ptr(v));
		return location > -1;
	}

	bool UpdateUniform(const std::string& name, const vec4& v)
	{
		GLint location = GetUniformLocation(name);
		if (location > -1)
			glProgramUniform4fv(program, location, 1, value_ptr(v));
		return location > -1;
	}

	bool UpdateUniform(int programId, int location, const vec4& v)
	{
		glProgramUniform4fv(programId, location, 1, value_ptr(v));
		return location > -1;
	}

	bool UpdateUniform(int programId, int location, const vec3& v)
	{
		glProgramUniform3fv(programId, location, 1, value_ptr(v));
		return location > -1;
	}

	bool UpdateUniform(const string& name, float f)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1) return false;
		glProgramUniform1f(program, location, f);
		return location > -1;
	}

	bool UpdateUniformi(const string& name, int i)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1) return false;
		glProgramUniform1i(program, location, i);
		return location > -1;
	}

	bool UpdateUniformiv(const string& name, int nums, int* i)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1) return false;
		glProgramUniform1iv(program, location, nums, i);
		return location > -1;
	}

	bool UpdateUniformui(const string& name, GLuint ui)
	{
		GLint location = GetUniformLocation(name);
		if (location == -1) return false;
		glProgramUniform1ui(program, location, ui);
		return location > -1;
	}

	int GetUniformLocation(GLuint program, const std::string& name)
	{
		return glGetUniformLocation(program, name.c_str());
	}

	int GetUniformLocation(const string& name)
	{
		const auto& f = m_uniformLocations.find(name);
		if (f != m_uniformLocations.end())
		{
			return f->second;
		}

		GLint location = glGetUniformLocation(program, name.c_str());
		m_uniformLocations[name] = location;

		//cout << "Locate " << name << endl;

		return location;
	}

protected:
	GLuint program;

	map<string, GLint> m_uniformLocations;
};


