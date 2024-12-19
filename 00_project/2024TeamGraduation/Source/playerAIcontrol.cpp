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
	// �L���b�`�֘A
	const float CATCH_JUMP_LENGTH = 100.0f;		// �W�����v�L���b�`�̋���
	const float CATCH_JUMP_HEIGHT = 300.0f;		// �W�����v�L���b�`���鍂��

	// �s���^�C�v
	const int MOVETYPE_WAIT_MODE_MAX		= 10;		// �ҋ@�ɓ���m���� (�Œ��0�Œ�)
	const int MOVETYPE_WAIT_MODE_IN			= 6;		// �ҋ@�ɓ���m��(���̐��l�ȉ��̏ꍇ�A�ҋ@���)
	const int MOVETYPE_WAIT_TIME_MAX		= 20;		// �ő�F�ҋ@����
	const int MOVETYPE_WAIT_TIME_MIN		= 10;		// �Œ�F
	const int MOVETYPE_ATYAKOTYA_TIME_MAX	= 10;		// �ő�F�������Ⴑ�����᎞��
	const int MOVETYPE_ATYAKOTYA_TIME_MIN	= 5;		// �Œ�F
	const int MOVETYPE_LEFTRIGHT_TIME_MAX	= 10;		// �ő�F���E�ړ�����
	const int MOVETYPE_LEFTRIGHT_TIME_MIN	= 5;		// �Œ�F
	const int MOVETYPE_UPDOWN_TIME_MAX		= 10;		// �ő�F�㉺�ړ�����
	const int MOVETYPE_UPDOWN_TIME_MIN		= 5;			// �Œ�F

	// �p�X
	const float STEAL_CANCEL_LENGTH = 100.0f;	// ������߂鋗��

	// �X�e�[�^�X�֘A
	const float JUMP_RATE = 1.0f;				// �W�����v�̊���(����)
	const float MOTIVATION_MAX = 100;			// ���`�x�[�V����(MAX)
		
	// ������(�f�t�H���g)
	const float LENGTH_TARGET	= 400.0f;			// �^�[�Q�b�g�Ƃ̋���(�f�t�H���g)
	const float LENGTH_FRIEND	= 100.0f;		// ����F�����Ƃ̋���(�f�t�H���g)
	const float LENGTH_OUT		= 100.0f;		// �O��Ƃ̋���(�f�t�H���g)

	const float BALL_DISTANCE = 300.0f;				// 
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CPlayerAIControl::MODE_FUNC CPlayerAIControl::m_ModeFunc[] =	// ���[�h�֐�
{
	&CPlayerAIControl::ModeIdle,				// �ҋ@
	&CPlayerAIControl::ModeAttack,				// �U��
	&CPlayerAIControl::ModeDefense,				// ���
};

CPlayerAIControl::MOVEFORCIBLY_FUNC CPlayerAIControl::m_MoveForciblyFunc[] =	// �����s���֐�
{
	&CPlayerAIControl::ForciblyNone,			// �Ȃ�
	&CPlayerAIControl::ForciblyStop,			// �~�܂�
	&CPlayerAIControl::ForciblyReturn,			// �߂�
	&CPlayerAIControl::ForciblyStart,			// ����
};

CPlayerAIControl::THROWTYPE_FUNC CPlayerAIControl::m_ThrowTypeFunc[] =	// �����^�C�v�֐�
{
	&CPlayerAIControl::ThrowTypeNone,			// �Ȃ�
	&CPlayerAIControl::ThrowTypeNormal,			// �ʏ�
	&CPlayerAIControl::ThrowTypeJump,			// �W�����v
	&CPlayerAIControl::ThrowTypeSpecial,		// �X�y�V����
};

CPlayerAIControl::ACTION_FUNC CPlayerAIControl::m_ActionFunc[] =	// �L���b�`�֐�
{
	&CPlayerAIControl::MoveIdle,				// �Ȃ�
	&CPlayerAIControl::MoveDodge,				// ���
	&CPlayerAIControl::MoveSupport,				// �T�|�[�g
	&CPlayerAIControl::MoveChaseBall,			// �{�[����ǂ�������
	&CPlayerAIControl::MoveRetreat,				// ���
	&CPlayerAIControl::MoveRandom,				// �����_��
};

// �t���O
CPlayerAIControl::MOVEFLAG_FUNC CPlayerAIControl::m_MoveFlagFunc[] =	// �s���t���O�֐�
{
	&CPlayerAIControl::MoveFlagStop,			// �~�܂�
	&CPlayerAIControl::MoveFlagWalk,			// ����
	&CPlayerAIControl::MoveFlagBlink,			// ����
	&CPlayerAIControl::MoveFlagDash,			// ����
};

