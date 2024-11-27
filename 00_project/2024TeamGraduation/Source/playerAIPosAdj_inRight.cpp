//==========================================================================
// 
//  �v���C���[�ʒu�␳_�E����R�[�g���� [playerPosAdj_inRight.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerAIPosAdj_inRight.h"
#include "playerPosAdj_inRight.h"
#include "player.h"
#include "playerBase.h"
#include "playercontrol_move.h"
#include "input_gamepad.h"
#include "input_keyboard.h"
#include "manager.h"
#include "camera.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerAIPosAdjInRight::CPlayerAIPosAdjInRight()
{
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAIPosAdjInRight::~CPlayerAIPosAdjInRight()
{

}

//==========================================================================
// ���C���z���t���O�擾
//==========================================================================
bool CPlayerAIPosAdjInRight::IsLineOut(CPlayer* pPlayer)
{
	// ���C�����z���Ă���ꍇ�t���OON��Ԃ�
	if (pPlayer->GetPosition().x < 0.0f) { return true; }

	// ���C�����z���Ă��Ȃ��ꍇ�t���OOFF��Ԃ�
	return false;
}

//==========================================================================
// �����Ƃ��ƃt���O�擾
//==========================================================================
bool CPlayerAIPosAdjInRight::IsUnstable(CPlayer* pPlayer)
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
CPlayerPosAdjIn::EInputUnstable CPlayerAIPosAdjInRight::IsInputLine(CPlayer* pPlayer)
{
	// �ړ��ʂ̎擾
	MyLib::Vector3 move = pPlayer->GetMove();

	// �E����͂��Ă�����true
	if (move.x > 0.0f)
	{
		return CPlayerPosAdjIn::EInputUnstable::INPUT_ENEMY;
	}
	else if (move.x < 0.0f)
	{
		return CPlayerPosAdjIn::EInputUnstable::INPUT_FRIEND;
	}

	return CPlayerPosAdjIn::EInputUnstable::INPUT_NONE;
}
