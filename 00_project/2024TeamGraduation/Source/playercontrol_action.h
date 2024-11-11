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
// 前方宣言
//==========================================================================
class CPlayerUserControlAction;
class CPlayerAIControlAction;

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

	void Action(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 統括

	//=============================
	// 純粋仮想関数
	//=============================
	virtual CPlayerAIControlAction* GetAI() = 0;
	virtual CPlayerUserControlAction* GetUser() = 0;

protected:
	//=============================
	// メンバ関数
	//=============================
	void ThrowSetting(CPlayer* player);
	void JumpSetting(CPlayer* player);
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