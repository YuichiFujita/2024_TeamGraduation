//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playerAIcontrol_defense.h"
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

#include "playerAIcontrol_leftAttack.h"
#include "playerAIcontrol_leftDefense.h"
#include "playerAIcontrol_rightAttack.h"
#include "playerAIcontrol_rightDefense.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	// �s���^�C�v
	const int MOVETYPE_WAIT_MODE_MAX = 10;		// �ҋ@�ɓ���m���� (�Œ��0�Œ�)
	const int MOVETYPE_WAIT_MODE_IN = 6;		// �ҋ@�ɓ���m��(���̐��l�ȉ��̏ꍇ�A�ҋ@���)
	const int MOVETYPE_WAIT_TIME_MAX = 20;		// �ő�F�ҋ@����
	const int MOVETYPE_WAIT_TIME_MIN = 10;		// �Œ�F
	const int MOVETYPE_ATYAKOTYA_TIME_MAX = 10;		// �ő�F�������Ⴑ�����᎞��
	const int MOVETYPE_ATYAKOTYA_TIME_MIN = 5;		// �Œ�F
	const int MOVETYPE_LEFTRIGHT_TIME_MAX = 10;		// �ő�F���E�ړ�����
	const int MOVETYPE_LEFTRIGHT_TIME_MIN = 5;		// �Œ�F
	const int MOVETYPE_UPDOWN_TIME_MAX = 10;		// �ő�F�㉺�ړ�����
	const int MOVETYPE_UPDOWN_TIME_MIN = 5;		// �Œ�F

	// �p�X
	const float STEAL_CANCEL_LENGTH = 100.0f;	// ������߂鋗��

	const float MOTIVATION_MAX = 100.0f;		// ���`�x�[�V����(MAX)

	// ������(�f�t�H���g)
	const float LENGTH_TARGET = 400.0f;		// �^�[�Q�b�g�Ƃ̋���(�f�t�H���g)
	const float LENGTH_VALUE = 100.0f;

	const float BALL_DISTANCE = 300.0f;			// 

	const float CATCH_JUMP_LENGTH = 100.0f;
	const float CATCH_JUMP_HEIGHT = 140.0f;

	const float JUMP_RATE = 1.0f;				// �W�����v�̊���(����)
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CPlayerAIControlDefense::ACTION_FUNC CPlayerAIControlDefense::m_ActionFunc[] =	// �L���b�`�֐�
{
	&CPlayerAIControlDefense::MoveIdle,				// �Ȃ�
	&CPlayerAIControlDefense::MoveDodge,				// ���
	&CPlayerAIControlDefense::MoveSupport,				// �T�|�[�g
	&CPlayerAIControlDefense::MoveChaseBall,			// �{�[����ǂ�������
	&CPlayerAIControlDefense::MoveRetreat,				// ���
	&CPlayerAIControlDefense::MoveRandom,				// �����_��
	&CPlayerAIControlDefense::MoveLeave,				// �����
};


//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControlDefense::CPlayerAIControlDefense()
{

}

