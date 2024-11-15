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
	virtual CPlayerAIControlAction* GetAI() { return nullptr; }
	virtual CPlayerUserControlAction* GetUser() { return nullptr; }

	void SetThrowDrop();	// 投げ(ドロップボール)設定
	inline bool IsThrowDrop() { return m_bThrowDrop; }	// 投げ(ドロップボール)可能判定取得

	// ジャンプトリガー
	void SetEnableJumpTrigger(bool bTrigger)	{ m_bJumpTrigger = bTrigger; }	// ジャンプトリガー
	bool IsJumpTrigger()						{ return m_bJumpTrigger; }		// ジャンプトリガー

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

	// 基本アクション操作
	virtual void Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;		// キャッチ
	virtual void Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;		// 投げ
	virtual void Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;		// ジャンプ
	virtual void Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// スペシャル
	virtual void Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;		// モテ

	// その他
	void UpdateThrowDrop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 投げ(ドロップボール)の猶予

	//=============================
	// メンバ変数
	//=============================
	float m_fThrowDropTime;	// 投げ(ドロップボール)の猶予
	bool m_bThrowDrop;		// 投げ(ドロップボール)可能判定
	bool m_bJumpTrigger;	// ジャンプトリガー
};


#endif