//=============================================================================
// 
//  �v���C���[�R���g���[������ [playerAction.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playerAction.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "ball.h"
#include "playerStatus.h"

namespace
{
	const float DODGE_SLOW = 0.8f;			//������X���[�l
	const float JUMPTHROW_DOWN_COR = 0.5f;	//���~���W�����v�����ړ��ʕ␳�l
}

namespace ActionTime
{
	const float BLINK = 0.2f;		// �u�����N����
	const float DODGE = 0.5f;		// �������
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CPlayerAction::ACTION_FUNC CPlayerAction::m_ActionFunc[] =	// �s���֐�
{
	&CPlayerAction::ActionNone,			// �Ȃ�
	&CPlayerAction::ActionBlink,		// �u�����N
	&CPlayerAction::ActionDodge,		// ���
	&CPlayerAction::ActionRun,			// ����
	&CPlayerAction::ActionJump,			// �W�����v
	&CPlayerAction::ActionCatch,		// �L���b�`
	&CPlayerAction::ActionThrow,		// ����
	&CPlayerAction::ActionThrowJump,	// ����(�W�����v)
	&CPlayerAction::ActionSpecial,		// �X�y�V����
};

CPlayerAction::START_FUNC CPlayerAction::m_StartFunc[] =	// �s���֐�
{
	nullptr,								// �Ȃ�
	nullptr,								// �u�����N
	&CPlayerAction::StartDodge,				// ���
	nullptr,								// ����
	nullptr,								// �W�����v
	nullptr,								// �L���b�`
	nullptr,								// ����
	&CPlayerAction::StartThrowJump,			// ����(�W�����v)
	nullptr,								// �X�y�V����
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAction::CPlayerAction(CPlayer* player)
{
	m_bCharm = false;
	m_Action = CPlayer::EAction::ACTION_NONE;	// �A�N�V����
	m_fActionTime = 0.0f;						// �A�N�V��������
	m_pPlayer = player;							// �v���C���[�̃|�C���^
}

//==========================================================================
// �X�V
//==========================================================================
void CPlayerAction::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �A�N�V��������
	m_fActionTime += fDeltaTime * fDeltaRate * fSlowRate;

	// �s���X�V
	(this->*(m_ActionFunc[m_Action]))(fDeltaTime, fDeltaRate, fSlowRate);

#if _DEBUG //�f�o�b�O

	Debug();

#endif //�f�o�b�O
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CPlayerAction::ActionNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �u�����N
//==========================================================================
void CPlayerAction::ActionBlink(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fActionTime >= ActionTime::BLINK)
	{// �u�����N�o��
		
		CPlayer::SDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
		DmgInfo.fReceiveTime = 0.0f;
		m_pPlayer->SetDamageInfo(DmgInfo);

		SetAction(CPlayer::EAction::ACTION_NONE);
	}

	if (m_pPlayer->GetBall() != nullptr)
	{
		return;
	}

	//�_���[�W��t���Ȃ����Ԑݒ�
	CPlayer::SDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
	DmgInfo.fReceiveTime = 0.1f;
	m_pPlayer->SetDamageInfo(DmgInfo);

	//��𔻒�
	CListManager<CBall> sampleList = CBall::GetListObj();
	std::list<CBall*>::iterator itr = sampleList.GetEnd();
	CBall* pObj = nullptr;
	CGameManager::ETeamSide teamPlayer = m_pPlayer->GetStatus()->GetTeam();

	while (sampleList.ListLoop(itr))
	{
		pObj = (*itr);

		if (!pObj->IsAttack() ||
			pObj->GetTypeTeam() == teamPlayer)
		{
			return;
		}

		if (UtilFunc::Collision::CollisionCircleCylinder(
			pObj->GetPosition(), m_pPlayer->GetPosition(), pObj->GetRadius(), m_pPlayer->GetRadius(), m_pPlayer->GetParameter().fHeight))
		{
			// �A�N�V�����ݒ�
			SetAction(CPlayer::EAction::ACTION_DODGE);
		}
	}
}

//==========================================================================
// ���
//==========================================================================
void CPlayerAction::ActionDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayer::SDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
	DmgInfo.fReceiveTime = 0.1f;
	m_pPlayer->SetDamageInfo(DmgInfo);

	if (m_pPlayer->GetMotion()->IsFinish())
	//if (m_fActionTime >= ActionTime::DODGE)
	{// �I��

		//�X���[
		float fRate = GET_MANAGER->GetSlowRate();
		fRate += DODGE_SLOW;
		UtilFunc::Transformation::ValueNormalize(fRate, 1.0f, 0.0f);
		GET_MANAGER->SetSlowRate(fRate);

		m_pPlayer->SetEnableMove(true);
		SetAction(CPlayer::EAction::ACTION_NONE);
	}
}

