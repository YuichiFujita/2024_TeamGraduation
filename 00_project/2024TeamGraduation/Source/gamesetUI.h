//============================================================
//
//	�Q�[���Z�b�gUI�w�b�_�[ [gamesetUI.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GAME_SET_UI_H_
#define _GAME_SET_UI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
// �O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �Q�[���Z�b�gUI�N���X
class CGameSetUI : public CObject
{
public:
	// �R���X�g���N�^
	CGameSetUI();

	// �f�X�g���N�^
	~CGameSetUI() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void Draw() override;		// �`��
	void SetEnableDisp(const bool bDisp) override;	// �`��󋵐ݒ�

	// �ÓI�����o�֐�
	static CGameSetUI* Create();	// ����

	// �����o�֐�
	bool IsEnd();	// ���o�I���m�F

private:
	// �����o�֐�
	HRESULT CreateUI();	// UI����

	// �����o�ϐ�
	CObject2D* m_pGameSet;	// �Q�[���Z�b�g���
	float m_fCurTime;		// �o�ߎ���
};

#endif	// _GAME_SET_UI_H_
