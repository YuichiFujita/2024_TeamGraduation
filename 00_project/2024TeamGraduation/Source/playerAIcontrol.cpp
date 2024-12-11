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
	const float CHATCH_LENGTH_IN_PAIR = 700.0f;		// ����F����Ƃ̋���
	const float CHATCH_LENGTH_IN_ALLY = 100.0f;		// ����F�����Ƃ̋���

	const float CHATCH_LENGTH_OUT = 400.0f;		// �O��Ƃ̋���
	const float CHATCH_LENGTH_TARGET = 400.0f;	// �^�[�Q�b�g�Ƃ̋���

	const float CATCH_JUMP_LENGTH = 100.0f;		// �W�����v�L���b�`�̋���
	const float CATCH_JUMP_HEIGHT = 300.0f;		// �W�����v�L���b�`���鍂��

	// �s���^�C�v
	const int MOVETYPE_WAIT_MODE_MAX	= 10;		// �ҋ@�ɓ���m���� (�Œ��0�Œ�)
	const int MOVETYPE_WAIT_MODE_IN		= 6;		// �ҋ@�ɓ���m��(���̐��l�ȉ��̏ꍇ�A�ҋ@���)
	const int MOVETYPE_WAIT_TIME_MAX	= 20;		// �ő�F�ҋ@����
	const int MOVETYPE_WAIT_TIME_MIN	= 10;		// �Œ�F
	const int MOVETYPE_ATYAKOTYA_TIME_MAX = 10;		// �ő�F�������Ⴑ�����᎞��
	const int MOVETYPE_ATYAKOTYA_TIME_MIN = 5;		// �Œ�F
	const int MOVETYPE_LEFTRIGHT_TIME_MAX = 10;		// �ő�F���E�ړ�����
	const int MOVETYPE_LEFTRIGHT_TIME_MIN = 5;		// �Œ�F
	const int MOVETYPE_UPDOWN_TIME_MAX = 10;		// �ő�F�㉺�ړ�����
	const int MOVETYPE_UPDOWN_TIME_MIN = 5;			// �Œ�F

	// ������(�f�t�H���g)
	const float LENGTH = 500.0f;				// ����
	const float LENGTH_SPACE = 10.0f;			// 
	const float LENGTH_OUT = 200.0f;			// �O��
	const float LENGTH_LINE = 100.0f;			// ��
	const float BALL_DISTANCE = 300.0f;

	// �p�X
	const float STEAL_CANCEL_LENGTH = 100.0f;	// ������߂鋗��

	// �X�e�[�^�X�֘A
	const float JUMP_RATE = 1.0f;				// �W�����v�̊���(����)

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

CPlayerAIControl::THROW_FUNC CPlayerAIControl::m_ThrowFunc[] =	// �����֐�
{
	&CPlayerAIControl::ThrowNone,				// �Ȃ�
	&CPlayerAIControl::Throw,					// ����
	&CPlayerAIControl::ThrowPass,				// �p�X
	&CPlayerAIControl::ThrowSpecial,			// �X�y�V����
};

CPlayerAIControl::THROWMOVE_FUNC CPlayerAIControl::m_ThrowMoveFunc[] =	// �����s���֐�
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

CPlayerAIControl::MOVEFORCIBLY_FUNC CPlayerAIControl::m_MoveForciblyFunc[] =	// �s���֐�
{
	&CPlayerAIControl::ForciblyNone,			// �Ȃ�
	&CPlayerAIControl::ForciblyStop,			// �~�܂�
	&CPlayerAIControl::ForciblyReturn,			// �߂�
	&CPlayerAIControl::ForciblyStart,			// ����
};

CPlayerAIControl::MOVEFLAG_FUNC CPlayerAIControl::m_MoveFlagFunc[] =	// �s���֐�
{
	&CPlayerAIControl::MoveFlagStop,			// �~�܂�
	&CPlayerAIControl::MoveFlagWalk,			// ����
	&CPlayerAIControl::MoveFlagBlink,			// ����
	&CPlayerAIControl::MoveFlagDash,			// ����
};