//==========================================================================
// ����
//==========================================================================
void CPlayerAction::ActionRun(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �W�����v
//==========================================================================
void CPlayerAction::ActionJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!m_pPlayer->IsJump())
	{// �L���b�`�P�\
		SetAction(CPlayer::EAction::ACTION_NONE);
	}
}

//==========================================================================
// �L���b�`
//==========================================================================
void CPlayerAction::ActionCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pPlayer->GetMotion()->IsFinish())
	{// �L���b�`�P�\
		SetAction(CPlayer::EAction::ACTION_NONE);
	}
}

//==========================================================================
// ����
//==========================================================================
void CPlayerAction::ActionThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CMotion* pMotion = m_pPlayer->GetMotion();
	
	//�r���ŕύX�A�T�\�g
#if 0
	MyAssert::CustomAssert(pMotion->GetType() == CPlayer::MOTION_THROW, "�������Ⴄ���");
#endif

	if (pMotion->IsFinish())
	{// �I��
		SetAction(CPlayer::EAction::ACTION_NONE);
	}
}

//==========================================================================
// �W�����v����
//==========================================================================
void CPlayerAction::ActionThrowJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//�t���b��
	CCharacterStatus::CharParameter param = m_pPlayer->GetParameter();
	MyLib::Vector3 move = m_pPlayer->GetMove();
	float fMoveY = param.fJumpUpdateMove;

	//���~��
	if (move.y < 0.0f)
	{
		fMoveY *= JUMPTHROW_DOWN_COR;
	}
	move.y += fMoveY;
	m_pPlayer->SetMove(move);

	if (m_pPlayer->GetMotion()->IsFinish())
	{// �W�����v�ڍs
		SetAction(CPlayer::EAction::ACTION_JUMP);
	}
}

//==========================================================================
// �X�y�V����
//==========================================================================
void CPlayerAction::ActionSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �W�����v����(�X�^�[�g)
//==========================================================================
void CPlayerAction::StartThrowJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//�t���b��
	CCharacterStatus::CharParameter param = m_pPlayer->GetParameter();
	MyLib::Vector3 move = m_pPlayer->GetMove();
	float fMoveY = param.fJumpStartMove;

	//���~��
	if (move.y < 0.0f)
	{
		fMoveY *= JUMPTHROW_DOWN_COR;
	}

	move.y = fMoveY;
	m_pPlayer->SetMove(move);
}

//==========================================================================
// ���(�X�^�[�g)
//==========================================================================
void CPlayerAction::StartDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CGameManager::ETeamSide teamPlayer = m_pPlayer->GetStatus()->GetTeam();

	//�_���[�W��t���Ȃ����Ԑݒ�
	CPlayer::SDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
	DmgInfo.fReceiveTime = 0.5f;
	m_pPlayer->SetDamageInfo(DmgInfo);

	// �����ԂɈڍs
	// ����s�\���
	m_pPlayer->SetEnableMove(false);
	m_pPlayer->SetState(CPlayer::STATE_DODGE);
	m_pPlayer->SetMotion(CPlayer::MOTION_DODGE);

	// �Q�[���}�l�[�W���擾
	CGameManager* pGameMgr = CGameManager::GetInstance();
	if (pGameMgr == nullptr) return;

	// ���e���Z
	pGameMgr->AddCharmValue(teamPlayer, CCharmManager::ETypeAdd::ADD_DODGE);

	// �X�y�V�������Z
	pGameMgr->AddSpecialValue(teamPlayer, CSpecialValueManager::ETypeAdd::ADD_DODGE);
}

//==========================================================================
// �A�N�V�����ݒ�
//==========================================================================
void CPlayerAction::SetAction(CPlayer::EAction action)
{
	float fDeltaTime = CManager::GetInstance()->GetDeltaTime();
	float fDeltaRate = CManager::GetInstance()->GetDeltaRate();
	float fSlowRate = CManager::GetInstance()->GetSlowRate();

	m_Action = action;
	m_fActionTime = 0.0f;		// �A�N�V��������

	// �s���J�n
	if (m_StartFunc[m_Action] != nullptr)
	{
		(this->*(m_StartFunc[m_Action]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// �f�o�b�O
//==========================================================================
void CPlayerAction::Debug()
{

}

