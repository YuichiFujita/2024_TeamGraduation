//==========================================================================
// 
//  ユーザープレイヤー処理 [playerUserIn.cpp]
//  Author : 藤田勇一
//  Adder  : 髙田佳依
// 
//==========================================================================
#include "playerUserIn.h"

// 使用クラス
#include "playerUsercontrol_move.h"
#include "playerUsercontrol_action.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerUserIn::CPlayerUserIn(CPlayer* pPlayer, const CPlayer::EFieldArea typeArea) : CPlayerUser(pPlayer, typeArea)
{
	// 内野操作の割当
	ChangeMoveControl(DEBUG_NEW CPlayerUserControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerUserControlAction());
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerUserIn::~CPlayerUserIn()
{

}
