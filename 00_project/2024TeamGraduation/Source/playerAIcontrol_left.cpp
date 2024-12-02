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
	// �W�����v�����̍ő�ʒu(�W�����v��MAX�F150)
	const float JUMP_END_POS = 130.0f;
	const float JUMP_RATE = 1.0f;

	// ������(�f�t�H���g)
	const float LENGTH_IN = 500.0f;
	const float LENGTH_SPACE = 10.0f;
	const float LENGTH_OUT = 200.0f;
	const float LENGTH_LINE = 100.0f;

	// ���z������(���S(x)����̋���)
	const float LINE_DISTANCE_OVER = 10.0f;	// ����������̋���(���S x:0.0f)
	const float RETURN_POS = 500.0f;			// �߂�ʒu(���S x:0.0f)

	const float OK_LENGTH = 10.0f;				// ����͈̔�(�ړI�Ƃ̋���)

	// ����
	const float CHATCH_LENGTH_IN = 500.0f;
	const float CHATCH_LENGTH_OUT = 700.0f;
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CPlayerAIControlLeft::AREA_FUNC CPlayerAIControlLeft::m_AreaFunc[] =	// ���[�h�֐�
{
	&CPlayerAIControlLeft::AreaNone,				// �Ȃ�
	&CPlayerAIControlLeft::AreaLeft,				// ��
	&CPlayerAIControlLeft::AreaRight,				// �E
};

CPlayerAIControlLeft::MODE_FUNC CPlayerAIControlLeft::m_ModeFunc[] =	// ���[�h�֐�
{
	&CPlayerAIControlLeft::ModeNone,				// �Ȃ�
	&CPlayerAIControlLeft::ModeThrowManager,		// ����
	&CPlayerAIControlLeft::ModeCatchManager,		// �L���b�`
};

CPlayerAIControlLeft::THROWTYPE_FUNC CPlayerAIControlLeft::m_ThrowTypeFunc[] =	// �����^�C�v�֐�
{
	&CPlayerAIControlLeft::ThrowTypeNone,			// �Ȃ�
	&CPlayerAIControlLeft::ThrowTypeNormal,			// �ʏ�
	&CPlayerAIControlLeft::ThrowTypeJump,			// �W�����v
	&CPlayerAIControlLeft::ThrowTypeSpecial,		// �X�y�V����
};

CPlayerAIControlLeft::THROWMOVE_FUNC CPlayerAIControlLeft::m_ThrowMoveFunc[] =	// �����^�C�v�֐�
{
	&CPlayerAIControlLeft::ThrowMoveNone,			// ���̏�
	&CPlayerAIControlLeft::ThrowMoveWalk,			// ����
	&CPlayerAIControlLeft::ThrowMoveDash,			// ����
};

CPlayerAIControlLeft::THROWTIMING_FUNC CPlayerAIControlLeft::m_ThrowTimingFunc[] =	// �����^�C�~���O�֐�
{
	&CPlayerAIControlLeft::ThrowTimingNone,			// �Ȃ�
	&CPlayerAIControlLeft::ThrowTimingNormal,		// �ʏ�
	&CPlayerAIControlLeft::ThrowTimingQuick,		// ��
	&CPlayerAIControlLeft::ThrowTimingDelay,		// �x
	&CPlayerAIControlLeft::ThrowTimingJumpNormal,	// �W�����v�ʏ�
	&CPlayerAIControlLeft::ThrowTimingJumpQuick,	// �W�����v��
	&CPlayerAIControlLeft::ThrowTimingJumpDelay,	// �W�����v�x
};

CPlayerAIControlLeft::THROW_FUNC CPlayerAIControlLeft::m_ThrowFunc[] =	// �����^�C�~���O�֐�
{
	&CPlayerAIControlLeft::ThrowNone,				// �Ȃ�
	&CPlayerAIControlLeft::Throw,					// ����
	&CPlayerAIControlLeft::ThrowPass,				// �p�X
	&CPlayerAIControlLeft::ThrowSpecial,			// �X�y�V����
};

