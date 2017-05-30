#include "Main.h"
#include "Window.h"
#include "Camera.h"
#include "Shader.h"
#include "ObjMesh.h"
#include "Utils.h"
#include "Trackball.h"
#include "Quad.h"
#include "Light.h"

using namespace utils;

Transform T;

#define FBOWIDTH 2048	
#define FBOHEIGHT 2048

static GLint translucency = 0.0;

static GLfloat border[] = { 0.0f, 0.0f, 0.0f, 0.0f };
static vec4 ambient = vec4(0.3f, 0.3f, 0.3f, 1.0f);

GLuint g_m = 8;
GLfloat g_roughness = 0.0f;
GLfloat g_R0 = 0.3f;

GLint levels = 5.0;


GLuint attachTexture(GLint internal, GLint format, GLsizei width, GLsizei height, GLenum attachment) {
	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, (format == GL_BGRA || format == GL_RGBA) ? 4 : 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

	glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, format, GL_UNSIGNED_BYTE, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

GLuint prepareFBO(vector<GLenum> &buf, vector<GLuint> &tex){

	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//ägéUîΩéÀ D
	tex[0] = attachTexture(GL_RGBA, GL_RGBA, FBOWIDTH, FBOHEIGHT, buf[0]);
	
	//ãæñ îΩéÀ S
	tex[1] = attachTexture(GL_RGBA, GL_RGBA, FBOWIDTH, FBOHEIGHT, buf[1]);
	
	//à íu P
	tex[2] = attachTexture(GL_RGBA16F, GL_RGB, FBOWIDTH, FBOHEIGHT, buf[2]);
	
	//ñ@ê¸ N
	tex[3] = attachTexture(GL_RGBA16F, GL_RGB, FBOWIDTH, FBOHEIGHT, buf[3]);
	
	//ê[ìx
	tex[4] = attachTexture(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, FBOWIDTH, FBOHEIGHT, GL_DEPTH_ATTACHMENT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return fbo;
}

static int irselect = 0;

int main(int argc, const char* argv[]) {

	if (glfwInit() == GL_FALSE) {
		cerr << "Can't initialize GLFW" << endl;
		exit(1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	Window window("Default Window" , 640, 480);
	Camera camera = window.getCamera();
	camera.position = vec3(0.0f, 4.0f, 5.0f);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	vector<string> texfile =
	{
		//"white.hdr",
		//"ProvWash_Ref.hdr",
		//"Cave-Room_Ref.hdr",
		//"source.hdr",
		//"irradiance.hdr",
		"Gold-Room_Ref.hdr"
	};

	vector<GLuint> texname(texfile.size());
	glGenTextures(texfile.size(), &texname[0]);
	for (int i = 0; i < texfile.size(); ++i)
	{
		texname[i] = LoadMipMapImage(texfile[i].c_str() , levels);
	}

	vector<GLenum> buf = {
		GL_COLOR_ATTACHMENT0, //ägéUîΩéÀåı
		GL_COLOR_ATTACHMENT1, //ä¬ã´åı
		GL_COLOR_ATTACHMENT2, //à íu
		GL_COLOR_ATTACHMENT3  //ñ@ê¸
	};

	GLsizei texnum = buf.size() + 2;
	vector<GLuint> tex(texnum);

	GLuint fbo = prepareFBO(buf, tex);

	Light light;
	light.transform.position = vec3(4.0, 4.0, 4.0);

	Shader pass1("pass1.vert","pass1.frag");
	Shader pass2("PBR.vert", "PBR.frag");
	Shader shadow("shadow.vert", "shadow.frag");
	Shader prev("prev.vert", "prev.frag");

	
	Obj::mat mat(
		vec4(0.1f, 0.1f, 0.1f, 1.0f),
		vec4(0.6f, 0.6f, 0.6f, 1.0f),
		vec4(0.3f, 0.3f, 0.3f, 1.0f),
		60.0f, 1.0f);

	pass1.UpdateUniform("kamb", mat.amb);
	pass1.UpdateUniform("kdiff", mat.diff);
	pass1.UpdateUniform("kspec", mat.spec);
	pass1.UpdateUniform("kshi", mat.shi);
	

	GLint unit[6];
	for (GLint i = 0; i < texnum; ++i) unit[i] = i;

	pass2.use();

	pass2.UpdateUniformui("NumSamples", 1 << g_m);
	pass2.UpdateUniformui("u_m", g_m);
	pass2.UpdateUniform("roughness", g_roughness);
	pass2.UpdateUniform("f0", g_R0);

	pass2.UpdateUniform("ambient", ambient);
	pass2.UpdateUniformiv("unit", texnum, unit);

	pass2.UpdateUniform("translucency", translucency * 0.01f);

	pass2.unuse();


	ObjMesh obj("bunny.obj", true);
	obj.attachShader(pass1);
	obj.attachShadowShader(shadow);
	
	ObjMesh cube("cube.obj" , true);
	cube.attachShader(pass1);
	cube.attachShadowShader(shadow);

	cube.transform.position = vec3(0.0f, -1.0f, 0.0f);
	cube.transform.scale = vec3(4.0f, 0.2f, 4.0f);


	vector<vec3> position = {
		vec3( -1.0f, -1.0f, 0.0f),
		vec3(  1.0f, -1.0f, 0.0f),
		vec3( -1.0f,  1.0f, 0.0f),
		vec3(  1.0f,  1.0f, 0.0f)
	};

	Quad quad(position, GL_TRIANGLE_STRIP);

	vector<vec3> position2 = {
		vec3(-1.0f, -1.0f, 0.0f),
		vec3(-0.7f, -1.0f, 0.0f),
		vec3(-1.0f, -0.7f, 0.0f),
		vec3(-0.7f, -0.7f, 0.0f)
	};

	Quad quad2(position2, GL_TRIANGLE_STRIP);

	while(window.shouldClose() == GL_FALSE){

		/* shadow pass */
		shadow.use();

		shadow.UpdateUniform("lightMatrix" , light.getLightMatrix());

		shadow.unuse();

		light.use();

		obj.shadow_pass();

		cube.shadow_pass();

		light.unuse();

		/* shadow pass */

		/* main pass */

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glDrawBuffers(buf.size(), &buf[0]);

		glViewport(0, 0, FBOWIDTH, FBOHEIGHT);

		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);

		glEnable(GL_DEPTH_TEST);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pass1.UpdateUniform("ProjectionMatrix", camera.getProjectionMatrix());

		pass1.UpdateUniform("ViewMatrix", camera.getViewMatrix());

		pass1.UpdateUniform("LightMatrix", light.getLightMatrix());

		obj.transform.rotation = window.getLeftTrack().getQuat();

		obj.draw();

		cube.draw();

		glDisable(GL_CULL_FACE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/* main pass */

		/* post pass */

		glDrawBuffer(GL_BACK);

		window.setViewPort();

		glDisable(GL_DEPTH_TEST);

		tex[5] = texname[irselect];
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 proj = camera.getProjectionMatrix();
		mat4 view = camera.getViewMatrix();

		T.rotation = window.getRightTrack().getQuat();

		pass2.use();
		
		pass2.UpdateUniform("ProjectionMatrix", proj);
		pass2.UpdateUniform("ViewMatrix", view);
		pass2.UpdateUniform("ModelMatrix", T.asMatrix());
		pass2.UpdateUniform("lightMatrix", light.getLightMatrix());

		for (GLsizei i = 0; i < texnum; ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, tex[i]);
		}

		pass2.UpdateUniformi("shadowMap", texnum);

		glActiveTexture(GL_TEXTURE0 + texnum);
		glBindTexture(GL_TEXTURE_2D, light.shadowMapTexture);

		quad.draw();

		pass2.unuse();

		/* post pass */

		/* prev pass */
		prev.use();

		prev.UpdateUniformi("shadow", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, light.shadowMapTexture);

		quad2.draw();

		prev.unuse();

		/* prev pass */

		window.swapBuffers();
	}

	for (GLsizei i = 0; i < texnum; ++i) {
		glDeleteTextures(1, &tex[i]);
	}
	glDeleteFramebuffers(1, &fbo);

	glfwTerminate();

	return 0;
}