CPlayerAIControl::MOVETYPE_FUNC CPlayerAIControl::m_MoveTypeFunc[] =	// �s���֐�
{
	&CPlayerAIControl::MoveTypeNone,			// �Ȃ�
	&CPlayerAIControl::MoveTypeDistance,		// ����
	&CPlayerAIControl::MoveTypeAtyakotya,		// �����Ⴑ������
	&CPlayerAIControl::MoveTypeLeft,			// ��
	&CPlayerAIControl::MoveTypeRight,			// �E
	&CPlayerAIControl::MoveTypeUp,				// ��
	&CPlayerAIControl::MoveTypeDown,			// ��
};


CPlayerAIControl::ACTION_FUNC CPlayerAIControl::m_ActionFunc[] =	// �s���֐�
{
	&CPlayerAIControl::ActionNone,				// �Ȃ�
	&CPlayerAIControl::ActionJump,				// �W�����v
};

CPlayerAIControl::CATCH_FUNC CPlayerAIControl::m_CatchFunc[] =	// �L���b�`�֐�
{
	&CPlayerAIControl::CatchNone,				// �Ȃ�
	&CPlayerAIControl::CatchNormal,				// �ʏ�
	&CPlayerAIControl::CatchJust,				// �W���X�g
	&CPlayerAIControl::CatchDash,				// �_�b�V��
	&CPlayerAIControl::CatchPassGive,			// �p�X�Ⴄ
	&CPlayerAIControl::CatchPassSteal,			// �p�X�D��
	&CPlayerAIControl::CatchFindBall,			// ������
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControl::CPlayerAIControl()
{
	// �񋓂̏�����
	m_eMode = EMode::MODE_NONE;
	m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
	m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
	m_eMoveType = EMoveTypeChatch::MOVETYPE_NONE;
	m_eHeart = EHeart::HEART_NONE;
	m_eActionFlag = EActionFlag::ACTION_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
	m_eThrow = EThrowFlag::THROW_NONE;
	m_eThrowTiming = EThrowTiming::TIMING_NONE;
	m_eCatchType = ECatchType::CATCH_TYPE_NONE;

	// �\���̂̏�����
	ZeroMemory(&m_sThrow, sizeof(m_sThrow));
	ZeroMemory(&m_sMove, sizeof(m_sMove));
	ZeroMemory(&m_sDistance, sizeof(m_sDistance));
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
	m_sDistance.fInPair = CHATCH_LENGTH_IN_PAIR;
	m_sDistance.fInAlly = CHATCH_LENGTH_IN_ALLY;
	m_sDistance.fOut = CHATCH_LENGTH_OUT;
	m_sDistance.fTarget = CHATCH_LENGTH_TARGET;

	m_eForcibly = EMoveForcibly::FORCIBLY_START;
	m_eMoveType = EMoveTypeChatch::MOVETYPE_DISTANCE;
	m_eSee = ESee::SEE_NONE;

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
	// �Ǘ��F���[�h
	ModeManager(fDeltaTime, fDeltaRate, fSlowRate);

	// �X�V�F�����s��
	UpdateForcibly();
	
	{// �t���O�̍X�V

		// �X�V�F�s��
		UpdateMoveFlag(fDeltaTime, fDeltaRate, fSlowRate);
		// �X�V�F�A�N�V����
		UpdateActionFlag();
		// �X�V�F����
		UpdateThrowFlag();
	}
}

//==========================================================================
// ���[�h�Ǘ�
//==========================================================================
void CPlayerAIControl::ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) {// �{�[�������E�ɑ��݂��Ȃ��ꍇ
		m_eMode = EMode::MODE_NONE;
		return;
	}

	if (pBall->GetPlayer() == nullptr || pBall->GetTypeTeam() != m_pAI->GetTeam())
	{// �{�[�����擾����Ă��Ȃ��ꍇ||�����ƃ{�[���������Ă���`�[�����Ⴄ�ꍇ
		m_eMode = EMode::MODE_CATCH;
	}
	else if (pBall->GetPlayer() == m_pAI)
	{// �{�[���������Ă���̂������������ꍇ
		m_eMode = EMode::MODE_THROW;
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

//==========================================================================
// �����̊Ǘ�
//==========================================================================
void CPlayerAIControl::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����̗���
	PlanThrow();

	// �����̎��
	UpdateThrowType();

#ifdef _DEBUG

	// 1:�^�[�Q�b�g�����߂�
	CPlayer* pTarget = nullptr;
	pTarget = GetThrowTarget();

	if (!pTarget)
	{
		return;
	}

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
// �L���b�`�̊Ǘ�
//==========================================================================
void CPlayerAIControl::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �L���b�`�̍X�V
	UpdateCatch(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ��������
//==========================================================================
void CPlayerAIControl::PlanThrow()
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
			int n = 1;
			// ���̓����_���Ō���
			//int n = rand() % 2;

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
	m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
}

#if 0
//--------------------------------------------------------------------------
// �����F�߂�
//--------------------------------------------------------------------------
void CPlayerAIControl::ForciblyReturn()
{
	// �v���C���[���擾
	MyLib::Vector3 myPos = m_pAI->GetPosition();
	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();

	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ����
	m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

	// �߂Â�
	if (Approatch({ RETURN_POS, myPos.y, myPos.z }, OK_LENGTH))
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
		m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eActionFlag = EActionFlag::ACTION_NONE;
		return;
	}

	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall)
	{// �{�[�����Ȃ��ꍇ
		m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eActionFlag = EActionFlag::ACTION_NONE;
		return;
	}

	CPlayer* pPlayer = pBall->GetPlayer();
	if (pPlayer)
	{// �N�����{�[�����������ꍇ
		m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eActionFlag = EActionFlag::ACTION_NONE;
		return;
	}

	m_eActionFlag = EActionFlag::ACTION_JUMP;
	m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;
}

//================================================================================
// �s���̍X�V����
//================================================================================
void CPlayerAIControl::UpdateMoveFlag(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �s���t���O�X�V
	(this->*(m_MoveFlagFunc[m_eMoveFlag]))();
}

//--------------------------------------------------------------------------
// �s���F�~�܂�
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
// �s���F����
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
// �s���F�u�����N
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveFlagBlink()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �u�����NON
	pControlAIMove->SetIsBlink(false);

	// �t���OOFF
	pControlAIMove->SetIsWalk(false);
	pControlAIMove->SetIsDash(false);
}

//--------------------------------------------------------------------------
// �s���F����
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
// �s���^�C�v
//================================================================================
void CPlayerAIControl::UpdateMoveType(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_eMoveType == EMoveTypeChatch::MOVETYPE_NONE) return;

	//// �p�x�̐ݒ�
	//if (m_sMove.bSet)
	//{
	//	// �^�C�}�[�̃J�E���g�_�E��
	//	m_sMove.fTimer -= fDeltaTime * fDeltaRate * fSlowRate;

	//	if (m_sMove.fTimer <= 0.0f)
	//	{
	//		// �s���̍\���̏�����
	//		ZeroMemory(&m_sMove, sizeof(m_sMove));

	//		// �s���^�C�v�F�Ȃ�
	//		m_eMoveType = EMoveTypeChatch::MOVETYPE_NONE;

	//		// �s���F�~�܂�
	//		m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
	//	}

	//	return;
	//}
	//else if (!m_sMove.bSet && m_eMoveType != EMoveTypeChatch::MOVETYPE_DISTANCE)
	//{// �ݒ肪�������Ă��Ȃ� && �s���F���� �ȊO�̎�

	//	// �s�����Ԃ̐ݒ�
	//	float fRand = (float)UtilFunc::Transformation::Random(MOVETYPE_LEFTRIGHT_TIME_MIN, MOVETYPE_LEFTRIGHT_TIME_MAX);
	//	m_sMove.fTimer = fRand * 0.1f;
	//}

	// �s���^�C�v�X�V
	(this->*(m_MoveTypeFunc[m_eMoveType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//--------------------------------------------------------------------------
// �s���F���������
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveTypeDistance(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	float fMin = 10000000.0f;

	// �������
	MyLib::Vector3 posMy = m_pAI->GetPosition();			// �ʒu���̎擾
	CGameManager::ETeamSide TeamMy = m_pAI->GetTeam();	// �����`�[��

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���
		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// �v���C���[�ʒu
		CPlayer::EFieldArea areaPlayer = pPlayer->GetAreaType();	// �G���A�^�C�v

		// �����̏ꍇ
		if (pPlayer == m_pAI) continue;

		// �����`�[���̊O��
		if (TeamMy == pPlayer->GetTeam() && areaPlayer == CPlayer::EFieldArea::FIELD_OUT) continue;

		// �G�Ƃ̋��������߂�
		float fLength = posMy.DistanceXZ(posPlayer);

		// �����F�擾
		float length = GetDistance(areaPlayer, TeamMy, pPlayer->GetTeam());

		if (fLength < length) 
		{

			// �s���F����
			m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

			// �����
			if (Leave(posPlayer, length))
			{
				m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
				m_eMoveType = EMoveTypeChatch::MOVETYPE_NONE;
			}

			return;

		}
		else {
			// �s���F�~�܂�
			m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
			// �s���^�C�v�F�������Ⴑ������
			//m_eMoveType = EMoveTypeChatch::MOVETYPE_RANDOM;
		}

	}
}

