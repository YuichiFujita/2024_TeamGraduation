//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playercontrol.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "ball.h"
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
CPlayerControl::CPlayerControl()
{

}

//==========================================================================
// �A�N�V���������[�V�����̈ꊇ�ݒ�
//==========================================================================
void CPlayerControl::SetPattern(CPlayer* player, CPlayer::EMotion typeM, CPlayer::EAction typeA)
{
	player->SetMotion(typeM);
	player->GetActionPattern()->SetAction(typeA);
}
