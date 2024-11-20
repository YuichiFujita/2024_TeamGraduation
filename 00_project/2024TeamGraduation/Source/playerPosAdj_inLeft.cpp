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
bool CPlayerPosAdjInLeft::IsInputLine(CPlayer* pPlayer)
{
	CPlayer::EDashAngle* angle = pPlayer->GetBase()->GetPlayerControlMove()->IsInputAngle();
	if (angle == nullptr) return false;

	// 右を入力していたらtrue
	if (*angle == CPlayer::EDashAngle::ANGLE_RIGHT)
	{
		return true;
	}

	return false;
}
