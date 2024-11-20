//==========================================================================
// 
//  プレイヤー位置補正_右内野コート処理 [playerPosAdj_inRight.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerPosAdj_inRight.h"
#include "player.h"
#include "playerBase.h"
#include "playercontrol_move.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerPosAdjInRight::CPlayerPosAdjInRight()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerPosAdjInRight::~CPlayerPosAdjInRight()
{

}

//==========================================================================
// ライン越えフラグ取得
//==========================================================================
bool CPlayerPosAdjInRight::IsLineOut(CPlayer* pPlayer)
{
	// ラインを越えている場合フラグONを返す
	if (pPlayer->GetPosition().x < 0.0f) { return true; }

	// ラインを越えていない場合フラグOFFを返す
	return false;
}

//==========================================================================
// おっとっとフラグ取得
//==========================================================================
bool CPlayerPosAdjInRight::IsUnstable(CPlayer* pPlayer)
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
bool CPlayerPosAdjInRight::IsInputLine(CPlayer* pPlayer)
{
	CPlayer::EDashAngle* angle = pPlayer->GetBase()->GetPlayerControlMove()->IsInputAngle();
	if (angle == nullptr) return false;

	// 右を入力していたらtrue
	if (*angle == CPlayer::EDashAngle::ANGLE_LEFT)
	{
		return true;
	}

	return false;
}
