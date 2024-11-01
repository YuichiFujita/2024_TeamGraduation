//=============================================================================
// 
//  �v���C���[�R���g���[������ [playerUsercontrol_move.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playerUsercontrol_move.h"
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
	const float RATIO_STICKBLINK = 0.75f;	// �X�e�B�b�N�u�����N���̊���
	const float TIME_INTERVAL = 0.3f;	// �_�b�V���P�\
	const float BLINK_JUMP_COR = 0.5f;	// �󒆎��u�����N�␳�l
	const float INPUT_COUNTER = (4.0f / 60.0f);	// ���̓J�E���^�[
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerUserControlMove::CPlayerUserControlMove()
{

}

//==========================================================================
// �u�����N
//==========================================================================
void CPlayerUserControlMove::Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	

	// ���̓t���O
	bool bInput = false;

	// �_�b�V������
	CPlayer::SDashInfo info;

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	//�R���g���[���n�擾
	int* nCntTrigger = GetCntTrigger();						// �g���K�[�̃J�E���g�擾
	CPlayer::EDashAngle HoldDashAngle = GetHoldDashAngle();	// �_�b�V�������擾
	float fInputInterval = GetInputInterval();				// ���͂̎󂯕t���P�\�擾
	bool bDash = IsBlink();									// ����t���O�擾

	// �v���C���[�ԍ��擾
	int playerIdx = player->GetMyPlayerIdx();

	// L�X�e�B�b�N�̊���
	MyLib::Vector3 stickRatio = pPad->GetStickPositionRatioL(playerIdx);

	ImGui::Text("stickRatio.x: %.2f", stickRatio.x);
	ImGui::Text("stickRatio.y: %.2f", stickRatio.y);

	if (fInputInterval <= 0.0f)
	{// �P�\�󂯕t���I����


		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
			(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_Y) && stickRatio.y >= RATIO_STICKBLINK) ||
				pKey->GetTrigger(DIK_W))
		{// ��
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
				stickRatio.x <= -RATIO_STICKBLINK ||
				pKey->GetPress(DIK_A))
			{// ����
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
				stickRatio.x >= RATIO_STICKBLINK ||
				pKey->GetPress(DIK_D))
			{// �E��
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
			}
			else
			{// ��
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_UP;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, playerIdx) ||
			(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_Y) && stickRatio.y <= -RATIO_STICKBLINK) ||
				pKey->GetTrigger(DIK_S))
		{// ��
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
				stickRatio.x <= -RATIO_STICKBLINK ||
				pKey->GetPress(DIK_A))
			{// ����
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTDW;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
				stickRatio.x >= RATIO_STICKBLINK ||
				pKey->GetPress(DIK_D))
			{// �E��
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
			}
			else
			{// ��
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_DOWN;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
			(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_X) && stickRatio.x >= RATIO_STICKBLINK) ||
				pKey->GetTrigger(DIK_D))
		{// �E
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
				stickRatio.y >= RATIO_STICKBLINK ||
				pKey->GetPress(DIK_W))
			{// �E��
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				stickRatio.y <= -RATIO_STICKBLINK ||
				pKey->GetPress(DIK_S))
			{// �E��
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
			}
			else
			{// �E
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHT;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
			(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_X) && stickRatio.x <= -RATIO_STICKBLINK) ||
				pKey->GetTrigger(DIK_A))
		{// ��
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
				stickRatio.y >= RATIO_STICKBLINK ||
				pKey->GetPress(DIK_W))
			{// ����
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, playerIdx) ||
				stickRatio.y <= -RATIO_STICKBLINK ||
				pKey->GetPress(DIK_S))
			{// ����
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTDW;
			}
			else
			{// ��
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFT;
			}
		}
	}


	if (bInput)
	{// ���͂��ꂽ

		// ���͂̃C���^�[�o��
		fInputInterval = INTERVAL_INPUT;
	}

	if (fInputInterval > 0.0f && fInputInterval != INTERVAL_INPUT)
	{// �󂯕t���P�\��

		// ���̓��͔���
		bool bNextInput = false;
		switch (HoldDashAngle)
		{
		case CPlayer::EDashAngle::ANGLE_LEFT:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
				(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_Y) && stickRatio.y >= RATIO_STICKBLINK))
			{// ��
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, playerIdx) ||
				(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_Y) && stickRatio.y <= -RATIO_STICKBLINK))
			{// ��
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			break;

		case CPlayer::EDashAngle::ANGLE_RIGHT:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
				(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_Y) && stickRatio.y >= RATIO_STICKBLINK))
			{// ��
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, playerIdx) ||
				(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_Y) && stickRatio.y <= -RATIO_STICKBLINK))
			{// ��
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_RIGHTDW);
				bNextInput = true;
			}
			break;

		case CPlayer::EDashAngle::ANGLE_UP:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
				(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_X) && stickRatio.x <= -RATIO_STICKBLINK))
			{// ��
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
				(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_X) && stickRatio.x >= RATIO_STICKBLINK))
			{// �E
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			break;

		case CPlayer::EDashAngle::ANGLE_DOWN:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
				(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_X) && stickRatio.x <= -RATIO_STICKBLINK))
			{// ��
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
				(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_X) && stickRatio.x >= RATIO_STICKBLINK))
			{// �E
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_RIGHTDW);
				bNextInput = true;
			}
			break;

		default:
			info = Trigger(player, HoldDashAngle);
			bNextInput = true;
			break;
		}

		// �󂯕t������Ă��
		if (bNextInput)
		{
			// �󂯕t���P�\�I��
			fInputInterval = 0.0f;
		}
	}
	SetHoldDashAngle(HoldDashAngle);		// �_�b�V�������ݒ�

	// �󂯕t���P�\���Z
	float oldTime = fInputInterval;
	fInputInterval -= fDeltaTime * fDeltaRate * fSlowRate;
	fInputInterval = UtilFunc::Transformation::Clamp(fInputInterval, 0.0f, INTERVAL_INPUT);

	// ���͂̎󂯕t���P�\�ݒ�
	SetInputInterval(fInputInterval);

	if (oldTime > 0.0f &&
		fInputInterval <= 0.0f)
	{// �O��܂��P�\�� && ���͏I��
		info = Trigger(player, HoldDashAngle);
	}

	//

	// �_�b�V������
	if (info.bDash && !bDash)
	{
		float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// ����
		MyLib::Vector3 rot = player->GetRotation();

		if (player->GetBall() == nullptr)
		{//�{�[�������ł͎g�p�s��

			MyLib::Vector3 move = player->GetMove();
			float velocityBlink = player->GetParameter().fVelocityBlink;
			
			// �ړ��ʍX�V
			move.x += sinf(division * info.eAngle + Camerarot.y) * velocityBlink;
			move.z += cosf(division * info.eAngle + Camerarot.y) * velocityBlink;

			if (player->IsJump())
			{//�W�����v���␳
				move.x *= BLINK_JUMP_COR;
				move.z *= BLINK_JUMP_COR;
			}

			// �ړ��ʐݒ�
			player->SetMove(move);

			// ���[�V�����ݒ�
			player->SetMotion(CPlayer::EMotion::MOTION_BLINK);
		}

		// �����ݒ�
		player->SetRotDest(info.eAngle * division + D3DX_PI + Camerarot.y);

		// �g���K�[�̃J�E���g���Z�b�g
		memset(nCntTrigger, 0, sizeof(nCntTrigger) * CPlayer::EDashAngle::ANGLE_MAX);

		// �g���K�[�̃J�E���g�ݒ�
		SetCntTrigger(nCntTrigger);

		// �_�b�V���t���O
		bDash = true;
	}

	// �g���K�[�̃C���^�[�o���擾
	float fTriggerInterval = GetTriggerInterval();

	// �g���K�[�̗P�\���炵�Ă���
	fTriggerInterval -= fDeltaTime * fDeltaRate * fSlowRate;
	if (fTriggerInterval <= 0.0f)
	{
		// �g���K�[�̃J�E���g���Z�b�g
		memset(nCntTrigger, 0, sizeof(nCntTrigger) * CPlayer::EDashAngle::ANGLE_MAX);

		// �g���K�[�̃J�E���g�ݒ�
		SetCntTrigger(nCntTrigger);
	}

	// �g���K�[�̃C���^�[�o���ݒ�
	SetTriggerInterval(fTriggerInterval);

	// �R���g���[���n
	SetBlink(bDash);							// ����t���O�ݒ�
}

