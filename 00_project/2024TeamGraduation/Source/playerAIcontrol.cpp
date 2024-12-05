//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playerAIcontrol.h"
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

#include "playerAIcontrol_left.h"
#include "playerAIcontrol_right.h"

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
// �֐��|�C���^
//==========================================================================
CPlayerAIControl::MODE_FUNC CPlayerAIControl::m_ModeFunc[] =	// ���[�h�֐�
{
	&CPlayerAIControl::ModeNone,				// �Ȃ�
	&CPlayerAIControl::ModeThrowManager,		// ����
	&CPlayerAIControl::ModeCatchManager,		// �L���b�`
};


CPlayerAIControl::THROWTYPE_FUNC CPlayerAIControl::m_ThrowTypeFunc[] =	// �����^�C�v�֐�
{
	&CPlayerAIControl::ThrowTypeNone,			// �Ȃ�
	&CPlayerAIControl::ThrowTypeNormal,			// �ʏ�
	&CPlayerAIControl::ThrowTypeJump,			// �W�����v
	&CPlayerAIControl::ThrowTypeSpecial,		// �X�y�V����
};

CPlayerAIControl::THROW_FUNC CPlayerAIControl::m_ThrowFunc[] =	// �����֐�
{
	&CPlayerAIControl::ThrowNone,				// �Ȃ�
	&CPlayerAIControl::Throw,					// ����
	&CPlayerAIControl::ThrowPass,				// �p�X
	&CPlayerAIControl::ThrowSpecial,			// �X�y�V����
};

CPlayerAIControl::THROWMOVE_FUNC CPlayerAIControl::m_ThrowMoveFunc[] =	// �����s���֐�
{
	&CPlayerAIControl::ThrowMoveNone,			// ���̏�
	&CPlayerAIControl::ThrowMoveWalk,			// ����
	&CPlayerAIControl::ThrowMoveDash,			// ����
};

CPlayerAIControl::THROWTIMING_FUNC CPlayerAIControl::m_ThrowTimingFunc[] =	// �����^�C�~���O�֐�
{
	&CPlayerAIControl::ThrowTimingNone,			// �Ȃ�
	&CPlayerAIControl::ThrowTimingNormal,		// �ʏ�
	&CPlayerAIControl::ThrowTimingQuick,		// ��
	&CPlayerAIControl::ThrowTimingDelay,		// �x
	&CPlayerAIControl::ThrowTimingJumpNormal,	// �W�����v�ʏ�
	&CPlayerAIControl::ThrowTimingJumpQuick,	// �W�����v��
	&CPlayerAIControl::ThrowTimingJumpDelay,	// �W�����v�x
};

CPlayerAIControl::MOVEFORCIBLY_FUNC CPlayerAIControl::m_MoveForciblyFunc[] =	// �s���֐�
{
	&CPlayerAIControl::ForciblyNone,			// �Ȃ�
	&CPlayerAIControl::ForciblyStop,			// �~�܂�
	&CPlayerAIControl::ForciblyReturn,			// �߂�
	&CPlayerAIControl::ForciblyStart,			// ����
};

CPlayerAIControl::MOVE_FUNC CPlayerAIControl::m_MoveFunc[] =	// �s���֐�
{
	&CPlayerAIControl::MoveStop,				// �~�܂�
	&CPlayerAIControl::MoveWalk,				// ����
	&CPlayerAIControl::MoveDash,				// ����
};

CPlayerAIControl::ACTION_FUNC CPlayerAIControl::m_ActionFunc[] =	// �s���֐�
{
	&CPlayerAIControl::ActionNone,				// �Ȃ�
	&CPlayerAIControl::ActionJump,				// �W�����v
};

CPlayerAIControl::CATCH_FUNC CPlayerAIControl::m_CatchFunc[] =	// �L���b�`�֐�
{
	&CPlayerAIControl::CatchNone,				// �Ȃ�
	&CPlayerAIControl::CatchNormal,				// �ʏ�
	&CPlayerAIControl::CatchJust,				// �W���X�g
	&CPlayerAIControl::CatchDash,				// �_�b�V��
	&CPlayerAIControl::CatchPassGive,			// �p�X�Ⴄ
	&CPlayerAIControl::CatchPassSteal,			// �p�X�D��
	&CPlayerAIControl::CatchFindBall,			// ������
};


