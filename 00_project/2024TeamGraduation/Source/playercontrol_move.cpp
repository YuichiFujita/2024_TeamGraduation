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
}

//==========================================================================
// �ʏ�ړ�
//==========================================================================
void CPlayerControlMove::Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// �ڕW�̌����擾
	float fRotDest = player->GetRotDest();

	// �ړ��ʎ擾
	MyLib::Vector3 move = player->GetMove();

	// ���[�V�������擾
	CMotion* pMotion = player->GetMotion();
	int nMotionType = pMotion->GetType();
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	// ��Ԏ擾
	CPlayer::EState state = player->GetState();

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
			!bJump)
		{// �L�����Z���\ && �ړ���

		 //TODO: �����̗]���L�����Z���Ƃ��p ToggleFinish�͕K�v(���[�V�����o������)
		
		}
	}
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
