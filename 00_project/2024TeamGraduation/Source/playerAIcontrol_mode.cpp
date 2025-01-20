//=============================================================================
// 
//  �v���C���[AI�R���g���[��_�ړ����� [playerAIcontrol_move.cpp]
//  Author : ���c�E��
// 
//=============================================================================
#include "playerAIcontrol_mode.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "playerAction.h"

// player
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol_idle.h"
#include "playerAIcontrol_attack.h"
#include "playerAIcontrol_defense.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float JUMP_RATE = 1.0f;				// �W�����v�̊���(����)
}

// �t���O
CPlayerAIControlMode::MOVEFLAG_FUNC CPlayerAIControlMode::m_MoveFlagFunc[] =	// �s���t���O�֐�
{
	&CPlayerAIControlMode::MoveFlagStop,			// �~�܂�
	&CPlayerAIControlMode::MoveFlagWalk,			// ����
	&CPlayerAIControlMode::MoveFlagBlink,			// ����
	&CPlayerAIControlMode::MoveFlagDash,			// ����
};

CPlayerAIControlMode::ACTIONFLAG_FUNC CPlayerAIControlMode::m_ActionFlagFunc[] =	// �A�N�V�����t���O�֐�
{
	&CPlayerAIControlMode::ActionFlagNone,			// �Ȃ�
	&CPlayerAIControlMode::ActionFlagJump,			// �W�����v
};


//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControlMode::CPlayerAIControlMode()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAIControlMode::~CPlayerAIControlMode()
{

}

//==========================================================================
// ����
//==========================================================================
CPlayerAIControlMode* CPlayerAIControlMode::Create(CPlayer* player, EMode mode)
{
	if (!player) assert(("�v���C���[��񂪖�����", false));

	// �������̊m��
	CPlayerAIControlMode* pObj = nullptr;

	if (pObj == nullptr)
	{
		switch (mode)
		{
		case EMode::MODE_IDLE:
			//pObj = DEBUG_NEW CPlayerAIControlIdle;
			pObj = CPlayerAIControlAttack::Create(player);
			break;

		case EMode::MODE_ATTACK:
			//pObj = DEBUG_NEW CPlayerAIControlAttack;
			pObj = CPlayerAIControlAttack::Create(player);
			break;

		case EMode::MODE_DEFENCE:
			//pObj = DEBUG_NEW CPlayerAIControlDefense;
			pObj = CPlayerAIControlDefense::Create(player);
			break;

		default:
			break;
		}

		pObj->m_pAI = player;
		pObj->m_eMode = mode;

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

//================================================================================
// ����������
//================================================================================
HRESULT CPlayerAIControlMode::Init()
{
	m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
	m_eActionFlag = EActionFlag::ACTION_NONE;

	return S_OK;
}

//================================================================================
// �I������
//================================================================================
void CPlayerAIControlMode::Uninit()
{
	delete this;
}

//================================================================================
// �X�V����
//================================================================================
void CPlayerAIControlMode::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �A�N�V�����t���O�X�V
	UpdateActionFlag();

	// �s���t���O�X�V
	UpdateMoveFlag();
}

//================================================================================
// �s���t���O�̍X�V����
//================================================================================
void CPlayerAIControlMode::UpdateMoveFlag()
{
	// �s���t���O�X�V
	(this->*(m_MoveFlagFunc[m_eMoveFlag]))();
}

//--------------------------------------------------------------------------
// �~�܂�
//--------------------------------------------------------------------------
void CPlayerAIControlMode::MoveFlagStop()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �s���t���O���Z�b�g
	pControlAIMove->SetIsWalk(false);
	pControlAIMove->SetIsDash(false);
	pControlAIMove->SetIsBlink(false);
}

//--------------------------------------------------------------------------
// ����
//--------------------------------------------------------------------------
void CPlayerAIControlMode::MoveFlagWalk()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ����ON
	pControlAIMove->SetIsWalk(true);

	// �t���OOFF
	pControlAIMove->SetIsDash(false);
	pControlAIMove->SetIsBlink(false);
}

//--------------------------------------------------------------------------
// �u�����N
//--------------------------------------------------------------------------
void CPlayerAIControlMode::MoveFlagBlink()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �u�����NON
	pControlAIMove->SetIsBlink(true);

	// �t���OOFF
	pControlAIMove->SetIsWalk(false);
	pControlAIMove->SetIsDash(false);
}

//--------------------------------------------------------------------------
// ����
//--------------------------------------------------------------------------
void CPlayerAIControlMode::MoveFlagDash()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ����ON
	pControlAIMove->SetIsWalk(true);
	// �u�����NON
	pControlAIMove->SetIsBlink(true);
	// ����ON
	pControlAIMove->SetIsDash(true);
}

//================================================================================
// �A�N�V�����t���O�̍X�V����
//================================================================================
void CPlayerAIControlMode::UpdateActionFlag()
{
	// �A�N�V�����X�V
	(this->*(m_ActionFlagFunc[m_eActionFlag]))();
}

//--------------------------------------------------------------------------
// �Ȃɂ����Ȃ�
//--------------------------------------------------------------------------
void CPlayerAIControlMode::ActionFlagNone()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// �W�����v�Ftrue
	pControlAIAction->SetIsJump(false);
	pControlAIAction->SetIsJumpFloat(false);

	// �A�N�V�����񋓁F�Ȃ�
	m_eActionFlag = EActionFlag::ACTION_NONE;
}

//--------------------------------------------------------------------------
// �W�����v
//--------------------------------------------------------------------------
void CPlayerAIControlMode::ActionFlagJump()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// �W�����v�Ftrue
	pControlAIAction->SetIsJump(true);
	pControlAIAction->SetIsJumpFloat(true);

	// ����
	pControlAIAction->SetJumpRate(JUMP_RATE);

	// �A�N�V�����񋓁F�Ȃ�
	m_eActionFlag = EActionFlag::ACTION_NONE;
}