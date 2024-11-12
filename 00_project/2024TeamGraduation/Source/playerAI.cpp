//=============================================================================
// 
//  AI�v���C���[���� [playerAI.cpp]
//  Author : ���c�E��
// 
//=============================================================================
#include "playerAI.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "teamStatus.h"

#include "ball.h"

// �g�p�N���X
#include "playerStatus.h"
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"

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
CPlayerAI::MODE_FUNC CPlayerAI::m_ModeFunc[] =	// ���[�h�֐�
{
	&CPlayerAI::ModeNone,				// �Ȃ�
	&CPlayerAI::ModeThrowManager,		// �����}�l�[�W���[
	&CPlayerAI::ModeCatchManager,		// �L���b�`�}�l�[�W���[
};

CPlayerAI::TYPE_FUNC CPlayerAI::m_ThrowTypeFunc[] =	// ������ފ֐�
{
	&CPlayerAI::TypeNone,				// �Ȃ�
	&CPlayerAI::TypeThrow,				// �ʏ퓊��
	&CPlayerAI::TypeJumpThrow,			// �W�����v����
	&CPlayerAI::TypeSpecialThrow,		// �X�y�V��������
};

CPlayerAI::MOVE_FUNC CPlayerAI::m_ThrowMoveFunc[] =	// ������Ԋ֐�
{
	&CPlayerAI::MoveNormal,			// �ʏ�(���̏�)
	&CPlayerAI::MoveWalk,				// ����
	&CPlayerAI::MoveDash,				// ����
};

CPlayerAI::TIMING_FUNC CPlayerAI::m_ThrowTimingFunc[] =	// �^�C�~���O�֐�
{
	&CPlayerAI::TimingNormal,			// �ʏ�
	&CPlayerAI::TimingFeint,			// �t�F�C���g

	&CPlayerAI::TimingJumpNormal,			// �ʏ�
	&CPlayerAI::TimingJumpQuick,			// ����
	&CPlayerAI::TimingJumpDelay,			// �x��
	&CPlayerAI::TimingJumpFeint,			// �t�F�C���g
};