//--------------------------------------------------------------------------
// �s���F�������Ⴑ������
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveTypeAtyakotya(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sMove.bSet) return;

	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �s�����Ԃ̐ݒ�
	float fRand = (float)UtilFunc::Transformation::Random(MOVETYPE_ATYAKOTYA_TIME_MIN, MOVETYPE_ATYAKOTYA_TIME_MAX);
	m_sMove.fTimer = fRand * 0.1f;

	// �i�s���������߂�
	int randRot = UtilFunc::Transformation::Random(-316, 316);
	float randRotDest = randRot * 0.01f;

	// �J�j�i�s�����̐ݒ�
	pControlAIMove->SetClabDirection(randRotDest);

	// �s���F����
	m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

	// �ݒ肵�܂����[�I
	m_sMove.bSet = true;
}

//--------------------------------------------------------------------------
// �s���F��
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveTypeLeft(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[��������̎擾
	CPlayer* pTarget = GetBallOwner();
	if (!pTarget)
	{
		// �s���̍\���̏�����
		ZeroMemory(&m_sMove, sizeof(m_sMove));

		// �s���^�C�v�F�Ȃ�
		m_eMoveType = EMoveTypeChatch::MOVETYPE_NONE;

		return;
	}

	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	float rotDest = 0.0f;

	if (pTarget) {// �^�[�Q�b�g������ꍇ

		// �i�s���������߂�
		float rotDest = m_pAI->GetPosition().AngleXZ(pTarget->GetPosition());
		// �p�x�̐ݒ�
		rotDest = rotDest + (-D3DX_PI * 0.5f);
		// �p�x�m�[�}���C�Y
		UtilFunc::Transformation::RotNormalize(rotDest);

		// �J�j�i�s�����̐ݒ�
		pControlAIMove->SetClabDirection(rotDest);
	} 
	//else {
	//	// �i�s���������߂�
	//	//rotDest = 
	//	
	//	// �p�x�̐ݒ�
	//	m_pAI->SetRotDest(rotDest);
	//}
}

//--------------------------------------------------------------------------
// �s���F�E
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveTypeRight(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[��������̎擾
	CPlayer* pTarget = GetBallOwner();
	if (!pTarget)
	{
		// �s���̍\���̏�����
		ZeroMemory(&m_sMove, sizeof(m_sMove));

		// �s���^�C�v�F�Ȃ�
		m_eMoveType = EMoveTypeChatch::MOVETYPE_NONE;

		return;
	}

	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �i�s���������߂�
	float rotDest = m_pAI->GetPosition().AngleXZ(pTarget->GetPosition());

	// �p�x�̐ݒ�
	rotDest = rotDest + (D3DX_PI * 0.5f);
	// �p�x�m�[�}���C�Y
	UtilFunc::Transformation::RotNormalize(rotDest);

	// �J�j�i�s�����̐ݒ�
	pControlAIMove->SetClabDirection(rotDest);
}

//--------------------------------------------------------------------------
// �s���F��
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveTypeUp(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[��������̎擾
	CPlayer* pTarget = GetBallOwner();
	if (!pTarget)
	{
		// �s���̍\���̏�����
		ZeroMemory(&m_sMove, sizeof(m_sMove));

		// �s���^�C�v�F�Ȃ�
		m_eMoveType = EMoveTypeChatch::MOVETYPE_NONE;

		return;
	}

	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �i�s���������߂�
	float rotDest = m_pAI->GetPosition().AngleXZ(pTarget->GetPosition());
	
	// �J�j�i�s�����̐ݒ�
	pControlAIMove->SetClabDirection(rotDest);
}

//--------------------------------------------------------------------------
// �s���F��
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveTypeDown(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[��������̎擾
	CPlayer* pTarget = GetBallOwner();
	if (!pTarget)
	{
		// �s���̍\���̏�����
		ZeroMemory(&m_sMove, sizeof(m_sMove));

		// �s���^�C�v�F�Ȃ�
		m_eMoveType = EMoveTypeChatch::MOVETYPE_NONE;

		return;
	}

	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �i�s���������߂�
	float rotDest = m_pAI->GetPosition().AngleXZ(pTarget->GetPosition());
	UtilFunc::Transformation::RotNormalize(rotDest);

	// �J�j�i�s�����̐ݒ�
	pControlAIMove->SetClabDirection(rotDest);
}

//================================================================================
// �A�N�V�����̍X�V����
//================================================================================
void CPlayerAIControl::UpdateActionFlag()
{
	// �A�N�V�����X�V
	(this->*(m_ActionFunc[m_eActionFlag]))();
}

//--------------------------------------------------------------------------
// �A�N�V�����F��
//--------------------------------------------------------------------------
void CPlayerAIControl::ActionNone()
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
// �A�N�V�����F�W�����v
//--------------------------------------------------------------------------
void CPlayerAIControl::ActionJump()
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

	m_eThrow = EThrowFlag::THROW_NORMAL;
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
	m_eThrow = EThrowFlag::THROW_SPECIAL;
}

//================================================================================
// �����s���̍X�V����
//================================================================================
void CPlayerAIControl::UpdateThrowMove()
{

}

//--------------------------------------------------------------------------
// ���̏꓊��
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowMoveNone()
{
	// �^�C�~���O�͂ǂ�����H

	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �������Z�b�g
	pControlAIMove->SetIsWalk(false);

	// ������
	pControlAIAction->SetIsThrow(true);

	m_eThrowType = EThrowType::THROWTYPE_NONE;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;
}

//--------------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowMoveWalk()
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
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;
}

//--------------------------------------------------------------------------
// ���蓊��
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowMoveDash()
{
	if (IsLineOverPlayer())
	{// ���𒴂��Ă����ꍇ(���l�͌Œ�ϐ�)
		m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
		return;
	}

	// �^�C�~���O�͂ǂ�����H
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;
}

//================================================================================
// �����̍X�V����
//================================================================================
void CPlayerAIControl::UpdateThrowFlag()
{
	// �����X�V
	(this->*(m_ThrowFunc[m_eThrow]))();
}

//--------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------
void CPlayerAIControl::Throw()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// ������
	pControlAIAction->SetIsThrow(true);

	// ������ԁF��
	m_eThrow = EThrowFlag::THROW_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
}

//--------------------------------------------------------------------------
// �p�X
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowPass()
{

}

//--------------------------------------------------------------------------
// �X�y�V����
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowSpecial()
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// �X�y�V��������
	pControlAIAction->SetIsSpecial(true);

	// �����Ȃ�
	m_eThrow = EThrowFlag::THROW_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
}

