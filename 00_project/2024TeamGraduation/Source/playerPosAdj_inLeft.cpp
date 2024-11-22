//==========================================================================
// 
//  プレイヤー位置補正_左内野コート処理 [playerPosAdj_inLeft.cpp]
//  Author : 藤田勇一
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
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerPosAdjInLeft::CPlayerPosAdjInLeft()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerPosAdjInLeft::~CPlayerPosAdjInLeft()
{

}

//==========================================================================
// ライン越えフラグ取得
//==========================================================================
bool CPlayerPosAdjInLeft::IsLineOut(CPlayer* pPlayer)
{
	// ラインを越えている場合フラグONを返す
	if (pPlayer->GetPosition().x > 0.0f) { return true; }

	// ラインを越えていない場合フラグOFFを返す
	return false;
}

//==========================================================================
// おっとっとフラグ取得
//==========================================================================
bool CPlayerPosAdjInLeft::IsUnstable(CPlayer* pPlayer)
{
	// ラインを越えている場合フラグONを返す
	if (pPlayer->GetPosition().x > -UNSTABLE_LINE &&
		pPlayer->GetPosition().x < 0.0f)
	{
		return true;
	}

	// ラインを越えていない場合フラグOFFを返す
	return false;
}

//==========================================================================
// おっとっと時入力フラグ取得
//==========================================================================
CPlayerPosAdjIn::EInputUnstable CPlayerPosAdjInLeft::IsInputLine(CPlayer* pPlayer)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// プレイヤー番号取得
	int playerIdx = pPlayer->GetMyPlayerIdx();
	
	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// 入力方向を取得
	CPlayer::EDashAngle* angle = pPlayer->GetBase()->GetPlayerControlMove()->IsInputAngle();

	// 右を入力していたらtrue
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
			float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// 向き
			rot.y = division * (*angle) + D3DX_PI + Camerarot.y;
			UtilFunc::Transformation::RotNormalize(rot.y);
			pPlayer->SetRotation(rot);

			return CPlayerPosAdjIn::EInputUnstable::INPUT_FRIEND;
		}
	}

	return CPlayerPosAdjIn::EInputUnstable::INPUT_NONE;
}
