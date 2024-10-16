//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol_move.cpp]
//  Author : ���n�Ή�
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
	const float MULTIPLIY_DASH = 1.5f;		// �_�b�V���̔{��
	const float INTERVAL_INPUT = (2.0f / 60.0f);	// ����
	const float TIME_INTERVAL = 0.3f;	// �_�b�V���P�\
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerControlMove::CPlayerControlMove()
{
	memset(m_nCntTrigger, 0, sizeof(m_nCntTrigger));	// �g���K�[�̃J�E���g
	m_HoldDashAngle = CPlayer::DashAngle::ANGLE_UP;		// �ێ����Ă�_�b�V���̈ړ�����
	m_fInputInterval = 0.0f;							// ���͂̎󂯕t���P�\
	m_fTriggerInterval = 0.0f;							// �g���K�[�̃C���^�[�o��
	m_bDash = false;
}

//==========================================================================
// �ʏ�ړ�
//==========================================================================
void CPlayerControlMove::Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �_�b�V������
	bool bDash = false;

	// �ړ��ʎ擾
	float fMove = player->GetVelocity();
	if (bDash){
		// �_�b�V���{���|����
		fMove *= MULTIPLIY_DASH;
	}

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
			if (bDash)
			{
				//pMotion->Set(CPlayer::MOTION::MOTION_DASH);
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
void CPlayerControlMove::Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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

	if (m_fInputInterval <= 0.0f)
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
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				pPad->GetStickMoveL(0).x > 0 ||
				pKey->GetPress(DIK_D))
			{// �E��
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTUP;
			}
			else
			{// ��
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_UP;
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
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTDW;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				pPad->GetStickMoveL(0).x > 0 ||
				pKey->GetPress(DIK_D))
			{// �E��
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTDW;
			}
			else
			{// ��
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_DOWN;
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
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				pPad->GetStickMoveL(0).y > 0 ||
				pKey->GetPress(DIK_S))
			{// �E��
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTDW;
			}
			else
			{// �E
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHT;
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
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				pPad->GetStickMoveL(0).y > 0 ||
				pKey->GetPress(DIK_S))
			{// ����
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTDW;
			}
			else
			{// ��
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFT;
			}
		}
	}


	if (bInput)
	{// ���͂��ꂽ

		// ���͂̃C���^�[�o��
		m_fInputInterval = INTERVAL_INPUT;
	}

	if (m_fInputInterval > 0.0f)
	{// �󂯕t���P�\��

		// ���̓��͔���
		bool bNextInput = false;
		switch (m_HoldDashAngle)
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
			info = Trigger(player, m_HoldDashAngle);
			bNextInput = true;
			break;
		}

		// �󂯕t������Ă��
		if (bNextInput)
		{
			// �󂯕t���P�\�I��
			m_fInputInterval = 0.0f;
		}
	}

	// �󂯕t���P�\���Z
	float oldTime = m_fInputInterval;
	m_fInputInterval -= fDeltaTime * fDeltaRate * fSlowRate;
	m_fInputInterval = UtilFunc::Transformation::Clamp(m_fInputInterval, 0.0f, INTERVAL_INPUT);

	if (oldTime > 0.0f &&
		m_fInputInterval <= 0.0f)
	{// �O��܂��P�\�� && ���͏I��
		info = Trigger(player, m_HoldDashAngle);
	}

	// �_�b�V������
	if (info.bDash && !m_bDash)
	{
		MyLib::Vector3 move = player->GetMove();
		float division = (D3DX_PI * 2.0f) / CPlayer::DashAngle::ANGLE_MAX;	// ����
		move.x += sinf((D3DX_PI * 0.0f) + division * info.angle + Camerarot.y) * 15.0f;
		move.z += cosf((D3DX_PI * 0.0f) + division * info.angle + Camerarot.y) * 15.0f;

		// �ړ��ʐݒ�
		player->SetMove(move);

		// �g���K�[�̃J�E���g���Z�b�g
		memset(m_nCntTrigger, 0, sizeof(m_nCntTrigger));

		m_bDash = true;
	}

	// �g���K�[�̗P�\���炵�Ă���
	m_fTriggerInterval -= fDeltaTime * fDeltaRate * fSlowRate;
	if (m_fTriggerInterval <= 0.0f)
	{
		// �g���K�[�̃J�E���g���Z�b�g
		memset(m_nCntTrigger, 0, sizeof(m_nCntTrigger));
	}
}

//==========================================================================
// ����
//==========================================================================
void CPlayerControlMove::Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!m_bDash)
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
		m_bDash = false;
	}

	ImGui::Checkbox("UP", &bUP);
	ImGui::Checkbox("Down", &bDown);
	ImGui::Checkbox("Right", &bRight);
	ImGui::Checkbox("Left", &bLeft);
}

//==========================================================================
// �E�H�[�N
//==========================================================================
void CPlayerControlMove::Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

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
	float fMove = player->GetVelocity();
	fMove *= fDeltaRate;
	fMove *= fSlowRate;

	if (m_bDash)
	{
		fMove *= 1.5f;
	}

	MyLib::Vector3 move = player->GetMove();
	float division = (D3DX_PI * 2.0f) / CPlayer::DashAngle::ANGLE_MAX;	// ����
	move.x += sinf((D3DX_PI * 0.0f) + division * angle + Camerarot.y) * fMove ;
	move.z += cosf((D3DX_PI * 0.0f) + division * angle + Camerarot.y) * fMove;

	// �ړ��ʐݒ�
	player->SetMove(move);

	// �����ݒ�
	player->SetRotDest(angle * division + D3DX_PI + Camerarot.y);
}

//==========================================================================
// �g���K�[
//==========================================================================
CPlayer::SDashInfo CPlayerControlMove::Trigger(CPlayer* player, CPlayer::DashAngle angle)
{
	CPlayer::SDashInfo info;
	info.bDash = false;

	if (m_nCntTrigger[angle] == 1)
	{// �_�b�V��
		info.bDash = true;
		info.angle = angle;

		// �A�N�V�����p�^�[���ύX
		player->GetActionPattern()->SetAction(CPlayer::Action::ACTION_BLINK);
	}

	// �g���K�[�̃C���^�[�o�����Z�b�g
	m_fTriggerInterval = TIME_INTERVAL;

	// �g���K�[�̃J�E���g
	m_nCntTrigger[angle] = (m_nCntTrigger[angle] + 1) % 2;

	return info;
}