//================================================================================
// �^�C�~���O�̍X�V����
//================================================================================
void CPlayerAIControl::UpdateThrowTiming(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԍX�V
	(this->*(m_ThrowTimingFunc[m_eThrowTiming]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//--------------------------------------------------------------------------
// �����^�C�~���O�F(�ʏ�)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �~�܂�
	m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;

	// ������
	m_eThrow = EThrowFlag::THROW_NORMAL;
}

//--------------------------------------------------------------------------
// �����^�C�~���O�F(����)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sThrow.fTiming > 0.0f)
	{
		m_sThrow.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// �~�܂�
	m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;

	// ������
	m_eThrow = EThrowFlag::THROW_NORMAL;
}

//--------------------------------------------------------------------------
// �����^�C�~���O�F(�x��)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sThrow.fTiming > 0.0f)
	{
		m_sThrow.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// �~�܂�
	m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;

	// ������
	m_eThrow = EThrowFlag::THROW_NORMAL;
}

//--------------------------------------------------------------------------
// ���ѓ����^�C�~���O�F(�ʏ�)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingJumpNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
	m_eActionFlag = EActionFlag::ACTION_JUMP;

	if (m_pAI->GetPosition().y >= playerAIcontrol::THROW_JUMP_END)	// �����ɂ���ĕς��
	{
		m_eThrow = EThrowFlag::THROW_NORMAL;	// ������
	}
}

//--------------------------------------------------------------------------
// ���ѓ����^�C�~���O�F(����)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingJumpQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �ǂꂭ�炢�̋����ړ����邩������ǉ�


	pControlAIMove->SetIsWalk(false);	// �������Z�b�g
	pControlAIAction->SetIsJump(true);	// �W�����v�I��

	if (m_pAI->GetPosition().y >= playerAIcontrol::THROW_JUMP_END * 0.5f)
	{
		// ������
		pControlAIAction->SetIsThrow(true);

		// �ϐ����Z�b�g
		//ResetFlag();
	}
}

