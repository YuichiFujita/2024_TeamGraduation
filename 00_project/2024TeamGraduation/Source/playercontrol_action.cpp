//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : Kai Takada
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
#include "teamStatus.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float TIME_THROWDROP = 10.0f / 60.0f;	// ����(�h���b�v�{�[��)�̗P�\

	const float TAPTIME = 0.10f;		// �^�b�v�̓��͎���
	const float TAPRATE_MIN = 0.7f;	// �^�b�v�̍ŏ�����
	const float TAPRATE_MAX = 1.0f;	// �^�b�v�̍ő劄��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerControlAction::CPlayerControlAction()
{
	m_fThrowDropTime = 0.0f;	// ����(�h���b�v�{�[��)�̗P�\
	m_bThrowDrop = false;		// ����(�h���b�v�{�[��)�\����
	m_bJumpTrigger = false;		// �W�����v�g���K�[
}

//==========================================================================
// ����
//==========================================================================
void CPlayerControlAction::Action(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���[�V�������擾
	CMotion* pMotion = player->GetMotion();
	int nMotionType = pMotion->GetType();

	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		(player->IsPossibleMove() || player->IsPossibleAction()))
	{// �ړ��\���[�V�����̎�

		//--------------------------
		// �A�N�V��������
		//--------------------------
		ConditionalAction(player, fDeltaTime, fDeltaRate, fSlowRate);
	}

	Special(player, fDeltaTime, fDeltaRate, fSlowRate);
	Charm(player, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ��{�A�N�V��������
//==========================================================================
void CPlayerControlAction::ConditionalAction(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ����(�h���b�v�{�[��)�̗P�\
	UpdateThrowDrop(fDeltaTime, fDeltaRate, fSlowRate);

	// �A�N�V�����擾
	CPlayerAction* pPlayerAction = player->GetActionPattern();
	if (pPlayerAction == nullptr) return; 
	CPlayer::EAction action = pPlayerAction->GetAction();

	if (action != CPlayer::EAction::ACTION_BLINK ||
		action != CPlayer::EAction::ACTION_DODGE)
	{
		Catch(player, fDeltaTime, fDeltaRate, fSlowRate);
		Throw(player, fDeltaTime, fDeltaRate, fSlowRate);
		Jump(player, fDeltaTime, fDeltaRate, fSlowRate);
		UserChange(player, fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// �X�y�V��������
//==========================================================================
void CPlayerControlAction::SpecialSetting(CPlayer* player, CBall* pBall, CTeamStatus* pTeamStatus)
{
	pBall->Special(player);

	// �X�y�V�����Q�[�W����
	pTeamStatus->ZeroSpecialValue();

	// �A�N�V�����p�^�[���ύX
	SetPattern(player, CPlayer::EMotion::MOTION_HYPE, CPlayer::EAction::ACTION_SPECIAL);
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CPlayerControlAction::ThrowSetting(CPlayer* player)
{
	// �A�N�V�����p�^�[���ύX
	if (player->IsJump())
	{
		SetPattern(player, CPlayer::EMotion::MOTION_THROW_JUMP, CPlayer::EAction::ACTION_THROW_JUMP);
	}
	else if (player->GetBase()->GetPlayerControlAction()->IsThrowDrop())
	{
		SetPattern(player, CPlayer::EMotion::MOTION_THROW_DROP, CPlayer::EAction::ACTION_THROW);
	}
	else if (player->IsDash())
	{
		SetPattern(player, CPlayer::EMotion::MOTION_THROW_RUN, CPlayer::EAction::ACTION_THROW);
	}
	else
	{
		SetPattern(player, CPlayer::EMotion::MOTION_THROW, CPlayer::EAction::ACTION_THROW);
	}
}

//==========================================================================
// �W�����v�ݒ�
//==========================================================================
void CPlayerControlAction::JumpSetting(CPlayer* player)
{
	// �W�����v����擾
	bool bJump = player->IsJump();

	// �ړ��ʎ擾
	MyLib::Vector3 move = player->GetMove();

	// ���[�V�������擾
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	// �W�����v�ݒ�
	bJump = true;
	motionFrag.bJump = true;
	move.y = player->GetParameter().fVelocityJump * TAPRATE_MIN;

	player->SetMove(move);
	player->SetMotionFrag(motionFrag);

	// �W�����v����ݒ�
	player->SetEnableJump(bJump);

	//�{�[����������
	CPlayer::EMotion motion = CPlayer::EMotion::MOTION_JUMP;

	if (player->GetBall() != nullptr)
	{
		motion = CPlayer::EMotion::MOTION_JUMP_BALL;
	}

	// �A�N�V�����p�^�[���ύX
	SetPattern(player, motion, CPlayer::EAction::ACTION_JUMP);

	// �T�E���h�Đ�
	//CSound::GetInstance()->PlaySound(CSound::LABEL_SE_JUMP);
}

//==========================================================================
// �W�����v�㏸
//==========================================================================
void CPlayerControlAction::JumpFloat(CPlayer* player)
{
	// �W�����v����擾
	bool bJump = player->IsJump();

	if (bJump && IsJumpTrigger())
	{// �W�����v���͉����Ă�ԋ����L�тĂ���

		// �C���v�b�g���擾
		CInputGamepad* pPad = CInputGamepad::GetInstance();

		//�W�����v����
		CInputGamepad::STapInfo tapInfo = pPad->GetTap(CInputGamepad::BUTTON_A, player->GetMyPlayerIdx(), TAPTIME);

		if (tapInfo.fRatio < 1.0f && pPad->GetPress(CInputGamepad::BUTTON_A, player->GetMyPlayerIdx()))
		{// �^�b�v�͈� && ���͌p��

			// �ړ��ʎ擾
			MyLib::Vector3 move = player->GetMove();

			float jumpRatio = TAPRATE_MIN + (TAPRATE_MAX - TAPRATE_MIN) * tapInfo.fRatio;
			move.y = player->GetParameter().fVelocityJump * jumpRatio;

			// �ړ��ʐݒ�
			player->SetMove(move);
		}
	}
}

//==========================================================================
// ����(�h���b�v�{�[��)�\
//==========================================================================
void CPlayerControlAction::SetThrowDrop()
{
	// ����(�h���b�v�{�[��)�̗P�\�ݒ�
	m_fThrowDropTime = TIME_THROWDROP;

	// �����̔���
	m_bThrowDrop = true;
}

//==========================================================================
// ����(�h���b�v�{�[��)�̗P�\
//==========================================================================
void CPlayerControlAction::UpdateThrowDrop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ����(�h���b�v�{�[��)�̗P�\���Z
	m_fThrowDropTime -= fDeltaTime * fDeltaRate * fSlowRate;
	m_fThrowDropTime = UtilFunc::Transformation::Clamp(m_fThrowDropTime - fDeltaTime * fDeltaRate * fSlowRate, 0.0f, TIME_THROWDROP);

	// �����̔���
	m_bThrowDrop = m_fThrowDropTime > 0.0f;
}
