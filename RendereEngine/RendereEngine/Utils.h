#pragma once

#include "Main.h"
#include "Paths.h"
#include "GLSLCompiler.h"
#include "Obj.h"
#include "AABB.h"

#define GL_ERROR(msg) {\
	GLenum error = glGetError();\
	if (error != GL_NO_ERROR){\
		if (msg) std::cerr << msg << ": ";\
		switch (error){\
		case GL_INVALID_ENUM:\
			std::cerr << "An unacceptable value is specified for an enumerated argument" << std::endl;\
			break;\
		case GL_INVALID_VALUE:\
			std::cerr << "A numeric argument is out of range" << std::endl;\
			break;\
		case GL_INVALID_OPERATION:\
			std::cerr << "The specified operation is not allowed in the current state" << std::endl;\
			break;\
		case GL_OUT_OF_MEMORY:\
			std::cerr << "There is not enough memory left to execute the command" << std::endl;\
			break;\
		case GL_INVALID_FRAMEBUFFER_OPERATION:\
			std::cerr << "The specified operation is not allowed current frame buffer" << std::endl;\
			break;\
		default:\
			std::cerr << "An OpenGL error has occured: " << std::hex << std::showbase << error << std::endl;\
			break;\
		}\
	}\
}\

namespace utils {
	GLuint LoadImage(const char *imagename);
	GLuint LoadMipMapImage(const char *imagename , GLint levels);
	GLuint LoadShader(const char *vert, const char *frag);

	bool LoadObj(const char *name, vector<Obj::grp> &group, vector<vec3> &pos, vector<vec3> &norm, bool normalized, AABB &aabb);
}

static GLboolean printShaderInfoLog(GLuint shader, const char *str);
static GLboolean printProgramInfoLog(GLuint program);
static bool readShaderSource(GLuint shader, const char *name);