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
CPlayerAIOut::CPlayerAIOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerAI(pPlayer, typeTeam, typeArea),
	m_posLeft	(VEC3_ZERO),	// �ړ��\���ʒu
	m_posRight	(VEC3_ZERO)		// �ړ��\�E�ʒu
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

//==========================================================================
// �ʒu�̏�����
//==========================================================================
void CPlayerAIOut::InitPosition(const MyLib::Vector3& /*rPos*/)
{
	// ���E�ʒu�̒������W���v�Z
	MyLib::Vector3 posCenter;	// �������W
	D3DXVec3Lerp(&posCenter, &m_posLeft, &m_posRight, 0.5f);

	// �v���C���[�ʒu�̐ݒ�
	GetPlayer()->SetPosition(posCenter);
}
