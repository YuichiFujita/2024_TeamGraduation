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
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CPlayerAIOutControl::MODE_FUNC CPlayerAIOutControl::m_ModeFunc[] =	// ���[�h�֐�
{
	&CPlayerAIOutControl::ModeNone,				// �Ȃ�
	&CPlayerAIOutControl::ModeThrowManager,		// ����
	&CPlayerAIOutControl::ModeMoveManager,		// �L���b�`
};

CPlayerAIOutControl::THROW_FUNC CPlayerAIOutControl::m_ThrowFunc[] =	// �����֐�
{
	&CPlayerAIOutControl::ThrowNone,
	&CPlayerAIOutControl::ThrowNormal,
	&CPlayerAIOutControl::ThrowPass,
};

CPlayerAIOutControl::MOVE_FUNC CPlayerAIOutControl::m_MoveFunc[] =	// �s���֐�
{
	&CPlayerAIOutControl::MoveNone,			// �Ȃ�
	&CPlayerAIOutControl::MoveRetrieve,		// ���ɍs��
	&CPlayerAIOutControl::MoveMeeting,		// �Ζ�
};


//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIOutControl::CPlayerAIOutControl()
{
	// �l�̏�����
	m_eMode = EMode::MODE_NONE;
	m_eThrow = EThrow::THROW_NONE;
	m_eMove = EMove::MOVE_NONE;
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
	// �{�[��������
	LookBall();

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

	if (pBall->GetPlayer() == nullptr || pBall->GetTypeTeam() != m_pAIOut->GetTeam())
	{// �{�[�����擾����Ă��Ȃ��ꍇ||�����ƃ{�[���������Ă���`�[�����Ⴄ�ꍇ
		m_eMode = EMode::MODE_MOVE;
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
	// AI�R���g���[�����(�O��)�̎擾
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();
	CPlayerControlMove* pControlMove = m_pAIOut->GetBase()->GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// �����I�t
	pControlAIOutMove->SetIsWalk(false);

	if (m_eThrow == EThrow::THROW_NONE)
	{// �^�C�v���̏ꍇ
		int n = rand() % 10;
		//int n = 1;

		if (n < 3)
		{
			m_eThrow = EThrow::THROW_NORMAL;
		}
		else
		{
			m_eThrow = EThrow::THROW_PASS;
		}
	}
	
	// �������ލX�V
	(this->*(m_ThrowFunc[m_eThrow]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �ʏ퓊��
//==========================================================================
void CPlayerAIOutControl::ThrowNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����(�O��)�̎擾
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();

	if (!pControlAIOutAction->GetIsFlag().bThrow)
	{// �t���O���I�t�̎�
		m_eThrow = EThrow::THROW_NONE;
	}

	// ������
	Throw();
}

//==========================================================================
// �p�X
//==========================================================================
void CPlayerAIOutControl::ThrowPass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����(�O��)�̎擾
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();

	if (!pControlAIOutAction->GetIsFlag().bPass)
	{// �t���O���I�t�̎�
		m_eThrow = EThrow::THROW_NONE;
	}

	// �p�X
	Pass();
}

//==========================================================================
// �p�X����
//==========================================================================
void CPlayerAIOutControl::Pass()
{
	// AI�R���g���[�����(�O��)�̎擾
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();

	// ������
	pControlAIOutAction->SetIsPass(true);
}

//==========================================================================
// �{�[���𓊂���
//==========================================================================
void CPlayerAIOutControl::Throw()
{
	// AI�R���g���[�����(�O��)�̎擾
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();

	// �^�[�Q�b�g�ݒ�
	m_pAIOut->SetRotDest(m_pAIOut->GetPosition().AngleXZ(GetThrowTarget()->GetPosition()));

	// ������
	pControlAIOutAction->SetIsThrow(true);
}

//==========================================================================
// �{�[��������
//==========================================================================
void CPlayerAIOutControl::LookBall()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	if (m_pAIOut->GetBall()) return;

	// ��������{�[���̈ʒu������
	float rot = m_pAIOut->GetPosition().AngleXZ(pBall->GetPosition());

	// �ݒ�
	m_pAIOut->SetRotDest(rot);
}

//==========================================================================
// �{�[���̃G���A����
//==========================================================================
void CPlayerAIOutControl::AreaCheck()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	MyLib::Vector3 ballPos = pBall->GetPosition();

	// �����̃`�[�����擾
	CGameManager::ETeamSide team = m_pAIOut->GetTeam();
	
	if (team == CGameManager::ETeamSide::SIDE_LEFT)
	{// ���`�[��
		if (ballPos.x < 0)
		{
			m_eMove = EMove::MOVE_NONE;
			return;
		}
	}
	else if (team == CGameManager::ETeamSide::SIDE_RIGHT)
	{// �E�`�[��
		if (ballPos.x > 0)
		{
			m_eMove = EMove::MOVE_NONE;
			return;
		}
	}
}

