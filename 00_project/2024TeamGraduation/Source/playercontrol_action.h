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
class CPlayerAIOutControlAction;

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
	// 仮想関数
	//=============================
	virtual CPlayerAIControlAction* GetAI()			{ return nullptr; }
	virtual CPlayerAIOutControlAction* GetAIOut()	{ return nullptr; }
	virtual CPlayerUserControlAction* GetUser()		{ return nullptr; }

	void SetThrowDrop();								// 投げ(ドロップボール)設定
	inline bool IsThrowDrop() { return m_bThrowDrop; }	// 投げ(ドロップボール)可能判定取得

	void SetThrowJust();								// 投げ(ジャストキャッチ)設定
	inline bool IsThrowJust() { return m_bThrowJust; }	// 投げ(ジャストキャッチ)可能判定取得

	// ジャンプトリガー
	void SetEnableJumpTrigger(bool bTrigger)	{ m_bJumpTrigger = bTrigger; }	// ジャンプトリガー
	bool IsJumpTrigger()						{ return m_bJumpTrigger; }		// ジャンプトリガー

protected:
	//=============================
	// メンバ関数
	//=============================
	void PassSetting(CPlayer* player);
	void ThrowSetting(CPlayer* player);
	void JumpSetting(CPlayer* player);
	virtual void JumpFloat(CPlayer* playe, const float fDeltaTime, const float fDeltaRate, const float fSlowRater);	// ジャンプ上昇
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
	virtual void UserChange(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {}	// ユーザー変更

	// その他
	void UpdateThrowDrop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 投げ(ドロップボール)の猶予
	void UpdateThrowJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 投げ(ジャストキャッチ)の猶予

	//=============================
	// メンバ変数
	//=============================
	float m_fThrowDropTime;	// 投げ(ドロップボール)の猶予
	bool m_bThrowDrop;		// 投げ(ドロップボール)可能判定
	float m_fThrowJustTime;	// 投げ(ジャストキャッチ)の猶予
	bool m_bThrowJust;		// 投げ(ジャストキャッチ)可能判定
	bool m_bJumpTrigger;	// ジャンプトリガー
};


#endif