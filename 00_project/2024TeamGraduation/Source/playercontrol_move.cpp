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
	const float INPUT_COUNTER = (60.0f / 60.0f);	// ���̓J�E���^�[
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
// �f�X�g���N�^
//==========================================================================
CPlayerControlMove::~CPlayerControlMove()
{
	SAFE_DELETE(m_pInputAngle);
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

		motionFrag = player->GetMotionFrag();
	
		if (motionFrag.bMove &&
			pMotion->IsGetCancelable() && 
			!bJump)
		{// �L�����Z���\ && �ړ���

			// ���[�V�����L�����Z��
			pMotion->ToggleFinish(true);
			
			//TODO: �����̗]���L�����Z���Ƃ��p ToggleFinish�͕K�v(���[�V�����o������)
		}
	}

	// ���͕���
	UpdateInputAngle(player, fDeltaTime, fDeltaRate, fSlowRate);

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

//==========================================================================
// ���݂̓��͕����X�V
//==========================================================================
void CPlayerControlMove::UpdateInputAngle(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �v���C���[�ԍ��擾
	int playerIdx = player->GetMyPlayerIdx();

	CPlayer::EDashAngle eAngle = CPlayer::EDashAngle::ANGLE_UP;
	bool bInput = false;

	if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
		pPad->GetStickMoveL(playerIdx).y > 0 ||
		pKey->GetPress(DIK_W))
	{// ��
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
			pPad->GetStickMoveL(playerIdx).x < 0 ||
			pKey->GetPress(DIK_A))
		{// ����
			eAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
			pPad->GetStickMoveL(playerIdx).x > 0 ||
			pKey->GetPress(DIK_D))
		{// �E��
			eAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
		}
		else
		{// ��
			eAngle = CPlayer::EDashAngle::ANGLE_UP;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, playerIdx) ||
		pPad->GetStickMoveL(playerIdx).y < 0 ||
		pKey->GetPress(DIK_S))
	{// ��
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
			pPad->GetStickMoveL(playerIdx).x < 0 ||
			pKey->GetPress(DIK_A))
		{// ����
			eAngle = CPlayer::EDashAngle::ANGLE_LEFTDW;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
			pPad->GetStickMoveL(playerIdx).x > 0 ||
			pKey->GetPress(DIK_D))
		{// �E��
			eAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
		}
		else
		{// ��
			eAngle = CPlayer::EDashAngle::ANGLE_DOWN;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
		pPad->GetStickMoveL(playerIdx).x > 0 ||
		pKey->GetPress(DIK_D))
	{// �E
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
			pPad->GetStickMoveL(playerIdx).y < 0 ||
			pKey->GetPress(DIK_W))
		{// �E��
			eAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, playerIdx) ||
			pPad->GetStickMoveL(playerIdx).y > 0 ||
			pKey->GetPress(DIK_S))
		{// �E��
			eAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
		}
		else
		{// �E
			eAngle = CPlayer::EDashAngle::ANGLE_RIGHT;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
		pPad->GetStickMoveL(playerIdx).x < 0 ||
		pKey->GetPress(DIK_A))
	{// ��
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
			pPad->GetStickMoveL(playerIdx).y < 0 ||
			pKey->GetPress(DIK_W))
		{// ����
			eAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, playerIdx) ||
			pPad->GetStickMoveL(playerIdx).y > 0 ||
			pKey->GetPress(DIK_S))
		{// ����
			eAngle = CPlayer::EDashAngle::ANGLE_LEFTDW;
		}
		else
		{// ��
			eAngle = CPlayer::EDashAngle::ANGLE_LEFT;
		}
	}

	CPlayer::EDashAngle eInputOld = CPlayer::EDashAngle::ANGLE_UP;
	bool bOld = false;

	// ���݂̓��͕����ݒ�
	if (m_pInputAngle != nullptr)
	{
		eInputOld = *m_pInputAngle;
		bOld = true;

		delete m_pInputAngle;
		m_pInputAngle = nullptr;
	}

	if (bInput)
	{
		// ���݂̓��͕����ݒ�
		if (m_pInputAngle == nullptr)
		{
			m_pInputAngle = DEBUG_NEW CPlayer::EDashAngle;
			MyAssert::CustomAssert(m_pInputAngle != nullptr, "�Ȃ��new�o���Ă˂��񂾂�");
		}
		*m_pInputAngle = eAngle;

		if (eInputOld != eAngle || !bOld)
		{// �O�񖳓��� or �Ⴄ����

			// ���͕����J�E���^�[�ݒ�
			SetInputAngleCtr(INPUT_COUNTER);
		}
	}

	// ���݂̓��͕����J�E���^�[
	if (m_fInputAngleCtr >= 0.0f)
	{
		m_fInputAngleCtr -= fDeltaTime * fSlowRate;
		m_fInputAngleCtr = UtilFunc::Transformation::Clamp(m_fInputAngleCtr, 0.0f, INPUT_COUNTER);
	}
}
