//==========================================================================
// 
//  ����v���C���[���� [playerIn.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerIn.h"

// �g�p�N���X
#include "playercontrol_move.h"
#include "playercontrol_action.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerIn::CPlayerIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerBase(pPlayer, typeTeam, typeArea)
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerIn::~CPlayerIn()
{

}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerIn::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerIn::ChangeMoveControl(CPlayerControlMove* control)
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
void CPlayerIn::ChangeActionControl(CPlayerControlAction* control)
{
	// ����N���X�̎擾
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// ����N���X�̓���
	delete pControlAction;
	pControlAction = control;

	// ����N���X�̔��f
	SetPlayerControlAction(pControlAction);
}
