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

CPlayerAIControl::THROWMOVE_FUNC CPlayerAIControl::m_ThrowMoveFunc[] =	// �����^�C�v�֐�
{
	&CPlayerAIControl::ThrowMoveNone,			// ���̏�
	&CPlayerAIControl::ThrowMoveWalk,			// ����
	&CPlayerAIControl::ThrowMoveDash,			// ����
};

CPlayerAIControl::THROWTIMING_FUNC CPlayerAIControl::m_ThrowTimingFunc[] =	// �����^�C�~���O�֐�
{
	&CPlayerAIControl::ThrowJumpTimingNone,		// �Ȃ�
	&CPlayerAIControl::ThrowJumpTimingNormal,	// �ʏ�
	&CPlayerAIControl::ThrowJumpTimingQuick,	// ��
	&CPlayerAIControl::ThrowJumpTimingDelay,	// �x
};

CPlayerAIControl::CATCH_FUNC CPlayerAIControl::m_CatchFunc[] =	// �L���b�`�֐�
{
	&CPlayerAIControl::CatchNone,			// �Ȃ�
	&CPlayerAIControl::CatchNormal,			// �ʏ�
	&CPlayerAIControl::CatchJust,			// �W���X�g
	&CPlayerAIControl::CatchDash,			// �_�b�V��
	&CPlayerAIControl::FindBall,			// ������
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControl::CPlayerAIControl()
{
	// �����o�ϐ��̏�����
	ZeroMemory(&m_sInfo, sizeof(m_sInfo));
	ZeroMemory(&m_sLearn, sizeof(m_sLearn));

	m_pAI = nullptr;
	m_bStart = false;
	m_bEnd = false;
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
	m_sLearn.fDistanceIN = LENGTH_IN;
	m_sLearn.fDistanceOUT = LENGTH_OUT;
	m_sLearn.fDistanceLine = LENGTH_LINE;
	m_eLine = ELine::LINE_IN;

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
	ModeManager();

	// ��ԍX�V
	(this->*(m_ModeFunc[m_sInfo.sMode.eMode]))(fDeltaTime, fDeltaRate, fSlowRate);

#ifdef _DEBUG
	Debug();
#endif
}

//==========================================================================
// ���[�h�Ǘ�
//==========================================================================
void CPlayerAIControl::ModeManager()
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() == nullptr ||
		pBall->GetTypeTeam() != m_pAI->GetTeam())
	{// �{�[�����擾����Ă��Ȃ��ꍇ||�����ƃ{�[���������Ă���`�[�����Ⴄ�ꍇ
		m_sInfo.sMode.eMode = EMode::MODE_CATCH;
	}
	else if (pBall->GetPlayer() == m_pAI)
	{// �{�[���������Ă���̂������������ꍇ
		m_sInfo.sMode.eMode = EMode::MODE_THROW;
	}
}

