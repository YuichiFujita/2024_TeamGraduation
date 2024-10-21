//=============================================================================
// 
//  �v���C���[AI�R���g���[��_�ړ����� [playerAIcontrol_move.cpp]
//  Author : ���c�E��
// 
//=============================================================================
#include "playerAIcontrol_move.h"
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
	const float MULTIPLIY_DASH = 1.5f;		// �_�b�V���̔{��
	const float INTERVAL_INPUT = (2.0f / 60.0f);	// ����
	const float TIME_INTERVAL = 0.3f;	// �_�b�V���P�\
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIControlMove::CPlayerAIControlMove()
{

}

//==========================================================================
// �ʏ�ړ�
//==========================================================================
void CPlayerAIControlMove::Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
		// �u�����N����
		//--------------------------
		if (action != CPlayer::Action::ACTION_BLINK)
		{
			Blink(player, fDeltaTime, fDeltaRate, fSlowRate);
		}

		// ����
		Walk(player, fDeltaTime, fDeltaRate, fSlowRate);

		Dash(player, fDeltaTime, fDeltaRate, fSlowRate);

		// �ړ����ɂ���
		motionFrag.bMove = true;


		// �W�����v�󋵎擾
		bool bJump = player->IsJump();

		if (motionFrag.bMove &&
			!bJump)
		{// �L�����Z���\ && �ړ���

			// ���[�V�����I������
			pMotion->ToggleFinish(true);

			// �ړ����[�V����
			if (player->IsDash())
			{
				pMotion->Set(CPlayer::MOTION::MOTION_RUN);
			}
			else
			{
				pMotion->Set(CPlayer::MOTION::MOTION_WALK);
			}
		}

	}

	// ���[�V�����t���O�ݒ�
	player->SetMotionFrag(motionFrag);
}

