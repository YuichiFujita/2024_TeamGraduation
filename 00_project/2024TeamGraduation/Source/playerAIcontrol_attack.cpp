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
	& CPlayerAIControlAttack::PreparationGo,			// �Ȃ�
	&CPlayerAIControlAttack::PreparationLeave,			// �����
};

CPlayerAIControlAttack::THROWTYPE_FUNC CPlayerAIControlAttack::m_ThrowTypeFunc[] =	// �����t���O�֐�
{
	&CPlayerAIControlAttack::ThrowTypeNone,				// �Ȃ�
	&CPlayerAIControlAttack::ThrowTypeNormal,			// �ʏ�
	&CPlayerAIControlAttack::ThrowTypeJump,				// �W�����v
	&CPlayerAIControlAttack::ThrowTypeSpecial,			// �X�y�V����
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
	switch (player->GetTeam())
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
	m_nLevel = 0;

	m_pTarget = nullptr;

	ZeroMemory(&m_sTimig, sizeof(m_sTimig));

	m_eAttackMode = EATTACKMODE::ATTACKMODE_PREPARATION;
	m_ePreparation = EATTACKPREPATARION::ATTACKPREPATARION_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
	m_eThrowFlag = EThrowFlag::THROW_NONE;

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
		if (m_ePreparation != EATTACKPREPATARION::ATTACKPREPATARION_LEAVE)
		{// ������ԈȊO�̏ꍇ
			// ����
			SeeTarget(m_pTarget->GetPosition());
		}
		
		// �^�C�}�[�X�V
		UpdateActionTimer(fDeltaTime, fDeltaRate, fSlowRate);

		// ������ނ̍X�V
		(this->*(m_AttackModeFunc[m_eAttackMode]))();
	}
	SetActionFlag(EActionFlag::ACTION_JUMP);	// �A�N�V�����F����

	// �e�N���X�̍X�V�i�Ō���ɐݒu�j
	CPlayerAIControlMode::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// �U�����[�h�F����
//================================================================================
void CPlayerAIControlAttack::AttackModePreparation()
{
	if (m_ePreparation == EATTACKPREPATARION::ATTACKPREPATARION_NONE)
	{// ������Ԃ��ݒ肳��Ă��Ȃ��ꍇ

		// �����ƃ^�[�Q�b�g�Ƃ̋������Z�o
		float fDistance = GetPlayer()->GetPosition().DistanceXZ(m_pTarget->GetPosition());

		if (fDistance < 100.0f)
		{
			m_ePreparation = EATTACKPREPATARION::ATTACKPREPATARION_LEAVE;
		}
		else
		{
			m_ePreparation = EATTACKPREPATARION::ATTACKPREPATARION_GO;
		}
	}

	// �����̍X�V
	(this->*(m_PreparationFunc[m_ePreparation]))();
}

//================================================================================
// �����F����
//================================================================================
void CPlayerAIControlAttack::PreparationGo()
{
	m_eAttackMode = EATTACKMODE::ATTACKMODE_ATTACK;
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
void CPlayerAIControlAttack::AttackModeAttack()
{
	// �U���X�V
	UpdateAttack();
}

//================================================================================
// �U���X�V
//================================================================================
void CPlayerAIControlAttack::UpdateAttack()
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
	UpdateThrow();
}

//================================================================================
// �����X�V
//================================================================================
void CPlayerAIControlAttack::UpdateThrow()
{
	// ������ނ̍X�V
	(this->*(m_ThrowTypeFunc[m_eThrowType]))();

	// �����X�V
	(this->*(m_ThrowFlagFunc[m_eThrowFlag]))();
}

//--------------------------------------------------------------------------
// �ʏ퓊��
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowTypeNormal()
{
	//m_eThrowFlag = EThrowFlag::THROW_NORMAL;

	AttackNormal(m_pTarget);
}

//--------------------------------------------------------------------------
// �W�����v����
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowTypeJump()
{
	// ���蓊��
	AttackDashJump(m_pTarget);

	//AttackJump(m_pTarget);
}

