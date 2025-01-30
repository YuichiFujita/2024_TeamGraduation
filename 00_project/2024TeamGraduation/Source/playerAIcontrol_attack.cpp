//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playerAIcontrol_attack.h"
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
#include "playerAIcontrol.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol_move.h"
#include "playerManager.h"
#include "playerAIcontrol_rightAttack.h"
#include "playerAIcontrol_leftAttack.h"

//==========================================================================
// �֐��|�C���^
//==========================================================================
CPlayerAIControlAttack::ATTACKMODE_FUNC CPlayerAIControlAttack::m_AttackModeFunc[] =	// �U�����[�h�֐�
{
	&CPlayerAIControlAttack::AttackModePreparation,		// ����
	&CPlayerAIControlAttack::AttackModeAttack,			// �U��
};

CPlayerAIControlAttack::PREPARATION_FUNC CPlayerAIControlAttack::m_PreparationFunc[] =	// �U�����[�h�֐�
{
	&CPlayerAIControlAttack::PreparationNone,			// �Ȃ�
	&CPlayerAIControlAttack::PreparationWait,			// �҂�
	&CPlayerAIControlAttack::PreparationGo,				// �s��
	&CPlayerAIControlAttack::PreparationLeave,			// �����
};

CPlayerAIControlAttack::THROWTYPE_FUNC CPlayerAIControlAttack::m_ThrowTypeFunc[] =	// �����t���O�֐�
{
	&CPlayerAIControlAttack::ThrowTypeNone,				// �Ȃ�
	&CPlayerAIControlAttack::ThrowTypeNormal,			// �ʏ�
	&CPlayerAIControlAttack::ThrowTypeJump,				// �W�����v
	&CPlayerAIControlAttack::ThrowTypeSpecial,			// �X�y�V����
	&CPlayerAIControlAttack::ThrowTypePass,				// �p�X
};

CPlayerAIControlAttack::THROWFLAG_FUNC CPlayerAIControlAttack::m_ThrowFlagFunc[] =	// �����t���O�֐�
{
	&CPlayerAIControlAttack::ThrowFlagNone,				// �Ȃ�
	&CPlayerAIControlAttack::ThrowFlag,					// ����
	&CPlayerAIControlAttack::ThrowFlagPass,				// �p�X
	&CPlayerAIControlAttack::ThrowFlagSpecial,			// �X�y�V����
};

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	// ����
	const float DISTANSE_LINE = 400.0f;
	const float DISTANSE_TARGET = 100.0f;

	// �I�����a
	const float END_RADIUS = 30.0f;
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControlAttack::CPlayerAIControlAttack()
{

}

