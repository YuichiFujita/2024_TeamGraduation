//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playerAIcontrol_left.h"
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
namespace
{

};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControlLeft::CPlayerAIControlLeft()
{

}

#if 0
//==========================================================================
// ����
//==========================================================================
CPlayerAIControlLeft* CPlayerAIControlLeft::Create(CPlayer* player)
{
	// �������̊m��
	CPlayerAIControlLeft* pObj = DEBUG_NEW CPlayerAIControlLeft;
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
HRESULT CPlayerAIControlLeft::Init()
{
	// ���N���X�̏�������
	CPlayerAIControl::Init();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerAIControlLeft::Uninit()
{
	// ���N���X�̏I������
	CPlayerAIControl::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIControlLeft::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���N���X�̍X�V
	CPlayerAIControl::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���蓊��
//==========================================================================
void CPlayerAIControlLeft::AttackDash(CPlayer* pTarget)
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

	if (distanceTarget > JUMP_LENGTH_TARGET && distanceLine < JUMP_LENGTH_LINE)
	{// �����ƃ^�[�Q�b�g�̋�����700.0f�ȏ�&&�������Ƃ̋������͈͈ȏ�̏ꍇ

		// ����
		SetMove(EMoveFlag::MOVEFLAG_DASH);

		// ����̈ʒu�ɋ߂Â�
		if (Approatch(posTarget, JUMP_LENGTH_LINE))
		{// �͈͓��̏ꍇ
			SetForcibly(EMoveForcibly::FORCIBLY_NONE);	// �����s���F�Ȃ�
			SetMove(EMoveFlag::MOVEFLAG_STOP);			// �s���F�~�܂�
		}

		return;
	}

	if (distanceTarget > JUMP_LENGTH_TARGET)
	{// �^�[�Q�b�g�Ƃ̋������͈͈ȏ�&&�������Ƃ̋������͈͓��̏ꍇ
		SetAction(EActionFlag::ACTION_JUMP);	// �A�N�V�����F����
	}
	else
	{
		SetMove(EMoveFlag::MOVEFLAG_STOP);	// �s���F�~�܂�
		SetThrow(EThrowFlag::THROW_NORMAL);		// �����F������
	}

	if (pMy->GetPosition().y >= playerAIcontrol::THROW_JUMP_END)	// �����ɂ���ĕς��
	{
		SetMove(EMoveFlag::MOVEFLAG_STOP);	// �s���F�~�܂�
		SetThrow(EThrowFlag::THROW_NORMAL);		// �����F������
	}
}

//==========================================================================
// �߂�
//==========================================================================
void CPlayerAIControlLeft::ForciblyReturn()
{
	// �v���C���[���擾
	CPlayer* pMy = GetPlayer();
	if (!pMy) return;
	MyLib::Vector3 myPos = pMy->GetPosition();


	CPlayer::EState state = pMy->GetState();
	if (state == CPlayer::EState::STATE_INVADE_RETURN) {
		SetMove(EMoveFlag::MOVEFLAG_STOP);
		SetAction(EActionFlag::ACTION_NONE);
		return;
	}

	// ����
	SetMove(EMoveFlag::MOVEFLAG_DASH);
	//SetAction(EActionFlag::ACTION_JUMP);

	// �߂Â�
	if (Approatch({ -playerAIcontrol::RETURN_POS, myPos.y, myPos.z }, playerAIcontrol::OK_LENGTH))
	{
		SetForcibly(EMoveForcibly::FORCIBLY_NONE);
		SetMove(EMoveFlag::MOVEFLAG_STOP);
	}
}

//==========================================================================
// �s���F����
//==========================================================================
void CPlayerAIControlLeft::MoveTypeCrazy(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �v���C���[�͐��𒴂��Ă��܂����H
//==========================================================================
bool CPlayerAIControlLeft::IsLineOverPlayer()
{
	bool bOver = false;	// ����������p

	// �v���C���[���擾
	CPlayer* pMy = GetPlayer();
	if (!pMy) return bOver;

	MyLib::Vector3 myPos = pMy->GetPosition();

	if (myPos.x > -playerAIcontrol::LINE_DISTANCE_OVER && GetAction() != EActionFlag::ACTION_JUMP)
	{// �ʒu�������Ă���&&�W�����v���ĂȂ��ꍇ
		bOver = true;
	}

	return bOver;
}

//==========================================================================
// �{�[���͐��𒴂��Ă��܂����H
//==========================================================================
bool CPlayerAIControlLeft::IsLineOverBall()
{
	bool bOver = false;	// ����������p

	// �{�[�����擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) { return bOver; }

	// �v���C���[���擾
	CPlayer* pMy = GetPlayer();
	if (!pMy) return bOver;

	CGameManager::ETeamSide typeTeam = pMy->GetTeam();

	if (pBall->GetPosition().x > 0.0f)
	{
		bOver = true;
	}

	return bOver;
}