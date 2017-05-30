#version 420 core
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) out float fragmentdepth;

in vec3 p;

void main(){
	fragmentdepth = p.z;
}