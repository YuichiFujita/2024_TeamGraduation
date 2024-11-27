//==========================================================================
// 
//  プレイヤー位置補正_右内野コート処理 [playerPosAdj_inRight.cpp]
//  Author : 藤田勇一
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
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIPosAdjInRight::CPlayerAIPosAdjInRight()
{
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAIPosAdjInRight::~CPlayerAIPosAdjInRight()
{

}

//==========================================================================
// ライン越えフラグ取得
//==========================================================================
bool CPlayerAIPosAdjInRight::IsLineOut(CPlayer* pPlayer)
{
	// ラインを越えている場合フラグONを返す
	if (pPlayer->GetPosition().x < 0.0f) { return true; }

	// ラインを越えていない場合フラグOFFを返す
	return false;
}

//==========================================================================
// おっとっとフラグ取得
//==========================================================================
bool CPlayerAIPosAdjInRight::IsUnstable(CPlayer* pPlayer)
{
	// ラインを越えている場合フラグONを返す
	if (pPlayer->GetPosition().x < UNSTABLE_LINE &&
		pPlayer->GetPosition().x > 0.0f)
	{
		return true;
	}

	// ラインを越えていない場合フラグOFFを返す
	return false;
}

//==========================================================================
// おっとっと時入力フラグ取得
//==========================================================================
CPlayerPosAdjIn::EInputUnstable CPlayerAIPosAdjInRight::IsInputLine(CPlayer* pPlayer)
{
	// 移動量の取得
	MyLib::Vector3 move = pPlayer->GetMove();

	// 右を入力していたらtrue
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