//==========================================================================
// ����
//==========================================================================
void CPlayerUserControlMove::Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	bool bDash = IsBlink();	//����t���O�擾
	
	if (!bDash)
	{
		return;
	}

	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// ���[�V�����t���O�擾
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	// �v���C���[�ԍ��擾
	int playerIdx = player->GetMyPlayerIdx();

	// �S�Ă̕������̓`�F�b�N
	bool bUP = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) &&
		!pKey->GetPress(DIK_W);

	bool bDown = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) &&
		!pKey->GetPress(DIK_S);

	bool bRight = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) &&
		!pKey->GetPress(DIK_D);

	bool bLeft = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) &&
		!pKey->GetPress(DIK_A);

	// ���X�e�B�b�N����
	bool bStick = pPad->IsTipStickL(playerIdx, CInputGamepad::STICK_AXIS::STICK_X) || pPad->IsTipStickL(playerIdx, CInputGamepad::STICK_AXIS::STICK_Y);

	if (bUP && bDown && bRight && bLeft && !bStick)
	{// �_�b�V������
		bDash = false;

		if (player->GetMotion()->GetType() == CPlayer::EMotion::MOTION_RUN)
		{// �_�b�V������͔h��
			player->SetMotion(CPlayer::EMotion::MOTION_GRIP_FRONT);
		}
	}
	else
	{
		// �_�b�V���p��
		motionFrag.bMove = true;
		player->SetEnableDash(true);
	}

	// ���[�V�����t���O�ݒ�
	player->SetMotionFrag(motionFrag);
	
	SetBlink(bDash);	//����t���O�ݒ�
}

