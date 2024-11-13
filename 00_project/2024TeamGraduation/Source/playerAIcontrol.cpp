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
namespace timing
{
	// �^�C�~���O
	const float TIMING_NORMAL = 1.0f;	// �ʏ�
	const int TIMING_RAND_MAX = 100;
	const int TIMING_RAND_MIN = -80;

	// �W�����v�����̍ő�ʒu
	const float JUMP_END_POS = 140.0f;
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

CPlayerAIControl::TYPE_FUNC CPlayerAIControl::m_ThrowTypeFunc[] =	// ������ފ֐�
{
	&CPlayerAIControl::TypeNone,				// �Ȃ�
	&CPlayerAIControl::TypeThrow,				// �ʏ퓊��
	&CPlayerAIControl::TypeJumpThrow,			// �W�����v����
	&CPlayerAIControl::TypeSpecialThrow,		// �X�y�V��������
};

CPlayerAIControl::MOVE_FUNC CPlayerAIControl::m_ThrowMoveFunc[] =	// ������Ԋ֐�
{
	&CPlayerAIControl::MoveNormal,			// �ʏ�(���̏�)
	&CPlayerAIControl::MoveWalk,				// ����
	&CPlayerAIControl::MoveDash,				// ����
};

CPlayerAIControl::TIMING_FUNC CPlayerAIControl::m_ThrowTimingFunc[] =	// �^�C�~���O�֐�
{
	&CPlayerAIControl::TimingNormal,			// �ʏ�
	&CPlayerAIControl::TimingFeint,			// �t�F�C���g

	&CPlayerAIControl::TimingJumpNormal,			// �ʏ�
	&CPlayerAIControl::TimingJumpQuick,			// ����
	&CPlayerAIControl::TimingJumpDelay,			// �x��
	&CPlayerAIControl::TimingJumpFeint,			// �t�F�C���g
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
	m_eThrowType = EThrowType::TYPE_NONE;
	m_eThrowMove = EThrowMove::MOVE_NORMAL;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;
	m_eCatchType = ECatchType::CATCH_TYPE_NONE;
	m_eMoveType = EMoveType::MOVETYPE_NONE;
	m_fTiming = 0.0f;		// �^�C�~���O�J�E���g
	m_fTimingRate = 0.0f;	// �^�C�~���O�̊���
	m_bTiming = false;
	m_bFoldJump = false;
	m_fJumpEnd = timing::JUMP_END_POS;	// �����̍ő�ʒu

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
	
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerAIControl::Uninit()
{

}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIControl::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall != nullptr && pBall->GetPlayer() == nullptr) {

		m_eMode = EMode::MODE_CATCH;
	}

	// ��ԍX�V
	(this->*(m_ModeFunc[m_eMode]))(fDeltaTime, fDeltaRate, fSlowRate);

	//// AI�R���g���[�����̎擾
	//CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	//CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	//// �����t���O�ݒ�
	//if (m_eMoveType == EMoveType::MOVETYPE_DISTANCE)
	//{
	//	pControlAIMove->SetIsWalk(true);
	//}
	//else
	//{
	//	pControlAIMove->SetIsWalk(false);
	//}

	//Distance();
}

