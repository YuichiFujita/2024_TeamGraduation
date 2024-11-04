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
	
#if 0
	// �J�j��������
	if (player->IsCrab())
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
	float fRotDest = player->GetRotDest();
	MyLib::Vector3 sizeCourt = CGame::GetInstance()->GetGameManager()->GetCourtSize();

	MyLib::Vector3 posCourt = MyLib::Vector3();
	MyLib::Vector3 pos = player->GetPosition();
	posCourt.z = pos.z;

	// ����R�[�g��������
	switch (player->GetStatus()->GetTeam())
	{
	case CGameManager::TeamSide::SIDE_LEFT:
	
		posCourt.x = sizeCourt.x;
		
		break;

	case CGameManager::TeamSide::SIDE_RIGHT:
	
		posCourt.x = -sizeCourt.x;
		
		break;

	default:
		break;
	}

	D3DXVECTOR3 vecDiff = D3DXVECTOR3(pos.x - posCourt.x,
		0.0f,
		pos.z - posCourt.z);
	float fAngle = atan2f(vecDiff.x, vecDiff.z);		//�ڕW�̌���
	UtilFunc::Transformation::RotNormalize(fAngle);

	//pos�擾�[���ɑ΂���p�x
	fRotDest = fAngle;

	player->SetRotDest(fRotDest);
}
