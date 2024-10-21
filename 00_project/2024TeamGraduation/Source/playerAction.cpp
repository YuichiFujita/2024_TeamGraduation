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
	const float DODGE_SLOW = 0.8f;		//������X���[�l
}

namespace ActionTime
{
	const float BLINK = 0.35f;		// �u�����N����
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

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAction::CPlayerAction(CPlayer* player)
{
	m_bCharm = false;
	m_Action = CPlayer::Action::ACTION_NONE;	// �A�N�V����
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
		
		CPlayer::sDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
		DmgInfo.reciveTime = 0.0f;
		m_pPlayer->SetDamageInfo(DmgInfo);

		SetAction(CPlayer::Action::ACTION_NONE);
	}

	if (m_pPlayer->GetBall() != nullptr)
	{
		return;
	}

	//�_���[�W��t���Ȃ����Ԑݒ�
	CPlayer::sDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
	DmgInfo.reciveTime = 0.1f;
	m_pPlayer->SetDamageInfo(DmgInfo);

	//��𔻒�
	CListManager<CBall> sampleList = CBall::GetListObj();
	std::list<CBall*>::iterator itr = sampleList.GetEnd();
	CBall* pObj = nullptr;

	while (sampleList.ListLoop(itr))
	{
		pObj = (*itr);

		if (!pObj->IsAttack() ||
			pObj->GetTypeTeam() == m_pPlayer->GetStatus()->GetTeam())
		{
			return;
		}

		if (UtilFunc::Collision::CollisionCircleCylinder(
			pObj->GetPosition(), m_pPlayer->GetPosition(), pObj->GetRadius(), m_pPlayer->GetRadius(), m_pPlayer->GetParameter().fHeight))
		{
			//�_���[�W��t���Ȃ����Ԑݒ�
			CPlayer::sDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
			DmgInfo.reciveTime = 0.5f;
			m_pPlayer->SetDamageInfo(DmgInfo);

			// �����ԂɈڍs
			// ����s�\���
			m_pPlayer->SetEnableMove(false);
			SetAction(CPlayer::Action::ACTION_DODGE);
			m_pPlayer->SetState(CPlayer::STATE_DODGE);
			//m_pPlayer->SetMotion(CPlayer::MOTION_DODGE);
		}
	}
}

//==========================================================================
// ���
//==========================================================================
void CPlayerAction::ActionDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayer::sDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
	DmgInfo.reciveTime = 0.1f;
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
		SetAction(CPlayer::Action::ACTION_NONE);
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
		SetAction(CPlayer::Action::ACTION_NONE);
	}
}

//==========================================================================
// �L���b�`
//==========================================================================
void CPlayerAction::ActionCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pPlayer->GetMotion()->IsFinish())
	{// �L���b�`�P�\
		SetAction(CPlayer::Action::ACTION_NONE);
	}
}

//==========================================================================
// ����
//==========================================================================
void CPlayerAction::ActionThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pPlayer->GetMotion()->IsFinish())
	{// �I��
		SetAction(CPlayer::Action::ACTION_NONE);
	}
}

//==========================================================================
// �W�����v����
//==========================================================================
void CPlayerAction::ActionThrowJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pPlayer->GetMotion()->IsFinish())
	{// �W�����v�ڍs
		SetAction(CPlayer::Action::ACTION_JUMP);
	}
}

//==========================================================================
// �X�y�V����
//==========================================================================
void CPlayerAction::ActionSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �A�N�V�����ݒ�
//==========================================================================
void CPlayerAction::SetAction(CPlayer::Action action)
{
	m_Action = action;
	m_fActionTime = 0.0f;		// �A�N�V��������
}
