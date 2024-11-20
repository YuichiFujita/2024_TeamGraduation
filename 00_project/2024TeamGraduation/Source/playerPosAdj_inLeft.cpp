//==========================================================================
// 
//  �v���C���[�ʒu�␳_������R�[�g���� [playerPosAdj_inLeft.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerPosAdj_inLeft.h"
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
CPlayerPosAdjInLeft::CPlayerPosAdjInLeft()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerPosAdjInLeft::~CPlayerPosAdjInLeft()
{

}

//==========================================================================
// ���C���z���t���O�擾
//==========================================================================
bool CPlayerPosAdjInLeft::IsLineOut(CPlayer* pPlayer)
{
	// ���C�����z���Ă���ꍇ�t���OON��Ԃ�
	if (pPlayer->GetPosition().x > 0.0f) { return true; }

	// ���C�����z���Ă��Ȃ��ꍇ�t���OOFF��Ԃ�
	return false;
}

//==========================================================================
// �����Ƃ��ƃt���O�擾
//==========================================================================
bool CPlayerPosAdjInLeft::IsUnstable(CPlayer* pPlayer)
{
	// ���C�����z���Ă���ꍇ�t���OON��Ԃ�
	if (pPlayer->GetPosition().x > -UNSTABLE_LINE &&
		pPlayer->GetPosition().x < 0.0f)
	{
		return true;
	}

	// ���C�����z���Ă��Ȃ��ꍇ�t���OOFF��Ԃ�
	return false;
}

//==========================================================================
// �����Ƃ��Ǝ����̓t���O�擾
//==========================================================================
bool CPlayerPosAdjInLeft::IsInputLine(CPlayer* pPlayer)
{
	CPlayer::EDashAngle* angle = pPlayer->GetBase()->GetPlayerControlMove()->IsInputAngle();
	if (angle == nullptr) return false;

	// �E����͂��Ă�����true
	if (*angle == CPlayer::EDashAngle::ANGLE_RIGHT)
	{
		return true;
	}

	return false;
}
