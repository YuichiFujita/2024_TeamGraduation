//=============================================================================
// 
//  プレイヤーアクションヘッダー [playerAction.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYERACTION_H_
#define _PLAYERACTION_H_	// 二重インクルード防止

#include "player.h"

//==========================================================================
// プレイヤーアクションクラス
//==========================================================================
class CPlayerAction
{
public:

	// コンストラクタ
	CPlayerAction(CPlayer* player);

	//=============================
	// メンバ関数
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 更新
	void SetAction(CPlayer::EAction action);						// アクション設定
	CPlayer::EAction GetAction() { return m_Action; }			// アクション取得
	void SetEnableCharm(bool bCharm) { m_bCharm = bCharm; }			// モテボタン状態設定
	bool IsCharm() { return m_bCharm; }							// モテボタン状態取得
	void BindPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }	// プレイヤー割り当て
private:
	
	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayerAction::* ACTION_FUNC)(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	static ACTION_FUNC m_ActionFunc[];	// 行動関数

	typedef void(CPlayerAction::* START_FUNC)(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	static START_FUNC m_StartFunc[];	// 行動関数

	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void ActionNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// なし
	void ActionBlink(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ブリンク
	void ActionDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 回避
	void ActionRun(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 走り
	void ActionJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ジャンプ
	void ActionCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// キャッチ
	void ActionThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 投げ
	void ActionThrowJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 投げ(ジャンプ)
	void ActionSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// スペシャル

	// 状態スタート系
	void StartThrowJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 投げ(ジャンプ)

	void Debug();

	//=============================
	// メンバ変数
	//=============================
	bool m_bCharm;				// モテボタン状態
	CPlayer::EAction m_Action;	// アクション
	float m_fActionTime;		// アクション時間
	CPlayer* m_pPlayer;			// プレイヤーのポインタ
};


#endif