//==========================================================================
// 
//  外野プレイヤー移動コントロールヘッダー [playerUserOut_controlMove.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYERUSER_OUT_CONTROL_MOVE_H_
#define _PLAYERUSER_OUT_CONTROL_MOVE_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playercontrol_move.h"
#include "bindKey.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerUserOutControlMove;
class CPlayerAIControlMove;

//==========================================================================
// 外野プレイヤー移動コントロールクラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerUserOutControlMove : public CPlayerControlMove
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerUserOutControlMove();
	virtual ~CPlayerUserOutControlMove() override;

	//=============================
	// オーバーライド関数
	//=============================
	virtual CPlayerUserOutControlMove* GetUserOut() override { return this; }
	virtual void CrabSetting(CPlayer* player) override;				// カニ歩き状態

	//=============================
	// メンバ関数
	//=============================
	void BindLeftKey(CBindKey* pKey)	{ SAFE_DELETE(m_pLeftKey);	m_pLeftKey = pKey; }	// 左移動キー割当
	void BindRightKey(CBindKey* pKey)	{ SAFE_DELETE(m_pRightKey);	m_pRightKey = pKey; }	// 右移動キー割当

private:

	//=============================
	// オーバーライド関数
	//=============================
	void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// ブリンク
	void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// ダッシュ
	void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// ウォーク
	CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::EDashAngle eAngle) override;	// ダッシュ時のトリガー

	//=============================
	// メンバ関数
	//=============================
	void BilnkKey(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// キー入力のブリンク
	void BilnkStick(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// スティック入力のブリンク

	//=============================
	// メンバ変数
	//=============================
	CBindKey* m_pLeftKey;	// 左移動キー情報
	CBindKey* m_pRightKey;	// 右移動キー情報
};

#endif
