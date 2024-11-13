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


// player�֘A
#include "playerBase.h"
#include "playerStatus.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol_move.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	// �^�C�~���O
	const float TIMING_NORMAL = 1.0f;	// �ʏ�
	const int TIMING_RAND_MAX = 100;
	const int TIMING_RAND_MIN = -80;

	// �W�����v�����̍ő�ʒu
	const float JUMP_END_POS = 140.0f;

	// ��������钷��
	const float LENGTH_MAX = 400.0f;
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CPlayerAIControl::MODE_FUNC CPlayerAIControl::m_ModeFunc[] =	// ���[�h�֐�
{
	&CPlayerAIControl::ModeNone,				// �Ȃ�
	&CPlayerAIControl::ModeThrowManager,		// �����}�l�[�W���[
	&CPlayerAIControl::ModeCatchManager,		// �L���b�`�}�l�[�W���[
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
	m_eMode = EMode::MODE_NONE;
	m_eHeart = EHeart::HEART_NONE;
	m_eCatchType = ECatchType::CATCH_TYPE_NONE;
	m_eMoveType = EMoveType::MOVETYPE_NONE;
	m_fTiming = 0.0f;		// �^�C�~���O�J�E���g
	m_fTimingRate = 0.0f;	// �^�C�~���O�̊���
	m_bTiming = false;
	m_bFoldJump = false;
	m_fJumpEnd = JUMP_END_POS;	// �����̍ő�ʒu

	m_pAI = nullptr;

	m_bGOThrow = false;
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
	(this->*(m_ModeFunc[m_eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���[�h�Ǘ�
//==========================================================================
void CPlayerAIControl::ModeManager()
{
	if (m_eMode == EMode::MODE_THROW) return;

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() == nullptr ||
		pBall->GetTypeTeam() != m_pAI->GetStatus()->GetTeam())
	{// �{�[�����擾����Ă��Ȃ��ꍇ||�����ƃ{�[���������Ă���`�[�����Ⴄ�ꍇ
		m_eMode = EMode::MODE_CATCH;
	}
	else if (pBall->GetPlayer() == m_pAI)
	{// �{�[���������Ă���̂������������ꍇ
		m_eMode = EMode::MODE_THROW;
	}
}

//==========================================================================
// �����̊Ǘ�
//==========================================================================
void CPlayerAIControl::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����̗���
	PlanThrowFlow();
}

//==========================================================================
// �L���b�`�̊Ǘ�
//==========================================================================
void CPlayerAIControl::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() != nullptr) {	// �L���b�`���
		m_eCatchType = ECatchType::CATCH_TYPE_NORMAL;
	}
	else if (pBall->GetPlayer() == nullptr)
	{
		// �{�[�������ɍs��
		m_eCatchType = ECatchType::CATCH_TYPE_FIND;
	}

	// �������ލX�V
	(this->*(m_CatchFunc[m_eCatchType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �ʏ�L���b�`
//==========================================================================
void CPlayerAIControl::CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���������
	DistanceCatch();
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
// �^�[�Q�b�g�ݒ�
//==========================================================================
void CPlayerAIControl::SetThrowTarget(CPlayer** ppTarget)
{
	CPlayer* pTarget = nullptr;	// �ڕW�^�[�Q�b�g
	float fMinDis = 1000000.0f;	// �߂��v���C���[

	MyLib::Vector3 pos = m_pAI->GetPosition();	// �ʒu���̎擾

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���
		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// �v���C���[�ʒu

		// �{�[���̎擾
		CBall* pBall = m_pAI->GetBall();
		if (pBall == nullptr) { return; }
		CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// �`�[���^�C�v�̎擾

		// �����`�[���̏ꍇ����
		if (typeTeam == pPlayer->GetStatus()->GetTeam()) { continue; }

		// �G�Ƃ̋��������߂�
		float fLength = sqrtf(
			(pPlayer->GetPosition().x - pos.x) * (pPlayer->GetPosition().x - pos.x) +
			(pPlayer->GetPosition().z - pos.z) * (pPlayer->GetPosition().z - pos.z));

		if (fLength < fMinDis)
		{ // ���߂�����v���C���[�������ꍇ

			// �^�[�Q�b�g���X�V
			pTarget = pPlayer;
			*ppTarget = pTarget;

			// �����ݒ�
			m_pAI->SetRotDest(pos.AngleXZ(pTarget->GetPosition()));
		}
	}
}

//==========================================================================
// �����鋗��
//==========================================================================
void CPlayerAIControl::DistanceThrow()
{

}

//==========================================================================
// �L���b�`�̎����������
//==========================================================================
void CPlayerAIControl::DistanceCatch()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	CPlayer* pTarget = nullptr;	// �ڕW�^�[�Q�b�g
	float fMinDis = LENGTH_MAX;	// �߂��v���C���[

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
		{ continue; }

		// �����͌��Ȃ�
		if (pPlayer->GetStatus()->GetTeam() == myTeam) continue;

		// ����Ƃ̋��������߂�
		float fLength = posPlayer.DistanceXZ(pos);

		if (fLength < fMinDis)
		{ // ���l���߂�����v���C���[�������ꍇ

			// �����s����Ԃ�
			m_eMoveType = EMoveType::MOVETYPE_DISTANCE;

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
			m_eMoveType = EMoveType::MOVETYPE_NONE;

			// �����I�t
			pControlAIMove->SetIsWalk(false);
		}
	}
}

//==========================================================================
// ��������܂ł̗�����l����
//==========================================================================
void CPlayerAIControl::PlanThrowFlow()
{
	// �^�[�Q�b�g
	CPlayer* pTarget = nullptr;

	// �^�[�Q�b�g�����߂�֐�
	SetThrowTarget(&pTarget);

	// ���������߂�֐�
	PlanThrowDistance(pTarget);

	// �W�����v����or�����Ȃ��֐�
	PlanIsJump(pTarget);

	// �����Ȃ�or����or����֐�

	// ���C���z��
	LineOverPlayer();

	// ���𓊂��邩�l����֐�
	//PlanThrow(pTarget);
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
void CPlayerAIControl::PlanThrow(CPlayer* pTarget)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	CGameManager* pGameManager = CGameManager::GetInstance();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(m_pAI->GetStatus()->GetTeam());

	if (pTeamStatus->IsMaxSpecial())
	{
		// �X�y�V��������
		pControlAIAction->SetIsSpecial(true);
	}
	else
	{
		if (!m_bGOThrow) return;

		pControlAIAction->SetIsThrow(true);
		m_bGOThrow = false;
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
	MyLib::Vector3 posMy = m_pAI->GetPosition();

	// �������瑊��̋���
	float fDistance = posMy.DistanceXZ(posTarget);

	if (fDistance < LENGTH_MAX - 50.0f)
	{// �����I
		m_eMoveType = EMoveType::MOVETYPE_DISTANCE;

		// ���肩�玩���̕���
		m_pAI->SetRotDest(posTarget.AngleXZ(posMy));

		// �����I
		pControlAIMove->SetIsWalk(true);

		return;
	}
	else if(fDistance > LENGTH_MAX + 50.0f)
	{// �߂Â��I
		m_eMoveType = EMoveType::MOVETYPE_DISTANCE;

		// ���肩�玩���̕���
		m_pAI->SetRotDest(posMy.AngleXZ(posTarget));

		// �����I
		pControlAIMove->SetIsWalk(true);

		return;
	}

	// �����񂶂�Ȃ��I
	m_eMoveType = EMoveType::MOVETYPE_NONE;

	// �����ȁI
	pControlAIMove->SetIsWalk(false);

	// �����Ă悵�I
	m_bGOThrow = true;

	// �ϐ����Z�b�g����I
	Reset();
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
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ����������
	MyLib::Vector3 posTarget = pTarget->GetPosition();
	MyLib::Vector3 posMy = m_pAI->GetPosition();

	// �������瑊��̋���
	float fDistance = posMy.DistanceXZ(posTarget);
	
	if (fDistance > LENGTH_MAX - 50.0f)
	{// ����Ă�����
		pControlAIAction->SetIsJump(true);
	}
	else if (fDistance < LENGTH_MAX)
	{

	}
}

//==========================================================================
// �����A����A���̑�
//==========================================================================
void CPlayerAIControl::PlanMove(CPlayer* pTarget)
{
	if (pTarget == nullptr) return;


}

//==========================================================================
// �����A����A���̑�
//==========================================================================
void CPlayerAIControl::LineDistance()
{

}

//==========================================================================
// ������(�v���C���[)
//==========================================================================
void CPlayerAIControl::LineOverPlayer()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �`�[���^�C�v�̎擾
	CGameManager::ETeamSide typeTeam = m_pAI->GetStatus()->GetTeam();

	// ����������
	MyLib::Vector3 posTarget = {};
	MyLib::Vector3 posMy = m_pAI->GetPosition();

	// �������瑊��̋���
	float fDistance = posMy.DistanceXZ(posTarget);

	if (typeTeam == CGameManager::ETeamSide::SIDE_LEFT)
	{
		if (fDistance < 100.0f)
		{// �����I
			m_eMoveType = EMoveType::MOVETYPE_DISTANCE;

			// ���肩�玩���̕���
			//m_pAI->SetRotDest(posTarget.AngleXZ(posMy));

			// ���肩�玩���̕���
			m_pAI->SetRotDest(posMy.AngleXZ(posTarget));

			// �����I
			pControlAIMove->SetIsWalk(true);

			return;
		}
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{
		if (m_pAI->GetPosition().x < 0.0f)
		{
			// 
		}
	}
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
			Reset();
			bOver = true;
		}
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{
		if (pBall->GetPosition().x < 0.0f)
		{
			Reset();
			bOver = true;
		}
	}

	return bOver;
}

//==========================================================================
// ���ɍs��
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
	{// �{�[����nullptr���v���C���[���{�[��������Ă���ꍇ
		Reset();	// �ϐ����Z�b�g
		return;
	}

	// �p�x�����߂�(player����݂��{�[��)
	float fAngle = m_pAI->GetPosition().AngleXZ(pBall->GetPosition());

	// �����I��!
	pControlAIMove->SetIsWalk(true);

	// �����ݒ�
	m_pAI->SetRotDest(fAngle);
}

//==========================================================================
// �ϐ��̃��Z�b�g
//==========================================================================
void CPlayerAIControl::Reset()
{
	// �񋓃��Z�b�g
	m_eMode = EMode::MODE_NONE;

	// �^�C�~���O���Z�b�g
	m_fTiming = TIMING_NORMAL;
	m_bTiming = false;

	// �܂�Ԃ����Z�b�g
	m_bFoldJump = false;

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
