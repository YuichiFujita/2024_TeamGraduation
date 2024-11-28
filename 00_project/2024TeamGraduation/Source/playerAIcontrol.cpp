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
	// �W�����v�����̍ő�ʒu(�W�����v��MAX�F150)
	const float JUMP_END_POS = 140.0f;

	// ������(�f�t�H���g)
	const float LENGTH_IN = 500.0f;
	const float LENGTH_SPACE = 10.0f;
	const float LENGTH_OUT = 200.0f;
	const float LENGTH_LINE = 100.0f;

	// ���z������(���S(x)����̋���)
	const float LINE_DISTANCE_OVER = 100.0f;	// ����������̋���(���S x:0.0f)
	const float RETURN_POS = 700.0f;			// �߂�ʒu(���S x:0.0f)

	const float OK_LENGTH = 10.0f;				// ����͈̔�(�ړI�Ƃ̋���)

	// ����
	const float CHATCH_LENGTH_IN = 500.0f;
	const float CHATCH_LENGTH_OUT = 700.0f;
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CPlayerAIControl::AREA_FUNC CPlayerAIControl::m_AreaFunc[] =	// ���[�h�֐�
{
	&CPlayerAIControl::AreaNone,				// �Ȃ�
	&CPlayerAIControl::AreaLeft,				// ��
	&CPlayerAIControl::AreaRight,				// �E
};

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

CPlayerAIControl::THROWMOVE_FUNC CPlayerAIControl::m_ThrowMoveFunc[] =	// �����^�C�v�֐�
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

CPlayerAIControl::THROW_FUNC CPlayerAIControl::m_ThrowFunc[] =	// �����^�C�~���O�֐�
{
	&CPlayerAIControl::ThrowNone,				// �Ȃ�
	&CPlayerAIControl::Throw,					// ����
	&CPlayerAIControl::ThrowPass,				// �p�X
	&CPlayerAIControl::ThrowSpecial,			// �X�y�V����
};

CPlayerAIControl::CATCH_FUNC CPlayerAIControl::m_CatchFunc[] =	// �L���b�`�֐�
{
	&CPlayerAIControl::CatchNone,			// �Ȃ�
	&CPlayerAIControl::CatchNormal,			// �ʏ�
	&CPlayerAIControl::CatchJust,			// �W���X�g
	&CPlayerAIControl::CatchDash,			// �_�b�V��
	&CPlayerAIControl::CatchPass,			// �p�X
	&CPlayerAIControl::CatchFindBall,		// ������
};

CPlayerAIControl::MOVEFORCIBLY_FUNC CPlayerAIControl::m_MoveForciblyFunc[] =	// �s���֐�
{
	&CPlayerAIControl::ForciblyNone,		// �Ȃ�
	&CPlayerAIControl::ForciblyReturn,		// �߂�
};

CPlayerAIControl::MOVE_FUNC CPlayerAIControl::m_MoveFunc[] =	// �s���֐�
{
	&CPlayerAIControl::MoveStop,			// �~�܂�
	&CPlayerAIControl::MoveWalk,			// ����
	&CPlayerAIControl::MoveDash,			// ����
};

CPlayerAIControl::ACTION_FUNC CPlayerAIControl::m_ActionFunc[] =	// �s���֐�
{
	&CPlayerAIControl::ActionNone,
	&CPlayerAIControl::ActionJump,
};


//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControl::CPlayerAIControl()
{
	// �����o�ϐ��̏�����
	ZeroMemory(&m_sInfo, sizeof(m_sInfo));
	ZeroMemory(&m_sTarget, sizeof(m_sTarget));

	m_pAI = nullptr;
	m_pTarget = nullptr;
	m_bStart = false;
	m_bEnd = false;
	m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
	m_eMove = EMoveType::MOVETYPE_STOP;
	m_eHeart = EHeart::HEART_NONE;
	m_eAction = EAction::ACTION_NONE;
	m_eThrow = EThrow::THROW_NONE;
}

