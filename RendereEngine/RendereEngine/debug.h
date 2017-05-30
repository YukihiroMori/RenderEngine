#pragma once

#include "Main.h"

static void error_callback(int error, const char* description)
{
	cerr << description << endl;
}

void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void* userParam);