CPlayerAIControlLeft::CATCH_FUNC CPlayerAIControlLeft::m_CatchFunc[] =	// �L���b�`�֐�
{
	&CPlayerAIControlLeft::CatchNone,			// �Ȃ�
	&CPlayerAIControlLeft::CatchNormal,			// �ʏ�
	&CPlayerAIControlLeft::CatchJust,			// �W���X�g
	&CPlayerAIControlLeft::CatchDash,			// �_�b�V��
	&CPlayerAIControlLeft::CatchPass,			// �p�X
	&CPlayerAIControlLeft::CatchFindBall,		// ������
};

CPlayerAIControlLeft::MOVEFORCIBLY_FUNC CPlayerAIControlLeft::m_MoveForciblyFunc[] =	// �s���֐�
{
	&CPlayerAIControlLeft::ForciblyNone,		// �Ȃ�
	&CPlayerAIControlLeft::ForciblyStop,		// �~�܂�
	&CPlayerAIControlLeft::ForciblyReturn,		// �߂�
	&CPlayerAIControlLeft::ForciblyStart,		// ����
};

CPlayerAIControlLeft::MOVE_FUNC CPlayerAIControlLeft::m_MoveFunc[] =	// �s���֐�
{
	&CPlayerAIControlLeft::MoveStop,			// �~�܂�
	&CPlayerAIControlLeft::MoveWalk,			// ����
	&CPlayerAIControlLeft::MoveDash,			// ����
};

CPlayerAIControlLeft::ACTION_FUNC CPlayerAIControlLeft::m_ActionFunc[] =	// �s���֐�
{
	&CPlayerAIControlLeft::ActionNone,
	&CPlayerAIControlLeft::ActionJump,
};


