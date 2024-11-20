//==========================================================================
// 
//  �v���C���[�ʒu�␳_����R�[�g���� [playerPosAdj_in.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerPosAdj_in.h"
#include "player.h"
#include "playerStatus.h"
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
CPlayerPosAdjIn::CPlayerPosAdjIn()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerPosAdjIn::~CPlayerPosAdjIn()
{

}

//==========================================================================
// ����
//==========================================================================
void CPlayerPosAdjIn::UpdateAdjuster(CPlayer* pPlayer)
{
	CPlayer::SMotionFrag flagMotion = pPlayer->GetMotionFrag();	// ���[�V�����t���O
	MyLib::Vector3 pos = pPlayer->GetPosition();	// �ʒu
	CPlayer::EState state = pPlayer->GetState();	// ���
	bool bJump = pPlayer->IsJump();					// �W�����v�t���O

	if (state != CPlayer::EState::STATE_OUTCOURT
	&&  state != CPlayer::EState::STATE_OUTCOURT_RETURN)
	{ // �R�[�g�z����ԈȊO�̓R�[�g���ɕ␳

		// �R�[�g���ɕ␳
		CGameManager::GetInstance()->SetPosLimit(pos);
	}

	if (pos.y <= CGameManager::FIELD_LIMIT)
	{ // �n�ʂ�艺�̏ꍇ

		// �n�ʂɒ��n������
		pos.y = CGameManager::FIELD_LIMIT;

		if (bJump && !flagMotion.bDead)
		{ // �W�����v�����n

			// ���n���[�V�����̍Đ�
			pPlayer->SetMotion(CPlayer::EMotion::MOTION_LAND);
		}

		// �d�͂̏�����
		MyLib::Vector3 move = pPlayer->GetMove();
		move.y = 0.0f;
		pPlayer->SetMove(move);

		// �W�����v���Ă��Ȃ���Ԃɂ���
		pPlayer->SetEnableJump(false);
	}

	if (!pPlayer->IsJump() && !flagMotion.bDead)
	{ // ����R�[�g�ɐN�������Ƃ��̓R�[�g���ɕ␳

		// �`�[���R�[�g�ɖ߂�
		CheckReturn(pPlayer);

#if 0
		// �����Ƃ��Ƃ���
		CheckUnstable(pPlayer);
#endif
	}

	// �ʒu�𔽉f
	pPlayer->SetPosition(pos);
}

//==========================================================================
// �`�[���R�[�g���A
//==========================================================================
void CPlayerPosAdjIn::CheckReturn(CPlayer* pPlayer)
{
	MyLib::Vector3 pos = pPlayer->GetPosition();	// �ʒu
	CPlayer::EState state = pPlayer->GetState();	// ���
	CBall* pBall = pPlayer->GetBall();				// �{�[�����

	// �����߂��Ă�ꍇ�͔�����
	if (state == CPlayer::EState::STATE_INVADE_RETURN
	||  state == CPlayer::EState::STATE_INVADE_TOSS) { return; }

	if (IsLineOut(pPlayer))
	{ // ���C���𒴂��Ă����ꍇ

		// �m�b�N�o�b�N�̊J�n�ʒu��ݒ�
		CPlayer::SKnockbackInfo infoKnock = pPlayer->GetKnockBackInfo();	// �擾
		infoKnock.posStart = pos;				// �ʒu�ݒ�
		pPlayer->SetKnockBackInfo(infoKnock);	// ���f

		if (pBall == nullptr)
		{ // �{�[���������Ă��Ȃ��ꍇ

			// �N������߂��Ԃɂ���
			pPlayer->SetState(CPlayer::EState::STATE_INVADE_RETURN);
		}
		else
		{ // �{�[���������Ă�ꍇ

			// �g�X���[�V�����̍Đ�
			pPlayer->SetMotion(CPlayer::EMotion::MOTION_TOSS);

			// �g�X��Ԃɂ���
			pPlayer->SetState(CPlayer::EState::STATE_INVADE_TOSS);
		}
	}
}

//==========================================================================
// �����Ƃ��Ƃ���
//==========================================================================
void CPlayerPosAdjIn::CheckUnstable(CPlayer* pPlayer)
{
	// �N������߂���
	if (pPlayer->GetState() == CPlayer::EState::STATE_INVADE_RETURN) return;

	CMotion* motion = pPlayer->GetMotion();
	CPlayerAction* pAction = pPlayer->GetActionPattern();
	CPlayer::SMotionFrag motionFrag = pPlayer->GetMotionFrag();	// ���[�V�����t���O

	CPlayer::EAction action = CPlayer::EAction::ACTION_NONE;	// �A�N�V�������

	bool bBrake = pPlayer->IsBrake();						// �u���[�L�t���O
	MyLib::Vector3 move = pPlayer->GetMove();	// �ړ���

	if (IsUnstable(pPlayer))
	{ // �����Ƃ��ƃ��C���𒴂��Ă����ꍇ

		// �u���[�L�t���O
		if (!bBrake)
		{
			bBrake = true;
		}
		else
		{
			move.x = 0.0f;
			move.z = 0.0f;
			pPlayer->SetEnableMove(false);
		}

		if (motionFrag.bMove)
		{// �ړ����Ȃ�

			// �ݒ�
			pPlayer->SetMove(move);
			pPlayer->SetEnableBrake(bBrake);
			pAction->SetAction(action);
			return;
		}

		// �G���ɓ��͂��ꂽ
		if (IsInputLine(pPlayer))
		{
			// �`�[���R�[�g�ɖ߂�
			CheckReturn(pPlayer);
		}

		// �㏑���h�~
		if (motion->GetType() == CPlayer::EMotion::MOTION_UNSTABLE)	return;

		// �����Ƃ��ƃ��[�V�����̍Đ�
		pPlayer->SetMotion(CPlayer::EMotion::MOTION_UNSTABLE);

		// �A�N�V�����ݒ�
		action = CPlayer::EAction::ACTION_UNSTABLE;
		pAction->SetAction(action);
	}
	else
	{
		bBrake = false;
	}

	// �ݒ�
	pPlayer->SetMove(move);
	pPlayer->SetEnableBrake(bBrake);
}
