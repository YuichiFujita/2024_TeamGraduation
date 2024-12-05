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
namespace timing
{
	// �^�C�~���O
	const float TIMING_NORMAL = 1.0f;	// �ʏ�
	const int TIMING_RAND_MAX = 100;	// randMAX�l
	const int TIMING_RAND_MIN = -80;	// randMIN�l
}

namespace
{
	// �����֘A
	const float THROW_JUMP_END = 130.0f;			// �W�����v�����̍ő�ʒu(�W�����v��MAX�F150)

	// �L���b�`�֘A
	const float CHATCH_LENGTH_IN = 500.0f;		// ����Ƃ̋���
	const float CHATCH_LENGTH_OUT = 700.0f;		// �O��Ƃ̋���
	const float CATCH_JUMP_LENGTH = 100.0f;		// �W�����v�L���b�`�̋���
	const float CATCH_JUMP_HEIGHT = 300.0f;		// �W�����v�L���b�`���鍂��

	// ������(�f�t�H���g)
	const float LENGTH_IN = 500.0f;
	const float LENGTH_SPACE = 10.0f;
	const float LENGTH_OUT = 200.0f;
	const float LENGTH_LINE = 100.0f;

	// ���z������(���S(x)����̋���)
	const float LINE_DISTANCE_OVER = 10.0f;		// ����������̋���
	const float RETURN_POS = 500.0f;			// �߂�ʒu
	const float OK_LENGTH = 10.0f;				// ����͈̔�(�ړI�Ƃ̋���)

	// �X�e�[�^�X�֘A
	const float JUMP_RATE = 1.0f;				// �W�����v�̊���(����)

}

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
// �߂�
//==========================================================================
void CPlayerAIControlLeft::ForciblyReturn()
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
	if (Approatch({ -RETURN_POS, myPos.y, myPos.z }, OK_LENGTH))
	{
		SetForcibly(EMoveForcibly::FORCIBLY_NONE);
		SetMove(EMoveType::MOVETYPE_STOP);
	}
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

	if (pMy->GetPosition().y >= THROW_JUMP_END)	// �����ɂ���ĕς��
	{
		SetMove(EMoveType::MOVETYPE_STOP);	// �s���F�~�܂�
		SetThrow(EThrow::THROW_NORMAL);		// �����F������
	}
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

	if (myPos.x > -LINE_DISTANCE_OVER)
	{// �ʒu�������Ă����ꍇ
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