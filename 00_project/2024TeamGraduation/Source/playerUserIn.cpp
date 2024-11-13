//==========================================================================
// 
//  ���[�U�[�v���C���[���� [playerUserIn.cpp]
//  Author : ���c�E��
//  Adder  : ���c����
// 
//==========================================================================
#include "playerUserIn.h"

// �g�p�N���X
#include "playerUsercontrol_move.h"
#include "playerUsercontrol_action.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerUserIn::CPlayerUserIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerUser(pPlayer, typeTeam, typeArea)
{
	// ���쑀��̊���
	ChangeMoveControl(DEBUG_NEW CPlayerUserControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerUserControlAction());
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerUserIn::~CPlayerUserIn()
{

}
