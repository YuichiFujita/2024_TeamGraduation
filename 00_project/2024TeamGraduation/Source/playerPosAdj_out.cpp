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
#if 1
	CPlayerBase* pBase = pPlayer->GetBase();				// �v���C���[�x�[�X���
	CPlayerOut* pPlayerOut = pBase->GetPlayerOut();			// �v���C���[�O����
	MyLib::Vector3 posCur = pPlayer->GetPosition();			// �v���C���[���݈ʒu
	MyLib::Vector3 posOld = pPlayer->GetOldPosition();		// �v���C���[�ߋ��ʒu
	MyLib::Vector3 posLeft = pPlayerOut->GetPosLeft();		// �ړ��\�ȍ��ʒu
	MyLib::Vector3 posRight = pPlayerOut->GetPosRight();	// �ړ��\�ȉE�ʒu
	MyLib::Vector3 vecMove = posRight - posLeft;			// �v���C���[�ړ��x�N�g��
	float fAngleMove = vecMove.AngleXZ(VEC3_ZERO);			// �v���C���[�ړ��p�x

	// �v���C���[���猩�ĉ����̈ʒu�␳
	{
		MyLib::Vector3 posFarLeft = posLeft;
		posFarLeft.x += sinf(fAngleMove - HALF_PI) * 0.5f;
		posFarLeft.z += cosf(fAngleMove - HALF_PI) * 0.5f;
		posFarLeft.x += sinf(fAngleMove) * 1000.0f;
		posFarLeft.z += cosf(fAngleMove) * 1000.0f;

		MyLib::Vector3 posFarRight = posRight;
		posFarRight.x += sinf(fAngleMove - HALF_PI) * 0.5f;
		posFarRight.z += cosf(fAngleMove - HALF_PI) * 0.5f;
		posFarRight.x += sinf(fAngleMove) * 1000.0f;
		posFarRight.z += cosf(fAngleMove) * 1000.0f;

		// ���̕␳
		UtilFunc::Collision::CollisionLimitLine(posFarLeft, posFarRight, posCur, posOld);
	}

	// �v���C���[���猩�Ď�O���̈ʒu�␳
	{
		MyLib::Vector3 posNearLeft = posRight;
		posNearLeft.x -= sinf(fAngleMove - HALF_PI) * 0.5f;
		posNearLeft.z -= cosf(fAngleMove - HALF_PI) * 0.5f;
		posNearLeft.x -= sinf(fAngleMove) * 1000.0f;
		posNearLeft.z -= cosf(fAngleMove) * 1000.0f;

		MyLib::Vector3 posNearRight = posLeft;
		posNearRight.x -= sinf(fAngleMove - HALF_PI) * 0.5f;
		posNearRight.z -= cosf(fAngleMove - HALF_PI) * 0.5f;
		posNearRight.x -= sinf(fAngleMove) * 1000.0f;
		posNearRight.z -= cosf(fAngleMove) * 1000.0f;

		// ��O�̕␳
		UtilFunc::Collision::CollisionLimitLine(posNearLeft, posNearRight, posCur, posOld);
	}

	// �v���C���[���猩�č����̈ʒu�␳
	{
		MyLib::Vector3 posLeftLeft = posLeft;
		posLeftLeft.x += sinf(fAngleMove + HALF_PI) * 1000.0f;
		posLeftLeft.z += cosf(fAngleMove + HALF_PI) * 1000.0f;

		MyLib::Vector3 posLeftRight = posLeft;
		posLeftRight.x += sinf(fAngleMove - HALF_PI) * 1000.0f;
		posLeftRight.z += cosf(fAngleMove - HALF_PI) * 1000.0f;

		// ���̕␳
		UtilFunc::Collision::CollisionLimitLine(posLeftLeft, posLeftRight, posCur, pPlayer->GetOldPosition());
	}

	// �v���C���[���猩�ĉE���̈ʒu�␳
	{
		MyLib::Vector3 posRightLeft = posRight;
		posRightLeft.x += sinf(fAngleMove - HALF_PI) * 1000.0f;
		posRightLeft.z += cosf(fAngleMove - HALF_PI) * 1000.0f;

		MyLib::Vector3 posRightRight = posRight;
		posRightRight.x += sinf(fAngleMove + HALF_PI) * 1000.0f;
		posRightRight.z += cosf(fAngleMove + HALF_PI) * 1000.0f;

		// �E�̕␳
		UtilFunc::Collision::CollisionLimitLine(posRightLeft, posRightRight, posCur, pPlayer->GetOldPosition());
	}
#else
	// �x�[�X���̎擾
	CPlayerBase* pBase = pPlayer->GetBase();		// �v���C���[�x�[�X���
	CPlayerOut* pPlayerOut = pBase->GetPlayerOut();	// �v���C���[�O����

	// ���W���̎擾
	MyLib::Vector3 posCur = pPlayer->GetPosition();			// �v���C���[���݈ʒu
	MyLib::Vector3 posLeft = pPlayerOut->GetPosLeft();		// �ړ��\�ȍ��ʒu
	MyLib::Vector3 posRight = pPlayerOut->GetPosRight();	// �ړ��\�ȉE�ʒu

	// �␳���W�̌v�Z
	MyLib::Vector3 vecToLeft = posCur - posLeft;		// �v���C���[�ƍ��ʒu�Ԃ̃x�N�g��
	MyLib::Vector3 vecMove = posRight - posLeft;		// �v���C���[�ړ��x�N�g��
	//float fRate = D3DXVec3Dot(&vecMove, &vecLine);	// �ړ��x�N�g���̕␳����
	float fRate = (vecMove.x * vecToLeft.x) + (vecMove.z * vecToLeft.z);	// TODO�F��`�Ԉ���ĂȂ��ˁH���ς̈ʒu�␳
	if (fRate > 1.0f)
	{
		posCur.x = posRight.x;
		posCur.z = posRight.z;
	}
	else if (fRate < 0.0f)
	{
		posCur.x = posLeft.x;
		posCur.z = posLeft.z;
	}
	else
	{
		posCur.x = vecMove.x * fRate;
		posCur.z = vecMove.z * fRate;
	}
#endif

	if (posCur.y <= CGameManager::FIELD_LIMIT)
	{ // �n�ʂ�艺�̏ꍇ

		// �n�ʂɒ��n������
		posCur.y = CGameManager::FIELD_LIMIT;

		if (pPlayer->IsJump() && !pPlayer->GetMotionFrag().bDead)
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
	pPlayer->SetPosition(posCur);
}
