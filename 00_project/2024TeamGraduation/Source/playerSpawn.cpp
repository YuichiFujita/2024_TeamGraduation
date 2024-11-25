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
	// プレイヤーの取得
	CPlayer* pPlayer = GetPlayer();

	
}

//==========================================================================
// ヒット
//==========================================================================
CPlayer::SHitInfo CPlayerSpawn::Hit(CBall* pBall)
{
	return CPlayer::SHitInfo();
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayerSpawn::ChangeMoveControl(CPlayerControlMove* control)
{
	// 操作クラスの取得
	CPlayerControlMove* pControlMove = GetPlayerControlMove();

	// 操作クラスの入替
	delete pControlMove;
	pControlMove = control;

	// 操作クラスの反映
	SetPlayerControlMove(pControlMove);
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayerSpawn::ChangeActionControl(CPlayerControlAction* control)
{
	// 操作クラスの取得
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// 操作クラスの入替
	delete pControlAction;
	pControlAction = control;

	// 操作クラスの反映
	SetPlayerControlAction(pControlAction);
}