//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControl::CPlayerAIControl()
{
	// �񋓂̏�����
	m_eMode = EMode::MODE_NONE;
	m_eForcibly = EMoveForcibly::FORCIBLY_START;
	m_eMove = EMoveType::MOVETYPE_STOP;
	m_eHeart = EHeart::HEART_NONE;
	m_eAction = EAction::ACTION_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
	m_eThrow = EThrow::THROW_NONE;
	m_eThrowTiming = EThrowTiming::TIMING_NONE;
	m_eCatchType = ECatchType::CATCH_TYPE_NONE;
}

//==========================================================================
// ����
//==========================================================================
CPlayerAIControl* CPlayerAIControl::Create(CPlayer* player)
{
	if (!player) assert(("�v���C���[��񂪖�����", false));

	// �������̊m��
	CPlayerAIControl* pObj = nullptr;

	switch (player->GetTeam())
	{// �`�[����
	case CGameManager::ETeamSide::SIDE_LEFT:
		pObj = DEBUG_NEW CPlayerAIControlLeft;
		break;

	case CGameManager::ETeamSide::SIDE_RIGHT:
		pObj = DEBUG_NEW CPlayerAIControlRight;
		break;

	default:
		assert(("�`�[����������", false));
		break;
	}

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

//==========================================================================
// �I������
//==========================================================================
void CPlayerAIControl::Uninit()
{
	if (!m_pAI) return;
	m_pAI->Uninit();
	m_pAI = nullptr;
}

#if 1
//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIControl::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���[�h�̍X�V
	UpdateMode();

	// ����X�V
	//UpdateSee();

	// ���[�h�Ǘ�
	ModeManager(fDeltaTime, fDeltaRate, fSlowRate);

	{// �񋓂��Ƃ̍s��
		// �s���X�V
		UpdateMove();

		// �A�N�V�����X�V
		UpdateAction();

		// �����X�V
		UpdateThrow();
	}
}
#endif

//==========================================================================
// ���[�h�X�V
//==========================================================================
void CPlayerAIControl::UpdateMode()
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) {// �{�[�����Ȃ��ꍇ
		m_eMode = EMode::MODE_NONE;
		return;
	}

	if (pBall->GetPlayer() == nullptr || pBall->GetTypeTeam() != m_pAI->GetTeam())
	{// �{�[�����擾����Ă��Ȃ��ꍇ||�����ƃ{�[���������Ă���`�[�����Ⴄ�ꍇ
		m_eMode = EMode::MODE_CATCH;
	}
	else if (pBall->GetPlayer() == m_pAI)
	{// �{�[���������Ă���̂������������ꍇ
		m_eMode = EMode::MODE_THROW;
	}
}