CPlayerAIControl::THROWFLAG_FUNC CPlayerAIControl::m_ThrowFlagFunc[] =	// �����t���O�֐�
{
	&CPlayerAIControl::ThrowFlagNone,			// �Ȃ�
	&CPlayerAIControl::ThrowFlag,				// ����
	&CPlayerAIControl::ThrowFlagPass,			// �p�X
	&CPlayerAIControl::ThrowFlagSpecial,		// �X�y�V����
};

CPlayerAIControl::ACTIONFLAG_FUNC CPlayerAIControl::m_ActionFlagFunc[] =	// �A�N�V�����t���O�֐�
{
	&CPlayerAIControl::ActionFlagNone,			// �Ȃ�
	&CPlayerAIControl::ActionFlagJump,			// �W�����v
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControl::CPlayerAIControl()
{
	// �񋓂̏�����
	m_eMode = EMode::MODE_IDLE;
	m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
	m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
	m_eActionFlag = EActionFlag::ACTION_NONE;
	m_eThrowFlag = EThrowFlag::THROW_NONE;
	m_eAction = EAction::IDLE;

	// �\���̂̏�����
	ZeroMemory(&m_sThrow, sizeof(m_sThrow));
	ZeroMemory(&m_sMove, sizeof(m_sMove));
	ZeroMemory(&m_sParameter, sizeof(m_sParameter));

	// �ϐ��̏�����
	m_fDistance = 0.0f;
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
// ����������
//==========================================================================
HRESULT CPlayerAIControl::Init()
{
	// �l�̏�����
	//m_eForcibly = EMoveForcibly::FORCIBLY_START;
	m_sParameter.nMotivation = MOTIVATION_MAX;

	// �S�̏�����
	InitHeart();

	return S_OK;
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

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIControl::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �p�����[�^�X�V
	UpdateParameter();

	// �Ǘ��F���[�h
	ModeManager(fDeltaTime, fDeltaRate, fSlowRate);

	// �X�V�F�����s��
	UpdateForcibly();
	
	{// �t���O�̍X�V

		// �X�V�F�s��
		UpdateMoveFlag();
		// �X�V�F�A�N�V����
		UpdateActionFlag();
		// �X�V�F����
		UpdateThrowFlag();
	}

	// �X�V�F�s������
	UpdateMoveTimer(fDeltaTime, fDeltaRate, fSlowRate);

#ifdef _DEBUG

	MyLib::Vector3 pos = m_pAI->GetPosition();

	// �S���ƂɃp�����[�^�̊��蓖��
	switch (m_sParameter.eHeartMain)
	{
	case EHeartMain::HEART_MAIN_NORMAL:	// �ʏ�
		CEffect3D::Create
		(// �f�o�b�O�p�G�t�F�N�g(�^�[�Q�b�g)
			{ pos.x, pos.y + 90.0f, pos.z },
			VEC3_ZERO,
			MyLib::color::White(),
			10.0f,
			0.1f,
			1,
			CEffect3D::TYPE::TYPE_NORMAL
		);	//
		break;

	case EHeartMain::HEART_MAIN_STRONG:	// ���C
		CEffect3D::Create
		(// �f�o�b�O�p�G�t�F�N�g(�^�[�Q�b�g)
			{ pos.x, pos.y + 90.0f, pos.z },
			VEC3_ZERO,
			MyLib::color::Red(),
			10.0f,
			0.1f,
			1,
			CEffect3D::TYPE::TYPE_NORMAL
		);	//
		break;

	case EHeartMain::HEART_MAIN_TIMID:	// ��C
		CEffect3D::Create
		(// �f�o�b�O�p�G�t�F�N�g(�^�[�Q�b�g)
			{ pos.x, pos.y + 90.0f, pos.z },
			VEC3_ZERO,
			MyLib::color::Blue(),
			10.0f,
			0.1f,
			1,
			CEffect3D::TYPE::TYPE_NORMAL
		);	//
		break;

	default:
		assert(false);
		break;
	}
#endif
}

//==========================================================================
// ���[�h�Ǘ�
//==========================================================================
void CPlayerAIControl::ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) {// �{�[�������E�ɑ��݂��Ȃ��ꍇ
		m_eMode = EMode::MODE_IDLE;
		return;
	}

	if (pBall->GetPlayer() == m_pAI)
	{// �{�[���������Ă���̂������������ꍇ
		m_eMode = EMode::MODE_ATTACK;	// ����
	}
	else if (pBall->GetPlayer() == nullptr || pBall->GetPlayer() != nullptr && pBall->GetTypeTeam() != m_pAI->GetTeam())
	{// �{�[�����擾����Ă��Ȃ��ꍇ||�����ƃ{�[���������Ă���`�[�����Ⴄ�ꍇ
		m_eMode = EMode::MODE_DEFENSE;	// �L���b�`
	}
	else if (pBall->GetPlayer() != nullptr && pBall->GetTypeTeam() == m_pAI->GetTeam())
	{// �{�[�����擾����Ă��Ȃ��ꍇ||�����ƃ{�[���������Ă���`�[���������ꍇ
		m_eMode = EMode::MODE_IDLE;	// �ҋ@
	}

	// ���[�h�̍X�V
	UpdateMode(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���[�h�X�V
//==========================================================================
void CPlayerAIControl::UpdateMode(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsLineOverPlayer() && m_eForcibly != EMoveForcibly::FORCIBLY_START)
	{// ���𒴂��Ă���&&�����s���F���߈ȊO�̏ꍇ
		m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
		return;
	}

	// ���[�h�X�V
	(this->*(m_ModeFunc[m_eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
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
	m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
}

#if 0
//--------------------------------------------------------------------------
// �����F�߂�
//--------------------------------------------------------------------------
void CPlayerAIControl::ForciblyReturn()
{
	// �v���C���[���擾
	MyLib::Vector3 posMy = m_pAI->GetPosition();
	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();

	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ����
	m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

	// �߂Â�
	if (Approatch({ RETURN_POS, posMy.y, posMy.z }, OK_LENGTH))
	{
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
	}
}
#endif

//--------------------------------------------------------------------------
// �����F���߂̃_�b�V��
//--------------------------------------------------------------------------
void CPlayerAIControl::ForciblyStart()
{
	if (IsWhoPicksUpTheBall())
	{// �������߂��v���C���[�������ꍇ
		m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eActionFlag = EActionFlag::ACTION_NONE;
		return;
	}

	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall)
	{// �{�[�����Ȃ��ꍇ
		m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eActionFlag = EActionFlag::ACTION_NONE;
		return;
	}

	CPlayer* pPlayer = pBall->GetPlayer();
	if (pPlayer)
	{// �N�����{�[�����������ꍇ
		m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eActionFlag = EActionFlag::ACTION_NONE;
		return;
	}

	m_eActionFlag = EActionFlag::ACTION_JUMP;
	m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;
}

//==========================================================================
// ���[�h�F�ҋ@
//==========================================================================
void CPlayerAIControl::ModeIdle(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �s���F�~�܂�
	m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
}

//==========================================================================
// ���[�h�F����
//==========================================================================
void CPlayerAIControl::ModeAttack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
			//int n = 1;
			// ���̓����_���Ō���
			int n = rand() % 2;

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

	// �����̍X�V
	UpdateThrowType();
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
	//m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

	//// ������܂ł̍s���̍X�V
	//(this->*(m_ThrowMoveFunc[m_eMoveFlag]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);

	m_eThrowFlag = EThrowFlag::THROW_NORMAL;
}

//--------------------------------------------------------------------------
// �W�����v����
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTypeJump()
{
	// ���̏�Ȃ̂������̂�����̂�
	//m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

	CPlayer* pTartget = GetThrowTarget();

	// ���蓊��
	AttackDash(pTartget);
}

//--------------------------------------------------------------------------
// �X�y�V��������
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTypeSpecial()
{
	m_eThrowFlag = EThrowFlag::THROW_SPECIAL;
}


//==========================================================================
// ���̊Ǘ�
//==========================================================================
void CPlayerAIControl::ModeDefense(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �L���b�`�̍X�V
	UpdateDefense(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// ���̍X�V����
//================================================================================
void CPlayerAIControl::UpdateDefense(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// �{�[���˂���
	CBall::EState stateBall = pBall->GetState();	// �{�[����Ԏ擾
	
	// ��������擾
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{// �{�[�������҂�����

		// ���(���S�n�т�)
		m_eAction = EAction::RNDOM;
	}
	else
	{
		if (IsPassTarget() && stateBall == CBall::EState::STATE_PASS)
		{// �p�X�������ɗ���&&�{�[�����p�X��Ԃ̍�

			// �������Ȃ�
			m_eAction = EAction::IDLE;
		}

		if (pBall->GetTarget() == m_pAI)
		{// �^�[�Q�b�g������

			float distanceBall = GetDistanceBall();		// �{�[���Ƃ̋���

			//if (distanceBall < 50.0f) {
			//	// ���
			//	m_eAction = EAction::IDLE;
			//}
			//else if (distanceBall < 300.0f) {
			//	// ���(���S�n�т�)
			//	m_eAction = EAction::RETREAT;
			//}
		}
		else {

			m_eAction = EAction::SUPPORT;	// �T�|�[�g���
		}

		if (IsLineOverBall())
		{// �{�[�������𒴂��Ă���ꍇ

			// �������Ȃ�
			//m_eAction = EAction::IDLE;

			// ���(���S�n�т�)
			m_eAction = EAction::RETREAT;
		}
		else if (!IsLineOverBall() &&						// ���𒴂��Ă��Ȃ�
			stateBall == CBall::EState::STATE_FREE ||		// �t���[
			stateBall == CBall::EState::STATE_PASS ||		// �p�X
			stateBall == CBall::EState::STATE_HOM_PASS)		// �z�[�~���O�p�X
		{
			// �{�[����ǂ�
			m_eAction = EAction::CHASE_BALL;
		}
	}

	// �A�N�V������ލX�V
	(this->*(m_ActionFunc[m_eAction]))();
}

//================================================================================
// �J�o�[
//================================================================================
void CPlayerAIControl::MoveCover(CPlayer* pPlayer)
{
	if (!pPlayer) return;

	CPlayer* pPlayerBall = GetBallOwner();
	if (!pPlayerBall) return;

	MyLib::Vector3 pos = pPlayer->GetPosition();

}

//==========================================================================
// �s���֘A�̏���
//==========================================================================

//--------------------------------------------------------------------------
// �������Ȃ�
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveIdle()
{
	// �s���t���O�F�Ȃ�
	m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
}

//--------------------------------------------------------------------------
// ���
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveDodge()
{
	// �{�[���Ƃ̋���
	float distanceBall = GetDistanceBall();

	// ����������Ă���ꍇ
	if (distanceBall > 5.0f) return;

	// �s���F�u�����N
	m_eMoveFlag = EMoveFlag::MOVEFLAG_BLINK;
}

//--------------------------------------------------------------------------
// �T�|�[�g
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveSupport()
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// �{�[���˂���
	CBall::EState stateBall = pBall->GetState();	// �{�[����Ԏ擾

	// ���o�E���h�����
	if (stateBall == CBall::EState::STATE_REBOUND)
	{// �{�[����Ԃ����o�E���h�̏ꍇ

		if (IsLineOverBall()) return;	// �{�[�������𒴂��Ă���

		// �ʒu�̎擾
		MyLib::Vector3 posBall = pBall->GetPosition();		// �{�[��
		MyLib::Vector3 posEnd = pBall->GetPosPassEnd();		// �{�[���̃p�X�I���ʒu
		MyLib::Vector3 posMy = m_pAI->GetPosition();		// �����̈ʒu

		// �I���ʒu��x,z���Q�Ƃ����ʒu�̐ݒ�
		MyLib::Vector3 pos = { posEnd.x, posMy.y, posEnd.z };

		// �{�[���Ƃ̋���
		float distance = posMy.DistanceXZ(posBall);

		// �s����ԁF����
		m_eMoveFlag = EMoveFlag::MOVEFLAG_DASH;

		// �{�[���̕��֍s��
		if (Approatch(pos, CATCH_JUMP_LENGTH) || distance < CATCH_JUMP_LENGTH)
		{// �I���ʒu�ɋ߂Â���||�{�[���Ƃ̋������͈͓��̏ꍇ

			// �s���F�������Ȃ�
			m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
		}

		return;
	}

	// �̗͂̏��Ȃ��������J�o�[
	int nLife = m_pAI->GetLife();

	if (!m_sMove.pDefenseTarget)
	{
		int nMinLife = 100000000;

		// �������
		MyLib::Vector3 posMy = m_pAI->GetPosition();		// �ʒu���̎擾
		CGameManager::ETeamSide TeamMy = m_pAI->GetTeam();	// �����`�[��

		CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
		std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
		while (list.ListLoop(itr))
		{ // ���X�g���̗v�f�����J��Ԃ�

			CPlayer* pPlayer = (*itr);	// �v���C���[���

			// ����ȊO&&�����`�[������Ȃ�
			if (pPlayer->GetAreaType() != CPlayer::EFieldArea::FIELD_IN &&
				pPlayer->GetTeam() != m_pAI->GetTeam()) continue;

			// ����
			if (pPlayer == m_pAI) continue;

			// ���l�̗̑�
			int nLifeOther = pPlayer->GetLife();

			if (nLifeOther < nLife && nLifeOther < nMinLife)
			{// �����̗̑͂�菭�Ȃ�&&�`�[���̒��ň�ԑ̗͂����Ȃ��ꍇ

				// �ŏ��̗͂̍X�V
				nMinLife = nLifeOther;

				// ����Ώۂ̐ݒ�
				m_sMove.pDefenseTarget = pPlayer;
			}
		}

		return;
	}


	// �����Ƃ̋��������(�W�I�𕪎U)

}

//--------------------------------------------------------------------------
// �{�[����ǂ�������
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveChaseBall()
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// �{�[���˂���

	// �{�[����Ԏ擾
	CBall::EState stateBall = pBall->GetState();

	if (stateBall == CBall::EState::STATE_PASS ||
		stateBall == CBall::EState::STATE_HOM_PASS ||
		stateBall == CBall::EState::STATE_MOVE)
	{// �p�X||�z�[�~���O�p�X||�ړ���Ԃ̏ꍇ

		// �{�[����D��
		BallSteal();
	}
	else if (!IsLineOverBall() && !IsWhoPicksUpTheBall())
	{
		// �L���b�`�F���ɍs��
		BallChase();
	}
}

#if 0	// �I�[�o�[���C�h����\������
//--------------------------------------------------------------------------
// ��ށi���S�n�тցj
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveRetreat()
{
	// �{�[��������̎擾
	CPlayer* pPlayer = GetBallOwner();
	if (!pPlayer) return;

	// �����̈ʒu
	MyLib::Vector3 posMy = m_pAI->GetPosition();

	// ���S�n��
	float posSafeX = m_fDistance + 300.0f;

	if (posMy.x > posSafeX) {// �ړ��^�C�v�F��
		m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
		m_eAction = EAction::IDLE;
		return;
	}

	// �s���t���O�F����
	m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

	// �E�ړ�
	MoveRight();
}
#endif

//--------------------------------------------------------------------------
// �����_���ړ�
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveRandom()
{
	// x950 z560
	MyLib::Vector3 posSafeMax = { 950.0f, 0.0f, 560.0f };
	MyLib::Vector3 posSafeMin = { m_fDistance, 0.0f, -560.0f };

	if (!m_sMove.bSet) {
		// �ʒu�̐ݒ�
		// x���W
		float fRand = (float)UtilFunc::Transformation::Random(posSafeMin.x, posSafeMax.x);
		m_sMove.pos.x = fRand;
		// z���W
		fRand = (float)UtilFunc::Transformation::Random(posSafeMin.z, posSafeMax.z);
		m_sMove.pos.z = fRand;

		m_sMove.bSet = true;
	}

	// �s���F����
	m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

	// �߂Â�
	if (Approatch(m_sMove.pos, 10.0f)) {
		m_sMove.bSet = false;
	}
}


//--------------------------------------------------------------------------
// ��ړ�
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveUp()
{
	// �{�[��������̎擾
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{// �����傪����

		// AI�R���g���[�����̎擾
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		float rotDest = D3DX_PI;

		// �J�j�i�s�����̐ݒ�
		pControlAIMove->SetClabDirection(rotDest);
	}
	else
	{
		// �J�j�i�s�����̐ݒ�
		float rotDest = D3DX_PI;
		m_pAI->SetRotDest(rotDest);
	}
}

//--------------------------------------------------------------------------
// ���ړ�
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveDown()
{
	// �{�[��������̎擾
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{
		// AI�R���g���[�����̎擾
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// �i�s���������߂�
		float rotDest = 0.0f;

		// �J�j�i�s�����̐ݒ�
		pControlAIMove->SetClabDirection(rotDest);
	}
	else
	{
		// �J�j�i�s�����̐ݒ�
		float rotDest = 0.0f;
		m_pAI->SetRotDest(rotDest);
	}
}

//--------------------------------------------------------------------------
// ���ړ�
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveLeft()
{
	// �{�[��������̎擾
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{
		// AI�R���g���[�����̎擾
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// �i�s���������߂�
		float rotDest = D3DX_PI * 0.5f;

		// �J�j�i�s�����̐ݒ�
		pControlAIMove->SetClabDirection(rotDest);
	}
	else
	{
		// �J�j�i�s�����̐ݒ�
		float rotDest = D3DX_PI * 0.5f;
		m_pAI->SetRotDest(rotDest);
	}
}

//--------------------------------------------------------------------------
// �E�ړ�
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveRight()
{
	// �{�[��������̎擾
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{
		// AI�R���g���[�����̎擾
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// �i�s���������߂�
		float rotDest = -D3DX_PI * 0.5f;

		// �J�j�i�s�����̐ݒ�
		pControlAIMove->SetClabDirection(rotDest);
	}
	else
	{
		// �J�j�i�s�����̐ݒ�
		float rotDest = -D3DX_PI * 0.5f;
		m_pAI->SetRotDest(rotDest);
	}
}




//==========================================================================
// �{�[��������Ƃ̋������v�Z
//==========================================================================
float CPlayerAIControl::GetDistanceBallowner()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return 0.0f;

	CPlayer* pPlayer = pBall->GetPlayer();
	if (!pPlayer) return 0.0f;
	if (pPlayer->GetTeam() == m_pAI->GetTeam()) return 0.0f;

	// �������v�Z
	float distance = m_pAI->GetPosition().DistanceXZ(pPlayer->GetPosition());

	// �v�Z���ʂ�Ԃ�
	return distance;
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
		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// �v���C���[�ʒu

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
// �{�[��������̎擾�i�G�̏ꍇ�j
//==========================================================================
CPlayer* CPlayerAIControl::GetBallOwner()
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

//==========================================================================
// �{�[���Ƃ̋���
//==========================================================================
float CPlayerAIControl::GetDistanceBall()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) { return 0.0f; }

	float distance = m_pAI->GetPosition().DistanceXZ(pBall->GetPosition());

	return distance;
}

//==========================================================================
// �G�Ƃ̋���
//==========================================================================
float CPlayerAIControl::GetDistanceEnemy()
{
	float fMin = 10000000.0f;	// �ŏ��l�̕ۑ��p

	// �������
	MyLib::Vector3 posMy = m_pAI->GetPosition();		// �ʒu���̎擾
	CGameManager::ETeamSide TeamMy = m_pAI->GetTeam();	// �����`�[��

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���

		// ������&&�����`�[���ꍇ
		if (pPlayer == m_pAI && TeamMy == pPlayer->GetTeam()) continue;

		// �G���A�^�C�v
		CPlayer::EFieldArea areaPlayer = pPlayer->GetAreaType();

		// �O��̏ꍇ
		if (areaPlayer == CPlayer::EFieldArea::FIELD_OUT) continue;

		// �v���C���[�ʒu
		MyLib::Vector3 pos = pPlayer->GetPosition();

		// �G�Ƃ̋��������߂�
		float fLength = posMy.DistanceXZ(pos);

		if (fLength < fMin)
		{// ���߂��ꍇ

			fMin = fLength;
		}
	}

	return fMin;
}

