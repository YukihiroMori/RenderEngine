#pragma once

#include "Main.h"

namespace Obj {
	class mat      // �}�e���A��
	{
	public:
		mat() {}
		mat(mat *m) {
			amb = m->amb;
			diff = m->diff;
			spec = m->spec;
			shi = m->shi;
			dis = m->dis;
		}
		mat(vec4 _amb, vec4 _diff, vec4 _spec, float shi, float dis)
			: amb(_amb), diff(_diff), spec(_spec), shi(shi), dis(dis) {}

		vec4 amb;      // ambient
		vec4 diff;     // diffuse
		vec4 spec;     // specular
		float shi;    // shininess
		float dis;    // dissolve
	};
	class face      // �ʃf�[�^
	{
	public:
		vector<int> p;  // ���_���W�ԍ�
		vector<int> t;  // �e�N�X�`�����W�ԍ�
		vector<int> n;  // ���_�@���ԍ�
		bool smooth;  // �X���[�Y�V�F�[�f�B���O�̗L��
	};
	class grp      // �ʃO���[�v
	{
	public:
		string name;
		int b;     // �ʃO���[�v�̊J�n�ԍ�
		int c;     // �ʃO���[�v�̒��_��
		mat m; // �ʃO���[�v�̃}�e���A��
		grp() {}
		grp(string name,int begin, int count, mat material)
			: name(name), b(begin), c(count), m(material) {}
	};
}