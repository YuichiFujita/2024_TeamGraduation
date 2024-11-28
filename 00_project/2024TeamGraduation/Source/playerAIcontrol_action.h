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
// 前方宣言
//==========================================================================
class CPlayerUserControlAction;

//==========================================================================
// プレイヤーAIコントロール_アクションクラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerAIControlAction : public CPlayerControlAction
{
private:
	//=============================
	// 構造体
	//=============================
	struct SFlag
	{
		bool bThrow;		// 投げるか
		bool bPass;			// パス
		bool bJump;			// ジャンプ
		bool bJumpFloat;	// ジャンプホールド
		bool bSpecial;		// スペシャル
	};

public:

	// コンストラクタ
	CPlayerAIControlAction();

	//=============================
	// 仮想関数
	//=============================
	virtual CPlayerAIControlAction* GetAI() override { return this; };

	//=============================
	// メンバ関数
	//=============================
	void SetIsThrow(bool bThrow) { m_sFlag.bThrow = bThrow; }				// 投げるか設定
	void SetIsPass(bool bPass) { m_sFlag.bPass = bPass; }					// パスするか設定
	void SetIsJump(bool bJump) { m_sFlag.bJump = bJump; }					// ジャンプするか設定
	void SetIsJumpFloat(bool passFloat) { m_sFlag.bJumpFloat = passFloat; }	// じゃんぷを継続するか設定
	void SetIsSpecial(bool bSpecial) { m_sFlag.bSpecial = bSpecial; }		// スペシャル撃つか設定
	void SetJumpRate(float rate) { m_fJumpRate = rate; }
	SFlag GetIsFlag() { return m_sFlag; }


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
	void JumpFloat(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;																			// ジャンプ上昇
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
	float m_fJumpTimer;
	float m_fJumpRate;
	SFlag m_sFlag;

};

#endif
