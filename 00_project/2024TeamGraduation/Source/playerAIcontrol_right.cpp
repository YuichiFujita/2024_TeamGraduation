//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playerAIcontrol_right.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "teamStatus.h"

// �f�o�b�O�p
#include "3D_effect.h"

// player�֘A
#include "playerBase.h"
#include "playerStatus.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol_move.h"
#include "playerManager.h"

//==========================================================================
// �萔��`
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControlRight::CPlayerAIControlRight()
{

}

#if 0
//==========================================================================
// ����
//==========================================================================
CPlayerAIControlRight* CPlayerAIControlRight::Create(CPlayer* player)
{
	// �������̊m��
	CPlayerAIControlRight* pObj = DEBUG_NEW CPlayerAIControlRight;
	if (pObj != nullptr)
	{
		pObj->m_pAI = player;

		// �N���X�̏�����
		if (FAILED(pObj->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}
#endif

//==========================================================================
// ����������
//==========================================================================
HRESULT CPlayerAIControlRight::Init()
{
	// ���N���X�̏�������
	CPlayerAIControl::Init();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerAIControlRight::Uninit()
{
	// ���N���X�̏I������
	CPlayerAIControl::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIControlRight::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���N���X�̍X�V
	CPlayerAIControl::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���蓊��
//==========================================================================
void CPlayerAIControlRight::AttackDash(CPlayer* pTarget)
{
	// �^�[�Q�b�g�̎擾
	if (!pTarget) return;
	MyLib::Vector3 posTarget = pTarget->GetPosition();

	// �����̏��
	CPlayer* pMy = GetPlayer();
	if (!pMy) return;
	MyLib::Vector3 posMy = pMy->GetPosition();

	// ���C���̈ʒu
	MyLib::Vector3 linePos = { 0.0f, posMy.y, posMy.z };

	// ���C���Ƃ̋���
	float distanceLine = 0.0f;
	float distanceTarget = 0.0f;
	float JUMP_LENGTH_TARGET = 500.0f;
	float JUMP_LENGTH_LINE = 300.0f;

	// �^�[�Q�b�g�̃G���A�̎擾
	CGameManager::ETeamSide side = pMy->GetTeam();

	if (pTarget)
	{// �^�[�Q�b�g�������ꍇ
		distanceTarget = posMy.DistanceXZ(pTarget->GetPosition());	// �����Ƒ���̋���
		distanceLine = posMy.DistanceXZ(linePos);	// �����ƒ��S���Ƃ̋���
	}
	else
	{
		return;
	}

	if (distanceTarget > JUMP_LENGTH_TARGET && distanceLine > JUMP_LENGTH_LINE)
	{// �����ƃ^�[�Q�b�g�̋�����700.0f�ȏ�&&�������Ƃ̋������͈͈ȏ�̏ꍇ

		// ����
		SetMove(EMoveType::MOVETYPE_DASH);

		// ����̈ʒu�ɋ߂Â�
		if (Approatch(posTarget, JUMP_LENGTH_LINE))
		{// �͈͓��̏ꍇ
			SetForcibly(EMoveForcibly::FORCIBLY_NONE);	// �����s���F�Ȃ�
			SetMove(EMoveType::MOVETYPE_STOP);			// �s���F�~�܂�
		}

		return;
	}

	if (distanceTarget > JUMP_LENGTH_TARGET)
	{// �^�[�Q�b�g�Ƃ̋������͈͈ȏ�&&�������Ƃ̋������͈͓��̏ꍇ
		SetAction(EAction::ACTION_JUMP);	// �A�N�V�����F����
	}
	else
	{
		SetMove(EMoveType::MOVETYPE_STOP);	// �s���F�~�܂�
		SetThrow(EThrow::THROW_NORMAL);		// �����F������
	}

	if (pMy->GetPosition().y >= playerAIcontrol::THROW_JUMP_END)	// �����ɂ���ĕς��
	{
		SetMove(EMoveType::MOVETYPE_STOP);	// �s���F�~�܂�
		SetThrow(EThrow::THROW_NORMAL);		// �����F������
	}
}

//==========================================================================
// �߂�
//==========================================================================
void CPlayerAIControlRight::ForciblyReturn()
{
	// �v���C���[���擾
	CPlayer* pMy = GetPlayer();
	if (!pMy) return;
	MyLib::Vector3 myPos = pMy->GetPosition();
	CGameManager::ETeamSide typeTeam = pMy->GetTeam();

	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = pMy->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ����
	SetMove(EMoveType::MOVETYPE_WALK);

	// �߂Â�
	if (Approatch({ playerAIcontrol::RETURN_POS, myPos.y, myPos.z }, playerAIcontrol::OK_LENGTH))
	{
		SetForcibly(EMoveForcibly::FORCIBLY_NONE);
		SetMove(EMoveType::MOVETYPE_STOP);
	}
}

//==========================================================================
// �v���C���[�͐��𒴂��Ă��܂����H
//==========================================================================
bool CPlayerAIControlRight::IsLineOverPlayer()
{
	bool bOver = false;	// ����������p

	// �v���C���[���擾
	CPlayer* pMy = GetPlayer();
	if (!pMy) return bOver;

	MyLib::Vector3 myPos = pMy->GetPosition();

	if (myPos.x < playerAIcontrol::LINE_DISTANCE_OVER && GetAction() != EAction::ACTION_JUMP)
	{// �ʒu�������Ă���&&�W�����v���ĂȂ��ꍇ
		bOver = true;
	}

	return bOver;
}

//==========================================================================
// �{�[���͐��𒴂��Ă��܂����H
//==========================================================================
bool CPlayerAIControlRight::IsLineOverBall()
{
	bool bOver = false;	// ����������p

	// �{�[�����擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) { return bOver; }

	// �v���C���[���擾
	CPlayer* pMy = GetPlayer();
	if (!pMy) return bOver;

	CGameManager::ETeamSide typeTeam = pMy->GetTeam();

	if (pBall->GetPosition().x < 0.0f)
	{
		bOver = true;
	}

	return bOver;
}