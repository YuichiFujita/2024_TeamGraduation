//=============================================================================
// 
//  AI�v���C���[�w�b�_�[ [playerAI.h]
//  Author : ���c�E��
// 
//=============================================================================

#ifndef _PLAYER_AI_
#define _PLAYER_AI_		// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "player.h"

//==========================================================================
// �N���X��`
//==========================================================================
// AI�v���C���[�N���X
class CPlayerAI : public CPlayer
{
public:
	
	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerAI();
	~CPlayerAI();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;

	//=============================
	// �����o�֐�
	//=============================
	void Kill();	// �폜

	//=============================
	// �p�^�[��
	//=============================
	void ChangeMoveControl(CPlayerControlMove* control);		// �ړ��̑���ύX
	void ChangeActionControl(CPlayerControlAction* control);	// �A�N�V�����̑���ύX

	//=============================
	// �ÓI�֐�
	//=============================
	/*
		@brief		��������
		@details	�v���C���[�C���f�b�N�X
		@details	�`�[���T�C�h
	*/
	static CPlayerAI *Create(const int nIdx, const CGameManager::TeamSide team);

private:

	//=============================
	// �����o�֐�
	//=============================
	//-----------------------------
	// ���̑��֐�
	//-----------------------------
	void Move(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �ړ�
	void DeleteControl() override;	// ����폜

	//=============================
	// �����o�ϐ�
	//=============================
	//-----------------------------
	// �p�^�[���p�C���X�^���X
	//-----------------------------
	CPlayerControlMove* m_pControlMove;		// �ړ�����
	CPlayerControlAction* m_pControlAction;	// �A�N�V��������
};

#endif
