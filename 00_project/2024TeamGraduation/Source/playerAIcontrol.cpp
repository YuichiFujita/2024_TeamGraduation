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
	&CPlayerAIControl::ModeNone,				// �Ȃ�
	&CPlayerAIControl::ModeIdle,				// �ҋ@
	&CPlayerAIControl::ModeThrow,				// ����
	&CPlayerAIControl::ModeCatch,				// �L���b�`
};

CPlayerAIControl::CATCH_FUNC CPlayerAIControl::m_CatchFunc[] =	// �L���b�`�֐�
{
	&CPlayerAIControl::CatchNone,				// �Ȃ�
	&CPlayerAIControl::CatchNormal,				// �ʏ�
	&CPlayerAIControl::CatchPassGive,			// �p�X�Ⴄ
	&CPlayerAIControl::CatchPassSteal,			// �p�X�D��
	&CPlayerAIControl::CatchFindBall,			// ������
};

CPlayerAIControl::MOVETYPE_FUNC CPlayerAIControl::m_MoveTypeFunc[] =	// �s���^�C�v�֐�
{
	&CPlayerAIControl::MoveTypeNone,			// �Ȃ�
	&CPlayerAIControl::MoveTypeDistance,		// ����
	& CPlayerAIControl::MoveTypeLeftRight,		// ����
	&CPlayerAIControl::MoveTypeAtyakotya,		// �����Ⴑ������
	&CPlayerAIControl::MoveTypeAvoid,			// ���
	&CPlayerAIControl::MoveTypeCrazy,			// ����
};


// �t���O�֘A

CPlayerAIControl::MOVEFLAG_FUNC CPlayerAIControl::m_MoveFlagFunc[] =	// �s���t���O�֐�
{
	&CPlayerAIControl::MoveFlagStop,			// �~�܂�
	&CPlayerAIControl::MoveFlagWalk,			// ����
	&CPlayerAIControl::MoveFlagBlink,			// ����
	&CPlayerAIControl::MoveFlagDash,			// ����
};

CPlayerAIControl::THROW_FUNC CPlayerAIControl::m_ThrowFlagFunc[] =	// �����t���O�֐�
{
	&CPlayerAIControl::ThrowFlagNone,			// �Ȃ�
	&CPlayerAIControl::ThrowFlag,				// ����
	&CPlayerAIControl::ThrowFlagPass,			// �p�X
	&CPlayerAIControl::ThrowFlagSpecial,		// �X�y�V����
};

