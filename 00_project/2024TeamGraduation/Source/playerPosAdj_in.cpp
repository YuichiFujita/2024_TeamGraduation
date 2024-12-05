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
#include "EffekseerObj.h"

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

		// �ʒu�𔽉f
		pPlayer->SetPosition(pos);
	}

	if (pos.y <= CGameManager::FIELD_LIMIT)
	{ // �n�ʂ�艺�̏ꍇ

		// �n�ʂɒ��n������
		pos.y = CGameManager::FIELD_LIMIT;

		// �ʒu�𔽉f
		pPlayer->SetPosition(pos);

		if (bJump && !flagMotion.bDead)
		{ // �W�����v�����n

			// ���n���[�V�����̍Đ�
			pPlayer->SetMotion(CPlayer::EMotion::MOTION_LAND);

			// �G�t�F�N�g
			CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_LAND,
				pos,
				MyLib::Vector3(),	// ����
				MyLib::Vector3(),
				15.0f, true);
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

#if 1
		// �����Ƃ��Ƃ���
		CheckUnstable(pPlayer);
#endif

		// �`�[���R�[�g�ɖ߂�
		if (pPlayer->GetActionPattern()->GetAction() != CPlayer::EAction::ACTION_UNSTABLE)
		{// �����Ƃ��ƒ�����Ȃ�
			CheckReturn(pPlayer);
		}

	}
}

//==========================================================================
// �`�[���R�[�g���A
//==========================================================================
void CPlayerPosAdjIn::CheckReturn(CPlayer* pPlayer)
{
	CPlayer::EState state = pPlayer->GetState();	// ���

	// �����߂��Ă�ꍇ�͔�����
	if (state == CPlayer::EState::STATE_INVADE_RETURN
	||  state == CPlayer::EState::STATE_INVADE_TOSS
	||	state == CPlayer::EState::STATE_DMG) { return; }

	if (IsLineOut(pPlayer))
	{ // ���C���𒴂��Ă����ꍇ

		// ���A�ݒ�
		ReturnSetting(pPlayer);
	}
}

//==========================================================================
// �����Ƃ��Ƃ���
//==========================================================================
void CPlayerPosAdjIn::CheckUnstable(CPlayer* pPlayer)
{
	CMotion* motion = pPlayer->GetMotion();
	CPlayerAction* pAction = pPlayer->GetActionPattern();
	CPlayer::SMotionFrag motionFrag = pPlayer->GetMotionFrag();	// ���[�V�����t���O

	CPlayer::EAction action = pAction->GetAction();	// �A�N�V�������

	// �N������߂���
	if (pPlayer->GetState() == CPlayer::EState::STATE_INVADE_RETURN ||
		pPlayer->GetState() == CPlayer::EState::STATE_INVADE_TOSS)
	{
		// �A�N�V�����ݒ�
		action = CPlayer::EAction::ACTION_NONE;
		pAction->SetAction(action);
		return;
	}

	bool bBrake = pPlayer->IsBrake();							// �u���[�L�t���O
	MyLib::Vector3 move = pPlayer->GetMove();					// �ړ���

	// �ʃA�N�V������
	if (pAction->GetAction() != CPlayer::EAction::ACTION_NONE &&
		pAction->GetAction() != CPlayer::EAction::ACTION_UNSTABLE)
	{
		pPlayer->SetEnableAction(true);
		return;
	}

	if (pPlayer->GetState() != CPlayer::EState::STATE_DMG &&
		(IsUnstable(pPlayer) || action == CPlayer::EAction::ACTION_UNSTABLE))
	{ // �����Ƃ��ƃ��C���𒴂��Ă����ꍇ

		// �A�N�V�����ݒ�
		action = CPlayer::EAction::ACTION_UNSTABLE;

		// �u���[�L�t���O
		if (!bBrake)
		{
			move.x = 0.0f;
			bBrake = true;
		}
		else
		{
			move.x = 0.0f;
			pPlayer->SetEnableAction(true);
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

		// ���͏�
		EInputUnstable inputUnstable = IsInputLine(pPlayer);

		if (inputUnstable == EInputUnstable::INPUT_ENEMY)
		{// �G���ɓ��͂��ꂽ

			// ���A�ݒ�
			ReturnSetting(pPlayer);

			// �A�N�V�����ݒ�
			action = CPlayer::EAction::ACTION_NONE;
			pAction->SetAction(action);
		}
		else if (inputUnstable == EInputUnstable::INPUT_FRIEND)
		{// �������ɓ��͂��ꂽ

			// �t���O�Đݒ�
			bBrake = false;
			pPlayer->SetEnableAction(true);
			pPlayer->SetEnableMove(true);

			// �A�N�V�����ݒ�
			action = CPlayer::EAction::ACTION_NONE;
			pAction->SetAction(action);

			return;
		}

		// �㏑���h�~
		if (motion->GetType() == CPlayer::EMotion::MOTION_UNSTABLE)	return;

		// �����Ƃ��ƃ��[�V�����̍Đ�
		if (motion->GetType() != CPlayer::EMotion::MOTION_TOSS)
		{
			pPlayer->SetMotion(CPlayer::EMotion::MOTION_UNSTABLE);
		}

		// �A�N�V�����ݒ�
		action = CPlayer::EAction::ACTION_UNSTABLE;
	}
	else
	{
		bBrake = false;

		// �A�N�V�����ݒ�
		action = CPlayer::EAction::ACTION_NONE;
		pAction->SetAction(action);
	}

	// �ݒ�
	pPlayer->SetMove(move);
	pPlayer->SetEnableBrake(bBrake);
	
	// �A�N�V�����ݒ�
	pAction->SetAction(action);
}

//==========================================================================
// ���A�ݒ�
//==========================================================================
void CPlayerPosAdjIn::ReturnSetting(CPlayer* pPlayer)
{
	MyLib::Vector3 pos = pPlayer->GetPosition();	// �ʒu
	CBall* pBall = pPlayer->GetBall();				// �{�[�����

	// �m�b�N�o�b�N�̊J�n�ʒu��ݒ�
	CPlayer::SKnockbackInfo infoKnock = pPlayer->GetKnockBackInfo();	// �擾
	infoKnock.posStart = pos;				// �ʒu�ݒ�
	pPlayer->SetKnockBackInfo(infoKnock);	// ���f

	CGameManager::ETeamSide team = pPlayer->GetTeam();		// �`�[��
	
	CGameManager* pGameMgr = CGameManager::GetInstance();

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

		// ���e����
		pGameMgr->SubCharmValue(team, CCharmValueManager::ETypeSub::SUB_INVADE_RUN);
	}
}
