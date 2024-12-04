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
	if (pPlayer->GetPosition().x < UNSTABLE_LINE)
	{
		return true;
	}

	// ���C�����z���Ă��Ȃ��ꍇ�t���OOFF��Ԃ�
	return false;
}

//==========================================================================
// �����Ƃ��Ǝ����̓t���O�擾
//==========================================================================
CPlayerPosAdjIn::EInputUnstable CPlayerPosAdjInRight::IsInputLine(CPlayer* pPlayer)
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
	CPlayer::EDashAngle* angle = pPlayer->GetBase()->GetPlayerControlMove()->GetInputAngle();

	// �E����͂��Ă�����true
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
		pPad->GetStickMoveL(playerIdx).x < 0 ||
		pKey->GetTrigger(DIK_A))
	{
		return CPlayerPosAdjIn::EInputUnstable::INPUT_ENEMY;
	}
	else if (angle != nullptr)
	{
		if (*angle != CPlayer::EDashAngle::ANGLE_LEFT &&
			*angle != CPlayer::EDashAngle::ANGLE_LEFTUP &&
			*angle != CPlayer::EDashAngle::ANGLE_LEFTDW) 
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
