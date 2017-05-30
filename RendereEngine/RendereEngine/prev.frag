#version 420 core
#extension GL_ARB_explicit_attrib_location : enable


uniform sampler2D shadow;

in vec2 tc;

layout (location = 0) out vec4 fc;

void main(void)
{
	fc = vec4(vec3(texture(shadow , tc).r) , 1.0);
}