CPlayerAI::CATCH_FUNC CPlayerAI::m_CatchFunc[] =	// �L���b�`�֐�
{
	&CPlayerAI::CatchNone,
	&CPlayerAI::CatchNormal,
	&CPlayerAI::CatchJust,
	&CPlayerAI::CatchDash,
	&CPlayerAI::FindBall,
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAI::CPlayerAI()
{
	m_eMode = EMode::MODE_NONE;
	m_eThrowType = EThrowType::TYPE_NONE;
	m_eThrowMove = EThrowMove::MOVE_NORMAL;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;

	m_eCatchType = ECatchType::CATCH_TYPE_NONE;

	m_fTiming = 0.0f;	// �^�C�~���O�J�E���g
	m_fTimingRate = 0.0f;	// �^�C�~���O�̊���
	m_bTiming = false;
	m_bFoldJump = false;

	m_fJumpEnd = 0.0f;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAI::~CPlayerAI()
{
	
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPlayerAI::Init()
{
	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// �e�N���X�̏�����
	HRESULT hr = CPlayer::Init();
	if (FAILED(hr)) { return E_FAIL; }

	// ����֘A
	ChangeMoveControl(DEBUG_NEW CPlayerAIControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());

	// �����̍ő�ʒu�ݒ�
	m_fJumpEnd = timing::JUMP_END_POS;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerAI::Uninit()
{
	// �e�N���X�̏I��
	CPlayer::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CPlayerAI::Kill()
{
	// �e�N���X�̏I��
	CPlayer::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�N���X�̍X�V
	CPlayer::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ��ԍX�V
	UpdateMode(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CPlayerAI::Draw()
{
	// �e�N���X�̕`��
	CPlayer::Draw();
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CPlayerAI::UpdateMode(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (pBall != nullptr && pBall->GetPlayer() == nullptr) {
		
		m_eMode = EMode::MODE_CATCH;
	}

	// ��ԍX�V
	(this->*(m_ModeFunc[m_eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �q�b�g����
//==========================================================================
CPlayerAI::SHitInfo CPlayerAI::Hit(CBall* pBall)
{
	CPlayer::SHitInfo hitInfo = CPlayer::Hit(pBall);	// �q�b�g���

	if (hitInfo.eHit == EHit::HIT_NONE){
		return hitInfo;
	}

	if (hitInfo.eHit == EHit::HIT_CATCH){// �L���b�`���
		// �������[�h
		m_eMode = EMode::MODE_THROW;
	}

	return hitInfo;
}

//==========================================================================
// �����̊Ǘ�
//==========================================================================
void CPlayerAI::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CGameManager* pGameManager = CGameManager::GetInstance();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(this->GetCharStatus()->GetTeam());

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
void CPlayerAI::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_eCatchType = ECatchType::CATCH_TYPE_FIND;

	// �������ލX�V
	(this->*(m_CatchFunc[m_eCatchType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �ʏ퓊������
//==========================================================================
void CPlayerAI::TypeThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����̐ݒ� : ����(���̏�)
	m_eThrowMove = EThrowMove::MOVE_WALK;

	// �����铮���X�V
	(this->*(m_ThrowMoveFunc[m_eThrowMove]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �W�����v��������
//==========================================================================
void CPlayerAI::TypeJumpThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����̐ݒ� : ����(���̏�)
	m_eThrowMove = EThrowMove::MOVE_WALK;

	// �����铮���X�V
	(this->*(m_ThrowMoveFunc[m_eThrowMove]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �X�y�V������������
//==========================================================================
void CPlayerAI::TypeSpecialThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// �X�y�V��������
	pControlAIAction->SetIsSpecial(true);

	// ���ꂼ��̏�Ԃ�߂�
	Reset();
}

//==========================================================================
// �ʏ퓊��(���̏�)
//==========================================================================
void CPlayerAI::MoveNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�~���O�Ǘ�
	TimingManager(fDeltaTime, fDeltaRate, fSlowRate);

	// �����铮���X�V
	(this->*(m_ThrowTimingFunc[m_eThrowTiming]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ��������
//==========================================================================
void CPlayerAI::MoveWalk(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
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
void CPlayerAI::MoveDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �^�C�~���O�Ǘ�
//==========================================================================
void CPlayerAI::TimingManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAI::TimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
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
void CPlayerAI::TimingFeint(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �ʏ�(�W�����v�̓���)
//==========================================================================
void CPlayerAI::TimingJumpNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_fTiming > 0.0f && (m_eThrowMove == EThrowMove::MOVE_WALK || m_eThrowMove == EThrowMove::MOVE_DASH))
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// �������Z�b�g
	pControlAIAction->SetIsJump(true);	// �W�����v�I��

	if (GetPosition().y >= timing::JUMP_END_POS)	// �����ɂ���ĕς��
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
void CPlayerAI::TimingJumpQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_fTiming > 0.0f && m_eThrowMove == EThrowMove::MOVE_WALK || m_eThrowMove == EThrowMove::MOVE_DASH)
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// �����I�t
	pControlAIAction->SetIsJump(true);	// �W�����v�I��

	if (GetPosition().y >= timing::JUMP_END_POS * 0.5f)	// �����ɂ���ĕς��
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
void CPlayerAI::TimingJumpDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_fTiming > 0.0f && m_eThrowMove == EThrowMove::MOVE_WALK || m_eThrowMove == EThrowMove::MOVE_DASH)
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// �����I�t
	pControlAIAction->SetIsJump(true);	// �W�����v�I��

	if (GetPosition().y >= timing::JUMP_END_POS)	// �����ɂ���ĕς��
	{
		m_bFoldJump = true;	// �܂�Ԃ��I��
	}
		
	if (!m_bFoldJump) { return; }

	if (GetPosition().y <= timing::JUMP_END_POS * 0.5f)	// �����ɂ���ĕς��
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
void CPlayerAI::TimingJumpFeint(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_fTiming > 0.0f && m_eThrowMove == EThrowMove::MOVE_WALK || m_eThrowMove == EThrowMove::MOVE_DASH)
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// �����I�t
	pControlAIAction->SetIsJump(true);	// �W�����v�I��

	if (GetPosition().y >= timing::JUMP_END_POS)	// �����ɂ���ĕς��
	{
		// ���ꂼ��̏�Ԃ̃��Z�b�g
		Reset();

		//m_eMode = EMode::MODE_THROW;
	}
}

//==========================================================================
// �ʏ�L���b�`
//==========================================================================
void CPlayerAI::CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �W���X�g�L���b�`
//==========================================================================
void CPlayerAI::CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �_�b�V���L���b�`
//==========================================================================
void CPlayerAI::CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �^�[�Q�b�g
//==========================================================================
void CPlayerAI::Target()
{
	CPlayer* pTarget = nullptr;	// �ڕW�^�[�Q�b�g
	float fMinDis = 1000000.0f;	// �{�[������߂��v���C���[

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���
		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// �v���C���[�ʒu

		// �{�[���̎擾
		CBall* pBall = GetBall();
		if (pBall == nullptr) { return; }
		CGameManager::TeamSide typeTeam = pBall->GetTypeTeam();	// �`�[���^�C�v�̎擾

		// �����`�[���̏ꍇ����
		if (typeTeam == pPlayer->GetStatus()->GetTeam()) { continue; }

		// �G�Ƃ̋��������߂�
		float fLength = sqrtf(
			(pPlayer->GetPosition().x - GetPosition().x) * (pPlayer->GetPosition().x - GetPosition().x) +
			(pPlayer->GetPosition().z - GetPosition().z) * (pPlayer->GetPosition().z - GetPosition().z));

		if (fLength < fMinDis)
		{ // ���߂�����v���C���[�������ꍇ

			// �^�[�Q�b�g���X�V
			pTarget = pPlayer;

			// �����ݒ�
			SetRotDest(GetPosition().AngleXZ(pTarget->GetPosition()));
		}
	}
}

//==========================================================================
// �҂�
//==========================================================================
bool CPlayerAI::IsWait()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	bool b = false;

	if (!pBall) { return b; }

	CGameManager::TeamSide typeTeam = GetStatus()->GetTeam();

	if (typeTeam == CGameManager::TeamSide::SIDE_LEFT)
	{
		if (pBall->GetPosition().x > 0.0f)
		{
			//m_eCatchType = ECatchType::CATCH_TYPE_NONE;
			Reset();
			b = true;
		}
	}
	else if (typeTeam == CGameManager::TeamSide::SIDE_RIGHT)
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
// ���ɍs��
//==========================================================================
void CPlayerAI::FindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsWait()) { return; }

	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (pBall == nullptr || pBall->GetPlayer() != nullptr) {
		Reset();
		return;
	}

	// �p�x�����߂�(player����݂��{�[��)
	float fAngle = GetPosition().AngleXZ(pBall->GetPosition());

	// ���������߂�
	//float fLength = sqrtf((pBall->GetPosition().x - player->GetPosition().x) * (pBall->GetPosition().x - player->GetPosition().x) +
	//	(pBall->GetPosition().z - player->GetPosition().z) * (pBall->GetPosition().z - player->GetPosition().z));

	// �����I��
	pControlAIMove->SetIsWalk(true);

	// �����ݒ�
	SetRotDest(fAngle);
}

//==========================================================================
// �ϐ��̃��Z�b�g
//==========================================================================
void CPlayerAI::Reset()
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
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// �s���t���O���Z�b�g
	pControlAIMove->SetIsWalk(false);
	pControlAIMove->SetIsDash(false);
	pControlAIMove->SetIsBlink(false);

	pControlAIAction->SetIsJump(false);
}

//==========================================================================
// ���쏈��
//==========================================================================
void CPlayerAI::Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// �ړ�����	// TODO�FAI�ނ��`
	pControlMove->Move(this, fDeltaTime, fDeltaRate, fSlowRate);
	pControlAction->Action(this, fDeltaTime, fDeltaRate, fSlowRate);

	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerAI::ChangeMoveControl(CPlayerAIControlMove* control)
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();

	delete pControlMove;
	pControlMove = control;

	SetPlayerControlMove(pControlMove);
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayerAI::ChangeActionControl(CPlayerAIControlAction* control)
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	delete pControlAction;
	pControlAction = control;

	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// ����֘A�폜
//==========================================================================
void CPlayerAI::DeleteControl()
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	if (pControlMove != nullptr)
	{// �ړ�����
		delete pControlMove;
		pControlMove = nullptr;
	}

	if (pControlAction != nullptr)
	{// �A�N�V��������
		delete pControlAction;
		pControlAction = nullptr;
	}

	// ����֘A�폜
	CPlayer::DeleteControl();

	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// �f�o�b�O����
//==========================================================================
void CPlayerAI::Debug()
{
#if _DEBUG
	// �f�o�b�O����
	CPlayer::Debug();

	CPlayerAIControlAction* pControlAction = dynamic_cast<CPlayerAIControlAction*> (GetPlayerControlAction());
	if (pControlAction == nullptr) return;

	// ���������t���O�ݒ�
	bool autoThrow = pControlAction->IsAutoThrow();
	if (ImGui::Checkbox("Change Auto Throw", &autoThrow))
	{ 
		pControlAction->SetEnableAutoThrow(autoThrow);
	}
#endif
}