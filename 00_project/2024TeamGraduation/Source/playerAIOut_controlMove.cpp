//=============================================================================
// 
//  �O��v���C���[�R���g���[������ [playerAIOut_controlMove.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playerAIOut_controlMove.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"

#include "motion.h"
#include "playerAction.h"
#include "playerBase.h"
#include "playerAIOut.h"

#include "playerManager.h"

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
CPlayerAIOutControlMove::CPlayerAIOutControlMove()
{
	m_bBlink = false;
	m_bDash = false;
	m_bWalk = false;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAIOutControlMove::~CPlayerAIOutControlMove()
{

}

//==========================================================================
// �u�����N
//==========================================================================
void CPlayerAIOutControlMove::Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���̓t���O
	bool bInput = false;
	// �_�b�V���t���O�擾
	bool bBlink = IsBlink();

	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// �_�b�V������
	if (!bBlink && m_bBlink)
	{
		float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// ����
		MyLib::Vector3 rot = player->GetRotation();
		if (player->GetBall() == nullptr)
		{//�{�[�������ł͎g�p�s��

			// �ړ��ʎ擾
			MyLib::Vector3 move = player->GetMove();
			float velocityBlink = player->GetParameter().fVelocityBlink;

			// �ړ��ʍX�V
			move.x += sinf(rot.y + (D3DX_PI * 1.0f)) * velocityBlink;
			move.z += cosf(rot.y + (D3DX_PI * 1.0f)) * velocityBlink;

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
		player->SetRotDest(rot.y + /*D3DX_PI +*/ Camerarot.y);

		// �_�b�V���t���O
		bBlink = true;
	}

	// �R���g���[���n
	SetBlink(bBlink);	// ����t���O�ݒ�
}

//==========================================================================
// ����
//==========================================================================
void CPlayerAIOutControlMove::Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	bool bBlink = IsBlink();	//����t���O�擾
	if (!bBlink)
	{
		return;
	}

	// ���[�V�����t���O�擾
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	if (!m_bDash)
	{// �_�b�V������
		bBlink = false;

		// �O�O���b�v
		int motionType = player->GetMotion()->GetType();
		if (motionType == CPlayer::EMotion::MOTION_RUN ||
			motionType == CPlayer::EMotion::MOTION_RUN_BALL)
		{// �_�b�V������͔h��
			player->SetMotion(CPlayer::EMotion::MOTION_GRIP_FRONT);
		}

		// ����t���O�ݒ�
		player->SetEnableDash(false);
	}
	else
	{
		// �_�b�V���p��
		motionFrag.bMove = true;
		player->SetEnableDash(true);
	}

	// ���[�V�����t���O�ݒ�
	player->SetMotionFrag(motionFrag);

	//����t���O�ݒ�
	SetBlink(bBlink);
}

//==========================================================================
// �E�H�[�N
//==========================================================================
void CPlayerAIOutControlMove::Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���[�V�����t���O�擾
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	if (!m_bWalk) {// �������I�t�̎����[�V�������I�t
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

	// TODO : �����ɉ����ăJ�j����

	if (player->GetBase()->IsCrab())
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
CPlayer::SDashInfo CPlayerAIOutControlMove::Trigger(CPlayer* player, CPlayer::EDashAngle eAngle)
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