//==========================================================================
// ����
//==========================================================================
CPlayerAIControl* CPlayerAIControl::Create(CPlayer* player)
{
	// �������̊m��
	CPlayerAIControl* pObj = DEBUG_NEW CPlayerAIControl;
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
// ����������
//==========================================================================
HRESULT CPlayerAIControl::Init()
{
	// �l�̏�����
	m_sTarget.fDistanceIN = LENGTH_IN;
	m_sTarget.fDistanceOUT = LENGTH_OUT;
	m_eLine = ELine::LINE_IN;
	m_eHeart = EHeart::HEART_NORMAL;
	m_eThrow = EThrow::THROW_NONE;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerAIControl::Uninit()
{
	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIControl::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���[�h�Ǘ�
	ModeManager(fDeltaTime, fDeltaRate, fSlowRate);

	AttackDash();

	// �s���Ǘ�
	MoveManager(fDeltaTime, fDeltaRate, fSlowRate);

	// �A�N�V�����X�V
	(this->*(m_ActionFunc[m_eAction]))();

	// �����X�V
	(this->*(m_ThrowFunc[m_eThrow]))();

	// �S
	//PlanHeart();
}

//==========================================================================
// �G���A�F�Ȃ�
//==========================================================================
void CPlayerAIControl::AreaNone()
{
	assert(false);
}

//==========================================================================
// �G���A�F��
//==========================================================================
void CPlayerAIControl::AreaLeft()
{

}

//==========================================================================
// �G���A�F�E
//==========================================================================
void CPlayerAIControl::AreaRight()
{

}


//==========================================================================
// ���[�h�Ǘ�
//==========================================================================
void CPlayerAIControl::ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsLineOverPlayer())
	{// ���𒴂��Ă�����
		m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
		return;
	}

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() == nullptr ||
		pBall->GetTypeTeam() != m_pAI->GetTeam())
	{// �{�[�����擾����Ă��Ȃ��ꍇ||�����ƃ{�[���������Ă���`�[�����Ⴄ�ꍇ
		m_sInfo.sMode.eMode = EMode::MODE_CATCH;
	}
	//else if (pBall->GetPlayer() == m_pAI)
	//{// �{�[���������Ă���̂������������ꍇ
	//	m_sInfo.sMode.eMode = EMode::MODE_THROW;
	//}

	// ���[�h�X�V
	(this->*(m_ModeFunc[m_sInfo.sMode.eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �����̊Ǘ�
//==========================================================================
void CPlayerAIControl::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//// 1:�^�[�Q�b�g�����߂�
	//m_pTarget = nullptr;
	//m_pTarget = GetThrowTarget();

	//// 2:���𓊂��邩�l����֐�
	//PlanThrow(m_pTarget, fDeltaTime, fDeltaRate, fSlowRate);


#ifdef _DEBUG
	if (!m_pTarget)
	{
		return;
	}

	CEffect3D::Create
	(// �f�o�b�O�p�G�t�F�N�g(�^�[�Q�b�g)
		m_pTarget->GetPosition(),
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
// �S�̃v����
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
		m_eHeart = EHeart::HEART_NORMAL;
	}
}

//==========================================================================
// ���𓊂��邩
//==========================================================================
void CPlayerAIControl::PlanThrow(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����Ă悵�I&&�^�C�~���O�ݒ�ςȂ�
	//if (!m_sInfo.sThrowInfo.bThrow && m_sInfo.sThrowInfo.bTiming) return;

	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	CGameManager* pGameManager = CGameManager::GetInstance();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(m_pAI->GetTeam());

	if (m_sInfo.sThrowInfo.eType == EThrowType::THROWTYPE_NONE)
	{
		if (pTeamStatus->IsMaxSpecial())
		{// �Q�[�W�����܂��Ă�����
			// �X�y�V��������
			m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_SPECIAL;
		}
		else
		{// ������t���O���I����������
			int n = 0;
			// ���̓����_���Ō���
			//n = rand() % 2;
			n = 0;

			switch (n)
			{
			case 0:	// �ʏ�
				m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_NORMAL;
				break;

			case 1:	// �W�����v
				m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_JUMP;
				break;

			default:
				assert(false);
				break;
			}
		}
	}

	// ������^�C�v�̍X�V
	(this->*(m_ThrowTypeFunc[m_sInfo.sThrowInfo.eType]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �ʏ퓊��
//==========================================================================
void CPlayerAIControl::ThrowTypeNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���̏�Ȃ̂������̂�����̂�
	m_eMove = EMoveType::MOVETYPE_WALK;

	// ������܂ł̍s���̍X�V
	(this->*(m_ThrowMoveFunc[m_eMove]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �W�����v����
//==========================================================================
void CPlayerAIControl::ThrowTypeJump(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���̏�Ȃ̂������̂�����̂�
	m_eMove = EMoveType::MOVETYPE_WALK;

	// ������܂ł̍s���̍X�V
	(this->*(m_ThrowMoveFunc[m_eMove]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �X�y�V��������
//==========================================================================
void CPlayerAIControl::ThrowTypeSpecial(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���̏�Ȃ̂������̂�����̂�

	// ������܂ł̍s���̍X�V
	(this->*(m_ThrowMoveFunc[m_eMove]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���̏꓊��
//==========================================================================
void CPlayerAIControl::ThrowMoveNone(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�~���O�͂ǂ�����H

	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	pControlAIMove->SetIsWalk(false);	// �������Z�b�g

	// ������
	pControlAIAction->SetIsThrow(true);

	m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_NONE;

	
	// ������܂ł̃^�C�~���O�̍X�V
	//(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ��������
//==========================================================================
void CPlayerAIControl::ThrowMoveWalk(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (IsLineOverPlayer())
	{// ���𒴂��Ă����ꍇ(���l�͌Œ�ϐ�)
		m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
		return;
	}

	// �^�C�~���O�͂ǂ�����H
	m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_NORMAL;

	//Timing(pTarget);

	// ������܂ł̃^�C�~���O�̍X�V
	(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���蓊��
//==========================================================================
void CPlayerAIControl::ThrowMoveDash(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsLineOverPlayer())
	{// ���𒴂��Ă����ꍇ(���l�͌Œ�ϐ�)
		m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
		return;
	}

	// �^�C�~���O�͂ǂ�����H
	m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_NORMAL;

	// ������܂ł̃^�C�~���O�̍X�V
	(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���Ԃ��ǂ��� �܂������Q�l�ɂ��邩�͖���
//==========================================================================
void CPlayerAIControl::PlanIsJump(CPlayer* pTarget)
{
	if (pTarget == nullptr) return;

	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// ����������
	MyLib::Vector3 posTarget = pTarget->GetPosition();
	MyLib::Vector3 MyPos = m_pAI->GetPosition();

	// �������瑊��̋���
	float fDistance = MyPos.DistanceXZ(posTarget);
	
	if (fDistance > m_sTarget.fDistanceIN + 50.0f && fDistance < m_sTarget.fDistanceIN - 50.0f)
	{// ����Ă�����
		//pControlAIAction->SetIsJump(true);

		/*m_sMoveInfo.bJump = false;*/
	}
}

//==========================================================================
// �W�����v�����^�C�~���O
//==========================================================================
void CPlayerAIControl::JumpThrowTiming(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�~���O�v�l
	Timing(pTarget);

	// ��ԍX�V
	(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �^�C�~���O�̎v�l ����RAND
//==========================================================================
void CPlayerAIControl::Timing(CPlayer* pTarget)
{
	if (m_sInfo.sThrowInfo.bTiming) { return; }

	if (m_sInfo.sThrowInfo.eTiming == EThrowType::THROWTYPE_NORMAL)
	{// �ʏ퓊��
		// �����_���Ń^�C�~���O�����߂�
		m_sInfo.sThrowInfo.fTiming = timing::TIMING_NORMAL + UtilFunc::Transformation::Random(timing::TIMING_RAND_MIN, timing::TIMING_RAND_MAX) * 0.01f;

		m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_NORMAL;	// �ʏ퓊��
		m_sInfo.sThrowInfo.bTiming = true;	// �^�C�~���O�t���O�I��


		//// ���S�Ƃ̋���������
		//MyLib::Vector3 myPos = m_pAI->GetPosition();
		//MyLib::Vector3 pos = { 0.0f, myPos.y, myPos.z };
		//float fRito = myPos.DistanceXZ(pos);

		//if (fRito > 0)
		//{
		//	return;
		//}

	}
	else if (m_sInfo.sThrowInfo.eTiming == EThrowType::THROWTYPE_JUMP)
	{// �W�����v����

		// ���g����݂��^�[�Q�b�g�̈ʒu
		float fDistance = m_pAI->GetPosition().DistanceXZ(pTarget->GetPosition());

		while (true)
		{
			// �^�C�~���O�̐ݒ�
			int n = rand() % 3;
			//int n = 3;

			if (n == 0)
			{// �ԍ�0
				m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_NORMAL;
				m_sInfo.sThrowInfo.bTiming = true;
				break;

			}
			else if (n == 1 && fDistance < m_sTarget.fDistanceIN - 100.0f)
			{// �ԍ�2&&�^�[�Q�b�g�Ƃ̋������ȓ��̏ꍇ
				m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_QUICK;
				m_sInfo.sThrowInfo.bTiming = true;
				break;
			}
			else if (n == 2 && fDistance < m_sTarget.fDistanceIN - 100.0f)
			{// �ԍ�1&&�^�[�Q�b�g�Ƃ̋������ȓ��̏ꍇ

				m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_DELAY;
				m_sInfo.sThrowInfo.bTiming = true;
				break;
			}
		}
	}
}

//--------------------------------------------------------------------------
// �����^�C�~���O(�ʏ�)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sInfo.sThrowInfo.fTiming > 0.0f)
	{
		m_sInfo.sThrowInfo.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// �~�܂�
	m_eMove = EMoveType::MOVETYPE_STOP;

	// ������
	m_eThrow = EThrow::THROW_THROW;

	// ������
	ZeroMemory(&m_sInfo, sizeof(m_sInfo));
}

//--------------------------------------------------------------------------
// �����^�C�~���O(����)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingQuick(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//--------------------------------------------------------------------------
// �����^�C�~���O(�x��)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingDelay(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//--------------------------------------------------------------------------
// ���ѓ����^�C�~���O(�ʏ�)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingJumpNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	m_eMove = EMoveType::MOVETYPE_STOP;
	m_eAction = EAction::ACTION_JUMP;

	if (m_pAI->GetPosition().y >= JUMP_END_POS)	// �����ɂ���ĕς��
	{
		m_eThrow = EThrow::THROW_THROW;	// ������
	}
}

//--------------------------------------------------------------------------
// ���ѓ����^�C�~���O(����)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingJumpQuick(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �ǂꂭ�炢�̋����ړ����邩������ǉ�


	pControlAIMove->SetIsWalk(false);	// �������Z�b�g
	pControlAIAction->SetIsJump(true);	// �W�����v�I��

	if (m_pAI->GetPosition().y >= JUMP_END_POS * 0.5f)
	{
		// ������
		pControlAIAction->SetIsThrow(true);

		// �ϐ����Z�b�g
		//ResetFlag();
	}
}

//--------------------------------------------------------------------------
// ���ѓ����^�C�~���O(�x��)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingJumpDelay(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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

	if (pos.y >= JUMP_END_POS)	// �����ɂ���ĕς��
	{
		m_sInfo.sThrowInfo.bFoldJump = true;	// �܂�Ԃ��I��
	}

	if (!m_sInfo.sThrowInfo.bFoldJump) { return; }

	if (pos.y <= JUMP_END_POS * 0.5f)	// �����ɂ���ĕς��
	{
		// ������
		pControlAIAction->SetIsThrow(true);

		// ���ꂼ��̏�Ԃ̃��Z�b�g
		//ResetFlag();
	}
}

// �L���b�`�֘A =========================================================================================================================================================

//==========================================================================
// �L���b�`�̊Ǘ�
//==========================================================================
void CPlayerAIControl::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;
	CPlayer* pPlayer = pBall->GetPlayer();

	if (pPlayer != nullptr)
	{// �N�����{�[���������Ă���&&�����Ɠ����ꍇ

		if (pPlayer->GetTeam() == m_pAI->GetTeam() && pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT)
		{// �����`�[��&&�O��̏ꍇ
			m_sInfo.sCatchInfo.eCatchType = ECatchType::CATCH_TYPE_PASS;
		}
		else
		{
			m_sInfo.sCatchInfo.eCatchType = ECatchType::CATCH_TYPE_NORMAL;
		}
	}
	else if (pBall->GetPlayer() == nullptr)
	{// �N���{�[���������Ă��Ȃ��ꍇ
		if (IsWhoPicksUpTheBall() && IsLineOverBall())
		{// ���������{�[���ɋ߂��l������&&�{�[�������葤�ɂ���
			m_sInfo.sCatchInfo.eCatchType = ECatchType::CATCH_TYPE_NORMAL;
			
		}
		else
		{// �{�[�������ɍs��
			m_sInfo.sCatchInfo.eCatchType = ECatchType::CATCH_TYPE_FIND;
		}
	}

	// �L���b�`��ލX�V
	(this->*(m_CatchFunc[m_sInfo.sCatchInfo.eCatchType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//--------------------------------------------------------------------------
// �ʏ�L���b�`
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �p�X�^�[�Q�b�g�������̏ꍇ
	if (IsPassTarget()) return;

	// �{�[������������擾
	CPlayer* pTarget = GetCatchTarget();
	if (!pTarget) { return; }

	// ����(�{�[���������Ă���z�Ƃ�)
	CatchDistance(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//--------------------------------------------------------------------------
// �L���b�`�F�W���X�g
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//--------------------------------------------------------------------------
// �L���b�`�F�_�b�V��
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//--------------------------------------------------------------------------
// �L���b�`�F�p�X
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchPass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
// �L���b�`�F���ɍs��
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchFindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
// �L���b�`���̋���
//==========================================================================
void CPlayerAIControl::CatchDistance(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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

		if (Leave(pTarget->GetPosition(), 300.0f))
		{
			m_eMove = EMoveType::MOVETYPE_STOP;
		}
	}
	else if (area = CPlayer::EFieldArea::FIELD_OUT)
	{// �^�[�Q�b�g���O��

		m_eMove = EMoveType::MOVETYPE_WALK;

		if (Leave(pTarget->GetPosition(), 300.0f))
		{
			m_eMove = EMoveType::MOVETYPE_STOP;
		}
	}
	else
	{// �G���A�̎擾�Ɏ��s
		assert(false);
	}
}


// �s���֘A =========================================================================================================================================================

//==========================================================================
// �����F�߂�
//==========================================================================
void CPlayerAIControl::ForciblyReturn()
{
	// �v���C���[���擾
	MyLib::Vector3 myPos = m_pAI->GetPosition();
	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();

	if (typeTeam == CGameManager::ETeamSide::SIDE_LEFT)
	{// ��
		// AI�R���g���[�����̎擾
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// ����
		m_eMove = EMoveType::MOVETYPE_WALK;

		// �߂Â�
		if (Approatch({ -RETURN_POS, myPos.y, myPos.z }, OK_LENGTH))
		{
			m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
			m_eMove = EMoveType::MOVETYPE_STOP;;
		}
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{// �E
		// AI�R���g���[�����̎擾
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// ����
		m_eMove = EMoveType::MOVETYPE_WALK;

		// �߂Â�
		if (Approatch({ RETURN_POS, myPos.y, myPos.z }, OK_LENGTH))
		{
			m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
			m_eMove = EMoveType::MOVETYPE_STOP;;
		}
	}
	else
	{// �G���[
		assert(false);
	}
}

//==========================================================================
// �s���Ǘ�
//==========================================================================
void CPlayerAIControl::MoveManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����s���X�V
	(this->*(m_MoveForciblyFunc[m_eForcibly]))();

	//if (m_eForcibly == EMoveForcibly::FORCIBLY_RETURN) return;

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

// �A�N�V���� =========================================================================================================================================================

//==========================================================================
// �A�N�V�����F��
//==========================================================================
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

//==========================================================================
// �A�N�V�����F�W�����v
//==========================================================================
void CPlayerAIControl::ActionJump()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// �W�����v�Ftrue
	pControlAIAction->SetIsJump(true);
	pControlAIAction->SetIsJumpFloat(true);

	pControlAIAction->SetJumpRate(1.0f);

	// �A�N�V�����񋓁F�Ȃ�
	m_eAction = EAction::ACTION_NONE;
}


// �����֘A =========================================================================================================================================================

//==========================================================================
// ������
//==========================================================================
void CPlayerAIControl::Throw()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// ������
	pControlAIAction->SetIsThrow(true);

	// �����Ȃ�
	m_eThrow = EThrow::THROW_NONE;
}

//==========================================================================
// �p�X
//==========================================================================
void CPlayerAIControl::ThrowPass()
{

}

//==========================================================================
// �X�y�V����
//==========================================================================
void CPlayerAIControl::ThrowSpecial()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// �X�y�V����
	pControlAIAction->SetIsSpecial(true);

	// �����Ȃ�
	m_eThrow = EThrow::THROW_SPECIAL;
}


// �߂Â��E����� =========================================================================================================================================================
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
	float direction = myPos.AngleXZ(targetPos);

	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall && pBall->GetPlayer())
	{
		if (pBall->GetPlayer()->GetTeam() != m_pAI->GetTeam() ||
			pBall->GetPlayer() == m_pAI)
		{// �Ⴄ�`�[���������Ă���ꍇ
			// �J�j�i�s�����̐ݒ�
			pControlAIMove->SetClabDirection(direction);
		}
	}
	else
	{
		m_pAI->SetRotDest(direction);
	}

	return false;
}

//==========================================================================
// �����O�� : Leave()
//==========================================================================
void CPlayerAIControl::LeaveOut(float distance)
{
	MyLib::Vector3 Mypos = m_pAI->GetPosition();	// �ʒu���̎擾

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayerOut = (*itr);	// �v���C���[���

		if ((m_pAI->GetTeam() == pPlayerOut->GetTeam()) ||
			(pPlayerOut->GetAreaType() == CPlayer::EFieldArea::FIELD_IN))
		{// �����`�[��||����̏ꍇ
			continue;
		}
		// �v���C���[�ʒu
		MyLib::Vector3 posPlayerOut = pPlayerOut->GetCenterPosition();

		// �G�Ƃ̋��������߂�
		float fLength = Mypos.DistanceXZ(posPlayerOut);

		if (fLength < distance)
		{ // �͈͂��߂��O�삪�����ꍇ
			Leave(posPlayerOut, m_sTarget.fDistanceOUT);	// �����F�O��
		}
	}
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
			pBall->GetPlayer() == m_pAI)
		{// �Ⴄ�`�[���������Ă���ꍇ
			// �J�j�i�s�����̐ݒ�
			pControlAIMove->SetClabDirection(direction);
		}
	}
	else
	{
		m_pAI->SetRotDest(direction);
	}

#ifdef _DEBUG
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
// ���蓊��
//==========================================================================
void CPlayerAIControl::AttackDash()
{
	// �^�[�Q�b�g�̂���Ƃ�
	CPlayer* pTarget = GetThrowTarget();
	if (!pTarget) return;
	MyLib::Vector3 posTarget = pTarget->GetPosition();
	// �����̈ʒu�擾
	MyLib::Vector3 posMy = m_pAI->GetPosition();

	// ���C���̈ʒu
	MyLib::Vector3 linePos = { 0.0f, posMy.y, posMy.z };

	// ���C���Ƃ̋���
	float distance = posMy.DistanceXZ(linePos);

	const float JUMP_POS_X = 300.0f;	// �W�����v�J�n�ʒu

	if (distance > JUMP_POS_X)
	{
		// ����
		m_eMove = EMoveType::MOVETYPE_DASH;

		// ����̈ʒu�ɋ߂Â�
		if (Approatch(posTarget, 0.0f))
		{
			m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
			m_eMove = EMoveType::MOVETYPE_STOP;
		}

		return;
	}

	m_eAction = EAction::ACTION_JUMP;

	if (m_pAI->GetPosition().y >= JUMP_END_POS)	// �����ɂ���ĕς��
	{
		m_eMove = EMoveType::MOVETYPE_STOP;
		m_eThrow = EThrow::THROW_THROW;	// ������
	}
}



//==========================================================================
// �L���b�`���̋���(�O��)
//==========================================================================
void CPlayerAIControl::CatchOutDistance()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	CPlayer* pTarget = nullptr;	// �ڕW�^�[�Q�b�g

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) { return; }
	CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// �`�[���^�C�v�̎擾

	// �������̎擾
	MyLib::Vector3 myPos = m_pAI->GetPosition();		// �ʒu���̎擾
	CGameManager::ETeamSide myTeam = m_pAI->GetTeam();	// �����`�[���̎擾

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[

	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���
		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// �v���C���[�ʒu		

		// ����||�O��ȊO�̏ꍇ
		if ((pPlayer->GetTeam() == myTeam) ||
			(pPlayer->GetAreaType() != CPlayer::EFieldArea::FIELD_OUT))
		{
			continue;
		}

		// ����Ƃ̋��������߂�
		float fLength = posPlayer.DistanceXZ(myPos);

		if (fLength < m_sTarget.fDistanceIN)
		{ // ���l���߂�����v���C���[�������ꍇ

			// �^�[�Q�b�g�ݒ�
			pTarget = pPlayer;

			// �J�j�i�s�����̐ݒ�
			float direction = pTarget->GetPosition().AngleXZ(myPos);
			pControlAIMove->SetClabDirection(direction);
		}
	}
}

//==========================================================================
// ���ɑ΂��Ă̋���(�L���b�`)
//==========================================================================
void CPlayerAIControl::CatchLineLeftDistance()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ���g�̈ʒu
	MyLib::Vector3 myPos = m_pAI->GetPosition();

	// �`�[���^�C�v�̎擾
	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();

	if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{// �E�`�[���̏ꍇ

		// �ڕW�ʒu
		MyLib::Vector3 Destpos = { 500.0f, myPos.y, myPos.z };

		// �������猩���ʒu
		myPos.AngleXZ(Destpos);

		//if (myPos.x < LINE_DISTANCE_OVER)
		//{// �������w��l�ȓ��̏ꍇ
		//	// �ړ���Ԃ𗣂��I
		//	m_eLine = ELine::LINE_OVER;
		//	// �����񂶂�Ȃ��I
		//	m_eMove = EMoveType::MOVETYPE_STOP;
		//}
		//else if (myPos.x > LINE_DISTANCE_OVER)
		//{// �������w��l�ȊO�̏ꍇ
		//	// �ړ���Ԃ��߂Â��I
		//	m_eLine = ELine::LINE_IN;
		//}
	}
}


// ����֘A ===================================================================================================================

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
// �N���{�[�������ɂ���������
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

//==========================================================================
// ����������(�v���C���[)
//==========================================================================
bool CPlayerAIControl::IsLineOverPlayer()
{
	bool bOver = false;	// ����������p

	// �v���C���[���擾
	MyLib::Vector3 myPos = m_pAI->GetPosition();
	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();

	if (typeTeam == CGameManager::ETeamSide::SIDE_LEFT)
	{// ��
		if (myPos.x > -LINE_DISTANCE_OVER)
		{// �ʒu�������Ă����ꍇ
			bOver = true;
		}
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{// �E
		if (myPos.x < LINE_DISTANCE_OVER)
		{// �ʒu�������Ă����ꍇ
			bOver = true;
		}
	}

	return bOver;
}

//==========================================================================
// ����������(�{�[��)
//==========================================================================
bool CPlayerAIControl::IsLineOverBall()
{
	bool bOver = false;	// ����������p

	// �{�[�����擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) { return bOver; }

	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();

	if (typeTeam == CGameManager::ETeamSide::SIDE_LEFT)
	{
		if (pBall->GetPosition().x > 0.0f)
		{
			bOver = true;
		}
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{
		if (pBall->GetPosition().x < 0.0f)
		{
			bOver = true;
		}
	}

	return bOver;
}


// �^�[�Q�b�g�ݒ� ===================================================================================================================
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
		{ continue; }

		// �G�Ƃ̋��������߂�
		float fLength = Mypos.DistanceXZ(posPlayer);

		if (fLength < fMinDis)
		{ // ���߂�����v���C���[�������ꍇ

			// �ŏ������̍X�V
			fMinDis = fLength;

			// �^�[�Q�b�g���X�V
			pTarget = pPlayer;

			// �����ݒ�(������������)
			m_pAI->SetRotDest(Mypos.AngleXZ(pTarget->GetPosition()));
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

	return pTarget;
}

// ���̑� ===================================================================================================================
//==========================================================================
// �t���O�̃��Z�b�g
//==========================================================================
void CPlayerAIControl::ResetFlag()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �s���t���O���Z�b�g
	pControlAIMove->SetIsWalk(false);
	pControlAIMove->SetIsDash(false);
	pControlAIMove->SetIsBlink(false);
	pControlAIAction->SetIsJump(false);
}