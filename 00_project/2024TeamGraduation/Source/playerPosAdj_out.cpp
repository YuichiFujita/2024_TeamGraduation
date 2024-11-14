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

// TODO
#include "3D_effect.h"

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

	// TODO�F�ʒu��␳
#if 0
	MyLib::Vector3 posSize = (posRight.Absolute() - posLeft.Absolute()) * 0.5f;
	posSize.z = posSize.y = 0.0f;

	MyLib::Vector3 posOrigin;
	D3DXVec3Lerp(&posOrigin, &posLeft, &posRight, 0.5f);

	UtilFunc::Collision::InBoxPillar(pos, posOrigin, posSize, posSize, VEC3_ZERO, VEC3_ZERO);

	CEffect3D::Create(pos,					VEC3_ZERO, MyLib::color::White(),  10.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);	// 
	CEffect3D::Create(posOrigin,			VEC3_ZERO, MyLib::color::Purple(), 10.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);	// 
	CEffect3D::Create(posOrigin + posSize,	VEC3_ZERO, MyLib::color::Green(),  10.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);	// 
	CEffect3D::Create(posOrigin - posSize,	VEC3_ZERO, MyLib::color::Green(),  10.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);	// 
#else
	// ���̕␳
	MyLib::Vector3 pposLeft = posLeft + MyLib::Vector3(1000.0f, 0.0f, 0.5f);
	MyLib::Vector3 pposRight = posRight + MyLib::Vector3(1000.0f, 0.0f, 0.5f);
	UtilFunc::Collision::CollisionLimitLine(pposLeft, pposRight, pos, pPlayer->GetOldPosition());

	// ��O�̕␳
	MyLib::Vector3 ppposLeft = posRight + MyLib::Vector3(-1000.0f, 0.0f, 0.0f);
	MyLib::Vector3 ppposRight = posLeft + MyLib::Vector3(-1000.0f, 0.0f, 0.0f);
	UtilFunc::Collision::CollisionLimitLine(ppposLeft, ppposRight, pos, pPlayer->GetOldPosition());

	// ���̕␳
	UtilFunc::Collision::CollisionLimitLine(posLeft - MyLib::Vector3(0.0f, 0.0f, 1000.0f), posLeft + MyLib::Vector3(0.0f, 0.0f, 1000.0f), pos, pPlayer->GetOldPosition());

	// �E�̕␳
	UtilFunc::Collision::CollisionLimitLine(posRight + MyLib::Vector3(0.0f, 0.0f, 1000.0f), posRight - MyLib::Vector3(0.0f, 0.0f, 1000.0f), pos, pPlayer->GetOldPosition());
#endif

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
