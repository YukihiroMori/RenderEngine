#version 420 core
#extension GL_ARB_explicit_attrib_location : enable


//
// pass1.frag
//

// �ގ�
uniform vec4 kamb;                                                    // �����̔��ˌW��
uniform vec4 kdiff;                                                   // �g�U���ˌW��
uniform vec4 kspec;                                                   // ���ʔ��ˌW��
uniform float kshi;                                                   // �P���W��

// ���X�^���C�U����󂯎�钸�_�����̕�Ԓl
in vec4 p;                                                            // �ʒu P
in vec3 n;                                                            // �@�� N

// �t���[���o�b�t�@�ɏo�͂���f�[�^
layout (location = 0) out vec4 albedo;
layout (location = 1) out vec4 fresnel;
layout (location = 2) out vec3 position;
layout (location = 3) out vec3 normal;

void main(void)
{
  albedo = kdiff;
  fresnel = kspec;
  position = p.xyz / p.w;
  normal = normalize(n);
}
