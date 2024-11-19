//==========================================================================
// 
//  ���[�U�[�v���C���[���� [playerUserOut.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerUserOut.h"

// �g�p�N���X
#include "playerUserOut_controlMove.h"
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
CPlayerUserOut::CPlayerUserOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerOut(pPlayer, typeTeam, typeArea, CPlayer::EBaseType::TYPE_USER)
{
	// �O�쑀��̊���
	ChangeMoveControl(DEBUG_NEW CPlayerUserOutControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerUserControlAction());
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerUserOut::~CPlayerUserOut()
{

}