//==========================================================================
// �{�[����D��
//==========================================================================
void CPlayerAIControl::BallSteal()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	// �����`�[���̏ꍇ
	if (pBall->GetTypeTeam() == m_pAI->GetTeam()) return;

	// �ʒu�̎擾
	MyLib::Vector3 posBall = pBall->GetPosition();		// �{�[��
	MyLib::Vector3 posEnd = pBall->GetPosPassEnd();		// �{�[���̃p�X�I���ʒu
	MyLib::Vector3 posMy = m_pAI->GetPosition();		// �����̈ʒu

	// �I���ʒu��x,z���Q�Ƃ����ʒu�̐ݒ�
	MyLib::Vector3 pos = { posEnd.x, posMy.y, posEnd.z };

	// �p�X����̎擾
	CPlayer* pTarget = pBall->GetTarget();
	if (pTarget)
	{
		// �^�[�Q�b�g�ƃ{�[���̈ʒu
		float distanth0 = pTarget->GetPosition().DistanceXZ(posBall);

		if (distanth0 < STEAL_CANCEL_LENGTH)
		{// �{�[���ƃp�X��̋������͈͓��Ȃ炠����߂�
			m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
			return;
		}
	}

	// �{�[���Ƃ̋���
	float distance = posMy.DistanceXZ(posBall);

	// �s����ԁF����
	m_eMoveFlag = EMoveFlag::MOVEFLAG_DASH;

	// �{�[���̕��֍s��
	if (Approatch(pos, CATCH_JUMP_LENGTH) || distance < CATCH_JUMP_LENGTH)
	{// �I���ʒu�ɋ߂Â���||�{�[���Ƃ̋������͈͓��̏ꍇ

		if (posBall.y < CATCH_JUMP_HEIGHT)
		{// ��ꂻ���ȍ����ɗ����I
			m_eActionFlag = EActionFlag::ACTION_JUMP;
		}
	}
}