//==========================================================================
// ���[�h�Ǘ�
//==========================================================================
void CPlayerAIControl::ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsLineOverPlayer() && GetForcibly() != EMoveForcibly::FORCIBLY_START)
	{// ���𒴂��Ă���&&�����s���F���߈ȊO�̏ꍇ
		SetForcibly(EMoveForcibly::FORCIBLY_RETURN);
		return;
	}

	// ���[�h�X�V
	(this->*(m_ModeFunc[m_eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �����̊Ǘ�
//==========================================================================
void CPlayerAIControl::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����̗���
	PlanThrow();

	// �����̎��
	UpdateThrowType();

#ifdef _DEBUG

	// 1:�^�[�Q�b�g�����߂�
	CPlayer* pTarget = nullptr;
	pTarget = GetThrowTarget();

	if (!pTarget)
	{
		return;
	}

	CEffect3D::Create
	(// �f�o�b�O�p�G�t�F�N�g(�^�[�Q�b�g)
		pTarget->GetPosition(),
		VEC3_ZERO,
		MyLib::color::Red(),
		20.0f,
		0.1f,
		1,
		CEffect3D::TYPE::TYPE_NORMAL
	);	//
#endif
}

//==========================================================================
// �L���b�`�̊Ǘ�
//==========================================================================
void CPlayerAIControl::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �L���b�`�̍X�V
	UpdateCatch();
}

//==========================================================================
// ��������
//==========================================================================
void CPlayerAIControl::PlanThrow()
{
	// �擾
	CGameManager* pGameManager = CGameManager::GetInstance();	// �Q�[���}�l�[�W��
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(m_pAI->GetTeam());	// �����̃`�[�����

	if (m_eThrowType == EThrowType::THROWTYPE_NONE)
	{
		if (pTeamStatus->IsMaxSpecial())
		{// �Q�[�W�����܂��Ă�����
			// �X�y�V��������
			m_eThrowType = EThrowType::THROWTYPE_SPECIAL;
		}
		else
		{// ������t���O���I����������
			int n = 1;
			// ���̓����_���Ō���
			//int n = rand() % 2;

			/*float distance = m_pAI->GetPosition().DistanceXZ(m_pTarget->GetPosition());
			if (distance > 500.0f)
			{
				m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_JUMP;
			}
			else
			{
				m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_NORMAL;
			}*/

			switch (n)
			{
			case 0:	// �ʏ�
				m_eThrowType = EThrowType::THROWTYPE_NORMAL;
				break;

			case 1:	// �W�����v
				m_eThrowType = EThrowType::THROWTYPE_JUMP;
				break;

			default:
				assert(false);
				break;
			}
		}
	}
}

//================================================================================
// �����s���̍X�V����
//================================================================================
void CPlayerAIControl::UpdateForcibly()
{
	// �����s���X�V
	(this->*(m_MoveForciblyFunc[m_eForcibly]))();
}

//--------------------------------------------------------------------------
// �����F�~�܂�
//--------------------------------------------------------------------------
void CPlayerAIControl::ForciblyStop()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ����
	m_eMove = EMoveType::MOVETYPE_STOP;
}

#if 0
//--------------------------------------------------------------------------
// �����F�߂�
//--------------------------------------------------------------------------
void CPlayerAIControl::ForciblyReturn()
{
	// �v���C���[���擾
	MyLib::Vector3 myPos = m_pAI->GetPosition();
	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();

	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ����
	m_eMove = EMoveType::MOVETYPE_WALK;

	// �߂Â�
	if (Approatch({ RETURN_POS, myPos.y, myPos.z }, OK_LENGTH))
	{
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eMove = EMoveType::MOVETYPE_STOP;
	}
}
#endif

//--------------------------------------------------------------------------
// �����F���߂̃_�b�V��
//--------------------------------------------------------------------------
void CPlayerAIControl::ForciblyStart()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall)
	{// �{�[�����Ȃ��ꍇ
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		return;
	}

	CPlayer* pPlayer = pBall->GetPlayer();
	if (pPlayer)
	{// �N�����{�[�����������ꍇ
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		return;
	}

	m_eAction = EAction::ACTION_JUMP;
	m_eMove = EMoveType::MOVETYPE_WALK;

}

//================================================================================
// �s���̍X�V����
//================================================================================
void CPlayerAIControl::UpdateMove()
{
	// �����s���X�V
	(this->*(m_MoveForciblyFunc[m_eForcibly]))();

	// �s���X�V
	(this->*(m_MoveFunc[m_eMove]))();
}

//--------------------------------------------------------------------------
// �s���F�~�܂�
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveStop()
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
// �s���F����
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveWalk()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ����
	pControlAIMove->SetIsWalk(true);
}

//--------------------------------------------------------------------------
// �s���F����
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveDash()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ����
	pControlAIMove->SetIsWalk(true);
	// �u�����N
	pControlAIMove->SetIsBlink(true);
	// ����
	pControlAIMove->SetIsDash(true);
}

//================================================================================
// �A�N�V�����̍X�V����
//================================================================================
void CPlayerAIControl::UpdateAction()
{
	// �A�N�V�����X�V
	(this->*(m_ActionFunc[m_eAction]))();
}

//--------------------------------------------------------------------------
// �A�N�V�����F��
//--------------------------------------------------------------------------
void CPlayerAIControl::ActionNone()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// �W�����v�Ftrue
	pControlAIAction->SetIsJump(false);
	pControlAIAction->SetIsJumpFloat(false);

	// �A�N�V�����񋓁F�Ȃ�
	m_eAction = EAction::ACTION_NONE;
}