//--------------------------------------------------------------------------
// ���ѓ����^�C�~���O�F(�x��)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingJumpDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �ǂꂭ�炢�̋����ړ����邩������ǉ�
	m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
	pControlAIAction->SetIsJump(true);	// �W�����v�I��

	// �ʒu���擾
	MyLib::Vector3 pos = m_pAI->GetPosition();

	if (pos.y >= playerAIcontrol::THROW_JUMP_END)	// �����ɂ���ĕς��
	{
		m_sThrow.bFoldJump = true;	// �܂�Ԃ��I��
	}

	if (!m_sThrow.bFoldJump) { return; }

	if (pos.y <= playerAIcontrol::THROW_JUMP_END * 0.5f)	// �����ɂ���ĕς��
	{
		// ������
		pControlAIAction->SetIsThrow(true);

		// ���ꂼ��̏�Ԃ̃��Z�b�g
		//ResetFlag();
	}
}

//================================================================================
// �L���b�`�̍X�V����
//================================================================================
void CPlayerAIControl::UpdateCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// �{�[���˂���

	// ��������擾
	CPlayer* pPlayer = pBall->GetPlayer();

	if (pPlayer != nullptr)
	{// �N�����{�[���������Ă���

		if (IsPassTarget())
		{// �p�X�������ɗ���ꍇ
			// �L���b�`�F�p�X�҂�
			m_eCatchType = ECatchType::CATCH_TYPE_PASS_GIVE;
		}
		else
		{
			// �L���b�`�F�ʏ�
			m_eCatchType = ECatchType::CATCH_TYPE_NORMAL;
		}
	}
	else
	{// �N���{�[���������Ă��Ȃ��ꍇ

		// �{�[���X�e�[�g�擾
		CBall::EState stateBall = pBall->GetState();

		if (stateBall == CBall::EState::STATE_PASS ||
			stateBall == CBall::EState::STATE_HOM_PASS ||
			stateBall == CBall::EState::STATE_MOVE)
		{// �p�X||�z�[�~���O�p�X||�ړ���Ԃ̏ꍇ

			// �L���b�`�F�D��
			m_eCatchType = ECatchType::CATCH_TYPE_PASS_STEAL;
		}
		else if (IsWhoPicksUpTheBall() && IsLineOverBall())
		{// ���������{�[���ɋ߂��l������&&�{�[�������葤�ɂ���	�܂葊�葤�Ƀ{�[���������Ă���

			// �L���b�`�F�ʏ�
			m_eCatchType = ECatchType::CATCH_TYPE_NORMAL;
		}
		else 
		{
			// �L���b�`�F���ɍs��
			m_eCatchType = ECatchType::CATCH_TYPE_FIND;
		}
	}

	// �L���b�`��ލX�V
	(this->*(m_CatchFunc[m_eCatchType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//--------------------------------------------------------------------------
// �ʏ�L���b�`
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �p�X�^�[�Q�b�g�������̏ꍇ
	//if (IsPassTarget()) return;

	// �{�[������������擾
	CPlayer* pTarget = GetBallOwner();
	if (!pTarget) { return; }

	// �{�[��������Ƃ̋���
	float distance = GetDistanceBallowner();

	if (distance < CHATCH_LENGTH_TARGET)
	{// ������Ƌ������߂��ꍇ
		m_eMoveType = EMoveTypeChatch::MOVETYPE_DISTANCE;
	}

	//m_eMoveType = EMoveTypeChatch::MOVETYPE_RANDOM;

	// �s���^�C�v�̍X�V
	UpdateMoveType(fDeltaTime, fDeltaRate, fSlowRate);	
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
	// �s���F����
	m_eMoveFlag = EMoveFlag::MOVEFLAG_DASH;
}

//--------------------------------------------------------------------------
// �L���b�`�F�p�X��Ⴄ
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchPassGive(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	if (m_pAI->GetBall()) return;

	// ��������{�[���̈ʒu������
	float rot = m_pAI->GetPosition().AngleXZ(pBall->GetPosition());

	// �ݒ�
	m_pAI->SetRotDest(rot);

	// �s����ށF�~�܂�
	m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
}

//--------------------------------------------------------------------------
// �L���b�`�F�p�X��D��
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchPassSteal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
			m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
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
		m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
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
	else{
		// �s���F����
		m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;
	}

	// �߂Â�
	if (Approatch(pBall->GetPosition(), 50.0f))
	{// �߂Â����ꍇ
		// �s���F�~�܂�
		m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
	}
}



//==========================================================================
// �S
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
		//m_eHeart = EHeart::HEART_NORMAL;
	}
}


