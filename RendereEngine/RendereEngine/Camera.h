#pragma once

#include "Main.h"

class Camera {
	int width;
	int height;

public:
	vec3 position;
	float fovY;
	vec3 lookat;
	float zNear, zFar;

	Camera() :
		 position(vec3(0.0f, 0.0f, 4.0f))
		, fovY(70)
		, lookat(vec3(0.0f, 0.0f, 0.0f))
		, zNear(1), zFar(100)
	{
	}

	mat4 getViewMatrix();

	mat4 getProjectionMatrix();

	void setSize(int w, int h) {
		width = w;
		height = h;
	}
};