//==========================================================================
// ����
//==========================================================================
CPlayerAIControlDefense* CPlayerAIControlDefense::Create(CPlayer* player)
{
	// �������̊m��
	CPlayerAIControlDefense* pObj = nullptr;

	if (!player) {
		assert(("�v���C���[��񂪖�����", false));
	}
	switch (player->GetTeam())
	{// �`�[����
	case CGameManager::ETeamSide::SIDE_LEFT:
		pObj = DEBUG_NEW CPlayerAIControlLeftDefense;
		break;

	case CGameManager::ETeamSide::SIDE_RIGHT:
		pObj = DEBUG_NEW CPlayerAIControlRightDefense;
		break;

	default:
		assert(("�`�[����������", false));
		break;
	}

	if (pObj != nullptr)
	{
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
HRESULT CPlayerAIControlDefense::Init()
{
	// �e�N���X�̏�����
	CPlayerAIControlMode::Init();

	// �񋓂̏�����
	m_eAction = EAction::IDLE;
	m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;

	// �\���̂̏�����
	ZeroMemory(&m_sAction, sizeof(m_sAction));

	// �ϐ��̏�����
	m_fDistanse = 200.0f;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerAIControlDefense::Uninit()
{
	delete this;

	// �e�N���X�̏I������
	CPlayerAIControlMode::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIControlDefense::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;
	CPlayer* pPlayer = pBall->GetPlayer();

	if (pBall->GetTarget() == GetPlayer())
	{// �{�[���̃^�[�Q�b�g�������̏ꍇ
		return;
	}

	if (pPlayer)
	{// �������{�[���������Ă���ꍇ
		TeammateBall(fDeltaTime, fDeltaRate, fSlowRate);
	}
	else
	{// �G���{�[���������Ă���ꍇ
		TeamEnemyBall(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// �e�N���X�̍X�V�i�Ō���ɐݒu�j
	CPlayerAIControlMode::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// �`�[�����C�g�{�[��
//================================================================================
void CPlayerAIControlDefense::TeammateBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;
	CPlayer* pPlayer = pBall->GetPlayer();

	// �`�[��
	CGameManager::ETeamSide side = pPlayer->GetTeam();
	CGameManager::ETeamSide side1 = GetPlayer()->GetTeam();

	switch (pPlayer->GetAreaType())
	{// �G���A��

	case CPlayer::EFieldArea::FIELD_IN:	// ����

		if (side == side1)
		{// �����`�[��
			MoveRandom();
		}
		else
		{
			// ���̍X�V
			UpdateDefense(fDeltaTime, fDeltaRate, fSlowRate);
		}

		break;

	case CPlayer::EFieldArea::FIELD_OUT:	// �O��

		MoveRandom();

		break;

	default:
		break;
	}
}

//================================================================================
// ����`�[���{�[��
//================================================================================
void CPlayerAIControlDefense::TeamEnemyBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!IsLineOverBall())
	{
		// �{�[����ǂ�
		m_eAction = EAction::CHASE_BALL;
	}

	UpdateDefense(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// ���̍X�V����
//================================================================================
void CPlayerAIControlDefense::UpdateDefense(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// �{�[���˂���
	CBall::EState stateBall = pBall->GetState();	// �{�[����Ԏ擾

	// ��������擾
	CPlayer* pPlayer = GetBallOwner();

	switch (m_eActionStatus)
	{
	case EActionStatus::ACTIONSTATUS_IDLE:		// �ҋ@

		if (pPlayer)
		{// �{�[�������҂�����
			Action0();
		}
		else
		{// ���Ȃ�
			Action1();
		}

		if (m_eAction != EAction::IDLE)
		{
			// �A�N�V������ԁF�A�N�V����
			m_eActionStatus = EActionStatus::ACTIONSTATUS_ACTION;
		}

		break;

	case EActionStatus::ACTIONSTATUS_ACTION:		// �s��

		if (pBall->GetTarget() == GetPlayer() &&	// �^�[�Q�b�g������
			m_eAction == EAction::RNDOM)			// �s���������_��
		{
			m_eAction = EAction::LEAVE;

			// �s���̍\���̏�����
			ZeroMemory(&m_sAction, sizeof(m_sAction));

			break;
		}

		// �A�N�V������ލX�V
		(this->*(m_ActionFunc[m_eAction]))();

		break;

	case EActionStatus::ACTIONSTATUS_COOLDOWN:	// �N�[���_�E��

		if (IsCancel())
		{
			// �A�N�V������ԁF�A�N�V����
			m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;

			// �s���̍\���̏�����
			ZeroMemory(&m_sAction, sizeof(m_sAction));

			return;
		}

		// �A�N�V�����^�C�}�[�̐ݒ�
		SetActionTimer(1, 5);

		// �X�V�F�A�N�V�����^�C�}�[
		UpdateActionTimer(fDeltaTime, fDeltaRate, fSlowRate);

		break;

	default:
		assert(false);
		break;
	}
}

//================================================================================
// �A�N�V��������
//================================================================================
void CPlayerAIControlDefense::Action0()
{
	// ��������擾
	CPlayer* pPlayer = GetBallOwner();

	CPlayer::EFieldArea area = pPlayer->GetAreaType();

	if (area == CPlayer::EFieldArea::FIELD_IN)
	{// �{�[�������傪����̏ꍇ
		m_eAction = EAction::RNDOM;
	}
	else
	{
		m_eAction = EAction::LEAVE;
	}
}

//================================================================================
// �A�N�V��������
//================================================================================
void CPlayerAIControlDefense::Action1()
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// �{�[���˂���
	CBall::EState stateBall = pBall->GetState();	// �{�[����Ԏ擾

	if (IsPassTarget() && stateBall == CBall::EState::STATE_PASS)
	{// �p�X�������ɗ���&&�{�[�����p�X��Ԃ̍�
		// �������Ȃ�
		m_eAction = EAction::IDLE;
	}

	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	if (pBall->GetTarget() == pAI)
	{// �^�[�Q�b�g������
		float distanceBall = GetDistanceBall();		// �{�[���Ƃ̋���

		if (distanceBall > 300.0f) {
			// ���(���S�n�т�)
			m_eAction = EAction::RNDOM;
		}
		else {

			m_eAction = EAction::IDLE;
		}
	}
}

//--------------------------------------------------------------------------
// �{�[����ǂ�������
//--------------------------------------------------------------------------
void CPlayerAIControlDefense::MoveChaseBall()
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// �{�[���˂���

	// �{�[����Ԏ擾
	CBall::EState stateBall = pBall->GetState();

	if (stateBall == CBall::EState::STATE_PASS ||
		stateBall == CBall::EState::STATE_HOM_PASS)
	{// �p�X||�z�[�~���O�p�X�̏ꍇ

		// �{�[����D��
		BallSteal();

		return;
	}
	else if (!IsLineOverBall())
	{// �����z���Ă��Ȃ��ꍇ

		if (!IsPicksUpBall())
		{// �������{�[���ɋ߂��ꍇ

			// �L���b�`�F���ɍs��
			BallChase();

			return;
		}
	}

	// �A�N�V������ԁF�Ȃ�
	m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;
}

//==========================================================================
// �{�[����D��
//==========================================================================
void CPlayerAIControlDefense::BallSteal()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// �����`�[���̏ꍇ
	//if (pBall->GetTypeTeam() == pAI->GetTeam()) return;

	// �ʒu�̎擾
	MyLib::Vector3 posBall = pBall->GetPosition();		// �{�[��
	MyLib::Vector3 posEnd = pBall->GetPosPassEnd();		// �{�[���̃p�X�I���ʒu
	MyLib::Vector3 posMy = pAI->GetPosition();		// �����̈ʒu

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
			SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
			m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;
			return;
		}
	}
	
	// �{�[���Ƃ̋���
	float distance = posMy.DistanceXZ(posBall);

	// �s����ԁF����
	SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

	// �{�[���̕��֍s��
	if (Approatch(pos, CATCH_JUMP_LENGTH) || distance < CATCH_JUMP_LENGTH)
	{// �I���ʒu�ɋ߂Â���||�{�[���Ƃ̋������͈͓��̏ꍇ

		if (posBall.y < CATCH_JUMP_HEIGHT)
		{// ��ꂻ���ȍ����ɗ����I
			SetActionFlag(EActionFlag::ACTION_JUMP);
		}

		m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;
	}
}