//==========================================================================
// �����̊Ǘ�
//==========================================================================
void CPlayerAIControl::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CGameManager* pGameManager = CGameManager::GetInstance();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(m_pAI->GetStatus()->GetTeam());

	if (m_eThrowType == EThrowType::TYPE_NONE)
	{
		if (pTeamStatus->IsMaxSpecial())
		{// �X�y�V����
			m_eThrowType = EThrowType::TYPE_SPECIAL;
		}
		else
		{
			int n = 0;
			// ���̓����_���Ō���
			n = rand() % 2;
			//n = 1;

			switch (n)
			{
			case 0:
				m_eThrowType = EThrowType::TYPE_NORMAL;
				break;

			case 1:
				m_eThrowType = EThrowType::TYPE_JUMP;
				break;

			default:
				break;
			}
		}
	}

	Target();

	// �������ލX�V
	(this->*(m_ThrowTypeFunc[m_eThrowType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �L���b�`�̊Ǘ�
//==========================================================================
void CPlayerAIControl::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_eCatchType = ECatchType::CATCH_TYPE_FIND;

	// �������ލX�V
	(this->*(m_CatchFunc[m_eCatchType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �ʏ퓊������
//==========================================================================
void CPlayerAIControl::TypeThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����̐ݒ� : ����(���̏�)
	m_eThrowMove = EThrowMove::MOVE_WALK;

	// �����铮���X�V
	(this->*(m_ThrowMoveFunc[m_eThrowMove]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �W�����v��������
//==========================================================================
void CPlayerAIControl::TypeJumpThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����̐ݒ� : ����(���̏�)
	m_eThrowMove = EThrowMove::MOVE_WALK;

	// �����铮���X�V
	(this->*(m_ThrowMoveFunc[m_eThrowMove]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �X�y�V������������
//==========================================================================
void CPlayerAIControl::TypeSpecialThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	
	// �X�y�V��������
	pControlAIAction->SetIsSpecial(true);

	// ���ꂼ��̏�Ԃ�߂�
	Reset();
}

//==========================================================================
// �ʏ퓊��(���̏�)
//==========================================================================
void CPlayerAIControl::MoveNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�~���O�Ǘ�
	TimingManager(fDeltaTime, fDeltaRate, fSlowRate);

	// �����铮���X�V
	(this->*(m_ThrowTimingFunc[m_eThrowTiming]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ��������
//==========================================================================
void CPlayerAIControl::MoveWalk(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �����t���O�ݒ�
	pControlAIMove->SetIsWalk(true);

	// �^�C�~���O�Ǘ�
	TimingManager(fDeltaTime, fDeltaRate, fSlowRate);

	// �����铮���X�V
	(this->*(m_ThrowTimingFunc[m_eThrowTiming]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���蓊��
//==========================================================================
void CPlayerAIControl::MoveDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �^�C�~���O�Ǘ�
//==========================================================================
void CPlayerAIControl::TimingManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_bTiming) { return; }

	if (m_eThrowType == EThrowType::TYPE_NORMAL)
	{// �ʏ퓊��
		// �����_���Ń^�C�~���O�����߂�
		m_fTiming = timing::TIMING_NORMAL + UtilFunc::Transformation::Random(timing::TIMING_RAND_MIN, timing::TIMING_RAND_MAX) * 0.01f;

		m_eThrowTiming = EThrowTiming::TIMING_NORMAL;	// �ʏ퓊��
		m_bTiming = true;	// �^�C�~���O�ݒ芮���t���O�I��
	}
	else if (m_eThrowType == EThrowType::TYPE_JUMP)
	{// �W�����v����

		// �^�C�~���O�̐ݒ�
		int n = rand() % 3;
		//int n = 3;

		switch (n)
		{
		case 0:
			m_eThrowTiming = EThrowTiming::TIMING_JUMP_NORMAL;
			m_bTiming = true;
			break;

		case 1:
			m_eThrowTiming = EThrowTiming::TIMING_JUMP_QUICK;
			m_bTiming = true;
			break;

		case 2:
			m_eThrowTiming = EThrowTiming::TIMING_JUMP_DELAY;
			m_bTiming = true;
			break;

		case 3:
			m_eThrowTiming = EThrowTiming::TIMING_JUMP_FEINT;
			m_bTiming = true;
			break;
		default:
			break;
		}
	}
}

//==========================================================================
// �����^�C�~���O(���̏�)
//==========================================================================
void CPlayerAIControl::TimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	if (m_fTiming > 0.0f)
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// ������
	pControlAIAction->SetIsThrow(true);

	// �ϐ����Z�b�g
	Reset();
}

//==========================================================================
// �t�F�C���g(���ʂ̓���)
//==========================================================================
void CPlayerAIControl::TimingFeint(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �ʏ�(�W�����v�̓���)
//==========================================================================
void CPlayerAIControl::TimingJumpNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_fTiming > 0.0f && (m_eThrowMove == EThrowMove::MOVE_WALK || m_eThrowMove == EThrowMove::MOVE_DASH))
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// �������Z�b�g
	pControlAIAction->SetIsJump(true);	// �W�����v�I��

	if (m_pAI->GetPosition().y >= timing::JUMP_END_POS)	// �����ɂ���ĕς��
	{
		// ������
		pControlAIAction->SetIsThrow(true);

		// �ϐ����Z�b�g
		Reset();
	}

}

//==========================================================================
// ����(�W�����v�̓���)
//==========================================================================
void CPlayerAIControl::TimingJumpQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_fTiming > 0.0f && m_eThrowMove == EThrowMove::MOVE_WALK || m_eThrowMove == EThrowMove::MOVE_DASH)
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// �����I�t
	pControlAIAction->SetIsJump(true);	// �W�����v�I��

	if (m_pAI->GetPosition().y >= timing::JUMP_END_POS * 0.5f)	// �����ɂ���ĕς��
	{
		// ������
		pControlAIAction->SetIsThrow(true);

		// ���ꂼ��̏�Ԃ̃��Z�b�g
		Reset();
	}
}

//==========================================================================
// �x��(�W�����v�̓���)
//==========================================================================
void CPlayerAIControl::TimingJumpDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_fTiming > 0.0f && m_eThrowMove == EThrowMove::MOVE_WALK || m_eThrowMove == EThrowMove::MOVE_DASH)
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// �����I�t
	pControlAIAction->SetIsJump(true);	// �W�����v�I��

	// �ʒu���擾
	MyLib::Vector3 pos = m_pAI->GetPosition();

	if (pos.y >= timing::JUMP_END_POS)	// �����ɂ���ĕς��
	{
		m_bFoldJump = true;	// �܂�Ԃ��I��
	}

	if (!m_bFoldJump) { return; }

	if (pos.y <= timing::JUMP_END_POS * 0.5f)	// �����ɂ���ĕς��
	{
		// ������
		pControlAIAction->SetIsThrow(true);

		// ���ꂼ��̏�Ԃ̃��Z�b�g
		Reset();
	}
}

//==========================================================================
// �t�F�C���g(�W�����v�̓���)
//==========================================================================
void CPlayerAIControl::TimingJumpFeint(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_fTiming > 0.0f && m_eThrowMove == EThrowMove::MOVE_WALK || m_eThrowMove == EThrowMove::MOVE_DASH)
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// �����I�t
	pControlAIAction->SetIsJump(true);	// �W�����v�I��

	if (m_pAI->GetPosition().y >= timing::JUMP_END_POS)	// �����ɂ���ĕς��
	{
		// ���ꂼ��̏�Ԃ̃��Z�b�g
		Reset();

		//m_eMode = EMode::MODE_THROW;
	}
}

//==========================================================================
// �ʏ�L���b�`
//==========================================================================
void CPlayerAIControl::CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

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
// �^�[�Q�b�g
//==========================================================================
void CPlayerAIControl::Target()
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

			// �����ݒ�
			m_pAI->SetRotDest(pos.AngleXZ(pTarget->GetPosition()));
		}
	}
}

