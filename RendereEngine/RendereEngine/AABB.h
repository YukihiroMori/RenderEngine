#pragma once

#include "Main.h"

class AABB {
public:
	AABB() : min(vec3(FLT_MAX)), max(vec3(-FLT_MAX)) {}

	vec3 min;
	vec3 max;
};