//--------------------------------------------------------------------------
// �A�N�V�����F�W�����v
//--------------------------------------------------------------------------
void CPlayerAIControl::ActionJump()
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
	m_eAction = EAction::ACTION_NONE;
}

//================================================================================
// �����^�C�v
//================================================================================
void CPlayerAIControl::UpdateThrowType()
{
	// ������ނ̍X�V
	(this->*(m_ThrowTypeFunc[m_eThrowType]))();
}

//--------------------------------------------------------------------------
// �ʏ퓊��
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTypeNormal()
{
	//// ���̏�Ȃ̂������̂�����̂�
	//m_eMove = EMoveType::MOVETYPE_WALK;

	//// ������܂ł̍s���̍X�V
	//(this->*(m_ThrowMoveFunc[m_eMove]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);

	m_eThrow = EThrow::THROW_NORMAL;
}

//--------------------------------------------------------------------------
// �W�����v����
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTypeJump()
{
	// ���̏�Ȃ̂������̂�����̂�
	//m_eMove = EMoveType::MOVETYPE_WALK;

	CPlayer* pTartget = GetThrowTarget();

	// ���蓊��
	AttackDash(pTartget);
}

//--------------------------------------------------------------------------
// �X�y�V��������
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTypeSpecial()
{
	m_eThrow = EThrow::THROW_SPECIAL;
}

//================================================================================
// �����s���̍X�V����
//================================================================================
void CPlayerAIControl::UpdateThrowMove()
{

}

//--------------------------------------------------------------------------
// ���̏꓊��
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowMoveNone()
{
	// �^�C�~���O�͂ǂ�����H

	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �������Z�b�g
	pControlAIMove->SetIsWalk(false);

	// ������
	pControlAIAction->SetIsThrow(true);

	m_eThrowType = EThrowType::THROWTYPE_NONE;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;
}

//--------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowMoveWalk()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (IsLineOverPlayer() && m_eForcibly != EMoveForcibly::FORCIBLY_START)
	{// ���𒴂��Ă����ꍇ(���l�͌Œ�ϐ�)
		m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
		return;
	}

	// �^�C�~���O�͂ǂ�����H
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;
}

//--------------------------------------------------------------------------
// ���蓊��
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowMoveDash()
{
	if (IsLineOverPlayer())
	{// ���𒴂��Ă����ꍇ(���l�͌Œ�ϐ�)
		m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
		return;
	}

	// �^�C�~���O�͂ǂ�����H
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;
}

//================================================================================
// �����̍X�V����
//================================================================================
void CPlayerAIControl::UpdateThrow()
{
	// �����X�V
	(this->*(m_ThrowFunc[m_eThrow]))();
}

//--------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------
void CPlayerAIControl::Throw()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// ������
	pControlAIAction->SetIsThrow(true);

	// ������ԁF��
	m_eThrow = EThrow::THROW_NONE;
}

//--------------------------------------------------------------------------
// �p�X
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowPass()
{

}

//--------------------------------------------------------------------------
// �X�y�V����
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowSpecial()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// �X�y�V��������
	pControlAIAction->SetIsSpecial(true);

	// �����Ȃ�
	m_eThrow = EThrow::THROW_NONE;
	//m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_NONE;
}

