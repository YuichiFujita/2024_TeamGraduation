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
// 前方宣言
//==========================================================================
class CPlayerAIOutControlMove;
class CPlayerAIControlMove;

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

private:

	//=============================
	// オーバーライド関数
	//=============================
	void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// ブリンク
	void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// ダッシュ
	void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// ウォーク
	CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::EDashAngle eAngle) override;	// ダッシュ時のトリガー
};

#endif