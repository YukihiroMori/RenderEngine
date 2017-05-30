#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;

class Transform{
public:
	mat4 asMatrix() const;

	vec3 position = vec3(0.0);
	quat rotation = quat(vec3(0.0));
	vec3 scale = vec3(1.0);

};

mat4 Transform::asMatrix() const {
	mat4 ModelMatrix;
	mat4 Transform = translate(position);
	mat4 Scale = glm::scale(scale);
	mat4 Rotation = mat4_cast(rotation);

	ModelMatrix = Transform * Rotation * Scale;

	return ModelMatrix;
}