//==========================================================================
// �{�[����ǂ�
//==========================================================================
void CPlayerAIControlDefense::BallChase()
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (pBall == nullptr || pBall->GetPlayer() != nullptr)
	{// �{�[����nullptr&&�v���C���[���{�[��������Ă���ꍇ
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
		return;
	}

	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// �{�[���ʒu
	MyLib::Vector3 posBall = pBall->GetPosition();

	// ��������{�[���̋���
	float distance = pAI->GetPosition().DistanceXZ(posBall);

	if (distance > BALL_DISTANCE) {
		// �s���F����
		SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);
	}
	else {
		// �s���F����
		SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);
	}

	// �߂Â�
	if (Approatch(pBall->GetPosition(), 30.0f))
	{// �߂Â����ꍇ
		// �s���F�~�܂�
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
	}
}

//==========================================================================
// �s���֘A�̏���
//==========================================================================

//--------------------------------------------------------------------------
// �������Ȃ�
//--------------------------------------------------------------------------
void CPlayerAIControlDefense::MoveIdle()
{
	// �s���t���O�F�Ȃ�
	SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);

	// �A�N�V������ԁF�Ȃ�
	m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;
}

//--------------------------------------------------------------------------
// ���
//--------------------------------------------------------------------------
void CPlayerAIControlDefense::MoveDodge()
{
	// �{�[���Ƃ̋���
	float distanceBall = GetDistanceBall();

	// ����������Ă���ꍇ
	if (distanceBall > 5.0f) return;

	// �s���F�u�����N
	//m_eMoveFlag = EMoveFlag::MOVEFLAG_BLINK;
}