//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControlLeft::CPlayerAIControlLeft()
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
	// �l�̏�����
	m_pAI = CPlayerAIControl::GetPlayer();

	m_sTarget.fDistanceIN = LENGTH_IN;
	m_sTarget.fDistanceOUT = LENGTH_OUT;
	m_eLine = ELine::LINE_IN;
	m_eHeart = EHeart::HEART_NORMAL;
	m_eThrow = EThrow::THROW_NONE;
	m_eForcibly = EMoveForcibly::FORCIBLY_START;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerAIControlLeft::Uninit()
{
	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIControlLeft::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���[�h�Ǘ�
	ModeManager(fDeltaTime, fDeltaRate, fSlowRate);

	IsJumpCatch();

	// �s���Ǘ�
	MoveManager(fDeltaTime, fDeltaRate, fSlowRate);

	// �A�N�V�����Ǘ�
	ActionManager();

	// �����Ǘ�
	ThrowManager();

	// �S
	//PlanHeart();
}

//==========================================================================
// �G���A�F�Ȃ�
//==========================================================================
void CPlayerAIControlLeft::AreaNone()
{
	assert(false);
}

//==========================================================================
// �G���A�F��
//==========================================================================
void CPlayerAIControlLeft::AreaLeft()
{

}

//==========================================================================
// �G���A�F�E
//==========================================================================
void CPlayerAIControlLeft::AreaRight()
{

}


//==========================================================================
// ���[�h�Ǘ�
//==========================================================================
void CPlayerAIControlLeft::ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsLineOverPlayer() && m_eForcibly != EMoveForcibly::FORCIBLY_START)
	{// ���𒴂��Ă���&&�����s���F���� �ȊO�̏ꍇ
		m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
		return;
	}

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() == nullptr ||
		pBall->GetTypeTeam() != m_pAI->GetTeam())
	{// �{�[�����擾����Ă��Ȃ��ꍇ||�����ƃ{�[���������Ă���`�[�����Ⴄ�ꍇ
		m_sInfo.eMode = EMode::MODE_CATCH;
	}
	else if (pBall->GetPlayer() == m_pAI)
	{// �{�[���������Ă���̂������������ꍇ
		m_sInfo.eMode = EMode::MODE_THROW;
	}

	// ���[�h�X�V
	(this->*(m_ModeFunc[m_sInfo.eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �����̊Ǘ�
//==========================================================================
void CPlayerAIControlLeft::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 1:�^�[�Q�b�g�����߂�
	m_pTarget = nullptr;
	m_pTarget = GetThrowTarget();

	// 2:���𓊂��邩�l����֐�
	PlanThrow(m_pTarget, fDeltaTime, fDeltaRate, fSlowRate);


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
void CPlayerAIControlLeft::PlanHeart()
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
void CPlayerAIControlLeft::PlanThrow(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControlLeft::ThrowTypeNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//// ���̏�Ȃ̂������̂�����̂�
	//m_eMove = EMoveType::MOVETYPE_WALK;

	//// ������܂ł̍s���̍X�V
	//(this->*(m_ThrowMoveFunc[m_eMove]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);

	m_eThrow = EThrow::THROW_NORMAL;
}

//==========================================================================
// �W�����v����
//==========================================================================
void CPlayerAIControlLeft::ThrowTypeJump(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���̏�Ȃ̂������̂�����̂�
	m_eMove = EMoveType::MOVETYPE_WALK;

	// ���蓊��
	AttackDash();
}

//==========================================================================
// �X�y�V��������
//==========================================================================
void CPlayerAIControlLeft::ThrowTypeSpecial(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���̏�Ȃ̂������̂�����̂�

	m_eThrow = EThrow::THROW_SPECIAL;

	// ������܂ł̍s���̍X�V
	//(this->*(m_ThrowMoveFunc[m_eMove]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���̏꓊��
//==========================================================================
void CPlayerAIControlLeft::ThrowMoveNone(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControlLeft::ThrowMoveWalk(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
	m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_NORMAL;

	//Timing(pTarget);

	// ������܂ł̃^�C�~���O�̍X�V
	(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���蓊��
//==========================================================================
void CPlayerAIControlLeft::ThrowMoveDash(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControlLeft::PlanIsJump(CPlayer* pTarget)
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
void CPlayerAIControlLeft::JumpThrowTiming(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�~���O�v�l
	Timing(pTarget);

	// ��ԍX�V
	(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �^�C�~���O�̎v�l ����RAND
//==========================================================================
void CPlayerAIControlLeft::Timing(CPlayer* pTarget)
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
void CPlayerAIControlLeft::ThrowTimingNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �~�܂�
	m_eMove = EMoveType::MOVETYPE_STOP;

	// ������
	m_eThrow = EThrow::THROW_NORMAL;

	// ������
	ZeroMemory(&m_sInfo, sizeof(m_sInfo));
}

//--------------------------------------------------------------------------
// �����^�C�~���O(����)
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::ThrowTimingQuick(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sInfo.sThrowInfo.fTiming > 0.0f)
	{
		m_sInfo.sThrowInfo.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// �~�܂�
	m_eMove = EMoveType::MOVETYPE_STOP;

	// ������
	m_eThrow = EThrow::THROW_NORMAL;

	// ������
	ZeroMemory(&m_sInfo, sizeof(m_sInfo));
}

//--------------------------------------------------------------------------
// �����^�C�~���O(�x��)
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::ThrowTimingDelay(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sInfo.sThrowInfo.fTiming > 0.0f)
	{
		m_sInfo.sThrowInfo.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// �~�܂�
	m_eMove = EMoveType::MOVETYPE_STOP;

	// ������
	m_eThrow = EThrow::THROW_NORMAL;

	// ������
	ZeroMemory(&m_sInfo, sizeof(m_sInfo));
}

//--------------------------------------------------------------------------
// ���ѓ����^�C�~���O(�ʏ�)
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::ThrowTimingJumpNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
		m_eThrow = EThrow::THROW_NORMAL;	// ������
	}
}

//--------------------------------------------------------------------------
// ���ѓ����^�C�~���O(����)
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::ThrowTimingJumpQuick(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControlLeft::ThrowTimingJumpDelay(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControlLeft::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;
	CPlayer* pPlayer = pBall->GetPlayer();

	if (pPlayer != nullptr)
	{// �N�����{�[���������Ă���&&�����Ɠ����ꍇ

		if (pPlayer->GetTeam() == m_pAI->GetTeam() && pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT)
		{// �����`�[��&&�O��̏ꍇ
			m_sInfo.eCatchType = ECatchType::CATCH_TYPE_PASS;
		}
		else
		{
			m_sInfo.eCatchType = ECatchType::CATCH_TYPE_NORMAL;
		}
	}
	else if (pBall->GetPlayer() == nullptr)
	{// �N���{�[���������Ă��Ȃ��ꍇ
		if (IsWhoPicksUpTheBall() && IsLineOverBall())
		{// ���������{�[���ɋ߂��l������&&�{�[�������葤�ɂ���
			m_sInfo.eCatchType = ECatchType::CATCH_TYPE_NORMAL;
			
		}
		else
		{// �{�[�������ɍs��
			m_sInfo.eCatchType = ECatchType::CATCH_TYPE_FIND;
		}
	}

	// �L���b�`��ލX�V
	(this->*(m_CatchFunc[m_sInfo.eCatchType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//--------------------------------------------------------------------------
// �ʏ�L���b�`
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControlLeft::CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//--------------------------------------------------------------------------
// �L���b�`�F�_�b�V��
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//--------------------------------------------------------------------------
// �L���b�`�F�p�X
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::CatchPass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControlLeft::CatchFindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControlLeft::CatchDistance(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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


// �s���֘A =========================================================================================================================================================

//==========================================================================
// �����F�~�܂�
//==========================================================================
void CPlayerAIControlLeft::ForciblyStop()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ����
	m_eMove = EMoveType::MOVETYPE_STOP;
}

//==========================================================================
// �����F�߂�
//==========================================================================
void CPlayerAIControlLeft::ForciblyReturn()
{
	// �v���C���[���擾
	MyLib::Vector3 myPos = m_pAI->GetPosition();
	
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ����
	m_eMove = EMoveType::MOVETYPE_WALK;

	// �߂Â�
	if (Approatch({ -RETURN_POS, myPos.y, myPos.z }, OK_LENGTH))
	{
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eMove = EMoveType::MOVETYPE_STOP;
	}
}

//==========================================================================
// �����F���߂̃_�b�V��
//==========================================================================
void CPlayerAIControlLeft::ForciblyStart()
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

//==========================================================================
// �s���Ǘ�
//==========================================================================
void CPlayerAIControlLeft::MoveManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControlLeft::MoveStop()
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
void CPlayerAIControlLeft::MoveWalk()
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
void CPlayerAIControlLeft::MoveDash()
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
// �A�N�V�����Ǘ�
//==========================================================================
void CPlayerAIControlLeft::ActionManager()
{
	// �A�N�V�����X�V
	(this->*(m_ActionFunc[m_eAction]))();
}

//==========================================================================
// �A�N�V�����F��
//==========================================================================
void CPlayerAIControlLeft::ActionNone()
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
void CPlayerAIControlLeft::ActionJump()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// �W�����v�Ftrue
	pControlAIAction->SetIsJump(true);
	pControlAIAction->SetIsJumpFloat(true);

	pControlAIAction->SetJumpRate(JUMP_RATE);

	// �A�N�V�����񋓁F�Ȃ�
	m_eAction = EAction::ACTION_NONE;
}


// �����֘A =========================================================================================================================================================

//==========================================================================
// �����Ǘ�
//==========================================================================
void CPlayerAIControlLeft::ThrowManager()
{
	// �����X�V
	(this->*(m_ThrowFunc[m_eThrow]))();
}

//==========================================================================
// ������
//==========================================================================
void CPlayerAIControlLeft::Throw()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// ������
	pControlAIAction->SetIsThrow(true);

	// �����Ȃ�
	m_eThrow = EThrow::THROW_NONE;
	m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_NONE;
}

//==========================================================================
// �p�X
//==========================================================================
void CPlayerAIControlLeft::ThrowPass()
{

}

//==========================================================================
// �X�y�V����
//==========================================================================
void CPlayerAIControlLeft::ThrowSpecial()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// �X�y�V��������
	pControlAIAction->SetIsSpecial(true);

	// �����Ȃ�
	m_eThrow = EThrow::THROW_NONE;
	m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_NONE;
}


// �߂Â��E����� =========================================================================================================================================================
//==========================================================================
// ����� : Leave(����鑊��A����鋗��)
//==========================================================================
bool CPlayerAIControlLeft::Leave(MyLib::Vector3 targetPos, float distance)
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
// �����O�� : Leave()
//==========================================================================
void CPlayerAIControlLeft::LeaveOut(float distance)
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
bool CPlayerAIControlLeft::Approatch(MyLib::Vector3 targetPos, float distance)
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
			//m_pAI->SetRotDest(direction);
		}
	}
	else
	{
		//pControlAIMove->SetClabDirection(direction);
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
void CPlayerAIControlLeft::AttackDash()
{
	// �^�[�Q�b�g�̎擾
	CPlayer* pTarget = GetThrowTarget();
	if (!pTarget) return;
	MyLib::Vector3 posTarget = pTarget->GetPosition();

	// �����̈ʒu�擾
	MyLib::Vector3 posMy = m_pAI->GetPosition();

	// ���C���̈ʒu
	MyLib::Vector3 linePos = { 0.0f, posMy.y, posMy.z };

	// ���C���Ƃ̋���
	float distanceLine = 0.0f;
	float distanceTarget = 0.0f;
	float JUMP_LENGTH_TARGET = 500.0f;
	float JUMP_LENGTH_LINE = 300.0f;

	if (m_pTarget)
	{// �^�[�Q�b�g�������ꍇ
		distanceTarget = posMy.DistanceXZ(m_pTarget->GetPosition());	// �����Ƒ���̋���
		distanceLine = posMy.DistanceXZ(linePos);	// �����ƒ��S���Ƃ̋���
	}
	else
	{
		return;
	}

	if (distanceTarget > JUMP_LENGTH_TARGET && distanceLine > JUMP_LENGTH_TARGET)
	{// �����ƃ^�[�Q�b�g�̋�����700.0f�ȏ�&&
		// ����
		m_eMove = EMoveType::MOVETYPE_DASH;

		// ����̈ʒu�ɋ߂Â�
		if (Approatch(posTarget, JUMP_LENGTH_LINE))
		{// �͈͓��̏ꍇ
			m_eForcibly = EMoveForcibly::FORCIBLY_NONE;	// �����s���F�Ȃ�
			m_eMove = EMoveType::MOVETYPE_STOP;			// �s���F�~�܂�
		}

		return;
	}
		
	if (distanceTarget > JUMP_LENGTH_TARGET && distanceLine > JUMP_LENGTH_LINE)
	{// �^�[�Q�b�g�Ƃ̋������͈͈ȏ�&&�������Ƃ̋������͈͓��̏ꍇ
		// �A�N�V�����F����
		m_eAction = EAction::ACTION_JUMP;
	}
	else
	{
		m_eThrow = EThrow::THROW_NORMAL;		// ������
		m_eMove = EMoveType::MOVETYPE_STOP;
	}

	if (m_pAI->GetPosition().y >= JUMP_END_POS)	// �����ɂ���ĕς��
	{
		m_eMove = EMoveType::MOVETYPE_STOP;
		m_eThrow = EThrow::THROW_NORMAL;		// ������
	}
}


//==========================================================================
// �L���b�`���̋���(�O��)
//==========================================================================
void CPlayerAIControlLeft::CatchOutDistance()
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
void CPlayerAIControlLeft::CatchLineLeftDistance()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ���g�̈ʒu
	MyLib::Vector3 myPos = m_pAI->GetPosition();

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


// ����֘A ===================================================================================================================

//==========================================================================
// �p�X�̑���͎����ł����H
//==========================================================================
bool CPlayerAIControlLeft::IsPassTarget()
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
bool CPlayerAIControlLeft::IsWhoPicksUpTheBall()
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
// �v���C���[�͐��𒴂��Ă��܂����H
//==========================================================================
bool CPlayerAIControlLeft::IsLineOverPlayer()
{
	bool bOver = false;	// ����������p

	// �v���C���[���擾
	MyLib::Vector3 myPos = m_pAI->GetPosition();
	
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

	if (pBall->GetPosition().x > 0.0f)
	{
		bOver = true;
	}

	return bOver;
}


//==========================================================================
// �W�����v�Ń{�[���͎��܂����H
//==========================================================================
void CPlayerAIControlLeft::IsJumpCatch()
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

		const float CATCH_JUMP_LENGTH = 100.0f;
		const float CATCH_JUMP_HEIGHT = 300.0f;

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


// �^�[�Q�b�g�ݒ� ===================================================================================================================

//==========================================================================
// �^�[�Q�b�g�ݒ�(�����鎞)
//==========================================================================
CPlayer* CPlayerAIControlLeft::GetThrowTarget()
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
CPlayer* CPlayerAIControlLeft::GetCatchTarget()
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