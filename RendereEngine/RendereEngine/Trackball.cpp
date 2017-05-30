#include "Trackball.h"

void Trackball::reset() {
	drag = false;

	temp = quat(1.0, 0.0, 0.0, 0.0);

	rotation = mat4_cast(temp); //temp;
}

void Trackball::region(int w,int h) {
	scale.x = 1.0f / float(w);
	scale.y = 1.0f / float(h);
}

void Trackball::start(float x, float y) {
	drag = true;

	cpos.x = x;
	cpos.y = y;
}

void Trackball::motion(float x, float y) {
	if (drag) {
		vec2 d = (vec2(x, y) - cpos) * scale;

		float a = length(d);

		if (a != 0.0) {
			float rad = a * M_PI * 0.2;
			float theta = sin(rad) / a;

			quat after = quat(cos(rad), d.y*theta , d.x*theta ,0.0f);

			temp = cross(after,temp);

			rotation = mat4_cast(temp);
		}
	}
}

void Trackball::rotate(quat q) {
	if (!drag) {
		temp = cross(q, temp);

		rotation = mat4_cast(temp);
	}
}

void Trackball::stop(float x, float y) {
	motion(x, y);

	drag = false;
}