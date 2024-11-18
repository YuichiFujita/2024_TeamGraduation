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
CPlayerUserOut::CPlayerUserOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerUser(pPlayer, typeTeam, typeArea),
	m_posLeft	(VEC3_ZERO),	// �ړ��\���ʒu
	m_posRight	(VEC3_ZERO)		// �ړ��\�E�ʒu
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

//==========================================================================
// �ʒu�̏�����
//==========================================================================
void CPlayerUserOut::InitPosition(const MyLib::Vector3& /*rPos*/)
{
	// ���E�ʒu�̒������W���v�Z
	MyLib::Vector3 posCenter;	// �������W
	D3DXVec3Lerp(&posCenter, &m_posLeft, &m_posRight, 0.5f);

	// �v���C���[�ʒu�̐ݒ�
	GetPlayer()->SetPosition(posCenter);
}