//==========================================================================
// �҂�
//==========================================================================
bool CPlayerAIControl::IsWait()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	bool b = false;

	if (!pBall) { return b; }

	CGameManager::ETeamSide typeTeam = m_pAI->GetStatus()->GetTeam();

	if (typeTeam == CGameManager::ETeamSide::SIDE_LEFT)
	{
		if (pBall->GetPosition().x > 0.0f)
		{
			//m_eCatchType = ECatchType::CATCH_TYPE_NONE;
			Reset();
			b = true;
		}
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{
		if (pBall->GetPosition().x < 0.0f)
		{
			//m_eCatchType = ECatchType::CATCH_TYPE_NONE;
			Reset();
			b = true;
		}
	}

	return b;
}

//==========================================================================
// ���������
//==========================================================================
void CPlayerAIControl::Distance()
{
	CPlayer* pTarget = nullptr;	// �ڕW�^�[�Q�b�g
	float fMinDis = 400.0f;	// �߂��v���C���[

	MyLib::Vector3 pos = m_pAI->GetPosition();	// �ʒu���̎擾

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���
		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// �v���C���[�ʒu

		// �{�[���̎擾
		CBall* pBall = CGameManager::GetInstance()->GetBall();
		if (pBall == nullptr) { return; }
		CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// �`�[���^�C�v�̎擾

		// �����`�[���܂��̓{�[�����ǂ̃T�C�h�ł������ꍇ
		if ((typeTeam == m_pAI->GetStatus()->GetTeam()) || 
			(typeTeam == CGameManager::ETeamSide::SIDE_NONE))
		{ continue; }

		// ����Ƃ̋��������߂�
		float fLength = sqrtf(
			(pPlayer->GetPosition().x - pos.x) * (pPlayer->GetPosition().x - pos.x) +
			(pPlayer->GetPosition().z - pos.z) * (pPlayer->GetPosition().z - pos.z));

		if (fLength < fMinDis)
		{ // ���߂�����v���C���[�������ꍇ

			m_eMoveType = EMoveType::MOVETYPE_DISTANCE;

			pTarget = pPlayer;

			float fRotDest = m_pAI->GetRotDest();

			float fSpeed = m_pAI->GetParameter().fVelocityNormal;

	

			/*m_pAI->SetMove({ 
				sinf(-fRotDest) * fSpeed,
				0.0f,
				cosf(-fRotDest) * fSpeed
				});*/
		}
	}
}

//==========================================================================
// ���ɍs��
//==========================================================================
void CPlayerAIControl::FindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsWait()) { return; }

	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (pBall == nullptr || pBall->GetPlayer() != nullptr) {
		Reset();
		return;
	}

	// �p�x�����߂�(player����݂��{�[��)
	float fAngle = m_pAI->GetPosition().AngleXZ(pBall->GetPosition());

	// ���������߂�
	//float fLength = sqrtf((pBall->GetPosition().x - player->GetPosition().x) * (pBall->GetPosition().x - player->GetPosition().x) +
	//	(pBall->GetPosition().z - player->GetPosition().z) * (pBall->GetPosition().z - player->GetPosition().z));

	// �����I��
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
	m_eThrowType = EThrowType::TYPE_NONE;
	m_eThrowMove = EThrowMove::MOVE_NORMAL;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;

	// �^�C�~���O���Z�b�g
	m_fTiming = timing::TIMING_NORMAL;
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
