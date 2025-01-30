//==========================================================================
// 
//  外野プレイヤーコントロールヘッダー [playerAIOut_controlMove.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYERAI_OUT_CONTROL_MOVE_H_
#define _PLAYERAI_OUT_CONTROL_MOVE_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playercontrol_move.h"

//==========================================================================
// 外野プレイヤーコントロールクラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerAIOutControlMove : public CPlayerControlMove
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerAIOutControlMove();
	virtual ~CPlayerAIOutControlMove() override;

	//=============================
	// オーバーライド関数
	//=============================
	virtual CPlayerAIOutControlMove* GetAIOut() override { return this; }

	//=============================
	// メンバ関数
	//=============================
	void SetIsBlink(bool bBlink) { m_bBlink = bBlink; }
	void SetIsDash(bool bDash) { m_bDash = bDash; }
	void SetIsWalk(bool bWalk) { m_bWalk = bWalk; }
	void SetIsClab(bool clab) { m_bClab = clab;}

	void SetClabDirection(float clabDirection) { m_fClabDirection = clabDirection; }
	float GetClabDirection() { return m_fClabDirection; }	// カニ進行方向の取得

	//=============================
	// メンバ変数
	//=============================

private:

	//=============================
	// オーバーライド関数
	//=============================
	void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// ブリンク
	void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// ダッシュ
	void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// ウォーク
	CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::EDashAngle eAngle) override;	// ダッシュ時のトリガー

	//=============================
	// メンバ変数
	//=============================
	bool m_bBlink;
	bool m_bDash;
	bool m_bWalk;
	bool m_bClab;
	float m_fClabDirection;	// カニのしんこう方向
};

#endif