//=============================================================================
// 
//  �}�b�v�w�b�_�[ [map.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MAP_H_
#define _MAP_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "listmanager.h"
class CObjectX;
//==========================================================================
// ���O���
//==========================================================================
namespace MyMap
{
	HRESULT Create(const char *pTextFile);	// ��������
	HRESULT ReadTexture();	// �e�N�X�`���ǂݍ��ݏ���
	HRESULT ReadXFile(const char *pTextFile);	// ���ł�ǂݍ��ݏ���
	HRESULT ReadText(const char *pTextFile);	// �O���t�@�C���ǂݍ��ݏ���
	HRESULT SaveText();	// �O���t�@�C���������ݏ���
	int GetNumModelAll();	// �����擾
	void Release();
	void Regist(int nType, MyLib::Vector3 pos, MyLib::Vector3 rot, bool bShadow);
	void Delete(CObjectX* obj);

	void ChangeMap(const char* pTextFile);	// �}�b�v�؂�ւ�
	std::string GetModelFileName(int nIdx);
	std::vector<std::string> GetModelFileNameAll();
	std::vector<std::string> GetTextureFileNameAll();
};

namespace MyMyMap
{
	// �\���̒�`
	struct sMapModel
	{
		int nType;				// ���
		int nIdxXFile;			// X�t�@�C���̃C���f�b�N�X�ԍ�
		MyLib::Vector3 pos;		// �ʒu
		MyLib::Vector3 rot;		// ����
		int nShadow;			// �e�g�����ǂ���
	};
};

#endif