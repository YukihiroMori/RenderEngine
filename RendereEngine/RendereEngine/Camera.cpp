#include "Camera.h"

mat4 Camera::getViewMatrix() {
	mat4 ViewMatrix = lookAt(position, lookat, vec3(0.0, 1.0, 0.0));

	return ViewMatrix;
}

mat4 Camera::getProjectionMatrix() {

	mat4 ProjectionMatrix = perspective((float)(M_PI*fovY / 360.0f), (0 != height) ? width / (float)height : 10000, zNear, zFar);

	return ProjectionMatrix;
}