//================================================================================
// �^�C�~���O�̍X�V����
//================================================================================
void CPlayerAIControl::UpdateThrowTiming(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԍX�V
	(this->*(m_ThrowTimingFunc[m_eThrowTiming]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//--------------------------------------------------------------------------
// �����^�C�~���O�F(�ʏ�)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �~�܂�
	m_eMove = EMoveType::MOVETYPE_STOP;

	// ������
	m_eThrow = EThrow::THROW_NORMAL;
}

//--------------------------------------------------------------------------
// �����^�C�~���O�F(����)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sThrow.fTiming > 0.0f)
	{
		m_sThrow.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// �~�܂�
	m_eMove = EMoveType::MOVETYPE_STOP;

	// ������
	m_eThrow = EThrow::THROW_NORMAL;
}

//--------------------------------------------------------------------------
// �����^�C�~���O�F(�x��)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sThrow.fTiming > 0.0f)
	{
		m_sThrow.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// �~�܂�
	m_eMove = EMoveType::MOVETYPE_STOP;

	// ������
	m_eThrow = EThrow::THROW_NORMAL;
}

//--------------------------------------------------------------------------
// ���ѓ����^�C�~���O�F(�ʏ�)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingJumpNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	m_eMove = EMoveType::MOVETYPE_STOP;
	m_eAction = EAction::ACTION_JUMP;

	if (m_pAI->GetPosition().y >= THROW_JUMP_END)	// �����ɂ���ĕς��
	{
		m_eThrow = EThrow::THROW_NORMAL;	// ������
	}
}

//--------------------------------------------------------------------------
// ���ѓ����^�C�~���O�F(����)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingJumpQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �ǂꂭ�炢�̋����ړ����邩������ǉ�


	pControlAIMove->SetIsWalk(false);	// �������Z�b�g
	pControlAIAction->SetIsJump(true);	// �W�����v�I��

	if (m_pAI->GetPosition().y >= THROW_JUMP_END * 0.5f)
	{
		// ������
		pControlAIAction->SetIsThrow(true);

		// �ϐ����Z�b�g
		//ResetFlag();
	}
}

//--------------------------------------------------------------------------
// ���ѓ����^�C�~���O�F(�x��)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingJumpDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �ǂꂭ�炢�̋����ړ����邩������ǉ�
	m_eMove = EMoveType::MOVETYPE_STOP;
	pControlAIAction->SetIsJump(true);	// �W�����v�I��

	// �ʒu���擾
	MyLib::Vector3 pos = m_pAI->GetPosition();

	if (pos.y >= THROW_JUMP_END)	// �����ɂ���ĕς��
	{
		m_sThrow.bFoldJump = true;	// �܂�Ԃ��I��
	}

	if (!m_sThrow.bFoldJump) { return; }

	if (pos.y <= THROW_JUMP_END * 0.5f)	// �����ɂ���ĕς��
	{
		// ������
		pControlAIAction->SetIsThrow(true);

		// ���ꂼ��̏�Ԃ̃��Z�b�g
		//ResetFlag();
	}
}

//================================================================================
// �L���b�`�̍X�V����
//================================================================================
void CPlayerAIControl::UpdateCatch()
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// �{�[���˂���
	CPlayer* pPlayer = pBall->GetPlayer();

	if (pPlayer != nullptr)
	{// �N�����{�[���������Ă���

		if (pPlayer->GetTeam() == m_pAI->GetTeam() && pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT)
		{// �����`�[��&&�O��̏ꍇ
			m_eCatchType = ECatchType::CATCH_TYPE_PASS_GIVE;	// �p�X�҂�
		}
		else
		{
			m_eCatchType = ECatchType::CATCH_TYPE_NORMAL;

		}
	}
	else
	{// �N���{�[���������Ă��Ȃ��ꍇ

		// �{�[���X�e�[�g�擾
		CBall::EState stateBall = pBall->GetState();

		if (stateBall == CBall::EState::STATE_PASS ||
			stateBall == CBall::EState::STATE_HOM_PASS ||
			stateBall == CBall::EState::STATE_MOVE)
		{// �p�X||�z�[�~���O�p�X||�ړ���Ԃ̏ꍇ
			m_eCatchType = ECatchType::CATCH_TYPE_PASS_STEAL;
		}
		else if (IsWhoPicksUpTheBall() && IsLineOverBall())
		{// ���������{�[���ɋ߂��l������&&�{�[�������葤�ɂ���
			m_eCatchType = ECatchType::CATCH_TYPE_NORMAL;

		}
		else
		{// �{�[�������ɍs��
			m_eCatchType = ECatchType::CATCH_TYPE_FIND;
		}
	}

	// �L���b�`��ލX�V
	(this->*(m_CatchFunc[m_eCatchType]))();
}

//--------------------------------------------------------------------------
// �ʏ�L���b�`
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchNormal()
{
	// �p�X�^�[�Q�b�g�������̏ꍇ
	//if (IsPassTarget()) return;

	// �{�[������������擾
	CPlayer* pTarget = GetCatchTarget();
	if (!pTarget) { return; }

	// ����(�{�[���������Ă���z�Ƃ�)
	CatchDistance(pTarget);
}

