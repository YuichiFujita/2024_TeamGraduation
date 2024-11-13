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
CPlayerUserOut::CPlayerUserOut(CPlayer* pPlayer, const CGameManager::TeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerUser(pPlayer, typeTeam, typeArea),
#if 0	// TODO
	m_posLeft	(VEC3_ZERO),	// �ړ��\���ʒu
	m_posRight	(VEC3_ZERO)		// �ړ��\�E�ʒu
#else
	m_posLeft	(MyLib::Vector3(900.0f, 0.0f, 650.0f)),	// �ړ��\���ʒu
	m_posRight	(MyLib::Vector3(50.0f, 0.0f, 650.0f))	// �ړ��\�E�ʒu
#endif
{
	// �O�쑀��̊���	// TODO�F�O�쑀��ɕύX
	ChangeMoveControl(DEBUG_NEW CPlayerUserOutControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerUserControlAction());
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerUserOut::~CPlayerUserOut()
{

}
