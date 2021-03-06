#version 420 core
#extension GL_ARB_explicit_attrib_location : enable


//
// pass2.vert
//

// 頂点属性
layout (location = 0) in vec4 VertexPosition;

// ラスタライザに送る頂点属性
out vec2 tc;           
                
void main(void)
{
	tc = VertexPosition.xy * 0.5 + 0.5;
	tc /= 0.15;

	gl_Position = VertexPosition;
}