//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playercontrol.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYERCONTROL_ACTION_H_
#define _PLAYERCONTROL_ACTION_H_	// 二重インクルード防止

#include "player.h"

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerControlAction
{
public:

	// コンストラクタ
	CPlayerControlAction();

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