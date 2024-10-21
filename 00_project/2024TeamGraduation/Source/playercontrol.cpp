//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playercontrol.h"
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
CPlayerControl::CPlayerControl()
{

}

//==========================================================================
// �A�N�V���������[�V�����̈ꊇ�ݒ�
//==========================================================================
void CPlayerControl::SetPattern(CPlayer* player, CPlayer::MOTION typeM, CPlayer::Action typeA)
{
	CMotion* pMotion = player->GetMotion();

	pMotion->Set(CPlayer::MOTION::MOTION_CATCH);
	player->GetActionPattern()->SetAction(CPlayer::Action::ACTION_CATCH);
}

//==========================================================================
// �W�����v�ݒ�
//==========================================================================
void CPlayerControl::SetJump(CPlayer* player)
{
	bool bJump = player->IsJump();

	if (bJump)
	{
		return;
	}

	// �ړ��ʎ擾
	MyLib::Vector3 move = player->GetMove();

	// ���[�V�������擾
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	bJump = true;
	motionFrag.bJump = true;
	move.y += 12.0f;

	player->SetMove(move);
	player->SetMotionFrag(motionFrag);

	// �W�����v����ݒ�
	player->SetEnableJump(bJump);

	// �A�N�V�����p�^�[���ύX
	SetPattern(player, CPlayer::MOTION::MOTION_JUMP, CPlayer::Action::ACTION_JUMP);

	// �T�E���h�Đ�
	//CSound::GetInstance()->PlaySound(CSound::LABEL_SE_JUMP);
}
