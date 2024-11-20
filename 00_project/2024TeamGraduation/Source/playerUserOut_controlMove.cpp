//=============================================================================
// 
//  �O��v���C���[�R���g���[������ [playerUserOut_controlMove.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playerUserOut_controlMove.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "playerAction.h"
#include "playerBase.h"
#include "playerUserOut.h"

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
CPlayerUserOutControlMove::CPlayerUserOutControlMove() :
	m_pLeftKey	(nullptr),	// ���ړ��L�[���
	m_pRightKey	(nullptr)	// �E�ړ��L�[���
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerUserOutControlMove::~CPlayerUserOutControlMove()
{
	// ���E�ړ��L�[�̔j��
	SAFE_DELETE(m_pLeftKey);
	SAFE_DELETE(m_pRightKey);
}

//==========================================================================
// �u�����N
//==========================================================================
void CPlayerUserOutControlMove::Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �O�샆�[�U�[�Ȃ̂Ƀ{�[�������ĂȂ�������G���[
	assert(player->GetBall() != nullptr);

	// �L�[���͂̃u�����N
	BilnkKey(player, fDeltaTime, fDeltaRate, fSlowRate);

	// �X�e�B�b�N���͂̃u�����N
	BilnkStick(player, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �L�[���͂̃u�����N
//==========================================================================
void CPlayerUserOutControlMove::BilnkKey(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �R���g���[���n�̎擾
	CPlayer::EDashAngle holdDashAngle = GetHoldDashAngle();	// �_�b�V������
	float fInputInterval = GetInputInterval();	// ���͂̎󂯕t���P�\
	int* pCntTrigger = GetCntTrigger();			// �g���K�[�J�E���g
	int nPadIdx = player->GetMyPlayerIdx();		// ���쌠�C���f�b�N�X

	//----------------------------------------------------------------------
	//	�����͂̎�t
	//----------------------------------------------------------------------
	if (fInputInterval <= 0.0f)
	{ // �P�\�󂯕t�����I�����̏ꍇ

		bool bInput = false;
		if (m_pLeftKey->IsTrigger(nPadIdx))
		{ // ���ړ��L�[�������ꂽ�ꍇ

			// ���蓖�Ă�ꂽ�L�[�̕������擾
			holdDashAngle = m_pLeftKey->GetAngle();
			bInput = true;
		}
		else if (m_pRightKey->IsTrigger(nPadIdx))
		{ // �E�ړ��L�[�������ꂽ�ꍇ

			// ���蓖�Ă�ꂽ�L�[�̕������擾
			holdDashAngle = m_pRightKey->GetAngle();
			bInput = true;
		}

		if (bInput)
		{ // �ړ����͂����m���ꂽ�ꍇ

			// ���͂̃C���^�[�o����ݒ�
			fInputInterval = INTERVAL_INPUT;
		}
	}

	// �_�b�V�������̔��f
	SetHoldDashAngle(holdDashAngle);

	//----------------------------------------------------------------------
	//	��t�P�\���Ԃ̌o��
	//----------------------------------------------------------------------
	CPlayer::SDashInfo infoDash;		// �_�b�V�����
	float fOldTime = fInputInterval;	// �ߋ��̎�t�P�\���� 

	// �󂯕t���P�\�̌��Z
	fInputInterval -= fDeltaTime * fSlowRate;
	fInputInterval = UtilFunc::Transformation::Clamp(fInputInterval, 0.0f, INTERVAL_INPUT);

	// ���͎󂯕t���P�\�̔��f
	SetInputInterval(fInputInterval);

	if (fOldTime > 0.0f && fInputInterval <= 0.0f)
	{ // �O�񂪂܂��P�\�����A���݂͌o�ߏI�����Ă���ꍇ

		// �u�����N���̔���
		infoDash = Trigger(player, holdDashAngle);	// �_�b�V�����̎擾
	}

	//----------------------------------------------------------------------
	//	�u�����N�̔���
	//----------------------------------------------------------------------
	if (infoDash.bDash && !IsBlink())
	{ // �u�����N���J�n�ł���ꍇ

		const float fDivision = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// ����
		if (player->GetBall() == nullptr)
		{ // �{�[�������ł͎g�p�s��

			MyLib::Vector3 move = player->GetMove();	// �ړ���
			const float fVeloBlink = player->GetParameter().fVelocityBlink;	// �u�����N���x

			// �ړ��ʂ̍X�V
			move.x += sinf(fDivision * infoDash.eAngle) * fVeloBlink;
			move.z += cosf(fDivision * infoDash.eAngle) * fVeloBlink;

			if (player->IsJump())
			{ // �W�����v���̏ꍇ

				// �ړ��ʂ�␳
				move.x *= BLINK_JUMP_COR;
				move.z *= BLINK_JUMP_COR;
			}

			// �ړ��ʂ̔��f
			player->SetMove(move);

			// �u�����N���[�V�������Đ�
			player->SetMotion(CPlayer::EMotion::MOTION_BLINK);
		}

		// �ڕW�����̐ݒ�
		player->SetRotDest(infoDash.eAngle * fDivision + D3DX_PI);

		// �g���K�[�J�E���g�̃��Z�b�g
		memset(pCntTrigger, 0, sizeof(pCntTrigger) * CPlayer::EDashAngle::ANGLE_MAX);

		// �g���K�[�J�E���g�̐ݒ�
		SetCntTrigger(pCntTrigger);

		// �_�b�V�����Ă����Ԃɂ���
		SetBlink(true);
	}

	//----------------------------------------------------------------------
	//	�g���K�[�C���^�[�o���̌o��
	//----------------------------------------------------------------------
	float fTriggerInterval = GetTriggerInterval();	// �g���K�[�C���^�[�o��

	// �g���K�[�C���^�[�o���̌��Z
	fTriggerInterval -= fDeltaTime * fSlowRate;
	if (fTriggerInterval <= 0.0f)
	{ // �g���K�[�C���^�[�o�����I�������ꍇ

		// �g���K�[�J�E���g�̃��Z�b�g
		memset(pCntTrigger, 0, sizeof(pCntTrigger) * CPlayer::EDashAngle::ANGLE_MAX);

		// �g���K�[�J�E���g�̐ݒ�
		SetCntTrigger(pCntTrigger);
	}

	// �g���K�[�C���^�[�o���̔��f
	SetTriggerInterval(fTriggerInterval);
}

//==========================================================================
// �X�e�B�b�N���͂̃u�����N
//==========================================================================
void CPlayerUserOutControlMove::BilnkStick(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#if 0
	// ���̓t���O
	bool bInput = false;

	// �_�b�V������
	CPlayer::SDashInfo info;

	// �C���v�b�g���擾
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// �R���g���[���n�擾
	int* nCntTrigger = GetCntTrigger();						// �g���K�[�̃J�E���g�擾
	CPlayer::EDashAngle HoldDashAngle = GetHoldDashAngle();	// �_�b�V�������擾
	bool bBlink = IsBlink();								// �u�����N�t���O�擾

	// �v���C���[�ԍ��擾
	int playerIdx = player->GetMyPlayerIdx();

	// L�X�e�B�b�N�̊���
	MyLib::Vector3 stickRatio = pPad->GetStickPositionRatioL(playerIdx);

	//--------------------------
	// �X�e�B�b�N
	//--------------------------
	bool bTipStick = false;
	if (pPad->GetStickMoveL(playerIdx).y > 0)
	{// ��
		bTipStick = true;

		if (pPad->GetStickMoveL(playerIdx).x < 0)
		{// ����
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
		}
		else if (pPad->GetStickMoveL(playerIdx).x > 0)
		{// �E��
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
		}
		else
		{// ��
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_UP;
		}
	}
	else if (pPad->GetStickMoveL(playerIdx).y < 0)
	{// ��
		bTipStick = true;

		if (pPad->GetStickMoveL(playerIdx).x < 0)
		{// ����
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTDW;
		}
		else if (pPad->GetStickMoveL(playerIdx).x > 0)
		{// �E��
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
		}
		else
		{// ��
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_DOWN;
		}
	}
	else if (pPad->GetStickMoveL(playerIdx).x > 0)
	{// �E
		bTipStick = true;

		if (pPad->GetStickMoveL(playerIdx).y < 0)
		{// �E��
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
		}
		else if (pPad->GetStickMoveL(playerIdx).y > 0)
		{// �E��
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
		}
		else
		{// �E
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHT;
		}
	}
	else if (pPad->GetStickMoveL(playerIdx).x < 0)
	{// ��
		bTipStick = true;

		if (pPad->GetStickMoveL(playerIdx).y < 0)
		{// ����
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
		}
		else if (pPad->GetStickMoveL(playerIdx).y > 0)
		{// ����
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTDW;
		}
		else
		{// ��
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFT;
		}
	}

	if (bTipStick && pPad->GetTrigger(CInputGamepad::BUTTON_LSTICKPUSH, playerIdx))
	{// �X�e�B�b�N�|�� && ��������
		bInput = true;
		info = Trigger(player, HoldDashAngle);
		info = Trigger(player, HoldDashAngle);
	}
	SetHoldDashAngle(HoldDashAngle);	// �_�b�V�������ݒ�

	// �_�b�V������
	if (info.bDash && !bBlink)
	{
		float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// ����
		MyLib::Vector3 rot = player->GetRotation();
		if (player->GetBall() == nullptr)
		{//�{�[�������ł͎g�p�s��

			// �ړ��ʎ擾
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
		bBlink = true;
	}

	// �R���g���[���n
	SetBlink(bBlink);	// ����t���O�ݒ�
#endif
}

//==========================================================================
// ����
//==========================================================================
void CPlayerUserOutControlMove::Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayer::SMotionFrag flagMotion = player->GetMotionFrag();	// ���[�V�����t���O
	bool bDash = IsBlink();	// �_�b�V���t���O
	if (!bDash) { return; }	// �_�b�V�����Ă��Ȃ��ꍇ������

	int nPadIdx = player->GetMyPlayerIdx();	// ���쌠�C���f�b�N�X
	bool bKeyInput = (m_pLeftKey->IsPress(nPadIdx) || m_pRightKey->IsPress(nPadIdx));	// �L�[���͌��m
	if (!bKeyInput)
	{ // �ړ����͂����m����Ȃ������ꍇ

		// �_�b�V������������
		bDash = false;
	}
	else
	{ // �ړ����͂����m���ꂽ�ꍇ

		// �_�b�V�����p������
		flagMotion.bMove = true;
		player->SetEnableDash(true);
	}

	// ���[�V�����t���O���f
	player->SetMotionFrag(flagMotion);
	
	// �_�b�V���t���O���f
	SetBlink(bDash);
}

//==========================================================================
// �E�H�[�N
//==========================================================================
void CPlayerUserOutControlMove::Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerBase* pBase = player->GetBase();						// �v���C���[�x�[�X���
	CPlayerOut* pPlayerOut = pBase->GetPlayerOut();				// �v���C���[�O����
	MyLib::Vector3 posLeft = pPlayerOut->GetPosLeft();			// �ړ��\�ȍ��ʒu
	MyLib::Vector3 posRight = pPlayerOut->GetPosRight();		// �ړ��\�ȉE�ʒu
	MyLib::Vector3 playerMove = player->GetMove();				// �v���C���[�ړ��x�N�g��
	CPlayer::SMotionFrag flagMotion = player->GetMotionFrag();	// ���[�V�����t���O

	int nPadIdx = player->GetMyPlayerIdx();	// ���쌠�C���f�b�N�X
	bool bKeyInput = (m_pLeftKey->IsTrigger(nPadIdx) || m_pRightKey->IsTrigger(nPadIdx));	// �L�[���͌��m
	if (!flagMotion.bMove && bKeyInput)
	{ // �ړ����ĂȂ����A���͂����m�����ꍇ

		// ���E�t���O���]
		player->InverseFootLR();
	}

	// �ړ������x�N�g���̍쐬
	MyLib::Vector3 vecMove = posRight - posLeft;	// �x�N�g���̍쐬
	vecMove = vecMove.Normal();						// �x�N�g���̐��K��

	// �ړ��ʂ̍쐬
	CCharacterStatus::CharParameter param = player->GetParameter();					// �v���C���[�p�����[�^
	float fMoveValue = (IsBlink()) ? param.fVelocityDash : param.fVelocityNormal;	// �ړ���
	fMoveValue *= fDeltaRate * fSlowRate;	// �o�ߎ��ԏ�Z

	// �ړ�����
	if (m_pLeftKey->IsPress(nPadIdx))
	{ // ���ړ������m���ꂽ�ꍇ

		// �x�N�g���t�����Ɉړ��ʂ�^����
		playerMove += -vecMove * fMoveValue;
	}
	else if (m_pRightKey->IsPress(nPadIdx))
	{ // �E�ړ������m���ꂽ�ꍇ

		// �x�N�g�������Ɉړ��ʂ�^����
		playerMove += vecMove * fMoveValue;
	}
	else
	{ // ���삪���m���ꂽ�ꍇ

		// �ړ����Ȃ���Ԃɂ���
		flagMotion.bMove = false;

		// ���[�V�����t���O���f
		player->SetMotionFrag(flagMotion);
		return;
	}

	// �ړ����ɂ���
	flagMotion.bMove = true;

	// ���[�V�����t���O���f
	player->SetMotionFrag(flagMotion);

	// �ړ��ʔ��f
	player->SetMove(playerMove);

	// �ڕW�����ݒ�
	player->SetRotDest(posLeft.AngleXZ(posRight) + D3DX_PI * 0.5f);
}

//==========================================================================
// �g���K�[
//==========================================================================
CPlayer::SDashInfo CPlayerUserOutControlMove::Trigger(CPlayer* player, CPlayer::EDashAngle eAngle)
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
