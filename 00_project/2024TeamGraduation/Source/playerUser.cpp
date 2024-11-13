//==========================================================================
// 
//  ユーザープレイヤー処理 [playerUser.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerUser.h"
#include "manager.h"
#include "calculation.h"

// 使用クラス
#include "playerStatus.h"
#include "playerUsercontrol_move.h"
#include "playerUsercontrol_action.h"
#include "playerUserOut_controlMove.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerUser::CPlayerUser(CPlayer* pPlayer, const CPlayer::EFieldArea typeArea) : CPlayerBase(pPlayer, typeArea)
{
	// 初期操作の設定
	switch (typeArea)
	{ // ポジションごとの処理
	case CPlayer::EFieldArea::FIELD_IN:

		// 内野操作の割当
		ChangeMoveControl(DEBUG_NEW CPlayerUserControlMove());
		ChangeActionControl(DEBUG_NEW CPlayerUserControlAction());
		break;

	case CPlayer::EFieldArea::FIELD_OUT:

		// 外野操作の割当
		ChangeMoveControl(DEBUG_NEW CPlayerUserOutControlMove());
		ChangeActionControl(DEBUG_NEW CPlayerUserControlAction());
		break;

	default:
		assert(false);
		break;
	}
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