//==========================================================================
// �{�[����ǂ�
//==========================================================================
void CPlayerAIControl::BallChase()
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
		m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
		return;
	}

	// �{�[���ʒu
	MyLib::Vector3 posBall = pBall->GetPosition();

	// ��������{�[���̋���
	float distance = m_pAI->GetPosition().DistanceXZ(posBall);

	if (distance > BALL_DISTANCE) {
		// �s���F����
		m_eMoveFlag = EMoveFlag::MOVEFLAG_DASH;
	}
	else {
		// �s���F����
		m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;
	}

	// �߂Â�
	if (Approatch(pBall->GetPosition(), 50.0f))
	{// �߂Â����ꍇ
		// �s���F�~�܂�
		m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
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
	MyLib::Vector3 posMy = m_pAI->GetPosition();

	// ����Ǝ����̋���
	float length = posMy.DistanceXZ(targetPos);	// �����F����Ǝ���

	if (distance < length) {// �ړ��^�C�v�F��
		return true;
	}

	// �^�[�Q�b�g���玩���̊p�x
	float direction = targetPos.AngleXZ(posMy);

	// �{�[��������̎擾
	CPlayer* pPlayerBall = GetBallOwner();

	if (pPlayerBall)
	{// �{�[�������݂��Ȃ�
		if (pPlayerBall->GetTeam() != m_pAI->GetTeam() &&
			pPlayerBall != m_pAI) {// �Ⴄ�`�[���������Ă��� && �����傪�����ȊO�ꍇ

			if (m_eMoveFlag == EMoveFlag::MOVEFLAG_DASH)
			{// �s���F����
				// �p�x�ݒ�
				m_pAI->SetRotDest(direction);
				return false;
			}

			// �J�j�i�s�����̐ݒ�
			pControlAIMove->SetClabDirection(direction);
			return false;
		}
	}

	// �p�x�ݒ�
	m_pAI->SetRotDest(direction);

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

	// �{�[�����̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	// �����̈ʒu���
	MyLib::Vector3 posMy = m_pAI->GetPosition();

	// ����Ǝ����̋���
	float length = posMy.DistanceXZ(targetPos);	// �����F����Ǝ���

	if (distance > length) {// �ړ��^�C�v�F��
		return true;
	}

	// ��������^�[�Q�b�g�̊p�x
	float direction = posMy.AngleXZ(targetPos);

	// �{�[��������̎擾
	CPlayer* pPlayerBall = GetBallOwner();

	if (pPlayerBall)
	{// �{�[�������݂��Ȃ�
		if (pPlayerBall->GetTeam() != m_pAI->GetTeam() &&
			pPlayerBall != m_pAI) {// �Ⴄ�`�[���������Ă��� && �����傪�����ȊO�ꍇ

			if (m_eMoveFlag == EMoveFlag::MOVEFLAG_DASH)
			{// �s���F����
				// �p�x�ݒ�
				m_pAI->SetRotDest(direction);
				return false;
			}

			// �J�j�i�s�����̐ݒ�
			pControlAIMove->SetClabDirection(direction);
			return false;
		}
	}

	// �p�x�ݒ�
	m_pAI->SetRotDest(direction);


#if 1
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
// �S�̏�����
//==========================================================================
void CPlayerAIControl::InitHeart()
{
	// �����_��
	int fRand = UtilFunc::Transformation::Random(EHeartMain::HEART_MAIN_NORMAL, EHeartMain::HEART_MAIN_MAX - 1);

	// �S�̊��蓖��
	m_sParameter.eHeartMain = (EHeartMain)fRand;
	m_sParameter.eHeartMain = EHeartMain::HEART_MAIN_STRONG;

	// �S���ƂɃp�����[�^�̊��蓖��
	switch (m_sParameter.eHeartMain)
	{
	case EHeartMain::HEART_MAIN_NORMAL:	// �ʏ�
		m_fDistance = LENGTH_TARGET;
		break;

	case EHeartMain::HEART_MAIN_STRONG:	// ���C
		m_fDistance = LENGTH_TARGET - 100.0f;
		break;

	case EHeartMain::HEART_MAIN_TIMID:	// ��C
		m_fDistance = LENGTH_TARGET + 200.0f;
		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// �p�����[�^�[�X�V
//==========================================================================
void CPlayerAIControl::UpdateParameter()
{
	// �s��

	// ����

	// �X�y�V����


	//// �̗͂̎擾
	int nLifeMax = m_pAI->GetLifeOrigin();
	int nLife = m_pAI->GetLife();

	//// ���������߂�
	//float rate = 0.0f;
	//rate = (float)nLife / (float)nLifeMax;


	// �l�̐ݒ�
	float value = 0.0f;

	// �^�[�Q�b�g�Ƃ̋���
	switch (m_sParameter.eHeartMain)
	{
	case EHeartMain::HEART_MAIN_NORMAL:
		m_fDistance = m_fDistance + (value);
		break;

	case EHeartMain::HEART_MAIN_STRONG:
		m_fDistance = m_fDistance + (value );
		break;

	case EHeartMain::HEART_MAIN_TIMID:
		m_fDistance = m_fDistance + (value );
		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// �s�����Ԃ̐ݒ�
//==========================================================================
void CPlayerAIControl::SetMoveTimer(int nMin, int nMax)
{
	// �p�x�̐ݒ�
	if (!m_sMove.bSet)
	{
		// �s�����Ԃ̐ݒ�
		float fRand = (float)UtilFunc::Transformation::Random(nMin, nMax);
		m_sMove.fTimer = fRand * 0.1f;

		// ���Ԑݒ�ON
		m_sMove.bSet = true;
	}
}

//==========================================================================
// �s�����Ԃ̍X�V
//==========================================================================
void CPlayerAIControl::UpdateMoveTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sMove.bCancel)
	{


		// �s���̍\���̏�����
		ZeroMemory(&m_sMove, sizeof(m_sMove));
	}

	if (m_sMove.bSet)
	{
		// �^�C�}�[�̃J�E���g�_�E��
		m_sMove.fTimer -= fDeltaTime * fDeltaRate * fSlowRate;

		if (m_sMove.fTimer <= 0.0f)
		{
			// �s���̍\���̏�����
			ZeroMemory(&m_sMove, sizeof(m_sMove));

			// �A�N�V�����F�Ȃ�
			m_eAction = EAction::IDLE;

			// �s���F�~�܂�
			m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
		}
	}
}


// �t���O�ŊǗ� ===================================================================================================

//================================================================================
// �s���t���O�̍X�V����
//================================================================================
void CPlayerAIControl::UpdateMoveFlag()
{
	// �s���t���O�X�V
	(this->*(m_MoveFlagFunc[m_eMoveFlag]))();
}

//--------------------------------------------------------------------------
// �~�܂�
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveFlagStop()
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
// ����
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveFlagWalk()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ����ON
	pControlAIMove->SetIsWalk(true);

	// �t���OOFF
	pControlAIMove->SetIsDash(false);
	pControlAIMove->SetIsBlink(false);
}

//--------------------------------------------------------------------------
// �u�����N
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveFlagBlink()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �u�����NON
	pControlAIMove->SetIsBlink(true);

	// �t���OOFF
	pControlAIMove->SetIsWalk(false);
	pControlAIMove->SetIsDash(false);
}

//--------------------------------------------------------------------------
// ����
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveFlagDash()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ����ON
	pControlAIMove->SetIsWalk(true);
	// �u�����NON
	pControlAIMove->SetIsBlink(true);
	// ����ON
	pControlAIMove->SetIsDash(true);
}

