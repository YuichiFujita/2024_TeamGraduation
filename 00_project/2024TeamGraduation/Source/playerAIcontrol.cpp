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
	// �W�����v�����̍ő�ʒu
	const float JUMP_END_POS = 140.0f;

	// ��������钷��
	const float LENGTH_MAX = 500.0f;
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

CPlayerAIControl::THOWTYPE_FUNC CPlayerAIControl::m_ThrowTimingFunc[] =	// �����^�C�v�֐�
{
	&CPlayerAIControl::ThrowJumpTimingNormal,		// �ʏ�
	&CPlayerAIControl::ThrowJumpTimingQuick,		// ��
	&CPlayerAIControl::ThrowJumpTimingDelay,		// �x
};

CPlayerAIControl::CATCH_FUNC CPlayerAIControl::m_CatchFunc[] =	// �L���b�`�֐�
{
	&CPlayerAIControl::CatchNone,
	&CPlayerAIControl::CatchNormal,
	&CPlayerAIControl::CatchJust,
	&CPlayerAIControl::CatchDash,
	&CPlayerAIControl::FindBall,
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControl::CPlayerAIControl()
{
	// �����o�ϐ��̏�����
	ZeroMemory(&m_sInfo, sizeof(m_sInfo));
	m_pAI = nullptr;
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
	m_fDistance = LENGTH_MAX;

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
		pBall->GetTypeTeam() != m_pAI->GetStatus()->GetTeam())
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
	// �^�[�Q�b�g
	CPlayer* pTarget = nullptr;

	// �^�[�Q�b�g�����߂�֐�	// ��
	SetThrowTarget(&pTarget);

	// ���������߂�֐�			// ��
	PlanThrowDistance(pTarget);

	// �s��
	//PlanMove(pTarget);

	// �W�����v����or�����Ȃ��֐�	// ��u
	//PlanIsJump(pTarget);

	// �����Ȃ�or����or����֐�		// ��

	// ���C���z��
	//LineOverPlayer();	// ��

	// ���𓊂��邩�l����֐�	// ��
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
	if (!m_sInfo.sThrowInfo.bThrow && m_sInfo.sThrowInfo.bTiming) return;

	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	CGameManager* pGameManager = CGameManager::GetInstance();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(m_pAI->GetStatus()->GetTeam());

	if (pTeamStatus->IsMaxSpecial())
	{// �Q�[�W�����܂��Ă�����
		// �X�y�V��������
		pControlAIAction->SetIsSpecial(true);
	}
	else
	{// ������t���O���I����������
		int n = 0;
		// ���̓����_���Ō���
		n = rand() % 2;
		//n = 1;

		switch (n)
		{
		case 0:
			pControlAIAction->SetIsThrow(true);
			break;

		case 1:
			JumpThrowTiming(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
			break;

		default:
			break;
		}

		m_sInfo.sThrowInfo.bTiming = true;
	}

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

	if (fDistance < m_fDistance - 50.0f)
	{// �����I
		m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_LEAVE;
		
		// ���肩�玩���̕���
		m_pAI->SetRotDest(posTarget.AngleXZ(MyPos));

		// �����I
		pControlAIMove->SetIsWalk(true);

		return;
	}
	else if(fDistance > m_fDistance + 50.0f)
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
	
	if (fDistance > m_fDistance + 50.0f && fDistance < m_fDistance - 50.0f)
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
	StrategyLine(pTarget);
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

	CGameManager::ETeamSide typeTeam = m_pAI->GetStatus()->GetTeam();

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
	StrategyLine(pTarget);

	// �^�C�~���O�v�l
	StrategyTiming(pTarget);

	// ��ԍX�V
	(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �^�C�~���O�̎v�l ����RAND
//==========================================================================
void CPlayerAIControl::StrategyTiming(CPlayer* pTarget)
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
			else if (n == 1 && fDistance < m_fDistance - 100.0f)
			{// �ԍ�2&&�^�[�Q�b�g�Ƃ̋������ȓ��̏ꍇ
				m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_QUICK;
				m_sInfo.sThrowInfo.bTiming = true;
				break;
			}
			else if (n == 2 && fDistance < m_fDistance - 100.0f)
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
void CPlayerAIControl::ThrowJumpTimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_sInfo.sThrowInfo.fTiming > 0.0f)
	{
		m_sInfo.sThrowInfo.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// �������Z�b�g
	pControlAIAction->SetIsJump(true);	// �W�����v�I��

	if (m_pAI->GetPosition().y >= JUMP_END_POS)	// �����ɂ���ĕς��
	{// �J�E���g0�ȏ�
		// ������
		pControlAIAction->SetIsThrow(true);

		// �ϐ����Z�b�g
		//Reset();
	}
}

//--------------------------------------------------------------------------
// ���ѓ����^�C�~���O(����)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowJumpTimingQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_sInfo.sThrowInfo.fTiming > 0.0f)
	{// �J�E���g0�ȏ�
		m_sInfo.sThrowInfo.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

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
void CPlayerAIControl::ThrowJumpTimingDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_sInfo.sThrowInfo.fTiming > 0.0f)
	{// �J�E���g0�ȏ�
		m_sInfo.sThrowInfo.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;
		return;
	}

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
void CPlayerAIControl::StrategyLine(CPlayer* pTarget)
{
	// �`�[���^�C�v�̎擾
	CGameManager::ETeamSide typeTeam = m_pAI->GetStatus()->GetTeam();

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
	{// �L���b�`���
		m_sInfo.sCatchInfo.eCatchType = ECatchType::CATCH_TYPE_NORMAL;
	}
	else if (pBall->GetPlayer() == nullptr)
	{// 

		if (!IsWhoPicksUpTheBall()) return;	// �������߂��ɂ����ꍇ

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
	if (m_sInfo.sMoveInfo.eType != EMoveType::MOVETYPE_APPROATCH)
	{// �s���^�C�v���߂Â��ȊO�̏ꍇ
		// ���������
		DistanceLeaveCatch();
	}
	else if (m_sInfo.sMoveInfo.eType != EMoveType::MOVETYPE_LEAVE)
	{// �s���^�C�v�������ȊO�̏ꍇ
		// �������k�߂�
		DistanceApproachCatch();
	}
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
// �{�[���������Ă���z���狗�������
//==========================================================================
void CPlayerAIControl::DistanceLeaveCatch()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	CPlayer* pTarget = nullptr;	// �ڕW�^�[�Q�b�g

	MyLib::Vector3 pos = m_pAI->GetPosition();	// �ʒu���̎擾
	CGameManager::ETeamSide myTeam = m_pAI->GetStatus()->GetTeam();

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���
		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// �v���C���[�ʒu

		// �{�[���̎擾
		CBall* pBall = CGameManager::GetInstance()->GetBall();
		if (pBall == nullptr) { return; }
		CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// �`�[���^�C�v�̎擾

		// �����`�[���܂��̓{�[�����ǂ̃T�C�h�ł������ꍇ
		if ((typeTeam == m_pAI->GetStatus()->GetTeam()) ||
			(typeTeam == CGameManager::ETeamSide::SIDE_NONE))
		{
			continue;
		}

		// �����͌��Ȃ�
		if (pPlayer->GetStatus()->GetTeam() == myTeam) continue;

		// ����Ƃ̋��������߂�
		float fLength = posPlayer.DistanceXZ(pos);

		if (fLength < m_fDistance)
		{ // ���l���߂�����v���C���[�������ꍇ

			// �����s����Ԃ�

			m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_LEAVE;

			// �^�[�Q�b�g�ݒ�
			pTarget = pPlayer;

			// �J�j�i�s�����̐ݒ�
			float direction = pTarget->GetPosition().AngleXZ(pos);
			pControlAIMove->SetClabDirection(direction);

			// �����I��
			pControlAIMove->SetIsWalk(true);
		}
		else
		{
			m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_NONE;

			// �����I�t
			pControlAIMove->SetIsWalk(false);
		}
	}
}

//==========================================================================
// �{�[���������Ă���z�Ɉ��̋������֋߂Â�
//==========================================================================
void CPlayerAIControl::DistanceApproachCatch()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	CPlayer* pTarget = nullptr;	// �ڕW�^�[�Q�b�g

	MyLib::Vector3 pos = m_pAI->GetPosition();	// �ʒu���̎擾
	CGameManager::ETeamSide myTeam = m_pAI->GetStatus()->GetTeam();

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���
		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// �v���C���[�ʒu

		// �{�[���̎擾
		CBall* pBall = CGameManager::GetInstance()->GetBall();
		if (pBall == nullptr) { return; }
		CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// �`�[���^�C�v�̎擾

		// �����`�[���܂��̓{�[�����ǂ̃T�C�h�ł������ꍇ
		if ((typeTeam == m_pAI->GetStatus()->GetTeam()) ||
			(typeTeam == CGameManager::ETeamSide::SIDE_NONE))
		{
			continue;
		}

		// �����͌��Ȃ�
		if (pPlayer->GetStatus()->GetTeam() == myTeam) continue;

		// ����Ƃ̋��������߂�
		float fLength = posPlayer.DistanceXZ(pos);

		if (fLength > m_fDistance)
		{ // ���l���߂�����v���C���[�������ꍇ

			// �����s����Ԃ�
			m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_APPROATCH;

			// �^�[�Q�b�g�ݒ�
			pTarget = pPlayer;

			// �J�j�i�s�����̐ݒ�
			float direction = pos.AngleXZ(pTarget->GetPosition());
			pControlAIMove->SetClabDirection(direction);

			// �����I��
			pControlAIMove->SetIsWalk(true);

#ifdef _DEBUG
			CEffect3D::Create
			(// �f�o�b�O�p�G�t�F�N�g(�^�[�Q�b�g)
				pTarget->GetPosition(),
				VEC3_ZERO,
				MyLib::color::Yellow(),
				20.0f,
				0.1f,
				1,
				CEffect3D::TYPE::TYPE_NORMAL
			);	//
#endif

		}
		else
		{
			m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_NONE;

			// �����I�t
			pControlAIMove->SetIsWalk(false);
		}
	}
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
		//Reset();	// �ϐ����Z�b�g
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
	CGameManager::ETeamSide typeTeam = m_pAI->GetStatus()->GetTeam();	// �`�[���^�C�v
	MyLib::Vector3 Mypos = m_pAI->GetPosition();	// �ʒu���

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	// �{�[���ʒu�擾
	MyLib::Vector3 posBall = pBall->GetPosition();
	fMyDis = Mypos.DistanceXZ(posBall);	// ��������{�[���Ƃ̋���

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���

		// �Ⴄ�`�[��||�O��̏ꍇ
		if ((typeTeam != pPlayer->GetStatus()->GetTeam()) ||
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
// �^�[�Q�b�g�ݒ�
//==========================================================================
void CPlayerAIControl::SetThrowTarget(CPlayer** ppTarget)
{
	CPlayer* pTarget = nullptr;	// �ڕW�^�[�Q�b�g
	float fMinDis = 1000000.0f;	// �߂��v���C���[

	MyLib::Vector3 Mypos = m_pAI->GetPosition();	// �ʒu���̎擾

	// �{�[���̎擾
	CBall* pBall = m_pAI->GetBall();
	if (pBall == nullptr) { return; }
	CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// �`�[���^�C�v�̎擾

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���
		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// �v���C���[�ʒu

		// �����`�[��||�O��̏ꍇ
		if ((typeTeam == pPlayer->GetStatus()->GetTeam()) || 
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
			*ppTarget = pTarget;

			// �����ݒ�
			m_pAI->SetRotDest(Mypos.AngleXZ(pTarget->GetPosition()));
		}
	}
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
	if (ImGui::TreeNode("AI Info"))
	{
		if (ImGui::Button("Reset"))
		{// ���Z�b�g
			ZeroMemory(&m_sInfo, sizeof(m_sInfo));
		}

		//ImGui::DragFloat("Distance", &m_fDistance, 1.0f, 0.0f, 1000.0f, "%.2f");

		ImGui::Text("mode : %d", m_sInfo.sMode.eMode);
		ImGui::Text("Distance : %0.2f", m_fDistance);
		ImGui::DragFloat("Distance", &m_fDistance, 1.0f, 0.0f, 1000.0f, "%.2f");

		ImGui::TreePop();
	}
}