//==========================================================================
// �s���Ǘ�
//==========================================================================
void CPlayerAIOutControl::ModeMoveManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() != nullptr)
	{// �N�����{�[���������Ă���ꍇ
		m_eMove = EMove::MOVE_NONE;
	}
	else if (pBall->GetPlayer() == nullptr)
	{// �N���{�[���������Ă��Ȃ��ꍇ

		// �{�[�������ɍs��
		m_eMove = EMove::MOVE_FIND;

		// �G���A
		AreaCheck();
	}

	// �������ލX�V
	(this->*(m_MoveFunc[m_eMove]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CPlayerAIOutControl::MoveNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����(�O��)�̎擾
	CPlayerControlMove* pControlMove = m_pAIOut->GetBase()->GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// �����I�t
	pControlAIOutMove->SetIsWalk(false);

	// �L���b�`��
	m_eMove = EMove::MOVE_NONE;
}

//==========================================================================
// �{�[���̉��
//==========================================================================
void CPlayerAIOutControl::MoveRetrieve(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����(�O��)�̎擾
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();
	CPlayerControlMove* pControlMove = m_pAIOut->GetBase()->GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (pBall == nullptr || pBall->GetPlayer() != nullptr)
	{// �{�[����nullptr||�v���C���[���{�[��������Ă���||�G���A�O�̏ꍇ

		// �����I�t�I
		pControlAIOutMove->SetIsWalk(false);

		return;
	}

	// �p�x�����߂�(player����݂��{�[��)
	float fAngle = m_pAIOut->GetPosition().AngleXZ(pBall->GetPosition());

	// �O��|�W�V�����̎擾
	CPlayerManager::EOutPos eOutPos = CPlayerManager::GetInstance()->GetOutPosition(m_pAIOut);

	float fDest = fAngle;

	if ((eOutPos == CPlayerManager::EOutPos::OUT_LEFT_FAR) ||
		(eOutPos == CPlayerManager::EOutPos::OUT_RIGHT_FAR))
	{// �|�W�V��������


	}
	else if (
		(eOutPos == CPlayerManager::EOutPos::OUT_LEFT_NEAR) ||
		(eOutPos == CPlayerManager::EOutPos::OUT_RIGHT_NEAR))
	{// �|�W�V��������O

		fDest *= -1.0f;

	}
	else if (eOutPos == CPlayerManager::EOutPos::OUT_LEFT)
	{// ��
		if (fAngle > -D3DX_PI * 0.5f)
		{
			fDest *= -1.0f;
		}
	}
	else if (eOutPos == CPlayerManager::EOutPos::OUT_RIGHT)
	{// �E
		if (fAngle < D3DX_PI * 0.5f)
		{
			fDest *= -1.0f;
		}
	}

	// �����̐ݒ�
	pControlAIOutMove->SetVecRot(fDest);

	// �����I��!
	pControlAIOutMove->SetIsWalk(true);

	// �����ݒ�
	m_pAIOut->SetRotDest(fAngle);
}

//==========================================================================
// �Ζ�
//==========================================================================
void CPlayerAIOutControl::MoveMeeting(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����(�O��)�̎擾
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();
	CPlayerControlMove* pControlMove = m_pAIOut->GetBase()->GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr)return;

	// �{�[���̎�������̎擾
	CPlayer* pBallPlayer = pBall->GetPlayer();

	if (pBallPlayer == nullptr) {// �{�[��������Ă��Ȃ�
		pControlAIOutMove->SetIsWalk(false);
		return;
	}

	// �{�[���̎�����ƈႤ�`�[���̏ꍇ
	if (pBallPlayer->GetTeam() != m_pAIOut->GetTeam()) return;

	// �p�x�����߂�(player����݂��{�[��)
	float fAngle = m_pAIOut->GetPosition().AngleXZ(pBallPlayer->GetPosition());

	// �O��|�W�V�����̎擾
	CPlayerManager::EOutPos eOutPos = CPlayerManager::GetInstance()->GetOutPosition(m_pAIOut);

	float fDest = fAngle;

	if (eOutPos == CPlayerManager::EOutPos::OUT_LEFT)
	{// ��
		if (fAngle > -D3DX_PI * 0.5f)
		{
			fDest *= -1.0f;
		}
	}
	else if (eOutPos == CPlayerManager::EOutPos::OUT_RIGHT)
	{// �E
		if (fAngle < D3DX_PI * 0.5f)
		{
			fDest *= -1.0f;
		}
	}

	// �������猩���{�[���̎�����
	float rot = m_pAIOut->GetPosition().AngleXZ(pBallPlayer->GetPosition());

	if (rot > 0)
	{// �J�j�����ݒ�
		m_pAIOut->GetBase()->GetPlayerOut()->SetIsClab(true);
	}
	else
	{
		m_pAIOut->GetBase()->GetPlayerOut()->SetIsClab(false);
	}

	// �����I��!
	pControlAIOutMove->SetIsWalk(true);

	// �����̐ݒ�
	pControlAIOutMove->SetVecRot(fDest);

	// �����ݒ�
	m_pAIOut->SetRotDest(fAngle);
}

//==========================================================================
// �p�X�s���̔��菈��
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

