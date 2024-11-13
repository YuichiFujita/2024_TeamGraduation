//==========================================================================
// 
//  ���[�U�[�v���C���[���� [playerUser.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerUser.h"
#include "manager.h"
#include "calculation.h"

// �g�p�N���X
#include "playerStatus.h"
#include "playerUsercontrol_move.h"
#include "playerUsercontrol_action.h"
#include "playerUserOut_controlMove.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerUser::CPlayerUser(CPlayer* pPlayer, const CPlayer::EFieldArea typeArea) : CPlayerBase(pPlayer, typeArea)
{
	// ��������̐ݒ�
	switch (typeArea)
	{ // �|�W�V�������Ƃ̏���
	case CPlayer::EFieldArea::FIELD_IN:

		// ���쑀��̊���
		ChangeMoveControl(DEBUG_NEW CPlayerUserControlMove());
		ChangeActionControl(DEBUG_NEW CPlayerUserControlAction());
		break;

	case CPlayer::EFieldArea::FIELD_OUT:

		// �O�쑀��̊���
		ChangeMoveControl(DEBUG_NEW CPlayerUserOutControlMove());
		ChangeActionControl(DEBUG_NEW CPlayerUserControlAction());
		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerUser::~CPlayerUser()
{

}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerUser::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �v���C���[�̎擾
	CPlayer* pPlayer = GetPlayer();

	// ����N���X�̎擾
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// ����̍X�V
	pControlMove->Move(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);
	pControlAction->Action(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);

	// ����N���X�̔��f
	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerUser::ChangeMoveControl(CPlayerControlMove* control)
{
	// ����N���X�̎擾
	CPlayerControlMove* pControlMove = GetPlayerControlMove();

	// ����N���X�̓���
	delete pControlMove;
	pControlMove = control;

	// ����N���X�̔��f
	SetPlayerControlMove(pControlMove);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerUser::ChangeActionControl(CPlayerControlAction* control)
{
	// ����N���X�̎擾
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// ����N���X�̓���
	delete pControlAction;
	pControlAction = control;

	// ����N���X�̔��f
	SetPlayerControlAction(pControlAction);
}
