//==========================================================================
// 
//  �v���C���[�ʒu�␳_�E����R�[�g���� [playerPosAdj_inRight.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerPosAdj_inRight.h"
#include "player.h"
#include "playerBase.h"
#include "playercontrol_move.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerPosAdjInRight::CPlayerPosAdjInRight()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerPosAdjInRight::~CPlayerPosAdjInRight()
{

}

//==========================================================================
// ���C���z���t���O�擾
//==========================================================================
bool CPlayerPosAdjInRight::IsLineOut(CPlayer* pPlayer)
{
	// ���C�����z���Ă���ꍇ�t���OON��Ԃ�
	if (pPlayer->GetPosition().x < 0.0f) { return true; }

	// ���C�����z���Ă��Ȃ��ꍇ�t���OOFF��Ԃ�
	return false;
}

//==========================================================================
// �����Ƃ��ƃt���O�擾
//==========================================================================
bool CPlayerPosAdjInRight::IsUnstable(CPlayer* pPlayer)
{
	// ���C�����z���Ă���ꍇ�t���OON��Ԃ�
	if (pPlayer->GetPosition().x < UNSTABLE_LINE &&
		pPlayer->GetPosition().x > 0.0f)
	{
		return true;
	}

	// ���C�����z���Ă��Ȃ��ꍇ�t���OOFF��Ԃ�
	return false;
}

//==========================================================================
// �����Ƃ��Ǝ����̓t���O�擾
//==========================================================================
bool CPlayerPosAdjInRight::IsInputLine(CPlayer* pPlayer)
{
	CPlayer::EDashAngle* angle = pPlayer->GetBase()->GetPlayerControlMove()->IsInputAngle();
	if (angle == nullptr) return false;

	// �E����͂��Ă�����true
	if (*angle == CPlayer::EDashAngle::ANGLE_LEFT)
	{
		return true;
	}

	return false;
}