//==========================================================================
// �E�H�[�N
//==========================================================================
void CPlayerUserControlMove::Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// ���[�V�����t���O�擾
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	bool bDash = IsBlink();	//����t���O

	// ���݂̓��͕����J�E���^�[
	float fInputAngleCtr = GetInputAngleCtr();
	fInputAngleCtr -= fDeltaTime, fDeltaRate, fSlowRate;
	UtilFunc::Transformation::Clamp(fInputAngleCtr, 0.0f, INPUT_COUNTER);
	SetInputAngleCtr(fInputAngleCtr);

	// ���݂̓��͕���
	CPlayer::EDashAngle* pInputAngle = GetInputAngle();
	if (pInputAngle != nullptr && fInputAngleCtr <= 0.0f)
	{
		delete pInputAngle;
		pInputAngle = nullptr;
	}
	SetInputAngle(pInputAngle);

	CPlayer::EDashAngle eAngle;
	bool bInput = false;

	// �v���C���[�ԍ��擾
	int playerIdx = player->GetMyPlayerIdx();

	//--------------------------
	// ���E
	//--------------------------
	// �X�e�B�b�N
	bool bStick = 
		pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_AXIS::STICK_Y) ||
		pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_AXIS::STICK_Y);
	
	// �����L�[
	bool bAngleKey =
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, playerIdx) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) || 
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx);

	// WASD
	bool bWASD =
		pKey->GetTrigger(DIK_W) || pKey->GetTrigger(DIK_A) || pKey->GetTrigger(DIK_S) || pKey->GetTrigger(DIK_D);

	if (!motionFrag.bMove &&
		(bStick || bAngleKey || bWASD))
	{// �ړ����ĂȂ� && �ǂ��������ꂽ

		// ���E�t���O���]
		InverseFootLR();
	}



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

	if (!bInput)
	{
		// �ړ����Ȃ�
		motionFrag.bMove = false;

		// ���[�V�����t���O�ݒ�
		player->SetMotionFrag(motionFrag);
		return;
	}

	// �ړ����ɂ���
	motionFrag.bMove = true;

	// �ړ��ʎ擾
	float fMove = player->GetParameter().fVelocityNormal;
	if (bDash)
	{// �_�b�V���͕ύX
		fMove = player->GetParameter().fVelocityDash;
	}
	fMove *= fDeltaRate;
	fMove *= fSlowRate;

	// �ړ��ʍX�V
	MyLib::Vector3 move = player->GetMove();
	MyLib::Vector3 rot = player->GetRotation();

	float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// ����
	move.x += sinf(rot.y + (D3DX_PI * 1.0f)) * fMove;
	move.z += cosf(rot.y + (D3DX_PI * 1.0f)) * fMove;

	// �ړ��ʐݒ�
	player->SetMove(move);

	// �����ݒ�
	player->SetRotDest(eAngle * division + D3DX_PI + Camerarot.y);

	// ���[�V�����t���O�ݒ�
	player->SetMotionFrag(motionFrag);

	// ���݂̓��͕����ݒ�
	if (pInputAngle == nullptr)
	{
		pInputAngle = DEBUG_NEW CPlayer::EDashAngle;
	}
	*pInputAngle = eAngle;
	SetInputAngle(pInputAngle);

	// ���͕����J�E���^�[�ݒ�
	SetInputAngleCtr(INPUT_COUNTER);

	SetBlink(bDash);	//����t���O�ݒ�
}

//==========================================================================
// �g���K�[
//==========================================================================
CPlayer::SDashInfo CPlayerUserControlMove::Trigger(CPlayer* player, CPlayer::EDashAngle eAngle)
{
	CPlayer::SDashInfo info;
	info.bDash = false;
	
	float fTriggerInterval = GetTriggerInterval();	//�g���K�[�̃C���^�[�o��
	int* nCntTrigger = GetCntTrigger();				//�g���K�[�̃J�E���g

	if (nCntTrigger[eAngle] == 1)
	{// �_�b�V��
		info.bDash = true;
		info.eAngle = eAngle;

		// �A�N�V�����p�^�[���ύX
		if (player->GetBall() == nullptr)
		{
			player->GetActionPattern()->SetAction(CPlayer::EAction::ACTION_BLINK);
		}
	}

	// �g���K�[�̃C���^�[�o�����Z�b�g
	fTriggerInterval = TIME_INTERVAL;

	// �g���K�[�̃J�E���g
	nCntTrigger[eAngle] = (nCntTrigger[eAngle] + 1) % 2;

	//�R���g���[���n
	SetCntTrigger(nCntTrigger);				// �g���K�[�̃J�E���g
	SetTriggerInterval(fTriggerInterval);	// �g���K�[�̃C���^�[�o��

	return info;
}