//==========================================================================
// �����̊Ǘ�
//==========================================================================
void CPlayerAIControl::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	PlanThrowFlow(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ������܂ł̗�����l����
//==========================================================================
void CPlayerAIControl::PlanThrowFlow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 1:�^�[�Q�b�g�����߂�
	CPlayer* pTarget = nullptr;
	pTarget = GetThrowTarget();

	// 2:���𓊂��邩�l����֐�	// ��
	PlanThrow(pTarget, fDeltaTime, fDeltaRate, fSlowRate);

#ifdef _DEBUG
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
// �S�̃v����
//==========================================================================
void CPlayerAIControl::PlanHeart()
{
	// �̗͂̎擾
	int nLife = m_pAI->GetParameter().nLife;

	// �c��̗͂ŐS�������߂�

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
				
				break;
			}

			// �^�C�~���O�̌��芮��
			m_sInfo.sThrowInfo.bTiming = true;
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

	m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_NONE;

	// ������܂ł̍s���̍X�V
	(this->*(m_ThrowMoveFunc[m_sInfo.sMoveInfo.eType]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �W�����v����
//==========================================================================
void CPlayerAIControl::ThrowTypeJump(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���̏�Ȃ̂������̂�����̂�
	m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_WALK;

	// ������܂ł̍s���̍X�V
	(this->*(m_ThrowMoveFunc[m_sInfo.sMoveInfo.eType]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �X�y�V��������
//==========================================================================
void CPlayerAIControl::ThrowTypeSpecial(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���̏�Ȃ̂������̂�����̂�

	// ������܂ł̍s���̍X�V
	(this->*(m_ThrowMoveFunc[m_sInfo.sMoveInfo.eType]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
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

	// �����I
	pControlAIMove->SetIsWalk(false);

	// ���蓊��
	//RunStartPos(pTarget);

	// �^�C�~���O�͂ǂ�����H
	m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_NORMAL;


	// ������܂ł̃^�C�~���O�̍X�V
	(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���蓊��
//==========================================================================
void CPlayerAIControl::ThrowMoveDash(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�~���O�͂ǂ�����H

	// ������܂ł̃^�C�~���O�̍X�V
	(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �����v����
//==========================================================================
void CPlayerAIControl::PlanThrowDistance(CPlayer* pTarget)
{
	if (pTarget == nullptr) return;

	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ����������
	MyLib::Vector3 posTarget = pTarget->GetPosition();
	MyLib::Vector3 MyPos = m_pAI->GetPosition();

	// �������瑊��̋���
	float fDistance = MyPos.DistanceXZ(posTarget);

	if (fDistance < m_sLearn.fDistanceIN - 50.0f)
	{// �����I
		m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_LEAVE;
		
		// ���肩�玩���̕���
		m_pAI->SetRotDest(posTarget.AngleXZ(MyPos));

		// �����I
		pControlAIMove->SetIsWalk(true);

		return;
	}
	else if(fDistance > m_sLearn.fDistanceIN + 50.0f)
	{// �߂Â��I
		m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_APPROATCH;

		// ���肩�玩���̕���
		m_pAI->SetRotDest(MyPos.AngleXZ(posTarget));

		// �����I
		pControlAIMove->SetIsWalk(true);

		return;
	}

	// �����񂶂�Ȃ��I
	m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_NONE;

	// �����ȁI
	pControlAIMove->SetIsWalk(false);

	// �����Ă悵�I
	m_sInfo.sThrowInfo.bThrow = true;
}

//==========================================================================
// ���Ԃ��ǂ��� �܂������Q�l�ɂ��邩�͖���
//==========================================================================
void CPlayerAIControl::PlanIsJump(CPlayer* pTarget)
{
	if (pTarget == nullptr && !m_sInfo.sMoveInfo.bJump) return;

	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// ����������
	MyLib::Vector3 posTarget = pTarget->GetPosition();
	MyLib::Vector3 MyPos = m_pAI->GetPosition();

	// �������瑊��̋���
	float fDistance = MyPos.DistanceXZ(posTarget);
	
	if (fDistance > m_sLearn.fDistanceIN + 50.0f && fDistance < m_sLearn.fDistanceIN - 50.0f)
	{// ����Ă�����
		pControlAIAction->SetIsJump(true);

		m_sInfo.sMoveInfo.bJump = false;
	}
}

//==========================================================================
// �s���v����
//==========================================================================
void CPlayerAIControl::PlanMove(CPlayer* pTarget)
{
	if (pTarget == nullptr) return;

	// ���ɑ΂���v�l
	Line(pTarget);
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
			//Reset();
			bOver = true;
		}
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{
		if (pBall->GetPosition().x < 0.0f)
		{
			//Reset();
			bOver = true;
		}
	}

	return bOver;
}

//==========================================================================
// �W�����v�����^�C�~���O
//==========================================================================
void CPlayerAIControl::JumpThrowTiming(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���ɑ΂��Ă̎v�l
	Line(pTarget);

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
			else if (n == 1 && fDistance < m_sLearn.fDistanceIN - 100.0f)
			{// �ԍ�2&&�^�[�Q�b�g�Ƃ̋������ȓ��̏ꍇ
				m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_QUICK;
				m_sInfo.sThrowInfo.bTiming = true;
				break;
			}
			else if (n == 2 && fDistance < m_sLearn.fDistanceIN - 100.0f)
			{// �ԍ�1&&�^�[�Q�b�g�Ƃ̋������ȓ��̏ꍇ

				m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_DELAY;
				m_sInfo.sThrowInfo.bTiming = true;
				break;
			}
		}
	}
}

//--------------------------------------------------------------------------
// ���ѓ����^�C�~���O(�ʏ�)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowJumpTimingNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	pControlAIMove->SetIsWalk(false);	// �������Z�b�g
	pControlAIAction->SetIsJump(true);	// �W�����v�I��

	if (m_pAI->GetPosition().y >= JUMP_END_POS)	// �����ɂ���ĕς��
	{// �J�E���g0�ȏ�
		// ������
		pControlAIAction->SetIsThrow(true);

		// �ϐ����Z�b�g
		Reset();
	}
}

//--------------------------------------------------------------------------
// ���ѓ����^�C�~���O(����)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowJumpTimingQuick(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
		//Reset();
	}
}

//--------------------------------------------------------------------------
// ���ѓ����^�C�~���O(�x��)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowJumpTimingDelay(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �ǂꂭ�炢�̋����ړ����邩������ǉ�


	pControlAIMove->SetIsWalk(false);	// �������Z�b�g
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
		//Reset();
	}
}

//==========================================================================
// ���ɑ΂��Ă̎v�l
//==========================================================================
void CPlayerAIControl::Line(CPlayer* pTarget)
{
	// �`�[���^�C�v�̎擾
	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();

	if (typeTeam == CGameManager::ETeamSide::SIDE_LEFT)
	{// �����`�[��
		LineLeftTeam(pTarget);
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{// �E���`�[��
		LineRightTeam(pTarget);
	}
}

//--------------------------------------------------------------------------
// ���ɑ΂��č����`�[��
//--------------------------------------------------------------------------
void CPlayerAIControl::LineLeftTeam(CPlayer* pTarget)
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �^�[�Q�b�g����(����)
	MyLib::Vector3 posTarget = {};					// �^�[�Q�b�g�ʒu
	MyLib::Vector3 MyPos = m_pAI->GetPosition();	// ���g�ʒu

	if (MyPos.x < 0.0f)
	{// �������w��l�ȓ��̏ꍇ

		// �ړ���Ԃ𗣂��I
		m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_LEAVE;

		// ���肩�玩���̕���
		m_pAI->SetRotDest(posTarget.AngleXZ(MyPos));

		// �����I
		pControlAIMove->SetIsWalk(true);
	}
}

//--------------------------------------------------------------------------
// ���ɑ΂��ĉE���`�[��
//--------------------------------------------------------------------------
void CPlayerAIControl::LineRightTeam(CPlayer* pTarget)
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �^�[�Q�b�g����(����)
	MyLib::Vector3 posTarget = {};	// �^�[�Q�b�g�ʒu
	MyLib::Vector3 MyPos = m_pAI->GetPosition();	// ���g�ʒu

	if (MyPos.x < 0.0f)
	{// �������w��l�ȓ��̏ꍇ

		// �ړ���Ԃ𗣂��I
		m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_LEAVE;

		// ���肩�玩���̕���
		m_pAI->SetRotDest(MyPos.AngleXZ(posTarget));

		// �����I
		pControlAIMove->SetIsWalk(true);
	}
}




// �L���b�` =========================================================================================================================================================
//==========================================================================
// �L���b�`�̊Ǘ�
//==========================================================================
void CPlayerAIControl::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() != nullptr)
	{// �N�����{�[���������Ă���ꍇ
		m_sInfo.sCatchInfo.eCatchType = ECatchType::CATCH_TYPE_NORMAL;
	}
	else if (pBall->GetPlayer() == nullptr)
	{// �N���{�[���������Ă��Ȃ��ꍇ

		// �������{�[���̋߂��ɖ����������ꍇ
		if (!IsWhoPicksUpTheBall()) return;

		// �{�[�������ɍs��
		m_sInfo.sCatchInfo.eCatchType = ECatchType::CATCH_TYPE_FIND;
	}

	// �������ލX�V
	(this->*(m_CatchFunc[m_sInfo.sCatchInfo.eCatchType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �ʏ�L���b�`
//==========================================================================
void CPlayerAIControl::CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[������������擾
	CPlayer* pTarget = GetCatchTarget();

	// ���Ƃ̋���
	CatchLineLeftDistance();

	// ����(�{�[���������Ă���z�Ƃ�)
	CatchDistance(pTarget);

	// �O��Ƃ̋���
	CatchOutDistance();


	CatchMoveFlag();
}

//==========================================================================
// �W���X�g�L���b�`
//==========================================================================
void CPlayerAIControl::CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �_�b�V���L���b�`
//==========================================================================
void CPlayerAIControl::CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �{�[�������ɍs��
//==========================================================================
void CPlayerAIControl::FindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsLineOverBall()) { return; }

	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (pBall == nullptr || pBall->GetPlayer() != nullptr) 
	{// �{�[����nullptr&&�v���C���[���{�[��������Ă���ꍇ
		Reset();	// �ϐ����Z�b�g
		return;
	}

	// �p�x�����߂�(player����݂��{�[��)
	float fAngle = m_pAI->GetPosition().AngleXZ(pBall->GetPosition());

	// �����I��!
	pControlAIMove->SetIsWalk(true);

	// �����ݒ�
	m_pAI->SetRotDest(fAngle);


#ifdef _DEBUG

	CEffect3D::Create
	(// �f�o�b�O�p�G�t�F�N�g(�^�[�Q�b�g)
		m_pAI->GetPosition(),
		VEC3_ZERO,
		MyLib::color::Purple(),
		20.0f,
		0.1f,
		1,
		CEffect3D::TYPE::TYPE_NORMAL
	);	// 
#endif

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
			(pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT))
		{
			continue;
		}

		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// �v���C���[�ʒu

		// �`�[������{�[���Ƃ̋��������߂�
		float fLength = posPlayer.DistanceXZ(posBall);

		if (fLength < fMyDis)
		{ // ���߂������v���C���[�������ꍇ

			return false;
		}
	}

	return true;
}

//==========================================================================
// �L���b�`���̋���(����)
//==========================================================================
void CPlayerAIControl::CatchDistance(CPlayer* pTarget)
{
	if (pTarget == nullptr) return;

	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ����������
	MyLib::Vector3 TargetPos = pTarget->GetPosition();
	MyLib::Vector3 MyPos = m_pAI->GetPosition();

	// �������瑊��̋���
	float fDistance = MyPos.DistanceXZ(TargetPos);

	if (fDistance < m_sLearn.fDistanceIN - LENGTH_SPACE)
	{// ����Ƃ̋������߂������ꍇ
		// �s����ԁF�����
		m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_LEAVE;

		// �J�j�i�s�����̐ݒ�
		float direction = TargetPos.AngleXZ(MyPos);
		pControlAIMove->SetClabDirection(direction);

		return;
	}
	else if (fDistance > m_sLearn.fDistanceIN + LENGTH_SPACE)
	{// ����Ƃ̋��������������ꍇ

		if (m_eLine == ELine::LINE_OVER) return;

		// �s����ԁF�߂Â�
		m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_APPROATCH;

		// �J�j�i�s�����̐ݒ�
		float direction = MyPos.AngleXZ(TargetPos.AngleXZ(MyPos));
		pControlAIMove->SetClabDirection(direction);

		return;
	}

	// �����񂶂�Ȃ��I
	m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_NONE;
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

		if (fLength < m_sLearn.fDistanceOUT)
		{ // ���l���߂�����v���C���[�������ꍇ

			// �����s����Ԃ�
			m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_LEAVE;

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

	// �^�[�Q�b�g����(����)
	MyLib::Vector3 myPos = m_pAI->GetPosition();	// ���g�ʒu
	MyLib::Vector3 targetPos = { 0.0f, 0.0f, myPos.x };	// �^�[�Q�b�g�ʒu

	// ��������^�[�Q�b�g�Ƃ̋���
	float fDistance = myPos.DistanceXZ(targetPos);

	if (fDistance < m_sLearn.fDistanceLine)
	{// �������w��l�ȓ��̏ꍇ

		// �ړ���Ԃ𗣂��I
		m_eLine = ELine::LINE_OVER;
	}
	else if (fDistance > m_sLearn.fDistanceLine + 20.0f)
	{
		m_eLine = ELine::LINE_IN;
	}
}

//==========================================================================
// �^�[�Q�b�g�ݒ�(�L���b�`��)
//==========================================================================
void CPlayerAIControl::CatchMoveFlag()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_sInfo.sMoveInfo.eType == EMoveType::MOVETYPE_NONE)
	{
		// �����I�t
		pControlAIMove->SetIsWalk(false);
	}
	else
	{
		// �����I��
		pControlAIMove->SetIsWalk(true);
	}
}




//==========================================================================
// ���蓊��(�X�^�[�g�ʒu��)
//==========================================================================
void CPlayerAIControl::RunStartPos(CPlayer* pTarget)
{
	//if (!pTarget) return;
	if (m_bStart) return;

	// �����̈ʒu
	MyLib::Vector3 MyPos = m_pAI->GetPosition();
	// ���蓊���X�^�[�g�ʒu
	MyLib::Vector3 StartPos = { 700.0f, 0.0f, 0.0f };

	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ��������X�^�[�g�ʒu�̊p�x
	float fAngle = MyPos.AngleXZ(StartPos);

	// �����ݒ�
	m_pAI->SetRotation(MyLib::Vector3(0.0f, fAngle, 0.0f));

	// �����ƃX�^�[�g�ʒu�̋���
	float fDistance = MyPos.DistanceXZ(StartPos);

	if (fDistance < 10.0f && !m_bStart)
	{
		m_bStart = true;	// �X�^�[�g�ʒu�֒�����
		pControlAIMove->SetIsWalk(false);	// �~�܂�
		return;
	}

	// ����
	pControlAIMove->SetIsWalk(true);
	
}

//==========================================================================
// ���蓊��(�I���ʒu��)
//==========================================================================
void CPlayerAIControl::RunEndPos(CPlayer* pTarget)
{
	//if (!pTarget) return;
	if (m_bEnd) return;

	// �����̈ʒu
	MyLib::Vector3 MyPos = m_pAI->GetPosition();
	// ���蓊���X�^�[�g�ʒu
	MyLib::Vector3 StartPos = { 200.0f, 0.0f, 0.0f };

	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ��������X�^�[�g�ʒu�̊p�x
	float fAngle = MyPos.AngleXZ(StartPos);

	// �����ݒ�
	m_pAI->SetRotation(MyLib::Vector3(0.0f, fAngle, 0.0f));

	// �����ƃX�^�[�g�ʒu�̋���
	float fDistance = MyPos.DistanceXZ(StartPos);

	if (fDistance < 10.0f && !m_bEnd)
	{
		m_bEnd = true;	// �I���ʒu�֒�����
		pControlAIMove->SetIsWalk(false);	// �~�܂�
		return;
	}

	// ����
	pControlAIMove->SetIsWalk(true);
}



// ���̑� ===================================================================================================================
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

//==========================================================================
// �ϐ��̃��Z�b�g
//==========================================================================
void CPlayerAIControl::Reset()
{
	// �\���̏�񃊃Z�b�g
	ZeroMemory(&m_sInfo, sizeof(m_sInfo));

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

// �f�o�b�O
void CPlayerAIControl::Debug()
{
	//if (ImGui::TreeNode("AI Info"))
	//{
	//	if (ImGui::Button("Reset"))
	//	{// ���Z�b�g
	//		ZeroMemory(&m_sInfo, sizeof(m_sInfo));
	//	}

	//	//ImGui::DragFloat("Distance", &m_fDistanceIN, 1.0f, 0.0f, 1000.0f, "%.2f");

	//	ImGui::Text("mode : %d", m_sInfo.sMode.eMode);
	//	ImGui::Text("Distance : %0.2f", m_fDistanceIN);
	//	ImGui::DragFloat("Distance", &m_fDistanceIN, 1.0f, 0.0f, 1000.0f, "%.2f");

	//	ImGui::TreePop();
	//}
}
