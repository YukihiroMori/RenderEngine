/*
���񂵂�̑���

OpenGL�̘b�@��2��

GLSL�R���p�C��

https://sites.google.com/site/monshonosuana/opengl/opengl_002
*/

#pragma once

#include "glext_win.h"
#include <string>
#include <vector>


// GLSL�R���p�C��
class GLSLCompiler
{
public:
	typedef std::pair<std::string, std::string>	DefineValue;

public:
	GLSLCompiler()
	{}
	~GLSLCompiler()
	{}

	// �C���N���[�h�p�X��ǉ�����
	int AddIncludePath(const std::string& path)
	{
		m_includePaths.push_back(path);
		return (int)m_includePaths.size();
	}
	int AddIncludePath(const char* path)
	{
		return AddIncludePath(std::string(path));
	}

	// �C���N���[�h�p�X���N���A����
	void ClearIncludePaths()
	{
		m_includePaths.clear();
	}

	// ��`��ǉ�����
	int AddDefine(const std::string& def, const std::string& val)
	{
		m_defines.push_back(DefineValue(def, val));
		return (int)m_defines.size();
	}
	int AddDefine(const char* def, const char* val)
	{
		return AddDefine(std::string(def), std::string(val));
	}

	// ��`���N���A����
	void ClearDefines()
	{
		m_defines.clear();
	}

	// �t�@�C������GLSL���R���p�C������
	bool CompileFromFile(GLuint shader, const char* filename);

	// �����񂩂�GLSL���R���p�C������
	bool CompileFromString(GLuint shader, const std::string& source);

public:
	// GLSL��#include�f�B���N�e�B�u���g�p�ł��邩�ǂ������ׂ�
	static bool HasIncludeARB();

private:
	std::vector<std::string>	m_includePaths;				// �C���N���[�h�t�@�C������������p�X
	std::vector<DefineValue>	m_defines;					// �W���I�Ɏg�p�����`
};	// class GLSLCompiler


//	EOF
