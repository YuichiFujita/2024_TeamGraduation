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
	m_bBlink = false;
	m_bDash = false;
	m_bWalk = false;
	m_fClabDirection = 0.0f;
}

//==========================================================================
// �_�b�V��
//==========================================================================
void CPlayerAIControlMove::Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!m_bBlink) { return; }

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
	CPlayer::EDashAngle HoldDashAngle = GetHoldDashAngle();	//�_�b�V�������擾
	float fInputInterval = GetInputInterval();				//���͂̎󂯕t���P�\�擾
	float fTriggerInterval = GetTriggerInterval();			//�g���K�[�̃C���^�[�o���擾
	bool bDash = IsBlink();									//����t���O�擾

	if (fInputInterval <= 0.0f)
	{// �P�\�󂯕t���I����

		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
			(pPad->GetLStickTrigger(0, CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y > 0 ||
				pKey->GetTrigger(DIK_W)))
		{// ��
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				pPad->GetStickMoveL(0).x < 0 ||
				pKey->GetPress(DIK_A))
			{// ����
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				pPad->GetStickMoveL(0).x > 0 ||
				pKey->GetPress(DIK_D))
			{// �E��
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
			}
			else
			{// ��
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_UP;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
			(pPad->GetLStickTrigger(0, CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y < 0 ||
				pKey->GetTrigger(DIK_S)))
		{// ��
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				pPad->GetStickMoveL(0).x < 0 ||
				pKey->GetPress(DIK_A))
			{// ����
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTDW;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				pPad->GetStickMoveL(0).x > 0 ||
				pKey->GetPress(DIK_D))
			{// �E��
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
			}
			else
			{// ��
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_DOWN;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
			(pPad->GetLStickTrigger(0, CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x > 0 ||
				pKey->GetTrigger(DIK_D)))
		{// �E
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				pPad->GetStickMoveL(0).y < 0 ||
				pKey->GetPress(DIK_W))
			{// �E��
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				pPad->GetStickMoveL(0).y > 0 ||
				pKey->GetPress(DIK_S))
			{// �E��
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
			}
			else
			{// �E
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHT;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
			(pPad->GetLStickTrigger(0, CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x < 0 ||
				pKey->GetTrigger(DIK_A)))
		{// ��
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				pPad->GetStickMoveL(0).y < 0 ||
				pKey->GetPress(DIK_W))
			{// ����
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				pPad->GetStickMoveL(0).y > 0 ||
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

	if (fInputInterval > 0.0f)
	{// �󂯕t���P�\��

		// ���̓��͔���
		bool bNextInput = false;
		switch (HoldDashAngle)
		{
		case CPlayer::EDashAngle::ANGLE_LEFT:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				(pPad->GetLStickTrigger(0, CInputGamepad::STICK_Y) && pPad->IsTipStickL(0, CInputGamepad::STICK_AXIS::STICK_Y)))
			{// ��
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				(pPad->GetLStickTrigger(0, CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y > 0))
			{// ��
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			break;

		case CPlayer::EDashAngle::ANGLE_RIGHT:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				(pPad->GetLStickTrigger(0, CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y < 0))
			{// ��
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				(pPad->GetLStickTrigger(0, CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y > 0))
			{// ��
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_RIGHTDW);
				bNextInput = true;
			}
			break;

		case CPlayer::EDashAngle::ANGLE_UP:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				(pPad->GetLStickTrigger(0, CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x < 0))
			{// ��
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				(pPad->GetLStickTrigger(0, CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x > 0))
			{// �E
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			break;

		case CPlayer::EDashAngle::ANGLE_DOWN:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				(pPad->GetLStickTrigger(0, CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x < 0))
			{// ��
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				(pPad->GetLStickTrigger(0, CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x > 0))
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
		float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// ����
		move.x += sinf((D3DX_PI * 0.0f) + division * info.eAngle + Camerarot.y) * 15.0f;
		move.z += cosf((D3DX_PI * 0.0f) + division * info.eAngle + Camerarot.y) * 15.0f;

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
	SetBlink(bDash);							//����t���O�ݒ�
}

//==========================================================================
// ����
//==========================================================================
void CPlayerAIControlMove::Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	bool bBlink = IsBlink();	//����t���O�擾
	if (!bBlink)
	{
		return;
	}

	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	SetBlink(bBlink);	//����t���O�ݒ�
}

//==========================================================================
// �E�H�[�N
//==========================================================================
void CPlayerAIControlMove::Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{	
	// ���[�V�����t���O�擾
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	if (!m_bWalk) {
		motionFrag.bMove = false;
		return; 
	}

	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	motionFrag.bMove = true;	// ���[�V�����t���O(����)

	bool bDash = IsBlink();	//����t���O

	// �ړ��ʎ擾
	float fMove = player->GetParameter().fVelocityNormal;
	if (bDash)
	{// �_�b�V���͕ύX
		fMove = player->GetParameter().fVelocityDash;
	}
	fMove *= fDeltaRate;
	fMove *= fSlowRate;

	// �ړ��ʍX�V
	MyLib::Vector3 move = {};
	move = player->GetMove();

	// �����擾
	MyLib::Vector3 rot = player->GetRotation();
	
	float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// ����

	// TODO : �����ɉ����ăJ�j����

	if (player->IsCrab())
	{// �J�j����
		move.x += sinf(m_fClabDirection + (D3DX_PI * 1.0f)) * fMove;
		move.z += cosf(m_fClabDirection + (D3DX_PI * 1.0f)) * fMove;
	}
	else
	{
		move.x += sinf(rot.y + (D3DX_PI * 1.0f)) * fMove;
		move.z += cosf(rot.y + (D3DX_PI * 1.0f)) * fMove;
	}

	// �ړ��ʐݒ�
	player->SetMove(move);

	// ���[�V�����t���O�ݒ�
	player->SetMotionFrag(motionFrag);
}

//==========================================================================
// �g���K�[
//==========================================================================
CPlayer::SDashInfo CPlayerAIControlMove::Trigger(CPlayer* player, CPlayer::EDashAngle eAngle)
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
		player->GetActionPattern()->SetAction(CPlayer::EAction::ACTION_BLINK);
	}

	// �g���K�[�̃C���^�[�o�����Z�b�g
	fTriggerInterval = TIME_INTERVAL;

	// �g���K�[�̃J�E���g
	nCntTrigger[eAngle] = (nCntTrigger[eAngle] + 1) % 2;

	//�R���g���[���n
	SetCntTrigger(nCntTrigger);				//�g���K�[�̃J�E���g
	SetTriggerInterval(fTriggerInterval);	//�g���K�[�̃C���^�[�o��

	return info;
}