//==========================================================================
// ����
//==========================================================================
CPlayerAIControlAttack* CPlayerAIControlAttack::Create(CPlayer* player)
{
	// �������̊m��
	CPlayerAIControlAttack* pObj = nullptr;

	if (!player) {
		assert(("�v���C���[��񂪖�����", false));
	}

	CGameManager::ETeamSide team = player->GetTeam();
	CGameManager::GetInstance()->AssertCheckTeam(team);

	switch (team)
	{// �`�[����
	case CGameManager::ETeamSide::SIDE_LEFT:
		pObj = DEBUG_NEW CPlayerAIControlLeftAttack;
		break;

	case CGameManager::ETeamSide::SIDE_RIGHT:
		pObj = DEBUG_NEW CPlayerAIControlRightAttack;
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
HRESULT CPlayerAIControlAttack::Init()
{
	// �e�N���X�̏�����
	CPlayerAIControlMode::Init();

	// �l�̃N���A
	m_pTarget = nullptr;

	ZeroMemory(&m_sTimig, sizeof(m_sTimig));

	m_eAttackMode = EAttackMode::ATTACKMODE_PREPARATION;
	m_ePreparation = EAttackPrepatarion::ATTACKPREPATARION_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
	m_eThrowFlag = EThrowFlag::THROW_NONE;
	m_eAttackNormal = EAttackNormal::ATTACK_NONE;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerAIControlAttack::Uninit()
{
	// �e�N���X�̏I������
	CPlayerAIControlMode::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerAIControlAttack::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsStop()) return;

	if (!m_pTarget)
	{// �^�[�Q�b�g���ݒ肳��Ă��Ȃ��ꍇ
		m_pTarget = GetThrowTarget();		// �^�[�Q�b�g�̎擾
	}
	else
	{
		if (m_ePreparation != EAttackPrepatarion::ATTACKPREPATARION_LEAVE)
		{// ������ԈȊO�̏ꍇ
			// ����
			SeeTarget(m_pTarget->GetPosition());
		}

		// ������ނ̍X�V
		(this->*(m_AttackModeFunc[m_eAttackMode]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// �e�N���X�̍X�V�i�Ō���ɐݒu�j
	CPlayerAIControlMode::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// �U�����[�h�F����
//================================================================================
void CPlayerAIControlAttack::AttackModePreparation(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_ePreparation == EAttackPrepatarion::ATTACKPREPATARION_NONE)
	{// ������Ԃ��ݒ肳��Ă��Ȃ��ꍇ

		// �����ƃ^�[�Q�b�g�Ƃ̋������Z�o
		float fDistance = GetPlayer()->GetPosition().DistanceXZ(m_pTarget->GetPosition());

		/*if (fDistance < 100.0f)
		{
			m_ePreparation = EAttackPrepatarion::ATTACKPREPATARION_LEAVE;
		}
		else*/
		{
			m_ePreparation = EAttackPrepatarion::ATTACKPREPATARION_GO;
		}

		//m_ePreparation = EAttackPrepatarion::ATTACKPREPATARION_WAIT;
	}

	// �����̍X�V
	(this->*(m_PreparationFunc[m_ePreparation]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// �����F�҂�
//================================================================================
void CPlayerAIControlAttack::PreparationWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�}�[�Z�b�g
	SetAttackTimer(1, 2);

	// �^�C�}�[�X�V
	if (IsUpdateAttackTimer(fDeltaTime, fDeltaRate, fSlowRate))
	{// �^�C�}�[���I������瓊����
		m_eAttackMode = EAttackMode::ATTACKMODE_ATTACK;
	}
}

//================================================================================
// �����F����
//================================================================================
void CPlayerAIControlAttack::PreparationGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_eAttackMode = EAttackMode::ATTACKMODE_ATTACK;
}

#if 0	// �I�[�o�[���C�h
//================================================================================
// �����F�����
//================================================================================
void CPlayerAIControlAttack::PreparationLeave()
{

}
#endif

//================================================================================
// �U�����[�h�F�U��
//================================================================================
void CPlayerAIControlAttack::AttackModeAttack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �U���X�V
	UpdateAttack(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// �U���X�V
//================================================================================
void CPlayerAIControlAttack::UpdateAttack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	if (m_eThrowType == EThrowType::THROWTYPE_NONE)
	{
		// �X�y�V�����Q�[�W�̎擾
		CGameManager* pGameManager = CGameManager::GetInstance();						// �Q�[���}�l�[�W��
		CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(pAI->GetTeam());			// �����`�[���̃X�y�V�������
		CTeamStatus* pTeamStatus1 = pGameManager->GetTeamStatus(m_pTarget->GetTeam());	// ����`�[���̃X�y�V�������

		float fSpecialValue = pTeamStatus->GetSpecialValue();
		float fSpecialValue1 = pTeamStatus1->GetSpecialValue();

		if (pTeamStatus->IsMaxSpecial())
		{// �Q�[�W�����܂��Ă�����
			// �X�y�V��������
			m_eThrowType = EThrowType::THROWTYPE_SPECIAL;
		}
		else
		{
			// �^�[�Q�b�g�Ƃ̋������擾
			float distanse = GetPlayer()->GetPosition().DistanceXZ(m_pTarget->GetPosition());

			// �����^�C�v�̐ݒ�
			m_eThrowType = (EThrowType)UtilFunc::Transformation::Random(EThrowType::THROWTYPE_NORMAL, EThrowType::THROWTYPE_JUMP);
		}
	}

	// �����̍X�V
	UpdateThrow(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// �����X�V
//================================================================================
void CPlayerAIControlAttack::UpdateThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ������ނ̍X�V
	(this->*(m_ThrowTypeFunc[m_eThrowType]))(fDeltaTime, fDeltaRate, fSlowRate);

	// �����X�V
	(this->*(m_ThrowFlagFunc[m_eThrowFlag]))();
}

//--------------------------------------------------------------------------
// �ʏ퓊��
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowTypeNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�[�Q�b�g�Ƃ̋������擾
	float distanse = GetPlayer()->GetPosition().DistanceXZ(m_pTarget->GetPosition());

	if (m_eAttackNormal == EAttackNormal::ATTACK_NONE)
	{// �ݒ肳��Ă��Ȃ��ꍇ
		m_eAttackNormal = (EAttackNormal)UtilFunc::Transformation::Random(EAttackNormal::ATTACK_THROW, EAttackNormal::ATTACK_DASH);
	}

	switch (m_eAttackNormal)
	{
	case EAttackNormal::ATTACK_THROW:	// �ʏ�

		if (distanse > 400.0f)
		{
			// ���蓊��
			AttackDash(m_pTarget, DISTANSE_TARGET, DISTANSE_LINE);
			return;
		}
		// �ʏ퓊��
		AttackNormal(m_pTarget, fDeltaTime, fDeltaRate, fSlowRate);
		break;

	case EAttackNormal::ATTACK_DASH:	// �W�����v
		// ���蓊��
		AttackDash(m_pTarget, DISTANSE_TARGET, DISTANSE_LINE);
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------------------------------
// �W�����v����
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowTypeJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�[�Q�b�g�Ƃ̋������擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;
	MyLib::Vector3 pos = pAI->GetPosition();

	float distanse = pAI->GetPosition().DistanceXZ(m_pTarget->GetPosition());

	if (distanse > 600.0f)
	{
		m_eThrowType = EThrowType::THROWTYPE_NORMAL;
		return;
	}

	// ����W�����v����
	AttackDashJump(m_pTarget, DISTANSE_TARGET, DISTANSE_LINE, false);
}

//--------------------------------------------------------------------------
// �X�y�V����
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowTypeSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_eThrowFlag = EThrowFlag::THROW_SPECIAL;
}

//--------------------------------------------------------------------------
// �p�X
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowTypePass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_eThrowFlag = EThrowFlag::THROW_PASS;
}

//--------------------------------------------------------------------------
// ������
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowFlag()
{
	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// ������
	pControlAIAction->SetIsThrow(true);
}

//--------------------------------------------------------------------------
// �p�X
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowFlagPass()
{
	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// �p�X
	pControlAIAction->SetIsPass(true);
}

//--------------------------------------------------------------------------
// �X�y�V����
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowFlagSpecial()
{
	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// AI�R���g���[�����̎擾
	CPlayerControlAction* pControlAction = pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// �X�y�V��������
	pControlAIAction->SetIsSpecial(true);
}

//================================================================================
// �L�����Z�����f
//================================================================================
bool CPlayerAIControlAttack::IsCancelJumpAttack()
{
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	// ���[�V�����擾
	CMotion* pMotion = pAI->GetMotion();
	int nType = pMotion->GetType();

	if (nType == CPlayer::EMotion::MOTION_JUMP ||			// �W�����v���
		nType == CPlayer::EMotion::MOTION_JUMP_BALL)		// �W�����v���(�{�[������)
	{
		// �����ƃ^�[�Q�b�g�Ƃ̋������Z�o
		float fDistance = GetPlayer()->GetPosition().DistanceXZ(m_pTarget->GetPosition());
		
		if (fDistance < 500.0f)
		{
			int n = rand() % 2;

			//if (n == 0)
			{
				// �����
				m_ePreparation = EAttackPrepatarion::ATTACKPREPATARION_LEAVE;

				return true;
			}
		}
	}

	return false;
}

//================================================================================
// �m�[�}������
//================================================================================
void CPlayerAIControlAttack::AttackNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �U���܂ł̎��Ԃ�ݒ�
	SetAttackTimer(0, 2);

	// �^�C�}�[�X�V
	if (IsUpdateAttackTimer(fDeltaTime, fDeltaRate, fSlowRate))
	{// �^�C�}�[���I������瓊����
		m_eThrowFlag = EThrowFlag::THROW_NORMAL;
	}
}

//================================================================================
// �W�����v����
//================================================================================
void CPlayerAIControlAttack::AttackJump(CPlayer* pTarget)
{
	// �����̏��
	CPlayer* pMy = GetPlayer();
	if (!pMy) return;
	MyLib::Vector3 posMy = pMy->GetPosition();

	// �A�N�V�����F����
	SetActionFlag(EActionFlag::ACTION_JUMP);

	// �s���F�~�܂�
	SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);

	CMotion* pMotion = pMy->GetMotion();
	int nType = pMotion->GetType();
	if (nType == CPlayer::EMotion::MOTION_JUMP_BALL)
	{
		// �W�����v�����̎擾
		float rate = GetJumpRate();

		if (rate >= 0.0f)
		{
			m_eThrowFlag = EThrowFlag::THROW_NORMAL;
		}
	}
}

//================================================================================
// �_�b�V������
//================================================================================
void CPlayerAIControlAttack::AttackDash(CPlayer* pTarget, float fTargetDis, float fLineDis)
{
	// �^�[�Q�b�g�̎擾
	if (!pTarget) return;
	MyLib::Vector3 posTarget = pTarget->GetPosition();

	// �����̏��
	CPlayer* pMy = GetPlayer();
	if (!pMy) return;
	MyLib::Vector3 posMy = pMy->GetPosition();

	// ���C���̈ʒu
	MyLib::Vector3 linePos = { 0.0f, posMy.y, posMy.z };

	// �����v�Z
	float distanceTarget = posMy.DistanceXZ(pTarget->GetPosition());	// �����Ƒ���̋���
	float distanceLine = posMy.DistanceXZ(linePos);						// �����ƒ��S���Ƃ̋���

	MyLib::Vector3 pos = { posTarget.x, posMy.y, posMy.z };
	
	if (distanceTarget > fTargetDis &&	// �^�[�Q�b�g�Ƃ̋������߂����ǂ���
		distanceLine > fLineDis)		// �������Ƃ̋������߂����ǂ���
	{
		// ����
		SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

		// ����̈ʒu�ɋ߂Â�
		if (Approatch(pos, END_RADIUS))
		{// �͈͓��̏ꍇ
			SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);		// �s���F�~�܂�
		}

		return;
	}

	SetThrowFlag(EThrowFlag::THROW_NORMAL);		// �����t���O�F������
}

//================================================================================
// �_�b�V���W�����v����
//================================================================================
void CPlayerAIControlAttack::AttackDashJump(CPlayer* pTarget, float fTargetDistanse, float LineDistanse, bool bOver)
{
	// �^�[�Q�b�g�̎擾
	if (!pTarget) return;
	MyLib::Vector3 posTarget = pTarget->GetPosition();

	// �����̏��
	CPlayer* pMy = GetPlayer();
	if (!pMy) return;
	MyLib::Vector3 posMy = pMy->GetPosition();

	// ���C���̈ʒu
	MyLib::Vector3 linePos = { 0.0f, posMy.y, posMy.z };

	// �^�[�Q�b�g�̃G���A�̎擾
	CGameManager::ETeamSide side = pMy->GetTeam();
	CGameManager::GetInstance()->AssertCheckTeam(side);

	float distanceTarget = posMy.DistanceXZ(pTarget->GetPosition());	// �����Ƒ���̋���
	float distanceLine = posMy.DistanceXZ(linePos);						// �����ƒ��S���Ƃ̋���
	
	if (distanceTarget > fTargetDistanse &&		// �^�[�Q�b�g�Ƃ̋���
		distanceLine > LineDistanse)			// ���Ƃ̋���
	{
		// ����
		SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

		// ����̈ʒu�ɋ߂Â�
		if (Approatch(posTarget, END_RADIUS))
		{// �͈͓��̏ꍇ

			//if (!bOver)
			//{
			//	// �~�܂�
			//	SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
			//}
		}

		return;
	}

	SetActionFlag(EActionFlag::ACTION_JUMP);	// �A�N�V�����F����

	CMotion* pMotion = pMy->GetMotion();
	int nType = pMotion->GetType();
	if (nType == CPlayer::EMotion::MOTION_JUMP_BALL)
	{
		// �W�����v�����̎擾
		float rate = GetJumpRate();

		if (rate >= 0.0f)
		{
			m_eThrowFlag = EThrowFlag::THROW_NORMAL;
		}
	}
}

//==========================================================================
// �t�F�C���g
//==========================================================================
void CPlayerAIControlAttack::AttackFeint()
{
	// �����F�Ȃ�
	m_ePreparation = EAttackPrepatarion::ATTACKPREPATARION_NONE;

	// �U�����[�h�F����
	m_eAttackMode = EAttackMode::ATTACKMODE_PREPARATION;
}

//==========================================================================
// ����� : Leave(����鑊��A����鋗��)
//==========================================================================
bool CPlayerAIControlAttack::Leave(MyLib::Vector3 targetPos, float distance)
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
bool CPlayerAIControlAttack::Approatch(MyLib::Vector3 targetPos, float distance)
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
// �{�[��������̎擾�i�G�̏ꍇ�j
//==========================================================================
CPlayer* CPlayerAIControlAttack::GetBallOwner()
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
// �^�[�Q�b�g�ݒ�
//==========================================================================
CPlayer* CPlayerAIControlAttack::GetThrowTarget()
{
	CPlayer* pTarget = nullptr;			// �ŏI�I�ȃ^�[�Q�b�g
	CPlayer* pTargetLength = nullptr;	// �����^�[�Q�b�g�̕ۑ��p
	CPlayer* pTargetLife = nullptr;		// �̗̓^�[�Q�b�g�̕ۑ��p
	float fMinDis = 1000000.0f;	// �߂��v���C���[
	int nMinLife = 1000000;

	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return pTarget;

	MyLib::Vector3 Mypos = pAI->GetPosition();	// �ʒu���̎擾

	// �{�[���̎擾
	CBall* pBall = pAI->GetBall();
	if (pBall == nullptr) { return pTarget; }
	CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// �`�[���^�C�v�̎擾

	CListManager<CPlayer> list = CPlayer::GetList();	// �v���C���[���X�g
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// �Ō���C�e���[�^�[
	while (list.ListLoop(itr))
	{ // ���X�g���̗v�f�����J��Ԃ�
		CPlayer* pPlayer = (*itr);	// �v���C���[���
		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// �v���C���[�ʒu

		if (typeTeam == pPlayer->GetTeam() ||
			pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT||
			pPlayer->GetMotionFrag().bDead)
		{// �����`�[��||�O��||���S���Ă���ꍇ
			continue;
		}

		// �G�Ƃ̋��������߂�
		float fLength = Mypos.DistanceXZ(posPlayer);

		// �̗�
		int nLife = pPlayer->GetLife();

		if (nLife < nMinLife)
		{// ���̗͂̏��Ȃ��v���C���[�������ꍇ

			// �ŏ��̗͂̍X�V
			nMinLife = nLife;

			// �^�[�Q�b�g���X�V
			pTargetLife = pPlayer;
		}

		if (fLength < fMinDis)
		{ // ���߂�����v���C���[�������ꍇ

			// �ŏ������̍X�V
			fMinDis = fLength;

			// �^�[�Q�b�g���X�V
			pTargetLength = pPlayer;
		}
	}

	int i = rand() % 2;

	if (i == 0)
	{
		pTarget = pTargetLife;
	}
	else
	{
		pTarget = pTargetLength;
	}

	return pTarget;
}

//==========================================================================
// �^�[�Q�b�g������
//==========================================================================
void CPlayerAIControlAttack::SeeTarget(MyLib::Vector3 pos)
{
	// AI�̎擾
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// �����ݒ�
	float angle = pAI->GetPosition().AngleXZ(pos);
	pAI->SetRotDest(angle);
}

//================================================================================
// �s�����Ă͂����Ȃ�����
//================================================================================
bool CPlayerAIControlAttack::IsStop()
{
	CPlayer* pPlayer = GetPlayer();
	CPlayer::EState state = pPlayer->GetState();

	if (state == CPlayer::EState::STATE_CATCH_NORMAL ||
		state == CPlayer::EState::STATE_CATCH_JUST ||
		state == CPlayer::EState::STATE_CATCH_SPECIAL)
	{
		return true;
	}

	return false;
}

//==========================================================================
// �X�V�F�A�N�V�����^�C�}�[
//==========================================================================
bool CPlayerAIControlAttack::IsUpdateAttackTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sTimig.bSet)
	{
		// �^�C�}�[�̃J�E���g�_�E��
		m_sTimig.fTimer -= fDeltaTime * fDeltaRate * fSlowRate;

		if (m_sTimig.fTimer <= 0.0f)
		{
			// �s���̍\���̏�����
			ZeroMemory(&m_sTimig, sizeof(m_sTimig));

			// ������
			m_eThrowFlag = EThrowFlag::THROW_NORMAL;

			return true;
		}
	}

	return false;
}

//==========================================================================
// �A�N�V�������Ԃ̐ݒ�
//==========================================================================
void CPlayerAIControlAttack::SetAttackTimer(int nMin, int nMax)
{
	// �p�x�̐ݒ�
	if (!m_sTimig.bSet)
	{// �ݒ肳��Ă��Ȃ��ꍇ

		// �s�����Ԃ̐ݒ�
		float fRand = (float)UtilFunc::Transformation::Random(nMin, nMax);
		m_sTimig.fTimer = fRand;

		// ���Ԑݒ�ON
		m_sTimig.bSet = true;
	}
}