//--------------------------------------------------------------------------
// �L���b�`�F�W���X�g
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchJust()
{

}

//--------------------------------------------------------------------------
// �L���b�`�F�_�b�V��
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchDash()
{
	m_eMove = EMoveType::MOVETYPE_DASH;
}

//--------------------------------------------------------------------------
// �L���b�`�F�p�X��Ⴄ
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchPassGive()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	if (m_pAI->GetBall()) return;

	// ��������{�[���̈ʒu������
	float rot = m_pAI->GetPosition().AngleXZ(pBall->GetPosition());

	// �ݒ�
	m_pAI->SetRotDest(rot);

	// �s����ށF�~�܂�
	m_eMove = EMoveType::MOVETYPE_STOP;
}

//--------------------------------------------------------------------------
// �L���b�`�F�p�X��D��
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchPassSteal()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	// �{�[���X�e�[�g�擾
	CBall::EState stateBall = pBall->GetState();

	if (stateBall == CBall::EState::STATE_PASS ||
		stateBall == CBall::EState::STATE_HOM_PASS ||
		stateBall == CBall::EState::STATE_MOVE)
	{// �p�X||�z�[�~���O�p�X||�ړ���Ԃ̏ꍇ

		// �ʒu�̎擾
		MyLib::Vector3 posBall = pBall->GetPosition();		// �{�[��
		MyLib::Vector3 posEnd = pBall->GetPosPassEnd();		// �{�[���̃p�X�I���ʒu
		MyLib::Vector3 posMy = m_pAI->GetPosition();		// �����̈ʒu

		// �I���ʒu��x,z���Q�Ƃ����ʒu�̐ݒ�
		MyLib::Vector3 pos = { posEnd.x, posMy.y, posEnd.z };

		// �{�[���Ƃ̋���
		float distance = posMy.DistanceXZ(posBall);

		// �s����ԁF����
		m_eMove = EMoveType::MOVETYPE_DASH;

		// �{�[���̕��֍s��
		if (Approatch(pos, CATCH_JUMP_LENGTH) || distance < CATCH_JUMP_LENGTH)
		{// �I���ʒu�ɋ߂Â���||�{�[���Ƃ̋������͈͓��̏ꍇ
			if (posBall.y < CATCH_JUMP_HEIGHT)
			{
				m_eAction = EAction::ACTION_JUMP;
			}
		}

#ifdef _DEBUG
		CEffect3D::Create
		(// �f�o�b�O�p�G�t�F�N�g(�^�[�Q�b�g)
			pos,
			VEC3_ZERO,
			MyLib::color::Red(),
			20.0f,
			0.1f,
			1,
			CEffect3D::TYPE::TYPE_NORMAL
		);
#endif

		return;
	}
}

//--------------------------------------------------------------------------
// �L���b�`�F���ɍs��
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchFindBall()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (pBall == nullptr || pBall->GetPlayer() != nullptr)
	{// �{�[����nullptr&&�v���C���[���{�[��������Ă���ꍇ
		m_eMove = EMoveType::MOVETYPE_STOP;
		return;
	}

	// �s����ԁF����
	m_eMove = EMoveType::MOVETYPE_WALK;

	// �߂Â�
	if (Approatch(pBall->GetPosition(), 5.0f))
	{
		m_eMove = EMoveType::MOVETYPE_STOP;
	}
}



//==========================================================================
// �S
//==========================================================================
void CPlayerAIControl::PlanHeart()
{
	// �̗͂̎擾
	int nLifeMax = m_pAI->GetLifeOrigin();
	int nLife = m_pAI->GetLife();

	float n = (float)nLifeMax / (float)nLife;

	// �c��̗͂ŐS�������߂�
	if ((nLifeMax / nLife) > 8)
	{
		//m_eHeart = EHeart::HEART_NORMAL;
	}
}



