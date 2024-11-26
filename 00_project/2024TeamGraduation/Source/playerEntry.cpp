//==========================================================================
// 
//  エントリー用プレイヤー処理 [playerEntry.cpp]
//  Author : 相馬靜雅
// 
//==========================================================================
#include "playerEntry.h"
#include "playerStatus.h"
#include "EffekseerObj.h"

// 使用クラス
#include "playerUsercontrol_move.h"
#include "playerUsercontrol_action.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerEntry::CPlayerEntry(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase) : CPlayerBase(pPlayer, typeTeam, typeArea, typeBase)
{
	// 操作の割当
	ChangeMoveControl(DEBUG_NEW CPlayerUserControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerUserControlAction());
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerEntry::~CPlayerEntry()
{

}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerEntry::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// プレイヤーの取得
	CPlayer* pPlayer = GetPlayer();
}
