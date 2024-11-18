//=============================================================================
// 
//  プレイヤーAIコントロール_移動ヘッダー [playerAIcontrol_move.h]
//  Author : 藤田勇一
// 
//=============================================================================

#ifndef _PLAYERAI_CONTROL_MOVE_H_
#define _PLAYERAI_CONTROL_MOVE_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playercontrol_move.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CSuffocation;
class CEffekseerObj;
class CPlayerUserControlMove;

//==========================================================================
// プレイヤーAIコントロール_移動クラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerAIControlMove : public CPlayerControlMove
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerAIControlMove();
	~CPlayerAIControlMove();

	void SetIsBlink(bool bBlink) { m_bBlink = bBlink; }
	void SetIsDash(bool bDash) { m_bDash = bDash; }
	void SetIsWalk(bool bWalk) { m_bWalk = bWalk; }

	void SetClabDirection(float clabDirection) { m_fClabDirection = clabDirection; }
	float GetClabDirection() { return m_fClabDirection; }	// カニ進行方向の取得

private:

	//=============================
	// 仮想関数
	//=============================
	virtual CPlayerAIControlMove* GetAI() override { return this; }
	
	//=============================
	// メンバ関数
	//=============================
	void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// ブリンク
	void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// ブリンク
	void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// ウォーク
	CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::EDashAngle eAngle) override;	// ダッシュ時のトリガー

	//=============================
	// メンバ変数
	//=============================
	bool m_bBlink;
	bool m_bDash;
	bool m_bWalk;
	float m_fClabDirection;	// カニのしんこう方向
};


#endif