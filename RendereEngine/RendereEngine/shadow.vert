#version 420 core
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec3 position;

uniform mat4 lightMatrix;
uniform mat4 ModelMatrix;

out vec3 p;

void main(){
	gl_Position = lightMatrix * ModelMatrix * vec4(position , 1.0);
	p = (lightMatrix * ModelMatrix * vec4(position , 1.0)).xyz;
}