//================================================================================
// �A�N�V�����t���O�̍X�V����
//================================================================================
void CPlayerAIControl::UpdateActionFlag()
{
	// �A�N�V�����X�V
	(this->*(m_ActionFlagFunc[m_eActionFlag]))();
}

//--------------------------------------------------------------------------
// �Ȃɂ����Ȃ�
//--------------------------------------------------------------------------
void CPlayerAIControl::ActionFlagNone()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// �W�����v�Ftrue
	pControlAIAction->SetIsJump(false);
	pControlAIAction->SetIsJumpFloat(false);

	// �A�N�V�����񋓁F�Ȃ�
	m_eActionFlag = EActionFlag::ACTION_NONE;
}

//--------------------------------------------------------------------------
// �W�����v
//--------------------------------------------------------------------------
void CPlayerAIControl::ActionFlagJump()
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
	m_eActionFlag = EActionFlag::ACTION_NONE;
}

//================================================================================
// �����t���O�̍X�V����
//================================================================================
void CPlayerAIControl::UpdateThrowFlag()
{
	// �����X�V
	(this->*(m_ThrowFlagFunc[m_eThrowFlag]))();
}

//--------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowFlag()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// ������
	pControlAIAction->SetIsThrow(true);

	// ������ԁF��
	m_eThrowFlag = EThrowFlag::THROW_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
}

