//==========================================================================
// 
//  �G���g���[�p�v���C���[���� [playerEntry.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerEntry.h"
#include "playerStatus.h"
#include "EffekseerObj.h"

// �g�p�N���X
#include "playerUsercontrol_move.h"
#include "playerUsercontrol_action.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerEntry::CPlayerEntry(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase) : CPlayerBase(pPlayer, typeTeam, typeArea, typeBase)
{
	// ����̊���
	ChangeMoveControl(DEBUG_NEW CPlayerUserControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerUserControlAction());
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerEntry::~CPlayerEntry()
{

}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerEntry::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �v���C���[�̎擾
	CPlayer* pPlayer = GetPlayer();

	
}

//==========================================================================
// �q�b�g
//==========================================================================
CPlayer::SHitInfo CPlayerEntry::Hit(CBall* pBall)
{
	return CPlayer::SHitInfo();
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerEntry::ChangeMoveControl(CPlayerControlMove* control)
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
void CPlayerEntry::ChangeActionControl(CPlayerControlAction* control)
{
	// ����N���X�̎擾
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// ����N���X�̓���
	delete pControlAction;
	pControlAction = control;

	// ����N���X�̔��f
	SetPlayerControlAction(pControlAction);
}
