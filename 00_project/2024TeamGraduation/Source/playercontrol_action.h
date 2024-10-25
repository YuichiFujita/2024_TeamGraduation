//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playercontrol_action.h]
//  Author : Kai Takada
// 
//=============================================================================

#ifndef _PLAYER_CONTROL_ACTION_H_
#define _PLAYER_CONTROL_ACTION_H_	// 二重インクルード防止

#include "playercontrol.h"

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerControlAction : public CPlayerControl
{
public:

	// コンストラクタ
	CPlayerControlAction();

	void EAction(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 統括
	void SetJump(CPlayer* player);

protected:
	//=============================
	// メンバ関数
	//=============================
	void SpecialSetting(CPlayer* player, CBall* pBall, CTeamStatus* pTeamStatus);		// スペシャル発動

private:
	
	//=============================
	// メンバ関数
	//=============================
	void ConditionalAction(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 基本アクション操作

	virtual void Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;		// キャッチ
	virtual void Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;		// 投げ
	virtual void Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;		// ジャンプ
	virtual void Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// スペシャル
	virtual void Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;		// モテ

	//=============================
	// メンバ変数
	//=============================
};


#endif