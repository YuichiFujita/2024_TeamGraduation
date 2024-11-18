//==========================================================================
// 
//  AIプレイヤー処理 [playerAIOut.cpp]
//  Author : 藤田勇一
//  Adder  : 小笠原彪
// 
//==========================================================================
#include "playerAIOut.h"

// 使用クラス
#include "playerAIOut_controlMove.h"
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
CPlayerAIOut::CPlayerAIOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerOut(pPlayer, typeTeam, typeArea)
{
	// 外野操作の割当
	ChangeMoveControl(DEBUG_NEW CPlayerAIOutControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAIOut::~CPlayerAIOut()
{

}
