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
class CPlayerUserControlMove;	// �ړ�(����)
class CPlayerUserControlAction;	// �A�N�V����(����)

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
	void ChangeMoveControl(CPlayerUserControlMove* control);		// �ړ��̑���ύX
	void ChangeActionControl(CPlayerUserControlAction* control);	// �A�N�V�����̑���ύX

private:

	//=============================
	// �����o�֐�
	//=============================
	//-----------------------------
	// ���̑��֐�
	//-----------------------------
	void Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// ����
	void DeleteControl() override;	// ����폜

	//=============================
	// �����o�ϐ�
	//=============================
};

#endif
