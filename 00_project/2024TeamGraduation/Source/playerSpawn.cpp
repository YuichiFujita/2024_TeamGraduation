//==========================================================================
// 
//  登場用プレイヤー処理 [playerSpawn.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerSpawn.h"
#include "playerStatus.h"
#include "EffekseerObj.h"

// 使用クラス
#include "playerUsercontrol_move.h"
#include "playerUsercontrol_action.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerSpawn::CPlayerSpawn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase) : CPlayerBase(pPlayer, typeTeam, typeArea, typeBase)
{
	// 操作の割当
	ChangeMoveControl(DEBUG_NEW CPlayerUserControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerUserControlAction());
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerSpawn::~CPlayerSpawn()
{

}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerSpawn::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}
