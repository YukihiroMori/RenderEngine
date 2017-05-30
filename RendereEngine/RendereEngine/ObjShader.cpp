#include "ObjShader.h"

ObjShader::ObjShader(const char *vert, const char *frag) : Shader(vert, frag), isresisterd(false) {
	GLuint program = get();

	//pl = glGetUniformLocation(program, "pl");
	//lamb = glGetUniformLocation(program, "lamb");
	//ldiff = glGetUniformLocation(program, "ldiff");
	//lspec = glGetUniformLocation(program, "lspec");
	kamb = glGetUniformLocation(program, "kamb");
	kdiff = glGetUniformLocation(program, "kdiff");
	kspec = glGetUniformLocation(program, "kspec");
	kshi = glGetUniformLocation(program, "kshi");

	uniform_projection = glGetUniformLocation(program, "ProjectionMatrix");
	uniform_view = glGetUniformLocation(program, "ViewMatrix");
	uniform_model = glGetUniformLocation(program, "ModelMatrix");
	uniform_light = glGetUniformLocation(program, "LightMatrix");
}