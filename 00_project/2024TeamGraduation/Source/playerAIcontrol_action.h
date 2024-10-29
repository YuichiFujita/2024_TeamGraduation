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
#include "playercontrol_action.h"

//==========================================================================
// プレイヤーAIコントロール_アクションクラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerAIControlAction : public CPlayerControlAction
{
public:

	// コンストラクタ
	CPlayerAIControlAction();

#ifdef _DEBUG
	void ChangeAutoThrow() { m_bAutoThrow = !m_bAutoThrow; }
	void SetEnableAutoThrow(bool bAuto) { m_bAutoThrow = bAuto; }
	bool IsAutoThrow() { return m_bAutoThrow; }
#endif

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
	// 静的メンバ変数
	//=============================
	static bool m_bAutoThrow;

	//=============================
	// メンバ変数
	//=============================
	float fThrowTime = 0.0f;

};

#endif
