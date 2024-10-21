//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playerUsercontrol_action.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYERUSER_CONTROL_ACTION_H_
#define _PLAYERUSER_CONTROL_ACTION_H_	// 二重インクルード防止

#include "playercontrol_action.h"

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerUserControlAction : public CPlayerControlAction
{
public:

	// コンストラクタ
	CPlayerUserControlAction();

	virtual void Action(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// キャッチ

private:
	
	//=============================
	// メンバ関数
	//=============================
	void Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// キャッチ
	void Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// 投げ
	void Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;			// ジャンプ
	void Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// スペシャル
	void Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// モテ

	//=============================
	// メンバ変数
	//=============================
};

#endif