//=============================================================================
// 
//  �L�[�R���t�B�O���j���[�w�b�_�[ [optionmenu_keyconfig.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OPTIONMENU_KEYCONFIG_H_
#define _OPTIONMENU_KEYCONFIG_H_	// ��d�C���N���[�h�h�~

#include "optionmenu.h"

class CMultiNumber;
class CSelectDraw;



#include "keyconfig.h"

// �O���錾
class CControlKeyDisp;

//==========================================================================
// �N���X��`
//==========================================================================
// �L�[�R���t�B�O���j���[�N���X
class COptionMenu_Keyconfig : public COptionMenu
{
private:

	// �L�[���
	struct KEYINFO
	{
		CObject2D* s_p2D;				// ��������
		CControlKeyDisp* s_pKeyDisp;	// �L�[�R���t�B�O�\��

		// �R���X�g���N�^
		KEYINFO() : s_p2D(nullptr), s_pKeyDisp(nullptr){}
	};

	// �؂�ւ����
	struct CHENGEINFO
	{
		CObject2D* s_p2D;				// ��������
		CControlKeyDisp* s_pKeyDispOK;	// �L�[�R���t�B�O�\��
		CControlKeyDisp* s_pKeyDispNO;	// �L�[�R���t�B�O�\��
		CSelectDraw* pSelect;			// �I�����M

		// �R���X�g���N�^
		CHENGEINFO() : pSelect(nullptr), s_p2D(nullptr), s_pKeyDispOK(nullptr), s_pKeyDispNO(nullptr) { }
	};

public:

	COptionMenu_Keyconfig(int nPriority = 7);
	~COptionMenu_Keyconfig();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Kill() override;	// �폜

private:

	//=============================
	// �����o�֐�
	//=============================
	virtual void StateEdit() override;		// �ҏW
	void Chenge();
	void SetAlpha();
	void ResetSelect();		// �I�������Z�b�g

	//=============================
	// �����o�ϐ�
	//=============================
	float m_Alpha;
	float m_fTime;
	int m_SelectKey;	// �I��
	bool m_bDeath;		// ���S�t���O
	KEYINFO m_aKeyConfig[INGAME::ACT_MAX];	// �L�[�R���t�B�O�p
	CHENGEINFO m_checkconfig;	// ����߂�ύX�p
	CObject2D* m_pTitle2D;		// �����p�|���S��
	CSelectDraw* m_pSelect;		// �I�����M
};

#endif