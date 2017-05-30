#version 420 core
#extension GL_ARB_explicit_attrib_location : enable


//
// pass1.vert
//

// �ϊ��s��
uniform mat4 ModelMatrix;                                             // ���f���}�g���b�N�X
uniform mat4 ViewMatrix;                                              // �r���[�}�g���b�N�X
uniform mat4 ProjectionMatrix;                                        // �v���W�F�N�V�����}�g���b�N�X

uniform mat4 lightMatrix;

// ���_����
layout (location = 0) in vec3 VertexPosition;                                     // ���[�J�����W�n�̒��_�ʒu
layout (location = 1) in vec3 VertexNormal;                                     // ���_�̖@���x�N�g��

// ���X�^���C�U�ɑ��钸�_����
out vec4 p;                                                           // ���̕\�ʏ�̈ʒu P
out vec3 n;                                                           // ���̕\�ʏ�̖@���x�N�g�� N
out vec4 shadowMapCoord;

void main(void)
{
	p = ViewMatrix * ModelMatrix * vec4(VertexPosition, 1.0);	 // �t���O�����g�̈ʒu
	gl_Position = ProjectionMatrix * p;                                              // ���_�ʒu

	mat3 MVI = transpose(inverse(mat3(ViewMatrix * ModelMatrix)));													  
	n = MVI * VertexNormal;                                       // �t���O�����g�̖@���x�N�g�� N

	shadowMapCoord = lightMatrix * ModelMatrix * vec4(VertexPosition, 1.0) * 0.5 + 0.5;
}
