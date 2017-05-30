#version 420 core
#extension GL_ARB_explicit_attrib_location : enable


//
// pass2.vert
//

// ���_����
layout (location = 0) in vec4 VertexPosition;

// ���X�^���C�U�ɑ��钸�_����
out vec2 tc;           
                
void main(void)
{
	tc = VertexPosition.xy * 0.5 + 0.5;
	tc /= 0.15;

	gl_Position = VertexPosition;
}