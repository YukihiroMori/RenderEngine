#pragma once

#include "Main.h"
#include "Transform.h"

class Light {
public:

	Light() {
		Init();
	}

	~Light() {
		glDeleteTextures(1, &shadowMapTexture);
		glDeleteFramebuffers(1, &shadowMapFBO);
	}

	void Init() {
		glGenTextures(1, &shadowMapTexture);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, shadowMapResolution, shadowMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		vec4 ones(1.0);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, value_ptr(ones));

		glGenFramebuffers(1, &shadowMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void use() {
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);

		glPolygonOffset(1.0f, 1.0f);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);

		glCullFace(GL_FRONT);
		glViewport(0, 0, shadowMapResolution, shadowMapResolution);

		glClear(GL_DEPTH_BUFFER_BIT);

		glEnable(GL_POLYGON_OFFSET_FILL);
	}

	void unuse() {
		glDisable(GL_POLYGON_OFFSET_FILL);

		glDisable(GL_CULL_FACE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLuint shadowMapFBO;
	GLuint shadowMapTexture;

	Transform transform;

	mat4 ProjectionMatrix;
	mat4 ViewMatrix;

	int shadowMapResolution = 512;

	mat4 getLightMatrix() {
		ProjectionMatrix = ortho(-4.0, 4.0, -4.0, 4.0, 1.0, 10.0); //perspective(70.0f, 1.0f, 1.0f, 10.0f);//if use perspective, it mean spot light 
		ViewMatrix = lookAt(transform.position, vec3(0, 0, 0), vec3(0, 1, 0));
		return ProjectionMatrix * ViewMatrix;
	}

};