//==========================================================================
// 
//  �v���C���[�ʒu�␳_������R�[�g���� [playerPosAdj_inLeft.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerAIPosAdj_inLeft.h"
#include "playerPosAdj_inLeft.h"
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
CPlayerAIPosAdjInLeft::CPlayerAIPosAdjInLeft()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerAIPosAdjInLeft::~CPlayerAIPosAdjInLeft()
{

}

//==========================================================================
// ���C���z���t���O�擾
//==========================================================================
bool CPlayerAIPosAdjInLeft::IsLineOut(CPlayer* pPlayer)
{
	// ���C�����z���Ă���ꍇ�t���OON��Ԃ�
	if (pPlayer->GetPosition().x > 0.0f) { return true; }

	// ���C�����z���Ă��Ȃ��ꍇ�t���OOFF��Ԃ�
	return false;
}

//==========================================================================
// �����Ƃ��ƃt���O�擾
//==========================================================================
bool CPlayerAIPosAdjInLeft::IsUnstable(CPlayer* pPlayer)
{
	// ���C�����z���Ă���ꍇ�t���OON��Ԃ�
	if (pPlayer->GetPosition().x > -UNSTABLE_LINE &&
		pPlayer->GetPosition().x <= 0.0f)
	{
		return true;
	}

	// ���C�����z���Ă��Ȃ��ꍇ�t���OOFF��Ԃ�
	return false;
}

//==========================================================================
// �����Ƃ��Ǝ����̓t���O�擾
//==========================================================================
CPlayerPosAdjIn::EInputUnstable CPlayerAIPosAdjInLeft::IsInputLine(CPlayer* pPlayer)
{
#if 0

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

#else
	return CPlayerPosAdjIn::EInputUnstable::INPUT_FRIEND;
#endif

	return CPlayerPosAdjIn::EInputUnstable::INPUT_NONE;
}