//--------------------------------------------------------------------------
// �T�|�[�g
//--------------------------------------------------------------------------
void CPlayerAIControlDefense::MoveSupport()
{
	//// �̗͂̏��Ȃ��������J�o�[
	//int nLife = pAI->GetLife();

	//if (!m_sMove.pDefenseTarget)
	//{
	//	int nMinLife = 100000000;

	//	// �������
	//	MyLib::Vector3 posMy = pAI->GetPosition();		// �ʒu���̎擾
	//	CGameManager::ETeamSide TeamMy = pAI->GetTeam();	// �����`�[��

	//	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	//	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	//	while (list.ListLoop(itr))
	//	{ // ���X�g���̗v�f�����J��Ԃ�

	//		CPlayer* pPlayer = (*itr);	// �v���C���[���

	//		// ����ȊO&&�����`�[������Ȃ�
	//		if (pPlayer->GetAreaType() != CPlayer::EFieldArea::FIELD_IN &&
	//			pPlayer->GetTeam() != pAI->GetTeam()) continue;

	//		// ����
	//		if (pPlayer == pAI) continue;

	//		// ���l�̗̑�
	//		int nLifeOther = pPlayer->GetLife();

	//		if (nLifeOther < nLife && nLifeOther < nMinLife)
	//		{// �����̗̑͂�菭�Ȃ�&&�`�[���̒��ň�ԑ̗͂����Ȃ��ꍇ

	//			// �ŏ��̗͂̍X�V
	//			nMinLife = nLifeOther;

	//			// ����Ώۂ̐ݒ�
	//			m_sMove.pDefenseTarget = pPlayer;
	//		}
	//	}

	//	return;
	//}


	// �����Ƃ̋��������(�W�I�𕪎U)

}

//--------------------------------------------------------------------------
// �����
//--------------------------------------------------------------------------
void CPlayerAIControlDefense::MoveLeave()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	CPlayer* pPlayer = pBall->GetPlayer();
	if (!pPlayer) return;

	// �~�܂�
	SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);

	// �����
	if (Leave(pPlayer->GetPosition(), 200.0f))
	{
		// �~�܂�
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);

		m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;
	}
}

//==========================================================================
// ��ړ�
//==========================================================================
void CPlayerAIControlDefense::MoveUp()
{
	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// �{�[��������̎擾
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{// �����傪����

		// AI�R���g���[�����̎擾
		CPlayerControlMove* pControlMove = pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		float rotDest = D3DX_PI;

		// �J�j�i�s�����̐ݒ�
		pControlAIMove->SetClabDirection(rotDest);
	}
	else
	{
		// �J�j�i�s�����̐ݒ�
		float rotDest = D3DX_PI;
		pAI->SetRotDest(rotDest);
	}
}

//==========================================================================
// ���ړ�
//==========================================================================
void CPlayerAIControlDefense::MoveDown()
{
	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// �{�[��������̎擾
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{
		// AI�R���g���[�����̎擾
		CPlayerControlMove* pControlMove = pAI->GetBase()->GetPlayerControlMove();
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
		pAI->SetRotDest(rotDest);
	}
}

//==========================================================================
// ���ړ�
//==========================================================================
void CPlayerAIControlDefense::MoveLeft()
{
	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// �{�[��������̎擾
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{
		// AI�R���g���[�����̎擾
		CPlayerControlMove* pControlMove = pAI->GetBase()->GetPlayerControlMove();
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
		pAI->SetRotDest(rotDest);
	}
}

//==========================================================================
// �E�ړ�
//==========================================================================
void CPlayerAIControlDefense::MoveRight()
{
	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// �{�[��������̎擾
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{
		// AI�R���g���[�����̎擾
		CPlayerControlMove* pControlMove = pAI->GetBase()->GetPlayerControlMove();
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
		pAI->SetRotDest(rotDest);
	}
}

//==========================================================================
// �X�V�F�A�N�V�����^�C�}�[
//==========================================================================
void CPlayerAIControlDefense::UpdateActionTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sAction.bSet)
	{
		// �^�C�}�[�̃J�E���g�_�E��
		m_sAction.fTimer -= fDeltaTime * fDeltaRate * fSlowRate;

		if (m_sAction.fTimer <= 0.0f)
		{
			// �s���̍\���̏�����
			ZeroMemory(&m_sAction, sizeof(m_sAction));

			// �A�N�V�����F�Ȃ�
			m_eAction = EAction::IDLE;

			// �A�N�V������ԁF�Ȃ�
			m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;

			// �s���F�~�܂�
			SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
		}
	}
}