CPlayerAIControl::ACTION_FUNC CPlayerAIControl::m_ActionFlagFunc[] =	// �A�N�V�����t���O�֐�
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
	m_eMode = EMode::MODE_NONE;
	//m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
	m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
	m_eMoveType = EMoveTypeChatch::MOVETYPE_NONE;
	m_eActionFlag = EActionFlag::ACTION_NONE;
	m_eThrow = EThrowFlag::THROW_NONE;
	m_eCatchType = ECatchType::CATCH_TYPE_NONE;

	// �\���̂̏�����
	ZeroMemory(&m_sThrow, sizeof(m_sThrow));
	ZeroMemory(&m_sMove, sizeof(m_sMove));
	ZeroMemory(&m_sMoveCount, sizeof(m_sMoveCount));
	ZeroMemory(&m_sDistance, sizeof(m_sDistance));
	ZeroMemory(&m_sParameter, sizeof(m_sParameter));
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
	m_eMoveType = EMoveTypeChatch::MOVETYPE_DISTANCE;
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
	//UpdateForcibly();
	
	{// �t���O�̍X�V

		// �X�V�F�s��
		UpdateMoveFlag(fDeltaTime, fDeltaRate, fSlowRate);
		// �X�V�F�A�N�V����
		UpdateActionFlag();
		// �X�V�F����
		UpdateThrowFlag();
	}

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
		m_eMode = EMode::MODE_NONE;
		return;
	}

	if (pBall->GetPlayer() == m_pAI)
	{// �{�[���������Ă���̂������������ꍇ
		m_eMode = EMode::MODE_THROW;	// ����
	}
	else if (pBall->GetPlayer() == nullptr && pBall->GetTypeTeam() != m_pAI->GetTeam())
	{// �{�[�����擾����Ă��Ȃ��ꍇ||�����ƃ{�[���������Ă���`�[�����Ⴄ�ꍇ
		m_eMode = EMode::MODE_CATCH;	// �L���b�`
	}
	else if (pBall->GetPlayer() == nullptr && pBall->GetTypeTeam() == m_pAI->GetTeam())
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
	//if (IsLineOverPlayer() && m_eForcibly != EMoveForcibly::FORCIBLY_START)
	//{// ���𒴂��Ă���&&�����s���F���߈ȊO�̏ꍇ
	//	m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
	//	return;
	//}

	// ���[�h�X�V
	(this->*(m_ModeFunc[m_eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���[�h�F����
//==========================================================================
void CPlayerAIControl::ModeThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}


//==========================================================================
// ���[�h�F�ҋ@
//==========================================================================
void CPlayerAIControl::ModeIdle(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �s���F�~�܂�
	m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;

	// �s���^�C�v�̍X�V
	UpdateMoveType(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// �s���^�C�v
//================================================================================
void CPlayerAIControl::UpdateMoveType(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_eMoveType == EMoveTypeChatch::MOVETYPE_NONE) return;

	// �s���^�C�}�[�̐ݒ�
	//SetMoveTimer(fDeltaTime, fDeltaRate, fSlowRate);
	
	// �s���^�C�v�X�V
	//(this->*(m_MoveTypeFunc[m_eMoveType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//--------------------------------------------------------------------------
// �s���F���������
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveTypeDistance(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayer* pTarget = GetBallOwner();
	if (!pTarget) return;

	MyLib::Vector3 posMy = m_pAI->GetPosition();
	MyLib::Vector3 posTarget = pTarget->GetPosition();

	// �����ƃ{�[��������(����)�̋���
	float distance = posMy.DistanceXZ(posTarget);

	if (distance < m_sDistance.fTarget)
	{// �����

		// �s���t���O�F����
		m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

		m_sMove.bReturn = false;

		if (Leave(posTarget, m_sDistance.fTarget))
		{
			m_sMove.bReturn = true;
			m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
		}
	}
}

//==========================================================================
// �s���F���E
//==========================================================================
void CPlayerAIControl::MoveTypeLeftRight(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	MyLib::Vector3 posMy = m_pAI->GetPosition();


	MyLib::Vector3 pos = { posMy.x, 0.0f, 560.0f };
	float distance = posMy.DistanceXZ(pos);

	if (distance < 100.0f)
	{
		m_sMove.bReturn = true;
	}

	MyLib::Vector3 pos1 = { posMy.x, 0.0f, -560.0f };
	float distance1 = posMy.DistanceXZ(pos1);

	if (distance1 < 100.0f)
	{
		m_sMove.bReturn = false;
	}

	if (m_sMove.bReturn)
	{
		MoveRight(GetBallOwner());
	}
	else
	{
		MoveLeft(GetBallOwner());
	}

	m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;
}

//==========================================================================
// �����F�擾
//==========================================================================
float CPlayerAIControl::GetDistance(CPlayer::EFieldArea area, CGameManager::ETeamSide teamMy, CPlayer* pPlayer)
{
	float distance = 0.0f;

	CGameManager::ETeamSide teamEnemy = pPlayer->GetTeam();


	if (area == CPlayer::EFieldArea::FIELD_IN && teamMy == teamEnemy)
	{// ����
		distance = m_sDistance.fInFriend;
		return distance;
	}
	else if (area == CPlayer::EFieldArea::FIELD_IN && teamMy != teamEnemy)
	{// �G
		distance = m_sDistance.fTarget;
		return distance;
	}
	else if (area == CPlayer::EFieldArea::FIELD_OUT)
	{// �O��
		if (pPlayer == GetBallOwner())
		{
			distance = m_sDistance.fTarget;
			return distance;
		}

		distance = m_sDistance.fOut;
	}

#if 0
	// �e�L�X�g�\��
	ImGui::Text("distance:%f", distance);
	ImGui::DragFloat("rateFriend", &m_sDistance.fInFriend, 1.0f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("rateEnemy", &m_sDistance.fInEnemy, 1.0f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("rateOut", &m_sDistance.fOut, 1.0f, 0.0f, 0.0f, "%.2f");
#endif
	return distance;
}

//--------------------------------------------------------------------------
// �s���F�������Ⴑ������
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveTypeAtyakotya(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�}�[�Z�b�g
	SetMoveTimer(fDeltaTime, fDeltaRate, fSlowRate);

	if (m_sMove.bSetMove) return;

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
	m_sMove.bSetMove = true;
}

//--------------------------------------------------------------------------
// �s���F���
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveTypeAvoid(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���������
	//MoveDistance();
}

//==========================================================================
// �L���b�`�̊Ǘ�
//==========================================================================
void CPlayerAIControl::ModeCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �L���b�`�̍X�V
	UpdateCatch(fDeltaTime, fDeltaRate, fSlowRate);
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
		else if (pBall->GetTarget() == m_pAI &&
				stateBall == CBall::EState::STATE_HOM_NOR ||
				stateBall == CBall::EState::STATE_HOM_JUMP)
		{// �^�[�Q�b�g������&&�{�[�����������
			m_eCatchType = ECatchType::CATCH_TYPE_NORMAL;
		}
		else if (!IsLineOverBall() && !IsWhoPicksUpTheBall())
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

	if (distance < m_sDistance.fTarget)
	{// ������Ƌ������߂��ꍇ
		m_eMoveType = EMoveTypeChatch::MOVETYPE_DISTANCE;
	}
	else
	{
		m_eMoveType = EMoveTypeChatch::MOVETYPE_RANDOM;
	}

	// �s���^�C�v�̍X�V
	UpdateMoveType(fDeltaTime, fDeltaRate, fSlowRate);
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
// ���������
//==========================================================================
void CPlayerAIControl::MoveDistance()
{
	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	float fMin = 10000000.0f;

	// �������
	MyLib::Vector3 posMy = m_pAI->GetPosition();		// �ʒu���̎擾
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
		float length = GetDistance(areaPlayer, TeamMy, pPlayer);

		if (fLength < length)
		{// �w�肵���������߂��ꍇ

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
		}

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
		m_sDistance.fInFriend	= LENGTH_FRIEND;
		m_sDistance.fOut		= LENGTH_OUT;
		m_sDistance.fTarget		= LENGTH_TARGET;
		break;

	case EHeartMain::HEART_MAIN_STRONG:	// ���C
		m_sDistance.fInFriend	= LENGTH_FRIEND;
		m_sDistance.fOut		= LENGTH_OUT - 50.0f;
		m_sDistance.fTarget		= LENGTH_TARGET - 100.0f;
		break;

	case EHeartMain::HEART_MAIN_TIMID:	// ��C
		m_sDistance.fInFriend	= LENGTH_FRIEND;
		m_sDistance.fOut		= LENGTH_OUT + 100.0f;
		m_sDistance.fTarget		= LENGTH_TARGET + 150.0f;
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
		m_sDistance.fTarget = m_sDistance.fTarget + (value);
		break;

	case EHeartMain::HEART_MAIN_STRONG:
		m_sDistance.fTarget = m_sDistance.fTarget + (value );
		break;

	case EHeartMain::HEART_MAIN_TIMID:
		m_sDistance.fTarget = m_sDistance.fTarget + (value );
		break;

	default:
		assert(false);
		break;

	}
}



//==========================================================================
// �s�����Ԃ̐ݒ�
//==========================================================================
void CPlayerAIControl::SetMoveTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �p�x�̐ݒ�
	if (m_sMoveCount.bSet)
	{
		// �^�C�}�[�̃J�E���g�_�E��
		m_sMoveCount.fTimer -= fDeltaTime * fDeltaRate * fSlowRate;

		if (m_sMoveCount.fTimer <= 0.0f)
		{
			// �s���̍\���̏�����
			ZeroMemory(&m_sMoveCount, sizeof(m_sMoveCount));

			// �s���^�C�v�F�Ȃ�
			m_eMoveType = EMoveTypeChatch::MOVETYPE_NONE;

			// �s���F�~�܂�
			m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
		}

		return;
	}
	else {// �ݒ肪�������Ă��Ȃ�
		// �s�����Ԃ̐ݒ�
		float fRand = (float)UtilFunc::Transformation::Random(MOVETYPE_LEFTRIGHT_TIME_MIN, MOVETYPE_LEFTRIGHT_TIME_MAX);
		m_sMoveCount.fTimer = fRand * 0.1f;

		// ���Ԑݒ�ON
		m_sMoveCount.bSet = true;
	}
}

//--------------------------------------------------------------------------
// �s���F��
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveLeft(CPlayer* pTarget)
{
	if (pTarget)
	{
		// AI�R���g���[�����̎擾
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// �i�s���������߂�
		float rotDest = m_pAI->GetPosition().AngleXZ(pTarget->GetPosition());
		// �p�x�̐ݒ�
		rotDest = rotDest + (-D3DX_PI * 0.5f);
		// �p�x�m�[�}���C�Y
		UtilFunc::Transformation::RotNormalize(rotDest);

		// �J�j�i�s�����̐ݒ�
		pControlAIMove->SetClabDirection(rotDest);

		// �s���ݒ�ON
		m_sMove.bSetMove = true;
	}
	else
	{
		float rot = D3DX_PI * 0.5f;
		m_pAI->SetRotDest(rot);

		// �s���ݒ�ON
		m_sMove.bSetMove = true;
	}
}

//--------------------------------------------------------------------------
// �s���F�E
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveRight(CPlayer* pTarget)
{
	if (pTarget)
	{
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

		// �s���ݒ�ON
		m_sMove.bSetMove = true;
	}
	else
	{
		float rot = -D3DX_PI * 0.5f;
		m_pAI->SetRotDest(rot);

		// �s���ݒ�ON
		m_sMove.bSetMove = true;
	}
}

//--------------------------------------------------------------------------
// �s���F����
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveFrontAndLeft(CPlayer* pPlayer)
{
	float rot = D3DX_PI * 0.75f;
	m_pAI->SetRotDest(rot);

	// �s���ݒ�ON
	m_sMove.bSetMove = true;
}

//--------------------------------------------------------------------------
// �s���F�E��
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveFrontAndRight(CPlayer* pPlayer)
{
	float rot = -D3DX_PI * 0.75f;
	m_pAI->SetRotDest(rot);

	// �s���ݒ�ON
	m_sMove.bSetMove = true;
}

//--------------------------------------------------------------------------
// �s���F����
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveDownAndLeft(CPlayer* pPlayer)
{
	float rot = D3DX_PI * 0.25f;
	m_pAI->SetRotDest(rot);

	// �s���ݒ�ON
	m_sMove.bSetMove = true;
}

//--------------------------------------------------------------------------
// �s���F�E��
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveDownAndRight(CPlayer* pPlayer)
{
	float rot = -D3DX_PI * 0.25f;
	m_pAI->SetRotDest(rot);

	// �s���ݒ�ON
	m_sMove.bSetMove = true;
}

//--------------------------------------------------------------------------
// �s���F��
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveFront(CPlayer* pTarget)
{
	if (pTarget)
	{
		// AI�R���g���[�����̎擾
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// �i�s���������߂�
		float rotDest = m_pAI->GetPosition().AngleXZ(pTarget->GetPosition());

		// �J�j�i�s�����̐ݒ�
		pControlAIMove->SetClabDirection(rotDest);

		// �s���ݒ�ON
		m_sMove.bSetMove = true;
	}
	else
	{
		float rot = D3DX_PI;
		m_pAI->SetRotDest(rot);

		// �s���ݒ�ON
		m_sMove.bSetMove = true;
	}
}

//--------------------------------------------------------------------------
// �s���F��
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveDown(CPlayer* pTarget)
{
	if (pTarget)
	{
		// AI�R���g���[�����̎擾
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// �i�s���������߂�
		float rotDest = m_pAI->GetPosition().AngleXZ(pTarget->GetPosition());
		UtilFunc::Transformation::RotNormalize(rotDest);

		// �J�j�i�s�����̐ݒ�
		pControlAIMove->SetClabDirection(rotDest);

		// �s���ݒ�ON
		m_sMove.bSetMove = true;
	}
	else
	{
		float rot = 0.0f;
		m_pAI->SetRotDest(rot);

		// �s���ݒ�ON
		m_sMove.bSetMove = true;
	}
}




// �t���O�ŊǗ� ===================================================================================================

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
	pControlAIMove->SetIsBlink(true);

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
// �A�N�V�����̍X�V����
//================================================================================
void CPlayerAIControl::UpdateActionFlag()
{
	// �A�N�V�����X�V
	(this->*(m_ActionFlagFunc[m_eActionFlag]))();
}

//--------------------------------------------------------------------------
// �A�N�V�����F��
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
// �A�N�V�����F�W�����v
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
// �����̍X�V����
//================================================================================
void CPlayerAIControl::UpdateThrowFlag()
{
	// �����X�V
	(this->*(m_ThrowFlagFunc[m_eThrow]))();
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
	m_eThrow = EThrowFlag::THROW_NONE;
	//m_eThrowType = EThrowType::THROWTYPE_NONE;
}

//--------------------------------------------------------------------------
// �p�X
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
	m_eThrow = EThrowFlag::THROW_NONE;
	//m_eThrowType = EThrowType::THROWTYPE_NONE;
}