//--------------------------------------------------------------------------
// �p�X ���������ĂȂ�
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowFlagPass()
{

}

//--------------------------------------------------------------------------
// �X�y�V����
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowFlagSpecial()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// �X�y�V��������
	pControlAIAction->SetIsSpecial(true);

	// �����Ȃ�
	m_eThrowFlag = EThrowFlag::THROW_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
}



// ���� ==========================================================================

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
	if (!pBall) return true;
	if (pBall->GetPlayer()) return true;

	// �{�[���ʒu�擾
	MyLib::Vector3 posBall = pBall->GetPosition();
	fMyDis = Mypos.DistanceXZ(posBall);	// ��������{�[���Ƃ̋���

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���

		if (pPlayer == m_pAI) continue;

		if (
			(pPlayer->GetTeam() != typeTeam) ||
			(pPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER) &&
			(pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT))
		{// �Ⴄ�`�[��||�O��&&���[�U�[&&�����̏ꍇ
			continue;
		}

		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// �v���C���[�ʒu

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
// �{�[��������͈͂ɂ��܂����H
//==========================================================================
bool CPlayerAIControl::IsDistanceBall()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return false;

	// �ʒu�̎擾
	MyLib::Vector3 posBall = pBall->GetPosition();		// �{�[��
	MyLib::Vector3 posMy = m_pAI->GetPosition();		// �����̈ʒu

	// �{�[���Ƃ̋���
	float distance = posMy.DistanceXZ(posBall);

	if (distance < STEAL_CANCEL_LENGTH)
	{// �{�[���ƃp�X��̋������͈͓��Ȃ炠����߂�
		return true;
	}

	return false;
}
