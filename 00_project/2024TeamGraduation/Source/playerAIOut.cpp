//==========================================================================
// 
//  AI�v���C���[���� [playerAIOut.cpp]
//  Author : ���c�E��
//  Adder  : ���}���V
// 
//==========================================================================
#include "playerAIOut.h"

// �g�p�N���X
#include "playerAIOut_controlMove.h"
#include "playerAIcontrol_action.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIOut::CPlayerAIOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerOut(pPlayer, typeTeam, typeArea)
{
	// �O�쑀��̊���
	ChangeMoveControl(DEBUG_NEW CPlayerAIOutControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAIOut::~CPlayerAIOut()
{

}
