//==========================================================================
// 
//  ユーザープレイヤー処理 [playerUser.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerUser.h"
#include "manager.h"

// 使用クラス
#include "playercontrol_move.h"
#include "playercontrol_action.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerUser::CPlayerUser(CPlayer* pPlayer, const CGameManager::TeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerBase(pPlayer, typeTeam, typeArea)
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerUser::~CPlayerUser()
{

}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerUser::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// プレイヤーの取得
	CPlayer* pPlayer = GetPlayer();

	// 操作クラスの取得
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// 操作の更新
	pControlMove->Move(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);
	pControlAction->Action(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);

	// 操作クラスの反映
	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayerUser::ChangeMoveControl(CPlayerControlMove* control)
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
void CPlayerUser::ChangeActionControl(CPlayerControlAction* control)
{
	// 操作クラスの取得
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// 操作クラスの入替
	delete pControlAction;
	pControlAction = control;

	// 操作クラスの反映
	SetPlayerControlAction(pControlAction);
}
