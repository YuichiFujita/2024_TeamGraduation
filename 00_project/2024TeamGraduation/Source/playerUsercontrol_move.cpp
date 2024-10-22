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
	const float TIME_INTERVAL = 0.3f;	// �_�b�V���P�\
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerUserControlMove::CPlayerUserControlMove()
{

}

//==========================================================================
// �ʏ�ړ�
//==========================================================================
void CPlayerUserControlMove::Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
		// �ړ�����
		//--------------------------
		if (action != CPlayer::Action::ACTION_BLINK &&
			action != CPlayer::Action::ACTION_DODGE)
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

		if (player->GetMotionFrag().bMove && 
			pMotion->IsGetCancelable() &&
			!bJump)
		{// �L�����Z���\ && �ړ���

			// ���[�V�����L�����Z��
			pMotion->ToggleFinish(true);

		 //TODO: �����̗]���L�����Z���Ƃ��p ToggleFinish�͕K�v(���[�V�����o������)
		
		}
	}
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
	CPlayer::DashAngle HoldDashAngle = GetHoldDashAngle();	// �_�b�V�������擾
	float fInputInterval = GetInputInterval();				// ���͂̎󂯕t���P�\�擾
	bool bDash = IsBlink();									// ����t���O�擾

	if (fInputInterval <= 0.0f)
	{// �P�\�󂯕t���I����

		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).y > 0 ||
				pKey->GetTrigger(DIK_W)))
		{// ��
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) ||
				pPad->GetStickMoveL(player->GetMyPlayerIdx()).x < 0 ||
				pKey->GetPress(DIK_A))
			{// ����
				HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) ||
				pPad->GetStickMoveL(player->GetMyPlayerIdx()).x > 0 ||
				pKey->GetPress(DIK_D))
			{// �E��
				HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTUP;
			}
			else
			{// ��
				HoldDashAngle = CPlayer::DashAngle::ANGLE_UP;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).y < 0 ||
				pKey->GetTrigger(DIK_S)))
		{// ��
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) ||
				pPad->GetStickMoveL(player->GetMyPlayerIdx()).x < 0 ||
				pKey->GetPress(DIK_A))
			{// ����
				HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTDW;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) ||
				pPad->GetStickMoveL(player->GetMyPlayerIdx()).x > 0 ||
				pKey->GetPress(DIK_D))
			{// �E��
				HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTDW;
			}
			else
			{// ��
				HoldDashAngle = CPlayer::DashAngle::ANGLE_DOWN;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).x > 0 ||
				pKey->GetTrigger(DIK_D)))
		{// �E
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) ||
				pPad->GetStickMoveL(player->GetMyPlayerIdx()).y < 0 ||
				pKey->GetPress(DIK_W))
			{// �E��
				HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				pPad->GetStickMoveL(player->GetMyPlayerIdx()).y > 0 ||
				pKey->GetPress(DIK_S))
			{// �E��
				HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTDW;
			}
			else
			{// �E
				HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHT;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).x < 0 ||
				pKey->GetTrigger(DIK_A)))
		{// ��
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) ||
				pPad->GetStickMoveL(player->GetMyPlayerIdx()).y < 0 ||
				pKey->GetPress(DIK_W))
			{// ����
				HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) ||
				pPad->GetStickMoveL(player->GetMyPlayerIdx()).y > 0 ||
				pKey->GetPress(DIK_S))
			{// ����
				HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTDW;
			}
			else
			{// ��
				HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFT;
			}
		}
	}


	if (bInput)
	{// ���͂��ꂽ

		// ���͂̃C���^�[�o��
		fInputInterval = INTERVAL_INPUT;
	}

	if (fInputInterval > 0.0f)
	{// �󂯕t���P�\��

		// ���̓��͔���
		bool bNextInput = false;
		switch (HoldDashAngle)
		{
		case CPlayer::DashAngle::ANGLE_LEFT:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).y < 0))
			{// ��
				info = Trigger(player, CPlayer::DashAngle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).y > 0))
			{// ��
				info = Trigger(player, CPlayer::DashAngle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			break;

		case CPlayer::DashAngle::ANGLE_RIGHT:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).y < 0))
			{// ��
				info = Trigger(player, CPlayer::DashAngle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).y > 0))
			{// ��
				info = Trigger(player, CPlayer::DashAngle::ANGLE_RIGHTDW);
				bNextInput = true;
			}
			break;

		case CPlayer::DashAngle::ANGLE_UP:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).x < 0))
			{// ��
				info = Trigger(player, CPlayer::DashAngle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).x > 0))
			{// �E
				info = Trigger(player, CPlayer::DashAngle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			break;

		case CPlayer::DashAngle::ANGLE_DOWN:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).x < 0))
			{// ��
				info = Trigger(player, CPlayer::DashAngle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).x > 0))
			{// �E
				info = Trigger(player, CPlayer::DashAngle::ANGLE_RIGHTDW);
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

	// �_�b�V������
	if (info.bDash && !bDash)
	{
		MyLib::Vector3 move = player->GetMove();
		float velocityBlink = player->GetParameter().fVelocityBlink;
		float division = (D3DX_PI * 2.0f) / CPlayer::DashAngle::ANGLE_MAX;	// ����
		MyLib::Vector3 rot = player->GetRotation();

		move.x += sinf(division * info.angle + Camerarot.y) * velocityBlink;
		move.z += cosf(division * info.angle + Camerarot.y) * velocityBlink;

		// �ړ��ʐݒ�
		player->SetMove(move);

		// �g���K�[�̃J�E���g���Z�b�g
		memset(nCntTrigger, 0, sizeof(nCntTrigger) * CPlayer::DashAngle::ANGLE_MAX);

		// �g���K�[�̃J�E���g�ݒ�
		SetCntTrigger(nCntTrigger);

		// �_�b�V���t���O
		bDash = true;

		// ���[�V�����ݒ�
		player->SetMotion(CPlayer::MOTION::MOTION_BLINK);
	}

	// �g���K�[�̃C���^�[�o���擾
	float fTriggerInterval = GetTriggerInterval();

	// �g���K�[�̗P�\���炵�Ă���
	fTriggerInterval -= fDeltaTime * fDeltaRate * fSlowRate;
	if (fTriggerInterval <= 0.0f)
	{
		// �g���K�[�̃J�E���g���Z�b�g
		memset(nCntTrigger, 0, sizeof(nCntTrigger) * CPlayer::DashAngle::ANGLE_MAX);

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

	// �S�Ă̕������̓`�F�b�N
	bool bUP = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) &&
		!pKey->GetPress(DIK_W);

	bool bDown = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) &&
		!pKey->GetPress(DIK_S);

	bool bRight = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) &&
		!pKey->GetPress(DIK_D);

	bool bLeft = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) &&
		!pKey->GetPress(DIK_A);

	// �X�e�B�b�N�̓|�ꔻ��
	bool bStick = pPad->IsTipStick();

	if (bUP && bDown && bRight && bLeft && !bStick)
	{// �_�b�V������
		bDash = false;

		if (player->GetMotion()->GetType() == CPlayer::MOTION::MOTION_RUN)
		{// �_�b�V������͔h��
			player->SetMotion(CPlayer::MOTION::MOTION_GRIP_FRONT);
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

	CPlayer::DashAngle angle;
	bool bInput = false;

	if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) ||
		pPad->GetStickMoveL(player->GetMyPlayerIdx()).y > 0 ||
		pKey->GetPress(DIK_W))
	{// ��
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) ||
			pPad->GetStickMoveL(player->GetMyPlayerIdx()).x < 0 ||
			pKey->GetPress(DIK_A))
		{// ����
			angle = CPlayer::DashAngle::ANGLE_LEFTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) ||
			pPad->GetStickMoveL(player->GetMyPlayerIdx()).x > 0 ||
			pKey->GetPress(DIK_D))
		{// �E��
			angle = CPlayer::DashAngle::ANGLE_RIGHTUP;
		}
		else
		{// ��
			angle = CPlayer::DashAngle::ANGLE_UP;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) ||
		pPad->GetStickMoveL(player->GetMyPlayerIdx()).y < 0 ||
		pKey->GetPress(DIK_S))
	{// ��
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) ||
			pPad->GetStickMoveL(player->GetMyPlayerIdx()).x < 0 ||
			pKey->GetPress(DIK_A))
		{// ����
			angle = CPlayer::DashAngle::ANGLE_LEFTDW;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) ||
			pPad->GetStickMoveL(player->GetMyPlayerIdx()).x > 0 ||
			pKey->GetPress(DIK_D))
		{// �E��
			angle = CPlayer::DashAngle::ANGLE_RIGHTDW;
		}
		else
		{// ��
			angle = CPlayer::DashAngle::ANGLE_DOWN;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) ||
		pPad->GetStickMoveL(player->GetMyPlayerIdx()).x > 0 ||
		pKey->GetPress(DIK_D))
	{// �E
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) ||
			pPad->GetStickMoveL(player->GetMyPlayerIdx()).y < 0 ||
			pKey->GetPress(DIK_W))
		{// �E��
			angle = CPlayer::DashAngle::ANGLE_RIGHTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) ||
			pPad->GetStickMoveL(player->GetMyPlayerIdx()).y > 0 ||
			pKey->GetPress(DIK_S))
		{// �E��
			angle = CPlayer::DashAngle::ANGLE_RIGHTDW;
		}
		else
		{// �E
			angle = CPlayer::DashAngle::ANGLE_RIGHT;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) ||
		pPad->GetStickMoveL(player->GetMyPlayerIdx()).x < 0 ||
		pKey->GetPress(DIK_A))
	{// ��
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) ||
			pPad->GetStickMoveL(player->GetMyPlayerIdx()).y < 0 ||
			pKey->GetPress(DIK_W))
		{// ����
			angle = CPlayer::DashAngle::ANGLE_LEFTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) ||
			pPad->GetStickMoveL(player->GetMyPlayerIdx()).y > 0 ||
			pKey->GetPress(DIK_S))
		{// ����
			angle = CPlayer::DashAngle::ANGLE_LEFTDW;
		}
		else
		{// ��
			angle = CPlayer::DashAngle::ANGLE_LEFT;
		}
	}

	if (!bInput)
	{
		// �ړ����ɂ���
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

	float division = (D3DX_PI * 2.0f) / CPlayer::DashAngle::ANGLE_MAX;	// ����
	move.x += sinf(rot.y + (D3DX_PI * 1.0f)) * fMove;
	move.z += cosf(rot.y + (D3DX_PI * 1.0f)) * fMove;

	// �ړ��ʐݒ�
	player->SetMove(move);

	// �����ݒ�
	player->SetRotDest(angle * division + D3DX_PI + Camerarot.y);

	// ���[�V�����t���O�ݒ�
	player->SetMotionFrag(motionFrag);

	SetBlink(bDash);	//����t���O�ݒ�
}

//==========================================================================
// �g���K�[
//==========================================================================
CPlayer::SDashInfo CPlayerUserControlMove::Trigger(CPlayer* player, CPlayer::DashAngle angle)
{
	CPlayer::SDashInfo info;
	info.bDash = false;
	
	float fTriggerInterval = GetTriggerInterval();	//�g���K�[�̃C���^�[�o��
	int* nCntTrigger = GetCntTrigger();				//�g���K�[�̃J�E���g

	if (nCntTrigger[angle] == 1)
	{// �_�b�V��
		info.bDash = true;
		info.angle = angle;

		// �A�N�V�����p�^�[���ύX
		player->GetActionPattern()->SetAction(CPlayer::Action::ACTION_BLINK);
	}

	// �g���K�[�̃C���^�[�o�����Z�b�g
	fTriggerInterval = TIME_INTERVAL;

	// �g���K�[�̃J�E���g
	nCntTrigger[angle] = (nCntTrigger[angle] + 1) % 2;

	//�R���g���[���n
	SetCntTrigger(nCntTrigger);				// �g���K�[�̃J�E���g
	SetTriggerInterval(fTriggerInterval);	// �g���K�[�̃C���^�[�o��

	return info;
}