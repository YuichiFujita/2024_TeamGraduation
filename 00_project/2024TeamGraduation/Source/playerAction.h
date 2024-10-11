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

	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 更新

	void SetAction(CPlayer::Action action);				// アクション設定
	CPlayer::Action GetAction() { return m_Action; }	// アクション取得


private:
	
	//=============================
	// メンバ関数
	//=============================
	typedef void(CPlayerAction::* ACTION_FUNC)(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	static ACTION_FUNC m_ActionFunc[];	// 行動関数

	void ActionNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// なし
	void ActionBlink(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ブリンク
	void ActionRun(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 走り

	//=============================
	// メンバ変数
	//=============================
	CPlayer::Action m_Action;	// アクション
	float m_fActionTime;		// アクション時間
	CPlayer* m_pPlayer;			// プレイヤーのポインタ
};


#endif