//--------------------------------------------------------------------------
// �X�y�V��������
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowTypeSpecial()
{
	m_eThrowFlag = EThrowFlag::THROW_SPECIAL;
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

	// ������ԁF��
	m_eThrowFlag = EThrowFlag::THROW_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
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

	// ������ԁF��
	m_eThrowFlag = EThrowFlag::THROW_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
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

	// �����Ȃ�
	m_eThrowFlag = EThrowFlag::THROW_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
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
				m_ePreparation = EATTACKPREPATARION::ATTACKPREPATARION_LEAVE;

				return true;
			}
		}
	}

	return false;
}

//================================================================================
// �m�[�}������
//================================================================================
void CPlayerAIControlAttack::AttackNormal(CPlayer* pTarget)
{
	SetActionTimer(0, 5);
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

	float rate = GetJumpRate();
	float rateMax = GetMaxjumpRate();

	if (rateMax >= rate)	// �����ɂ���ĕς��
	{
		m_eThrowFlag = EThrowFlag::THROW_NORMAL;
	}
}

//================================================================================
// �_�b�V������
//================================================================================
void CPlayerAIControlAttack::AttackDash(CPlayer* pTarget)
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

	// ���C���Ƃ̋���
	float distanceLine = 0.0f;
	float distanceTarget = 0.0f;
	float JUMP_LENGTH_TARGET = 100.0f;
	float JUMP_LENGTH_LINE = 200.0f;

	// �^�[�Q�b�g�̃G���A�̎擾
	CGameManager::ETeamSide side = pMy->GetTeam();

	if (pTarget)
	{// �^�[�Q�b�g�������ꍇ
		distanceTarget = posMy.DistanceXZ(pTarget->GetPosition());	// �����Ƒ���̋���
		distanceLine = posMy.DistanceXZ(linePos);	// �����ƒ��S���Ƃ̋���
	}
	else
	{
		return;
	}

	if (distanceTarget > JUMP_LENGTH_TARGET &&	// �^�[�Q�b�g�Ƃ̋��������l��艓��
		distanceLine > JUMP_LENGTH_LINE)		// �������Ƃ̋��������l��艓��
	{
		// ����
		SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

		// ����̈ʒu�ɋ߂Â�
		if (Approatch(posTarget, GetParameter().fRadius))
		{// �͈͓��̏ꍇ
			SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);			// �s���F�~�܂�
		}

		return;
	}

	SetThrowFlag(EThrowFlag::THROW_NORMAL);		// �����F������
}

//================================================================================
// �_�b�V���W�����v����
//================================================================================
void CPlayerAIControlAttack::AttackDashJump(CPlayer* pTarget)
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

	// ���C���Ƃ̋���
	float distanceLine = 0.0f;
	float distanceTarget = 0.0f;
	float JUMP_LENGTH_TARGET = 500.0f;
	float JUMP_LENGTH_LINE = 300.0f;

	// �^�[�Q�b�g�̃G���A�̎擾
	CGameManager::ETeamSide side = pMy->GetTeam();

	if (pTarget)
	{// �^�[�Q�b�g�������ꍇ
		distanceTarget = posMy.DistanceXZ(pTarget->GetPosition());	// �����Ƒ���̋���
		distanceLine = posMy.DistanceXZ(linePos);	// �����ƒ��S���Ƃ̋���
	}
	else
	{
		return;
	}

	if (distanceTarget > JUMP_LENGTH_TARGET && distanceLine > JUMP_LENGTH_LINE)
	{// 

		// ����
		SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

		// ����̈ʒu�ɋ߂Â�
		if (Approatch(posTarget, GetParameter().fRadius))
		{// �͈͓��̏ꍇ
			//SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);			// �s���F�~�܂�
		}

		return;
	}

	if (distanceTarget > JUMP_LENGTH_TARGET ||
		distanceLine > JUMP_LENGTH_LINE)
	{
		SetActionFlag(EActionFlag::ACTION_JUMP);	// �A�N�V�����F����
	}

	if (pMy->GetPosition().y >= GetParameter().fJumpEnd)	// �����ɂ���ĕς��
	{
		m_eThrowFlag = EThrowFlag::THROW_NORMAL;
	}
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
void CPlayerAIControlAttack::UpdateActionTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
		}
	}
}

//==========================================================================
// �A�N�V�������Ԃ̐ݒ�
//==========================================================================
void CPlayerAIControlAttack::SetActionTimer(int nMin, int nMax)
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
