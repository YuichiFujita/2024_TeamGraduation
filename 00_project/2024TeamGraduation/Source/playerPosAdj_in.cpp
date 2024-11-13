//==========================================================================
// 
//  �v���C���[�ʒu�␳_����R�[�g���� [playerPosAdj_in.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerPosAdj_in.h"
#include "player.h"
#include "playerStatus.h"

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

	if (!bJump && !flagMotion.bDead)
	{ // ����R�[�g�ɐN�������Ƃ��̓R�[�g���ɕ␳

		// �`�[���R�[�g�ɖ߂�
		ReturnTeamCourt(pPlayer, pos);
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

	// �ʒu�𔽉f
	pPlayer->SetPosition(pos);
}

//==========================================================================
// �`�[���R�[�g���A
//==========================================================================
void CPlayerPosAdjIn::ReturnTeamCourt(CPlayer* pPlayer, const MyLib::Vector3& rPos)
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