//==========================================================================
// �A�N�V�������Ԃ̐ݒ�
//==========================================================================
void CPlayerAIControlDefense::SetActionTimer(int nMin, int nMax)
{
	// �p�x�̐ݒ�
	if (!m_sAction.bSet)
	{// �ݒ肳��Ă��Ȃ��ꍇ

		// �s�����Ԃ̐ݒ�
		float fRand = (float)UtilFunc::Transformation::Random(nMin, nMax);
		m_sAction.fTimer = fRand;

		// ���Ԑݒ�ON
		m_sAction.bSet = true;
	}
}

//==========================================================================
// �s���L�����Z��
//==========================================================================
bool CPlayerAIControlDefense::IsCancel()
{
	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return false;	// �{�[���˂���
	CBall::EState stateBall = pBall->GetState();	// �{�[����Ԏ擾

	if (!IsLineOverBall() &&							// ���𒴂��Ă��Ȃ�
		!IsPicksUpBall() &&								// �������߂��v���C���[�����Ȃ�
		stateBall == CBall::EState::STATE_LAND)			// ���Ɏ������Ă���
	{
		return true;
	}

	// �����̏��擾
	CPlayer* pPlayer = GetPlayer();
	if (!pPlayer) return false;

	if (pBall->GetTarget() == pPlayer)
	{// �^�[�Q�b�g�������̏ꍇ
		return true;
	}

	return false;
}

//==========================================================================
// ����� : Leave(����鑊��A����鋗��)
//==========================================================================
bool CPlayerAIControlDefense::Leave(MyLib::Vector3 targetPos, float distance)
{
	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �����̈ʒu���
	MyLib::Vector3 posMy = pAI->GetPosition();

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
		if (pPlayerBall->GetTeam() != pAI->GetTeam() &&
			pPlayerBall != pAI) {// �Ⴄ�`�[���������Ă��� && �����傪�����ȊO�ꍇ

			if (GetMoveFlag() == EMoveFlag::MOVEFLAG_DASH)
			{// �s���F����
				// �p�x�ݒ�
				pAI->SetRotDest(direction);
				return false;
			}

			// �J�j�i�s�����̐ݒ�
			pControlAIMove->SetClabDirection(direction);
			return false;
		}
	}

	// �p�x�ݒ�
	pAI->SetRotDest(direction);

	return false;
}

//==========================================================================
// �߂Â� : Approatch(�߂Â�����A�߂Â�����)
//==========================================================================
bool CPlayerAIControlDefense::Approatch(MyLib::Vector3 targetPos, float distance)
{
	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	// AI�R���g���[�����̎擾
	CPlayerControlMove* pControlMove = pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// �{�[�����̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	// �����̈ʒu���
	MyLib::Vector3 posMy = pAI->GetPosition();

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
		if (pPlayerBall->GetTeam() != pAI->GetTeam() &&
			pPlayerBall != pAI) {// �Ⴄ�`�[���������Ă��� && �����傪�����ȊO�ꍇ

			if (GetMoveFlag() == EMoveFlag::MOVEFLAG_DASH)
			{// �s���F����
				// �p�x�ݒ�
				pAI->SetRotDest(direction);
				return false;
			}

			// �J�j�i�s�����̐ݒ�
			pControlAIMove->SetClabDirection(direction);
			return false;
		}
	}

	// �p�x�ݒ�
	pAI->SetRotDest(direction);

	return false;
}

//==========================================================================
// �X�V�F����
//==========================================================================
void CPlayerAIControlDefense::UpdateSee()
{
	//CBall* pBall = CGameManager::GetInstance()->GetBall();
	//if (!pBall) return;

	//CPlayer* pPlayer = pBall->GetPlayer();

	//if (pPlayer)
	//{
	//	// 
	//	SeeTarget(pPlayer->GetPosition());
	//}

	// �{�[��������
	//SeeBall();
}

//==========================================================================
// �^�[�Q�b�g������
//==========================================================================
void CPlayerAIControlDefense::SeeTarget(MyLib::Vector3 pos)
{
	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// �����ݒ�
	float angle = pAI->GetPosition().AngleXZ(pos);
	pAI->SetRotDest(angle);
}

