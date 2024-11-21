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
CPlayerPosAdjIn::EInputUnstable CPlayerPosAdjInLeft::IsInputLine(CPlayer* pPlayer)
{
	// �C���v�b�g���擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �v���C���[�ԍ��擾
	int playerIdx = pPlayer->GetMyPlayerIdx();
	
	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// ���͕������擾
	CPlayer::EDashAngle* angle = pPlayer->GetBase()->GetPlayerControlMove()->IsInputAngle();

	// �E����͂��Ă�����true
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
		pPad->GetStickMoveL(playerIdx).x > 0 ||
		pKey->GetTrigger(DIK_D))
	{
		return CPlayerPosAdjIn::EInputUnstable::INPUT_ENEMY;
	}
	else if (angle != nullptr)
	{
		if (*angle != CPlayer::EDashAngle::ANGLE_RIGHT &&
			*angle != CPlayer::EDashAngle::ANGLE_RIGHTUP &&
			*angle != CPlayer::EDashAngle::ANGLE_RIGHTDW)
		{
			MyLib::Vector3 rot = pPlayer->GetRotation();
			float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// ����
			rot.y = division * (*angle) + D3DX_PI + Camerarot.y;
			UtilFunc::Transformation::RotNormalize(rot.y);
			pPlayer->SetRotation(rot);

			return CPlayerPosAdjIn::EInputUnstable::INPUT_FRIEND;
		}
	}

	return CPlayerPosAdjIn::EInputUnstable::INPUT_NONE;
}
