//==========================================================================
// 
//  ユーザープレイヤー処理 [playerUserOut.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerUserOut.h"

// 使用クラス
#include "playerUserOut_controlMove.h"
#include "playerUserOut_controlAction.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerUserOut::CPlayerUserOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerOut(pPlayer, typeTeam, typeArea, CPlayer::EBaseType::TYPE_USER)
{
	// 外野操作の割当
	ChangeMoveControl(DEBUG_NEW CPlayerUserOutControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerUserOutControlAction());
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerUserOut::~CPlayerUserOut()
{

}
