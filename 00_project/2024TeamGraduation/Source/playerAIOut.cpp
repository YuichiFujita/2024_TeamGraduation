//==========================================================================
// 
//  AI�v���C���[���� [playerAIOut.cpp]
//  Author : ���c�E��
//  Adder  : ���}���V
// 
//==========================================================================
#include "playerAIOut.h"

// �g�p�N���X
#include "playerAIcontrol_move.h"
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
CPlayerAIOut::CPlayerAIOut(CPlayer* pPlayer, const CPlayer::EFieldArea typeArea) : CPlayerAI(pPlayer, typeArea)
{
	// �O�쑀��̊���	// TODO�F�O�쑀��ɕύX
	ChangeMoveControl(DEBUG_NEW CPlayerAIControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAIOut::~CPlayerAIOut()
{

}