//==========================================================================
// �L���b�`�F����
//==========================================================================
void CPlayerAIControl::CatchDistance(CPlayer* pTarget)
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �^�[�Q�b�g�̃G���A�̎擾
	CPlayer::EFieldArea area = pTarget->GetAreaType();

	MyLib::Vector3 myPos = m_pAI->GetPosition();

	// �G���A����
	if (area == CPlayer::EFieldArea::FIELD_IN)
	{// �^�[�Q�b�g������

		m_eMove = EMoveType::MOVETYPE_WALK;

		if (Leave(pTarget->GetPosition(), 500.0f))
		{
			m_eMove = EMoveType::MOVETYPE_STOP;
		}
	}
	else if (area = CPlayer::EFieldArea::FIELD_OUT)
	{// �^�[�Q�b�g���O��

		m_eMove = EMoveType::MOVETYPE_WALK;

		if (Leave(pTarget->GetPosition(), 500.0f))
		{
			m_eMove = EMoveType::MOVETYPE_STOP;
		}
	}
	else
	{// �G���A�̎擾�Ɏ��s
		assert(false);
	}
}

//==========================================================================
// �^�[�Q�b�g�ݒ�(�����鎞)
//==========================================================================
CPlayer* CPlayerAIControl::GetThrowTarget()
{
	CPlayer* pTarget = nullptr;	// �ڕW�^�[�Q�b�g
	float fMinDis = 1000000.0f;	// �߂��v���C���[

	MyLib::Vector3 Mypos = m_pAI->GetPosition();	// �ʒu���̎擾

	// �{�[���̎擾
	CBall* pBall = m_pAI->GetBall();
	if (pBall == nullptr) { return pTarget; }
	CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// �`�[���^�C�v�̎擾

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���
		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// �v���C���[�ʒu

		// �����`�[��||�O��̏ꍇ
		if ((typeTeam == pPlayer->GetTeam()) ||
			(pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT))
		{
			continue;
		}

		// �G�Ƃ̋��������߂�
		float fLength = Mypos.DistanceXZ(posPlayer);

		if (fLength < fMinDis)
		{ // ���߂�����v���C���[�������ꍇ

			// �ŏ������̍X�V
			fMinDis = fLength;

			// �^�[�Q�b�g���X�V
			pTarget = pPlayer;

			// ����
			SeeTarget(pTarget->GetPosition());
		}
	}

	return pTarget;
}

//==========================================================================
// �^�[�Q�b�g�ݒ�(�L���b�`��)
//==========================================================================
CPlayer* CPlayerAIControl::GetCatchTarget()
{
	CPlayer* pTarget = nullptr;	// �ڕW�^�[�Q�b�g

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) { return pTarget; }
	CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// �`�[���^�C�v�̎擾

	// �{�[���������Ă���z�Ǝ����������`�[���̏ꍇ
	if (typeTeam == m_pAI->GetTeam()) return pTarget;

	// �^�[�Q�b�g�̐ݒ�
	pTarget = pBall->GetPlayer();
	if (!pTarget) return nullptr;

	// ����
	SeeTarget(pTarget->GetPosition());

	return pTarget;
}


//================================================================================
// ����̍X�V����
//================================================================================
void CPlayerAIControl::UpdateSee()
{
	if (m_eMode == EMode::MODE_CATCH)
	{// �{�[��������
		SeeBall();
	}
	else if (m_eMode == EMode::MODE_THROW)
	{// �^�[�Q�b�g������		
		CPlayer* pTarget = GetThrowTarget();
		if (!pTarget) return;

		SeeTarget(pTarget->GetPosition());
	}
}

//==========================================================================
// �^�[�Q�b�g������
//==========================================================================
void CPlayerAIControl::SeeTarget(MyLib::Vector3 pos)
{
	if (!m_pAI) return;

	// �����ݒ�
	float angle = m_pAI->GetPosition().AngleXZ(pos);
	m_pAI->SetRotDest(angle);
}

//==========================================================================
// �{�[��������
//==========================================================================
void CPlayerAIControl::SeeBall()
{
	// �{�[�����̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;
	if (!m_pAI) return;

	// �{�[���Ƃ̊p�x
	float angle = m_pAI->GetPosition().AngleXZ(pBall->GetPosition());

	// �p�x�̐ݒ�
	m_pAI->SetRotDest(angle);
}


