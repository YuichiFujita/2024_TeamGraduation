//==========================================================================
// 
//  ���[�U�[�v���C���[�w�b�_�[ [playerUser.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYER_USER_
#define _PLAYER_USER_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerBase.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayerUserControlMove;	// �ړ�(����)
class CPlayerUserControlAction;	// �A�N�V����(����)

//==========================================================================
// �N���X��`
//==========================================================================
// ���[�U�[�v���C���[�N���X
class CPlayerUser : public CPlayerBase
{
public:
	
	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerUser(CPlayer* pPlayer, const CPlayer::EFieldArea typeArea);
	~CPlayerUser() override;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V

	//=============================
	// �p�^�[��
	//=============================
	void ChangeMoveControl(CPlayerUserControlMove* control);		// �ړ��̑���ύX
	void ChangeActionControl(CPlayerUserControlAction* control);	// �A�N�V�����̑���ύX
};

#endif
