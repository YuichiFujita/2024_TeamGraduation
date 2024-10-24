//============================================================
//
//	�L�����N�^�[�A�j���[�V�����w�b�_�[ [characterAnim.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _CHARACTER_ANIM_H_
#define _CHARACTER_ANIM_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectCharaAnim.h"

//************************************************************
//	�N���X��`
//************************************************************
// �L�����N�^�[�A�j���[�V�����N���X
class CCharacterAnim
{
public:
	// �R���X�g���N�^
	CCharacterAnim();

	// �f�X�g���N�^
	~CCharacterAnim();

	// ���[�V�������^�G�C���A�X��`
	using AMotion = CObjectCharaAnim::SInfo;

	// �����o�֐�
	HRESULT Init();		// �L�����N�^�[������
	void Uninit();		// �L�����N�^�[�I��
	HRESULT LoadAll();	// �L�����N�^�[�S�Ǎ�
	AMotion Regist(const char* pCharaPath);	// �L�����N�^�[�o�^

	// �ÓI�����o�֐�
	static CCharacterAnim* Create();	// ����
	static void Release(CCharacterAnim*& prCharacterAnim);	// �j��

private:
	// �����o�֐�
	HRESULT SearchFolderAll(std::string sFolderPath);	// �t�H���_�S����

	// �����o�֐�
	HRESULT LoadSetup(AMotion* pInfoChara, const char* pCharaPath);			// �L�����N�^�[���Z�b�g�A�b�v
	HRESULT LoadMotionSetup(AMotion* pInfoChara, const char* pMotionPath);	// ���[�V�������Z�b�g�A�b�v

	// �����o�ϐ�
	std::map<std::string, AMotion> m_mapCharacter;	// �L�����N�^�[�A�j���[�V�����A�z�z��
};

#endif	// _CHARACTER_ANIM_H_
