#pragma once

#include "Main.h"

class Trackball {
	vec2 cpos;
	bool drag;
	vec2 scale;

	quat temp;

	mat4 rotation;

public:

	virtual ~Trackball() {}

	Trackball() {
		reset();
	}

	void region(int w, int h);

	void start(float x, float y);

	void motion(float x, float y);

	void rotate(quat q);

	void stop(float x, float y);

	void reset();

	mat4 get() const {
		return rotation;
	}

	quat getQuat() const {
		return temp;
	}

};