//==========================================================================
// �����F�擾
//==========================================================================
float CPlayerAIControl::GetDistance(CPlayer::EFieldArea area, CGameManager::ETeamSide teamMy, CGameManager::ETeamSide teamPair)
{
	float distance = 0.0f;

	// �̗͂ɂ���ĕς�����
	// �S�ɂ���ĕς�����

	if (area == CPlayer::EFieldArea::FIELD_IN && teamMy == teamPair)
	{// ����œ����`�[��
		distance = m_sDistance.fInAlly;
	}
	else if (area == CPlayer::EFieldArea::FIELD_IN && teamMy != teamPair)
	{// ����ňႤ�`�[��
		distance = m_sDistance.fInPair;
	}
	else if (area == CPlayer::EFieldArea::FIELD_OUT)
	{// �O��
		distance = m_sDistance.fOut;
	}

	return distance;
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

//================================================================================
// ����̍X�V����
//================================================================================
void CPlayerAIControl::UpdateSee()
{
	if (m_eMode == EMode::MODE_CATCH)
	{// �{�[��������

		if (m_eSee != ESee::SEE_BALL) return;

		SeeBall();

	}
	else if (m_eMode == EMode::MODE_THROW)
	{// �^�[�Q�b�g������

		if (m_eSee != ESee::SEE_TARGET) return;

		CPlayer* pTarget = GetThrowTarget();
		if (!pTarget) return;

		SeeTarget(pTarget->GetPosition());
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

	// �{�[�����̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	// �����̈ʒu���
	MyLib::Vector3 myPos = m_pAI->GetPosition();

	// ����Ǝ����̋���
	float length = myPos.DistanceXZ(targetPos);	// �����F����Ǝ���

	if (distance < length) {// �ړ��^�C�v�F��
		return true;
	}

	// �^�[�Q�b�g���玩���̊p�x
	float direction = targetPos.AngleXZ(myPos);

	if (pBall)
	{// �{�[�������݂��Ȃ�
		CPlayer* pPlayerBall = pBall->GetPlayer();

		if (!pPlayerBall)
		{// �v���C���[�����Ȃ�
			// �p�x�ݒ�
			m_pAI->SetRotDest(direction);
			return false;

		}
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
	MyLib::Vector3 myPos = m_pAI->GetPosition();

	// ����Ǝ����̋���
	float length = myPos.DistanceXZ(targetPos);	// �����F����Ǝ���

	if (distance > length) {// �ړ��^�C�v�F��
		return true;
	}

	// ��������^�[�Q�b�g�̊p�x
	float direction = myPos.AngleXZ(targetPos);

	if (pBall)
	{// �{�[�������݂��Ȃ�
		CPlayer* pPlayerBall = pBall->GetPlayer();

		if (!pPlayerBall) {// �v���C���[�����Ȃ�

			// �p�x�ݒ�
			m_pAI->SetRotDest(direction);

			return false;

		} if (pPlayerBall->GetTeam() != m_pAI->GetTeam() &&
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





//==========================================================================
// �p�����[�^�[
//==========================================================================
void CPlayerAIControl::Parameter()
{
	// �ŏI�͂����ł����Ă��S���o����悤��
	// �����̏㉺�ł��

	// �s��


	// ����



	// �X�y�V����
}

//==========================================================================
// �s�����Ԃ̐ݒ�
//==========================================================================
void CPlayerAIControl::SetMoveTimer(float timer)
{
	// �s�����Ԃ̐ݒ�
	float fRand = (float)UtilFunc::Transformation::Random(MOVETYPE_LEFTRIGHT_TIME_MIN, (int)timer);
	m_sMove.fTimer = fRand * 0.1f;
}