//==========================================================================
// �_�b�V��
//==========================================================================
void CPlayerAIControlMove::Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
	int* nCntTrigger = GetCntTrigger();						//�g���K�[�̃J�E���g�擾
	CPlayer::DashAngle HoldDashAngle = GetHoldDashAngle();	//�_�b�V�������擾
	float fInputInterval = GetInputInterval();				//���͂̎󂯕t���P�\�擾
	float fTriggerInterval = GetTriggerInterval();			//�g���K�[�̃C���^�[�o���擾
	bool bDash = GetDash();									//����t���O�擾

	if (fInputInterval <= 0.0f)
	{// �P�\�󂯕t���I����

		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y > 0 ||
				pKey->GetTrigger(DIK_W)))
		{// ��
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				pPad->GetStickMoveL(0).x < 0 ||
				pKey->GetPress(DIK_A))
			{// ����
				HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				pPad->GetStickMoveL(0).x > 0 ||
				pKey->GetPress(DIK_D))
			{// �E��
				HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTUP;
			}
			else
			{// ��
				HoldDashAngle = CPlayer::DashAngle::ANGLE_UP;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y < 0 ||
				pKey->GetTrigger(DIK_S)))
		{// ��
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				pPad->GetStickMoveL(0).x < 0 ||
				pKey->GetPress(DIK_A))
			{// ����
				HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTDW;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				pPad->GetStickMoveL(0).x > 0 ||
				pKey->GetPress(DIK_D))
			{// �E��
				HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTDW;
			}
			else
			{// ��
				HoldDashAngle = CPlayer::DashAngle::ANGLE_DOWN;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x > 0 ||
				pKey->GetTrigger(DIK_D)))
		{// �E
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				pPad->GetStickMoveL(0).y < 0 ||
				pKey->GetPress(DIK_W))
			{// �E��
				HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				pPad->GetStickMoveL(0).y > 0 ||
				pKey->GetPress(DIK_S))
			{// �E��
				HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTDW;
			}
			else
			{// �E
				HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHT;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x < 0 ||
				pKey->GetTrigger(DIK_A)))
		{// ��
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				pPad->GetStickMoveL(0).y < 0 ||
				pKey->GetPress(DIK_W))
			{// ����
				HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				pPad->GetStickMoveL(0).y > 0 ||
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
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y < 0))
			{// ��
				info = Trigger(player, CPlayer::DashAngle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y > 0))
			{// ��
				info = Trigger(player, CPlayer::DashAngle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			break;

		case CPlayer::DashAngle::ANGLE_RIGHT:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y < 0))
			{// ��
				info = Trigger(player, CPlayer::DashAngle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y > 0))
			{// ��
				info = Trigger(player, CPlayer::DashAngle::ANGLE_RIGHTDW);
				bNextInput = true;
			}
			break;

		case CPlayer::DashAngle::ANGLE_UP:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x < 0))
			{// ��
				info = Trigger(player, CPlayer::DashAngle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x > 0))
			{// �E
				info = Trigger(player, CPlayer::DashAngle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			break;

		case CPlayer::DashAngle::ANGLE_DOWN:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x < 0))
			{// ��
				info = Trigger(player, CPlayer::DashAngle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x > 0))
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

	// �󂯕t���P�\���Z
	float oldTime = fInputInterval;
	fInputInterval -= fDeltaTime * fDeltaRate * fSlowRate;
	fInputInterval = UtilFunc::Transformation::Clamp(fInputInterval, 0.0f, INTERVAL_INPUT);

	if (oldTime > 0.0f &&
		fInputInterval <= 0.0f)
	{// �O��܂��P�\�� && ���͏I��
		info = Trigger(player, HoldDashAngle);
	}

	// �_�b�V������
	if (info.bDash && !bDash)
	{
		MyLib::Vector3 move = player->GetMove();
		float division = (D3DX_PI * 2.0f) / CPlayer::DashAngle::ANGLE_MAX;	// ����
		move.x += sinf((D3DX_PI * 0.0f) + division * info.angle + Camerarot.y) * 15.0f;
		move.z += cosf((D3DX_PI * 0.0f) + division * info.angle + Camerarot.y) * 15.0f;

		// �ړ��ʐݒ�
		player->SetMove(move);

		// �g���K�[�̃J�E���g���Z�b�g
		memset(nCntTrigger, 0, sizeof(nCntTrigger));

		bDash = true;
	}

	// �g���K�[�̗P�\���炵�Ă���
	fTriggerInterval -= fDeltaTime * fDeltaRate * fSlowRate;
	if (fTriggerInterval <= 0.0f)
	{
		// �g���K�[�̃J�E���g���Z�b�g
		memset(nCntTrigger, 0, sizeof(nCntTrigger));
	}

	//�R���g���[���n
	SetCntTrigger(nCntTrigger);				//�g���K�[�̃J�E���g�ݒ�
	SetHoldDashAngle(HoldDashAngle);		//�_�b�V�������ݒ�
	SetInputInterval(fInputInterval);		//���͂̎󂯕t���P�\�ݒ�
	SetTriggerInterval(fTriggerInterval);	//�g���K�[�̃C���^�[�o���ݒ�
	SetDash(bDash);							//����t���O�ݒ�
}

//==========================================================================
// ����
//==========================================================================
void CPlayerAIControlMove::Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	bool bDash = GetDash();	//����t���O�擾

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

	bool bUP = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) &&
		!pKey->GetPress(DIK_W);

	bool bDown = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) &&
		!pKey->GetPress(DIK_S);

	bool bRight = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) &&
		!pKey->GetPress(DIK_D);

	bool bLeft = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) &&
		!pKey->GetPress(DIK_A);

	bool bStick = pPad->IsTipStick();

	if (bUP && bDown && bRight && bLeft && !bStick)
	{
		bDash = false;
	}

	SetDash(bDash);	//����t���O�ݒ�

	ImGui::Checkbox("UP", &bUP);
	ImGui::Checkbox("Down", &bDown);
	ImGui::Checkbox("Right", &bRight);
	ImGui::Checkbox("Left", &bLeft);
}

