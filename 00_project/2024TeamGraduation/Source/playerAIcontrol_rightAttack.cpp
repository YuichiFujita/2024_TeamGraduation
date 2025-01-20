//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playerAIcontrol_rightAttack.h"
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
#include "playerAIcontrol.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControlRightAttack::CPlayerAIControlRightAttack()
{
	
}

#if 1
//==========================================================================
// ����
//==========================================================================
CPlayerAIControlRightAttack* CPlayerAIControlRightAttack::Create()
{
	// �������̊m��
	CPlayerAIControlRightAttack* pObj = DEBUG_NEW CPlayerAIControlRightAttack;
	if (pObj != nullptr)
	{
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
HRESULT CPlayerAIControlRightAttack::Init()
{
	CPlayerAIControlAttack::Init();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerAIControlRightAttack::Uninit()
{
	CPlayerAIControlAttack::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIControlRightAttack::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerAIControlAttack::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���蓊��
//==========================================================================
void CPlayerAIControlRightAttack::AttackDash(CPlayer* pTarget)
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
		SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

		// ����̈ʒu�ɋ߂Â�
		if (Approatch(posTarget, JUMP_LENGTH_LINE))
		{// �͈͓��̏ꍇ
			//SetForcibly(EMoveForcibly::FORCIBLY_NONE);	// �����s���F�Ȃ�
			SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);			// �s���F�~�܂�
		}

		return;
	}

	if (distanceTarget > JUMP_LENGTH_TARGET)
	{// �^�[�Q�b�g�Ƃ̋������͈͈ȏ�&&�������Ƃ̋������͈͓��̏ꍇ
		SetActionFlag(EActionFlag::ACTION_JUMP);	// �A�N�V�����F����
	}
	else
	{
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);		// �s���F�~�܂�
		SetThrowFlag(EThrowFlag::THROW_NORMAL);		// �����F������
	}

	if (pMy->GetPosition().y >= playerAIcontrol::THROW_JUMP_END)	// �����ɂ���ĕς��
	{
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);		// �s���F�~�܂�
		SetThrowFlag(EThrowFlag::THROW_NORMAL);		// �����F������
	}
}
