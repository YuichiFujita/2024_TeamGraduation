//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol_move.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "playercontrol_move.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "playerAction.h"
#include "playerStatus.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float INTERVAL_INPUT = (2.0f / 60.0f);	// ����
	const float TIME_INTERVAL = 0.3f;	// �_�b�V���P�\
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerControlMove::CPlayerControlMove()
{
	memset(m_nCntTrigger, 0, sizeof(m_nCntTrigger));	// �g���K�[�̃J�E���g
	m_HoldDashAngle = CPlayer::EDashAngle::ANGLE_UP;		// �ێ����Ă�_�b�V���̈ړ�����
	m_fInputInterval = 0.0f;							// ���͂̎󂯕t���P�\
	m_fTriggerInterval = 0.0f;							// �g���K�[�̃C���^�[�o��
	m_bBlink = false;
	m_pInputAngle = nullptr;					// ���݂̓��͕���
	m_fInputAngleCtr = 0.0f;					// ���݂̓��͕����̕ێ��J�E���^�[
	m_fCrabMoveEasingTime = 0.0f;				// ���݂̃J�j�����ړ��␳�l
}

//==========================================================================
// �ʏ�ړ�
//==========================================================================
void CPlayerControlMove::Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���[�V�������擾
	CMotion* pMotion = player->GetMotion();
	int nMotionType = pMotion->GetType();
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	// �A�N�V�����擾
	CPlayerAction* pPlayerAction = player->GetActionPattern();
	if (pPlayerAction == nullptr) return;
	CPlayer::EAction action = pPlayerAction->GetAction();

	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		player->IsPossibleMove())
	{// �ړ��\���[�V�����̎�

		//--------------------------
		// �ړ�����
		//--------------------------
		if (action != CPlayer::EAction::ACTION_BLINK ||
			action != CPlayer::EAction::ACTION_DODGE)
		{
			// �u�����N
			Blink(player, fDeltaTime, fDeltaRate, fSlowRate);

			// ����
			Walk(player, fDeltaTime, fDeltaRate, fSlowRate);

			// �_�b�V��
			Dash(player, fDeltaTime, fDeltaRate, fSlowRate);
		}

		// �W�����v�󋵎擾
		bool bJump = player->IsJump();

		if (motionFrag.bMove &&
			pMotion->IsGetCancelable() && 
			!bJump)
		{// �L�����Z���\ && �ړ���

			// ���[�V�����L�����Z��
			pMotion->ToggleFinish(true);
			
			//TODO: �����̗]���L�����Z���Ƃ��p ToggleFinish�͕K�v(���[�V�����o������)
		}
	}
	
	if (m_fCrabMoveEasingTime > 1.0f)
	{// 1�𒴂�����͈͓��ɂ���
		m_fCrabMoveEasingTime = static_cast<int>(m_fCrabMoveEasingTime * 1000) % static_cast<int>(1000);
	}
	
	// �␳�p���ԉ��Z
	m_fCrabMoveEasingTime += 1.0f * fDeltaTime * fDeltaRate * fSlowRate;

#if 1
	// �J�j��������
	if (player->GetBase()->IsCrab())
	{
		CrabSetting(player);
	}
#endif
}

//==========================================================================
// �g���K�[�̃J�E���g�ݒ�
//==========================================================================
void CPlayerControlMove::SetCntTrigger(int* nTrigger)
{
	if (nTrigger == nullptr) return;

	for (int i = 0; i < CPlayer::EDashAngle::ANGLE_MAX; i++)
	{
		m_nCntTrigger[i] = nTrigger[i];
	}
}

//==========================================================================
// �J�j�������
//==========================================================================
void CPlayerControlMove::CrabSetting(CPlayer* player)
{
	// �ڕW�̌����擾
	MyLib::Vector3 pos = player->GetPosition();

	// �R�[�g�T�C�Y�擾
	MyLib::Vector3 sizeCourt = CGameManager::GetInstance()->GetCourtSize();
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	// �J�j��������Ȃ��Ƃ��͔�����
	if (!player->GetBase()->IsCrab()) return;

	// �{�[���̈ʒu
	MyLib::Vector3 posBall = pBall->GetPosition();

	// �ڕW�̌���
	float fAngle = pos.AngleXZ(posBall);
	UtilFunc::Transformation::RotNormalize(fAngle);

	// �v���C���[�̖ڕW�̌����ݒ�
	player->SetRotDest(fAngle);
}