//==========================================================================
// �E�H�[�N
//==========================================================================
void CPlayerAIControlMove::Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	bool bDash = GetDash();	//����t���O

	CPlayer::DashAngle angle;
	bool bInput = false;

	if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
		pPad->GetStickMoveL(0).y > 0 ||
		pKey->GetPress(DIK_W))
	{// ��
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
			pPad->GetStickMoveL(0).x < 0 ||
			pKey->GetPress(DIK_A))
		{// ����
			angle = CPlayer::DashAngle::ANGLE_LEFTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
			pPad->GetStickMoveL(0).x > 0 ||
			pKey->GetPress(DIK_D))
		{// �E��
			angle = CPlayer::DashAngle::ANGLE_RIGHTUP;
		}
		else
		{// ��
			angle = CPlayer::DashAngle::ANGLE_UP;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
		pPad->GetStickMoveL(0).y < 0 ||
		pKey->GetPress(DIK_S))
	{// ��
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
			pPad->GetStickMoveL(0).x < 0 ||
			pKey->GetPress(DIK_A))
		{// ����
			angle = CPlayer::DashAngle::ANGLE_LEFTDW;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
			pPad->GetStickMoveL(0).x > 0 ||
			pKey->GetPress(DIK_D))
		{// �E��
			angle = CPlayer::DashAngle::ANGLE_RIGHTDW;
		}
		else
		{// ��
			angle = CPlayer::DashAngle::ANGLE_DOWN;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
		pPad->GetStickMoveL(0).x > 0 ||
		pKey->GetPress(DIK_D))
	{// �E
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
			pPad->GetStickMoveL(0).y < 0 ||
			pKey->GetPress(DIK_W))
		{// �E��
			angle = CPlayer::DashAngle::ANGLE_RIGHTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
			pPad->GetStickMoveL(0).y > 0 ||
			pKey->GetPress(DIK_S))
		{// �E��
			angle = CPlayer::DashAngle::ANGLE_RIGHTDW;
		}
		else
		{// �E
			angle = CPlayer::DashAngle::ANGLE_RIGHT;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
		pPad->GetStickMoveL(0).x < 0 ||
		pKey->GetPress(DIK_A))
	{// ��
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
			pPad->GetStickMoveL(0).y < 0 ||
			pKey->GetPress(DIK_W))
		{// ����
			angle = CPlayer::DashAngle::ANGLE_LEFTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
			pPad->GetStickMoveL(0).y > 0 ||
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
		return;
	}

	// �ړ��ʎ擾
	float fMove = player->GetParameter().fVelocityNormal;
	if (bDash)
	{// �_�b�V���͕ύX
		fMove = player->GetParameter().fVelocityDash;
	}
	fMove *= fDeltaRate;
	fMove *= fSlowRate;

	MyLib::Vector3 move = player->GetMove();
	float division = (D3DX_PI * 2.0f) / CPlayer::DashAngle::ANGLE_MAX;	// ����
	move.x += sinf((D3DX_PI * 0.0f) + division * angle + Camerarot.y) * fMove;
	move.z += cosf((D3DX_PI * 0.0f) + division * angle + Camerarot.y) * fMove;

	// �ړ��ʐݒ�
	player->SetMove(move);

	// �����ݒ�
	player->SetRotDest(angle * division + D3DX_PI + Camerarot.y);
}

//==========================================================================
// �g���K�[
//==========================================================================
CPlayer::SDashInfo CPlayerAIControlMove::Trigger(CPlayer* player, CPlayer::DashAngle angle)
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
	SetCntTrigger(nCntTrigger);				//�g���K�[�̃J�E���g
	SetTriggerInterval(fTriggerInterval);	//�g���K�[�̃C���^�[�o��

	return info;
}