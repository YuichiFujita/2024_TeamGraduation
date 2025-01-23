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

	// �s���F����
	SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);

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
// �{�[����D��
//==========================================================================
void CPlayerAIControlRightDefense::BallSteal()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// �ʒu�̎擾
	MyLib::Vector3 posBall = pBall->GetPosition();		// �{�[��
	MyLib::Vector3 posEnd = pBall->GetPosPassEnd();		// �{�[���̃p�X�I���ʒu
	MyLib::Vector3 posMy = pAI->GetPosition();		// �����̈ʒu

	// �I���ʒu��x,z���Q�Ƃ����ʒu�̐ݒ�
	MyLib::Vector3 pos = { posEnd.x, posMy.y, posEnd.z };

	if (pos.x < 0)
	{
		SetActionStatus(EActionStatus::ACTIONSTATUS_IDLE);
		return;
	}

	// �p�X����̎擾
	CPlayer* pTarget = pBall->GetTarget();
	if (pTarget)
	{
		// �^�[�Q�b�g�ƃ{�[���̈ʒu
		float distanth0 = pTarget->GetPosition().DistanceXZ(posBall);

		if (distanth0 < 100.0f)
		{// �{�[���ƃp�X��̋������͈͓��Ȃ炠����߂�
			SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
			SetActionStatus(EActionStatus::ACTIONSTATUS_IDLE);
			return;
		}
	}

	// �{�[���Ƃ̋���
	float distance = posMy.DistanceXZ(posBall);

	// �s����ԁF����
	SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

	// �{�[���̕��֍s��
	if (Approatch(pos, 100.0f) || distance < 100.0f)
	{// �I���ʒu�ɋ߂Â���||�{�[���Ƃ̋������͈͓��̏ꍇ

		if (posBall.y < 140.0f)
		{// ��ꂻ���ȍ����ɗ����I
			SetActionFlag(EActionFlag::ACTION_JUMP);
		}

		SetActionStatus(EActionStatus::ACTIONSTATUS_IDLE);
	}
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
	bool bOver = false;	// ����������p

	// �{�[�����擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) { return bOver; }

	// �v���C���[���擾
	CPlayer* pMy = GetPlayer();
	if (!pMy) return bOver;

	CGameManager::ETeamSide typeTeam = pMy->GetTeam();

	if (pBall->GetPosition().x < -5.0f)
	{
		bOver = true;
	}

	return bOver;
}