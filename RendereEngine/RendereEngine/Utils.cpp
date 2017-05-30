#include "Utils.h"


GLuint utils::LoadImage(const char *imagename) {

	string path(ImageResourcesPath);
	path += imagename;
	cv::Mat image = cv::imread(path);

	if (image.empty()) {
		cerr << "image empty" << endl;
		return 0;
	}

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	//cv::flip(image, image, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.ptr());

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

GLuint utils::LoadMipMapImage(const char *imagename, GLint levels) {

	string path(ImageResourcesPath);
	path += imagename;
	cv::Mat image = cv::imread(path);

	if (image.empty()) {
		cerr << "image empty" << endl;
		return 0;
	}

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	//cv::flip(image, image, 0);

	vector<cv::Mat> l(levels);
	for (GLint level = 0; level < levels; level++)
	{
		resize(image, l[level], cv::Size(), pow(0.5,level) , pow(0.5,level) , cv::INTER_AREA);
		glTexImage2D(GL_TEXTURE_2D, level, GL_RGB, l[level].cols, l[level].rows, 0, GL_BGR, GL_UNSIGNED_BYTE, l[level].ptr());
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

GLuint utils::LoadShader(const char *vert, const char *frag) {
	GLSLCompiler compiler;
	compiler.AddIncludePath(ShaderResourcesPath);

	GLuint program = glCreateProgram();

	string vpath(ShaderResourcesPath);
	vpath += vert;
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	if (!compiler.CompileFromFile(vertShader, vpath.c_str())) {
		glDeleteShader(vertShader);
		glDeleteProgram(program);

		return 0;
	}

	glAttachShader(program, vertShader);
	glDeleteShader(vertShader);

	string fpath(ShaderResourcesPath);
	fpath += frag;
	if (frag) {
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

		if (!compiler.CompileFromFile(fragShader, fpath.c_str())) {
			glDeleteShader(fragShader);
			glDeleteProgram(program);
			return 0;
		}

		glAttachShader(program, fragShader);
		glDeleteShader(fragShader);
	}

	glLinkProgram(program);
	if (printProgramInfoLog(program) == GL_FALSE) {
		glDeleteProgram(program);
		return 0;
	}

	return program;
}

static GLboolean printProgramInfoLog(GLuint program) {
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) cerr << "Link Error." << endl;

	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1)
	{
		vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
		cerr << &infoLog[0] << endl;
	}

	return (GLboolean)status;
}