//=============================================================================
// 
//  ���[�U�[�v���C���[�w�b�_�[ [playerUser.h]
//  Author : ���c�E��
// 
//=============================================================================

#ifndef _PLAYER_USER_
#define _PLAYER_USER_		// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "player.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerControlMove;	// �ړ�(����)
class CPlayerControlAction;	// �A�N�V����(����)

//==========================================================================
// �N���X��`
//==========================================================================
// ���[�U�[�v���C���[�N���X
class CPlayerUser : public CPlayer
{
public:
	
	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerUser();
	~CPlayerUser();

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
	static CPlayerUser *Create(const CGameManager::TeamSide team, const MyLib::Vector3& rPos);

private:

	//=============================
	// �����o�֐�
	//=============================
	//-----------------------------
	// ���̑��֐�
	//-----------------------------
	void Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �ړ�
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
