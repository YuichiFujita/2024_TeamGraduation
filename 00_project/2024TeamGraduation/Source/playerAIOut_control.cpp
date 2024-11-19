//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playerAIOut_control.h"
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

#include "playerAIOut_controlAction.h"
#include "playerAIOut_controlMove.h"

#include "playerAIOut.h"

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
CPlayerAIOutControl::MODE_FUNC CPlayerAIOutControl::m_ModeFunc[] =	// ���[�h�֐�
{
	&CPlayerAIOutControl::ModeNone,				// �Ȃ�
	& CPlayerAIOutControl::ModeThrowManager,		// ����
	& CPlayerAIOutControl::ModeCatchManager,		// �L���b�`
};

CPlayerAIOutControl::CATCH_FUNC CPlayerAIOutControl::m_CatchFunc[] =	// �L���b�`�֐�
{
	&CPlayerAIOutControl::CatchNone,		// �Ȃ�
	&CPlayerAIOutControl::FindBall,			// ������
};


//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIOutControl::CPlayerAIOutControl()
{
	// �l�̏�����
	m_eMode = EMode::MODE_NONE;
	m_eCatch = ECatch::CATCH_NONE;
	m_pAIOut = nullptr;
	m_bStart = false;
	m_bEnd = false;
}

//==========================================================================
// ����
//==========================================================================
CPlayerAIOutControl* CPlayerAIOutControl::Create(CPlayer* player)
{
	// �������̊m��
	CPlayerAIOutControl* pObj = DEBUG_NEW CPlayerAIOutControl;
	if (pObj != nullptr)
	{
		pObj->m_pAIOut = player;

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
HRESULT CPlayerAIOutControl::Init()
{
	// �l�̏�����


	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerAIOutControl::Uninit()
{
	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIOutControl::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���[�h�Ǘ�
	ModeManager();

	// ��ԍX�V
	(this->*(m_ModeFunc[m_eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���[�h�Ǘ�
//==========================================================================
void CPlayerAIOutControl::ModeManager()
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() == nullptr ||
		pBall->GetTypeTeam() != m_pAIOut->GetTeam())
	{// �{�[�����擾����Ă��Ȃ��ꍇ||�����ƃ{�[���������Ă���`�[�����Ⴄ�ꍇ
		m_eMode = EMode::MODE_CATCH;
	}
	else if (pBall->GetPlayer() == m_pAIOut)
	{// �{�[���������Ă���̂������������ꍇ
		m_eMode = EMode::MODE_THROW;
	}
}

//==========================================================================
// �����Ǘ�
//==========================================================================
void CPlayerAIOutControl::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_eMode == EMode::MODE_THROW)
	{// ������Ԃ̏ꍇ(�{�[���������Ă���)

		int n = rand() % 2;
		//int n = 0;

		if (n == 0)
		{
			// �p�X����
			Pass();
		}
		else
		{
			// ������
			Throw();
		}
	}
}

//==========================================================================
// �L���b�`�Ǘ�
//==========================================================================
void CPlayerAIOutControl::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() != nullptr)
	{// �N�����{�[���������Ă���ꍇ
		
	}
	else if (pBall->GetPlayer() == nullptr)
	{// �N���{�[���������Ă��Ȃ��ꍇ

		// �{�[�������ɍs��
		m_eCatch = ECatch::CATCH_FIND;
	}

	// �������ލX�V
	(this->*(m_CatchFunc[m_eCatch]))(fDeltaTime, fDeltaRate, fSlowRate);
}


//==========================================================================
// �p�X����
//==========================================================================
void CPlayerAIOutControl::Pass()
{
	// AI�R���g���[�����(�O��)�̎擾
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();
	CPlayerControlMove* pControlMove = m_pAIOut->GetBase()->GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// �{�[�����̎擾
	CBall* pBall = m_pAIOut->GetBall();
	if (!pBall) return;

	// �{�[�����p�X
	pControlAIOutAction->SetPattern(m_pAIOut, CPlayer::EMotion::MOTION_THROW_PASS, CPlayer::EAction::ACTION_THROW);
}

//==========================================================================
// �{�[���𓊂���
//==========================================================================
void CPlayerAIOutControl::Throw()
{
	// AI�R���g���[�����(�O��)�̎擾
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();
	CPlayerControlMove* pControlMove = m_pAIOut->GetBase()->GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// �^�[�Q�b�g�ݒ�
	GetThrowTarget();

	// ������
	pControlAIOutAction->SetIsThrow(true);
}


//==========================================================================
// �p�X�s������
//==========================================================================
bool CPlayerAIOutControl::IsPass()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return false;

	// �p�X���&&�^�[�Q�b�g�������̏ꍇ
	if (pBall->IsPass() && pBall->GetTarget() == m_pAIOut) return false;

	return true;
}

//==========================================================================
// �{�[����������
//==========================================================================
void CPlayerAIOutControl::FindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����(�O��)�̎擾
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();
	CPlayerControlMove* pControlMove = m_pAIOut->GetBase()->GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (pBall == nullptr || pBall->GetPlayer() != nullptr)
	{// �{�[����nullptr&&�v���C���[���{�[��������Ă���ꍇ
		
		return;
	}

	// �p�x�����߂�(player����݂��{�[��)
	float fAngle = m_pAIOut->GetPosition().AngleXZ(pBall->GetPosition());

	// �����I��!
	pControlAIOutMove->SetIsWalk(true);

	// �����ݒ�
	m_pAIOut->SetRotDest(fAngle);
}

//==========================================================================
// �^�[�Q�b�g�ݒ�(�����鎞)
//==========================================================================
CPlayer* CPlayerAIOutControl::GetThrowTarget()
{
	CPlayer* pTarget = nullptr;	// �ڕW�^�[�Q�b�g
	float fMinDis = 1000000.0f;	// �߂��v���C���[

	MyLib::Vector3 Mypos = m_pAIOut->GetPosition();	// �ʒu���̎擾

	// �{�[���̎擾
	CBall* pBall = m_pAIOut->GetBall();
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

		// �����Ƃ̋��������߂�
		float fLength = Mypos.DistanceXZ(posPlayer);

		if (fLength < fMinDis)
		{ // ���߂������������ꍇ

			// �ŏ������̍X�V
			fMinDis = fLength;

			// �^�[�Q�b�g���X�V
			pTarget = pPlayer;

			// �����ݒ�(������������)
			m_pAIOut->SetRotDest(Mypos.AngleXZ(pTarget->GetPosition()));
		}
	}

	return pTarget;
}