//==========================================================================
// ����� : Leave(����鑊��A����鋗��)
//==========================================================================
bool CPlayerAIControl::Leave(MyLib::Vector3 targetPos, float distance)
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �����̈ʒu���
	MyLib::Vector3 myPos = m_pAI->GetPosition();

	// ����Ǝ����̋���
	float length = myPos.DistanceXZ(targetPos);	// �����F����Ǝ���

	if (distance < length) {// �ړ��^�C�v�F��
		return true;
	}

	// ��������^�[�Q�b�g�̊p�x
	//float direction = myPos.AngleXZ(targetPos);

	float direction = targetPos.AngleXZ(myPos);

	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall && pBall->GetPlayer())
	{
		if (pBall->GetPlayer()->GetTeam() != m_pAI->GetTeam() ||
			pBall->GetPlayer() == m_pAI)
		{// �Ⴄ�`�[���������Ă���ꍇ
			// �J�j�i�s�����̐ݒ�
			pControlAIMove->SetClabDirection(direction);
			//m_pAI->SetRotDest(direction);
		}
	}
	else
	{
		//pControlAIMove->SetClabDirection(direction);
		m_pAI->SetRotDest(direction);
	}

	return false;
}

//==========================================================================
// �߂Â� : Approatch(�߂Â�����A�߂Â�����)
//==========================================================================
bool CPlayerAIControl::Approatch(MyLib::Vector3 targetPos, float distance)
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �����̈ʒu���
	MyLib::Vector3 myPos = m_pAI->GetPosition();

	// ����Ǝ����̋���
	float length = myPos.DistanceXZ(targetPos);	// �����F����Ǝ���

	if (distance > length) {// �ړ��^�C�v�F��
		return true;
	}

	// ��������^�[�Q�b�g�̊p�x
	float direction = myPos.AngleXZ(targetPos);

	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall && pBall->GetPlayer())
	{
		if (pBall->GetPlayer()->GetTeam() != m_pAI->GetTeam() ||
			pBall->GetPlayer() != m_pAI)
		{// �Ⴄ�`�[���������Ă���||�����傪�����ꍇ
			// �J�j�i�s�����̐ݒ�
			pControlAIMove->SetClabDirection(direction);
		}
	}
	else
	{
		m_pAI->SetRotDest(direction);
	}

#if 0
	CEffect3D::Create
	(// �f�o�b�O�p�G�t�F�N�g(�^�[�Q�b�g)
		targetPos,
		VEC3_ZERO,
		MyLib::color::Red(),
		20.0f,
		0.1f,
		1,
		CEffect3D::TYPE::TYPE_NORMAL
	);
#endif
	return false;
}

//==========================================================================
// �p�X�̑���͎����ł����H
//==========================================================================
bool CPlayerAIControl::IsPassTarget()
{
	// �{�[�����̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return false;

	bool b = pBall->IsPass();

	if (b)
	{
		return false;
	}

	if (pBall->GetTarget() == m_pAI)
	{
		return false;
	}

	// �{�[���p�X&&�^�[�Q�b�g������
	if (pBall->IsPass() && pBall->GetTarget() == m_pAI)
	{
		return true;
	}

	return false;
}

//==========================================================================
// �N���{�[�������ɍs���܂����H
//==========================================================================
bool CPlayerAIControl::IsWhoPicksUpTheBall()
{
	float fMyDis = 1000000.0f;	// �����̃{�[���Ƃ̋���
	float fTeamMemberDis = 1000000.0f;	// �`�[�������o�[�̃{�[���Ƃ̋���

	// �����̏��擾
	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();	// �`�[���^�C�v
	MyLib::Vector3 Mypos = m_pAI->GetPosition();	// �ʒu���

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return false;

	// �{�[���ʒu�擾
	MyLib::Vector3 posBall = pBall->GetPosition();
	fMyDis = Mypos.DistanceXZ(posBall);	// ��������{�[���Ƃ̋���

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���

		// �Ⴄ�`�[��||�O��̏ꍇ
		if ((typeTeam != pPlayer->GetTeam()) ||
			(pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT) &&
			(m_pAI == pPlayer))
		{
			continue;
		}

		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// �v���C���[�ʒu

		// �`�[������{�[���Ƃ̋��������߂�
		float fLength = posPlayer.DistanceXZ(posBall);

		if (fLength < fMyDis)
		{ // ���߂������v���C���[�������ꍇ

			return true;
		}
	}

	return false;
}