//==========================================================================
// �{�[��������
//==========================================================================
void CPlayerAIControlDefense::SeeBall()
{
	// �{�[�����̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;
	
	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// �{�[���Ƃ̊p�x
	float angle = pAI->GetPosition().AngleXZ(pBall->GetPosition());

	// �p�x�̐ݒ�
	pAI->SetRotDest(angle);
}

//==========================================================================
// �{�[��������Ƃ̋������v�Z
//==========================================================================
float CPlayerAIControlDefense::GetDistanceBallowner()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return 0.0f;

	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return 0.0f;

	CPlayer* pPlayer = pBall->GetPlayer();
	if (!pPlayer) return 0.0f;
	if (pPlayer->GetTeam() == pAI->GetTeam()) return 0.0f;

	// �������v�Z
	float distance = pAI->GetPosition().DistanceXZ(pPlayer->GetPosition());

	// �v�Z���ʂ�Ԃ�
	return distance;
}

//==========================================================================
// �{�[��������̎擾�i�G�̏ꍇ�j
//==========================================================================
CPlayer* CPlayerAIControlDefense::GetBallOwner()
{
	CPlayer* pTarget = nullptr;	// �ڕW�^�[�Q�b�g

	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return pTarget;

	// �{�[���̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) { return pTarget; }
	CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// �`�[���^�C�v�̎擾

	// �{�[���������Ă���z�Ǝ����������`�[���̏ꍇ
	if (typeTeam == pAI->GetTeam()) return pTarget;

	// �^�[�Q�b�g�̐ݒ�
	pTarget = pBall->GetPlayer();
	if (!pTarget) return nullptr;

	return pTarget;
}

//==========================================================================
// �{�[���Ƃ̋���
//==========================================================================
float CPlayerAIControlDefense::GetDistanceBall()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return 0.0f;

	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return 0.0f;

	float distance = pAI->GetPosition().DistanceXZ(pBall->GetPosition());

	return distance;
}

//==========================================================================
// �G�Ƃ̋���
//==========================================================================
float CPlayerAIControlDefense::GetDistanceEnemy()
{
	float fMin = 10000000.0f;	// �ŏ��l�̕ۑ��p

	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	// �������
	MyLib::Vector3 posMy = pAI->GetPosition();		// �ʒu���̎擾
	CGameManager::ETeamSide TeamMy = pAI->GetTeam();	// �����`�[��

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�

		CPlayer* pPlayer = (*itr);	// �v���C���[���

		// ������&&�����`�[���ꍇ
		if (pPlayer == pAI && TeamMy == pPlayer->GetTeam()) continue;

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


// ���� ==========================================================================

//==========================================================================
// �p�X�̑���͎����ł����H
//==========================================================================
bool CPlayerAIControlDefense::IsPassTarget()
{
	// �{�[�����̎擾
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return false;

	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	bool b = pBall->IsPass();

	if (b)
	{
		return false;
	}

	// �{�[���p�X&&�^�[�Q�b�g������
	if (pBall->IsPass() && pBall->GetTarget() == pAI)
	{
		return true;
	}

	return false;
}

//==========================================================================
// �N���{�[�������ɍs���܂����H
//==========================================================================
bool CPlayerAIControlDefense::IsPicksUpBall()
{
	float fMyDis = 1000000.0f;	// �����̃{�[���Ƃ̋���

	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	// �����̏��擾
	CGameManager::ETeamSide typeTeam = pAI->GetTeam();	// �`�[���^�C�v
	MyLib::Vector3 Mypos = pAI->GetPosition();	// �ʒu���

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

		if (pPlayer == pAI) continue;		// �����̏ꍇ
		if (pPlayer->GetTeam() != typeTeam) continue;		// �`�[�����Ⴄ�ꍇ
		if ((pPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER) ||
			(pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT))
		{// �O��&&���[�U�[&&�����̏ꍇ
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
bool CPlayerAIControlDefense::IsDistanceBall()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return false;

	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	// �ʒu�̎擾
	MyLib::Vector3 posBall = pBall->GetPosition();		// �{�[��
	MyLib::Vector3 posMy = pAI->GetPosition();		// �����̈ʒu

	// �{�[���Ƃ̋���
	float distance = posMy.DistanceXZ(posBall);

	if (distance < 100.0f)
	{// �{�[���ƃp�X��̋������͈͓��Ȃ炠����߂�
		return true;
	}

	return false;
}
