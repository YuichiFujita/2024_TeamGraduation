//=============================================================================
// 
//  プレイヤーAIコントロール_アクションヘッダー [playerAIcontrol_action.h]
//  Author : 藤田勇一
// 
//=============================================================================

#ifndef _PLAYERAI_CONTROL_ACTION_H_
#define _PLAYERAI_CONTROL_ACTION_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "player.h"

//==========================================================================
// プレイヤーAIコントロール_アクションクラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerAIControlAction
{
public:

	// コンストラクタ
	CPlayerAIControlAction();

	virtual void Action(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// キャッチ

private:
	
	//=============================
	// メンバ関数
	//=============================
	void Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// キャッチ
	void Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 投げ
	void Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ジャンプ
	void Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// スペシャル
	void Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// モテ

	//=============================
	// メンバ変数
	//=============================
};

#endif
