//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playercontrol_action.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "ball.h"
#include "playerAction.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerControlAction::CPlayerControlAction()
{

}

//==========================================================================
// ����
//==========================================================================
void CPlayerControlAction::Action(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
	CPlayer::STATE state = player->GetState();

	// �A�N�V�����擾
	CPlayerAction* pPlayerAction = player->GetActionPattern();
	if (pPlayerAction == nullptr) return;
	CPlayer::Action action = pPlayerAction->GetAction();


	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		player->IsPossibleMove())
	{// �ړ��\���[�V�����̎�

		//--------------------------
		// �A�N�V��������
		//--------------------------
		if (action != CPlayer::Action::ACTION_BLINK)
		{
			Catch(player, fDeltaTime, fDeltaRate, fSlowRate);
			Throw(player, fDeltaTime, fDeltaRate, fSlowRate);
			Jump(player, fDeltaTime, fDeltaRate, fSlowRate);
		}

	}

	Special(player, fDeltaTime, fDeltaRate, fSlowRate);
	Charm(player, fDeltaTime, fDeltaRate, fSlowRate);

	// ���[�V�����t���O�ݒ�
	player->SetMotionFrag(motionFrag);
}

//==========================================================================
// �L���b�`
//==========================================================================
void CPlayerControlAction::Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (player->GetBall() != nullptr)
	{
		return;
	}

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	CMotion* pMotion = player->GetMotion();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_B, player->GetMyPlayerIdx()))
	{
		// �A�N�V�����p�^�[���ύX
		pMotion->Set(CPlayer::MOTION::MOTION_CATCH);
		player->GetActionPattern()->SetAction(CPlayer::Action::ACTION_CATCH);
	}
}

//==========================================================================
// ����
//==========================================================================
void CPlayerControlAction::Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = player->GetBall();

	if (pBall == nullptr)
	{
		return;
	}

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	CMotion* pMotion = player->GetMotion();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_B, player->GetMyPlayerIdx()))
	{

		// �A�N�V�����p�^�[���ύX
		if (player->IsJump())
		{
			pBall->ThrowJump(player);
			
			pMotion->Set(CPlayer::MOTION::MOTION_THROW_JUMP);

			player->GetActionPattern()->SetAction(CPlayer::Action::ACTION_THROW_JUMP);
		}
		else
		{
			pMotion->Set(CPlayer::MOTION::MOTION_THROW);

			player->GetActionPattern()->SetAction(CPlayer::Action::ACTION_THROW);
		}
	}
}

//==========================================================================
// �W�����v
//==========================================================================
void CPlayerControlAction::Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	bool bJump = player->IsJump();

	if (bJump)
	{
		return;
	}

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �ړ��ʎ擾
	MyLib::Vector3 move = player->GetMove();

	// ���[�V�������擾
	CMotion* pMotion = player->GetMotion();
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	//�W�����v����
	if (pKey->GetTrigger(DIK_SPACE) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_A, player->GetMyPlayerIdx()))
	{
		bJump = true;
		motionFrag.bJump = true;
		move.y += 12.0f;

		pMotion->Set(CPlayer::MOTION::MOTION_JUMP);

		player->SetMove(move);
		player->SetMotionFrag(motionFrag);

		// �W�����v����ݒ�
		player->SetEnableJump(bJump);

		// �A�N�V�����p�^�[���ύX
		player->GetActionPattern()->SetAction(CPlayer::Action::ACTION_JUMP);

		// �T�E���h�Đ�
		//CSound::GetInstance()->PlaySound(CSound::LABEL_SE_JUMP);
	}
}

//==========================================================================
// �X�y�V����
//==========================================================================
void CPlayerControlAction::Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = player->GetBall();

	if (pBall == nullptr)
	{//�X�y�V�����Q�[�WMAX�{�{�[��������
		return;
	}

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	CMotion* pMotion = player->GetMotion();

	if (pKey->GetTrigger(DIK_X) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_LB, player->GetMyPlayerIdx()))
	{
		pBall->ThrowSpecial(player);

		pMotion->Set(CPlayer::MOTION::MOTION_SPECIAL);

		// �A�N�V�����p�^�[���ύX
		player->GetActionPattern()->SetAction(CPlayer::Action::ACTION_SPECIAL);
	}
}

//==========================================================================
// ���e�{�^��
//==========================================================================
void CPlayerControlAction::Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	player->GetActionPattern()->SetEnableCharm(false);

	if (pKey->GetPress(DIK_C) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_RB, player->GetMyPlayerIdx()))
	{
		//���e�A�N�V���������������
		player->GetActionPattern()->SetEnableCharm(true);
	}
}
