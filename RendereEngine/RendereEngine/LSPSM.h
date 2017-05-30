#pragma once

#include "Main.h"
#include "AABB.h"
#include "Transform.h"


class PointList {
public:

	PointList() {
		list.clear();
	}

	void Add(PointList point_list) {
		for each (vec3 value in point_list())
		{
			list.push_back(value);
		}
	}

	void Add(vector<vec3> value_list) {
		for each (vec3 value in value_list)
		{
			list.push_back(value);
		}
	}

	void Add(vec3 value) {
		list.push_back(value);
	}


	void Clear() {
		list.clear();
	}

	PointList& operator = (const PointList & value) {
		list.clear();
		for each(auto value in value.list) {
			list.push_back(value);
		}
		return (*this);
	}

	vec3& operator [] (int index) {
		return list[index];
	}

	vector<vec3> operator() () {
		return list;
	}

private:
	vector<vec3> list;

};

namespace lspsm_utils {
	mat4 scaleTranslateToFit(AABB aabb) {
		mat4 result;
		result = ortho(aabb.min.x,aabb.max.x,aabb.min .y,aabb.max.y,aabb.min.z,aabb.max.z);

		return result;
	}

	AABB calcCubicHull(PointList list) {
		AABB result;

		if (list().size() > 0) {
			result.min = list[0];
			result.max = list[0];

			for each (vec3 value in list())
			{
				result.min = min(result.min, value);
				result.max = max(result.max, value);
			}
		}

		return result;
	}

	PointList translate(PointList list, mat4 mat) {
		PointList result;
		for (int i = 0; i < list().size(); i++) {
			vec3 point = list[i];
			vec4 vec = vec4(point, 1.0) * mat;

			result.Add(vec.xyz());

		}
		return result;
	}

	PointList transform(PointList list, mat4 mat) {
		PointList result;
		for (int i = 0; i < list().size(); i++) {
			vec3 point = list[i];
			vec4 vec = vec4(point, 1.0) * mat;

			result.Add(vec.xyz() / vec.w);

		}
		return result;
	}

	vec3 linCombVector3( const vec3 pos, const vec3 dir, const float t) {
		vec3 result;
		result = pos + t * dir;
		return result;
	}

}


class LSPSM {
public:
	Transform transform;

	mat4 view;
	mat4 projection;

	PointList pointList;

	float nearDist;
	float farDist;

	LSPSM() : nearDist(0.1f) {
		pointList.Clear();
	}

	void compute_USM(vec3 viewDirection, vec3 lightDirection) {
		view = lookAt(transform.position, transform.position + lightDirection, vec3(0,1,0));

		for each(auto p in pointList()) {
			cout << to_string(p) << endl;
		}

		PointList LiSP = lspsm_utils::transform(pointList, view);

		for each(auto p in LiSP()) {
			cout << to_string(p) << endl;
		}
		
		AABB LiSBox = lspsm_utils::calcCubicHull(LiSP);
		projection = lspsm_utils::scaleTranslateToFit(LiSBox);

		//cout << to_string(projection) << endl;
	}

	void compute_LSPSM(vec3 viewDirection, vec3 lightDirection) {
		mat4 lispMtx;

		float angle = acos(dot(normalize(viewDirection), normalize(lightDirection)));

		if (angle == 0.0f || angle == M_PI) {
			compute_USM(viewDirection, lightDirection);
			return;
		}

		PointList listClone = pointList;

		float sinGamma = sqrtf(1.0f - angle * angle);

		vec3 left = cross(lightDirection, viewDirection);
		vec3 up = normalize(cross(left, lightDirection));

		view = lookAt(transform.position, transform.position + lightDirection, up);

		PointList LiSP = lspsm_utils::transform(pointList, view);
		AABB LiSBox = lspsm_utils::calcCubicHull(LiSP);

		{
			const float factor = 1.0f / sinGamma;
			const float z_n = factor*nearDist;
			const float d = abs(LiSBox.max.y - LiSBox.min.y);
			const float z_f = z_n + d * sinGamma;
			const float n = (z_n + sqrt(z_f * z_n)) / sinGamma;
			const float f = n; d;
			
			vec3 pos;
			pos = lspsm_utils::linCombVector3(transform.position, up, -(n - nearDist));

			view = lookAt(pos, lightDirection, up);
			lispMtx = mat4(1.0);
			lispMtx[1][1] = (f + n) / (f - n);
			lispMtx[3][1] = -2.0f * f * n / (f - n);
			lispMtx[1][3] = 1.0f;
			lispMtx[3][3] = 0.0f;

			projection = lispMtx * view;

			LiSP = lspsm_utils::transform(listClone, projection);
			LiSBox = lspsm_utils::calcCubicHull(LiSP);
		}

		projection = lspsm_utils::scaleTranslateToFit(LiSBox);

		projection = projection * lispMtx;
	}



};