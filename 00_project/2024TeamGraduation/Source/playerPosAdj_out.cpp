//==========================================================================
// 
//  �v���C���[�ʒu�␳_�O��R�[�g���� [playerPosAdj_out.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerPosAdj_out.h"
#include "player.h"
#include "playerStatus.h"
#include "playerBase.h"
#include "playerUserOut.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerPosAdjOut::CPlayerPosAdjOut()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerPosAdjOut::~CPlayerPosAdjOut()
{

}

//==========================================================================
// ����
//==========================================================================
void CPlayerPosAdjOut::UpdateAdjuster(CPlayer* pPlayer)
{
	CPlayer::SMotionFrag flagMotion = pPlayer->GetMotionFrag();	// ���[�V�����t���O
	MyLib::Vector3 pos = pPlayer->GetPosition();	// �ʒu
	CPlayer::EState state = pPlayer->GetState();	// ���
	bool bJump = pPlayer->IsJump();					// �W�����v�t���O

	CPlayerBase* pBase = pPlayer->GetBase();				// �v���C���[�x�[�X���
	CPlayerUserOut* pPlayerOut = pBase->GetPlayerUserOut();	// �v���C���[�O����
	MyLib::Vector3 posLeft = pPlayerOut->GetPosLeft();		// �ړ��\�ȍ��ʒu
	MyLib::Vector3 posRight = pPlayerOut->GetPosRight();	// �ړ��\�ȉE�ʒu

	// Z���W��␳
	pos.z = posLeft.z;

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
