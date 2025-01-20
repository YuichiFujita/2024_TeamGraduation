//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playerAIcontrol_leftDefense.h"
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
CPlayerAIControlLeftDefense::CPlayerAIControlLeftDefense()
{

}

#if 1
//==========================================================================
// ����
//==========================================================================
CPlayerAIControlLeftDefense* CPlayerAIControlLeftDefense::Create()
{
	// �������̊m��
	CPlayerAIControlLeftDefense* pObj = DEBUG_NEW CPlayerAIControlLeftDefense;
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
HRESULT CPlayerAIControlLeftDefense::Init()
{
	CPlayerAIControlDefense::Init();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerAIControlLeftDefense::Uninit()
{
	CPlayerAIControlDefense::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIControlLeftDefense::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerAIControlDefense::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

void CPlayerAIControlLeftDefense::MoveRetreat()
{

}

//==========================================================================
// �����_���ړ�
//==========================================================================
void CPlayerAIControlLeftDefense::MoveRandom()
{
	// x-950 z560
	MyLib::Vector3 posSafeMax = { -950.0f, 0.0f, 560.0f };
	MyLib::Vector3 posSafeMin = { -GetDistance(), 0.0f, -560.0f };

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
	if (Approatch(action.pos, 10.0f)) {
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
bool CPlayerAIControlLeftDefense::IsLineOverPlayer()
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

	//if (myPos.x > -10.0f && GetAction() != EActionFlag::ACTION_JUMP)
	{// �ʒu�������Ă���&&�W�����v���ĂȂ��ꍇ
		bOver = true;
	}

	return bOver;
}

//==========================================================================
// �{�[���͐��𒴂��Ă��܂����H
//==========================================================================
bool CPlayerAIControlLeftDefense::IsLineOverBall()
{
	bool bOver = false;	// ����������p

	// �{�[�����擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) { return bOver; }

	// �v���C���[���擾
	CPlayer* pMy = GetPlayer();
	if (!pMy) return bOver;

	CGameManager::ETeamSide typeTeam = pMy->GetTeam();

	if (pBall->GetPosition().x > 5.0f)
	{
		bOver = true;
	}

	return bOver;
}