//==========================================================================
// 
//  AIプレイヤー処理 [playerAIOut.cpp]
//  Author : 藤田勇一
//  Adder  : 小笠原彪
// 
//==========================================================================
#include "playerAIOut.h"

// 使用クラス
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIOut::CPlayerAIOut(CPlayer* pPlayer, const CPlayer::EFieldArea typeArea) : CPlayerAI(pPlayer, typeArea)
{
	// 外野操作の割当	// TODO：外野操作に変更
	ChangeMoveControl(DEBUG_NEW CPlayerAIControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAIOut::~CPlayerAIOut()
{

}
