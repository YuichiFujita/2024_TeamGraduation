//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : Takeru Ogasawara
// 
//=============================================================================
#include "playerAIcontrol_rightDefense.h"
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
CPlayerAIControlRightDefense::CPlayerAIControlRightDefense()
{
	
}

#if 1
//==========================================================================
// ����
//==========================================================================
CPlayerAIControlRightDefense* CPlayerAIControlRightDefense::Create(CPlayer* player)
{
	// �������̊m��
	CPlayerAIControlRightDefense* pObj = DEBUG_NEW CPlayerAIControlRightDefense;
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
HRESULT CPlayerAIControlRightDefense::Init()
{
	// �e�N���X�̏���������
	CPlayerAIControlDefense::Init();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerAIControlRightDefense::Uninit()
{
	// �e�N���X�̏I��
	CPlayerAIControlDefense::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIControlRightDefense::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�N���X�̍X�V
	CPlayerAIControlDefense::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���
//==========================================================================
void CPlayerAIControlRightDefense::MoveRetreat()
{
	CPlayer* pPlayer = GetPlayer();
	if (!pPlayer) return;

	// �����̈ʒu
	MyLib::Vector3 posMy = pPlayer->GetPosition();

	// ���S�n��
	float posSafeX = GetDistance();

	if (posMy.x > posSafeX) {// �ړ��^�C�v�F��
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
		SetAction(EAction::IDLE);
		return;
	}

	// �s���t���O�F����
	SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);

	// �E�ړ�
	MoveRight();
}

//==========================================================================
// �����_���ړ�
//==========================================================================
void CPlayerAIControlRightDefense::MoveRandom()
{
	// x950 z560
	MyLib::Vector3 posSafeMax = { 950.0f, 0.0f, 560.0f };
	MyLib::Vector3 posSafeMin = { GetDistance(), 0.0f, -560.0f };

	CPlayerAIControlDefense::SAction action = GetActionInfo();
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	if (!action.bSet) {
		// �ʒu�̐ݒ�
		// x���W
		float fRand = (float)UtilFunc::Transformation::Random((int)posSafeMin.x, (int)posSafeMax.x);
		action.pos.x = fRand;
		// z���W
		fRand = (float)UtilFunc::Transformation::Random((int)posSafeMin.z, (int)posSafeMax.z);
		action.pos.z = fRand;

		action.bSet = true;
	}

	// �����Ă��ʒu�Ƃ̋���
	float distanse = pAI->GetPosition().DistanceXZ(action.pos);
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	CMotion* pMotion = pAI->GetMotion();
	int nType = pMotion->GetType();

	if (distanse > 300.0f)
	{
		if (pBall)
		{
			if (!pBall->GetPlayer() &&						// �{�[�������҂����Ȃ�
				nType != CPlayer::EMotion::MOTION_JUMP)		// �W�����v������Ȃ�
			{
				// �s���F����
				SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);
			}
			else
			{
				// �s���F����
				SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);
			}
		}
		else
		{
			// �s���F����
			SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);
		}
	}
	else {
		// �s���F����
		SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);
	}

	// �߂Â�
	if (Approatch(action.pos, 30.0f)) {
		action.bSet = false;

		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
		SetActionStatus(EActionStatus::ACTIONSTATUS_COOLDOWN);
	}

	// �s�����̐ݒ�
	SetActionInfo(action);
}

//==========================================================================
// �v���C���[�͐��𒴂��Ă��܂����H
//==========================================================================
bool CPlayerAIControlRightDefense::IsLineOverPlayer()
{
	bool bOver = false;	// ����������p

	// �v���C���[���擾
	CPlayer* pMy = GetPlayer();
	if (!pMy) return bOver;

	MyLib::Vector3 myPos = pMy->GetPosition();

	if (myPos.y > 0)
	{
		return bOver;
	}

	if (myPos.x < 10.0f && GetAction() != GetActionFlag() != EActionFlag::ACTION_JUMP)
	{// �ʒu�������Ă���&&�W�����v���ĂȂ��ꍇ
		bOver = true;
	}

	return bOver;
}

//==========================================================================
// �{�[���͐��𒴂��Ă��܂����H
//==========================================================================
bool CPlayerAIControlRightDefense::IsLineOverBall()
{
	// �{�[�����擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) { return false; }

	if (pBall->GetPosition().x <= 0.0f)
	{
		return true;
	}

	return false;
}