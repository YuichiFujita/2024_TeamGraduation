//==========================================================================
// 
//  �O��v���C���[���� [playerOut.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerOut.h"

// �g�p�N���X
#include "playercontrol_move.h"
#include "playercontrol_action.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerOut::CPlayerOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase) : CPlayerBase(pPlayer, typeTeam, typeArea, typeBase),
	m_posLeft	(VEC3_ZERO),	// �ړ��\���ʒu
	m_posRight	(VEC3_ZERO)		// �ړ��\�E�ʒu
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerOut::~CPlayerOut()
{

}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerOut::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
// �ʒu�̏�����
//==========================================================================
void CPlayerOut::InitPosition(const MyLib::Vector3& /*rPos*/)
{
	// ���E�ʒu�̒������W���v�Z
	MyLib::Vector3 posCenter;	// �������W
	D3DXVec3Lerp(&posCenter, &m_posLeft, &m_posRight, 0.5f);

	// �v���C���[�ʒu�̐ݒ�
	GetPlayer()->SetPosition(posCenter);
}
