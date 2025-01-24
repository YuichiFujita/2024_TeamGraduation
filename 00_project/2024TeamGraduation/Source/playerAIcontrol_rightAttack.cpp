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
// �U�������F�����
//==========================================================================
void CPlayerAIControlRightAttack::PreparationLeave()
{
	// ����
	SetMoveFlag(CPlayerAIControlMode::EMoveFlag::MOVEFLAG_WALK);

	// �߂Â�
	if (Approatch({ 500.0f, GetPlayer()->GetPosition().y, GetPlayer()->GetPosition().z }, 50.0f))
	{
		// �ҋ@
		SetMoveFlag(CPlayerAIControlMode::EMoveFlag::MOVEFLAG_IDLE);

		// �U�����[�h�F�U��
		SetAttackMode(EATTACKMODE::ATTACKMODE_ATTACK);
	}
}

void CPlayerAIControlRightAttack::Preparation()
{
}
