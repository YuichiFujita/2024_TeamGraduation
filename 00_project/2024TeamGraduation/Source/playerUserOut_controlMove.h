#if 0
//==========================================================================
// 
//  プレイヤーコントロールヘッダー [playerUsercontrol_move.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYERUSER_CONTROL_MOVE_H_
#define _PLAYERUSER_CONTROL_MOVE_H_	// 二重インクルード防止

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
class CPlayerAIControlMove;

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerUserControlMove : public CPlayerControlMove
{
public:

	// コンストラクタ
	CPlayerUserControlMove();

	//=============================
	// 仮想関数
	//=============================
	virtual CPlayerAIControlMove* GetAI() { return nullptr; }
	virtual CPlayerUserControlMove* GetUser() { return this; }

private:
	
	//=============================
	// メンバ関数
	//=============================
	void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// ブリンク
	void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// ダッシュ
	void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// ウォーク
	CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::EDashAngle eAngle) override;	// ダッシュ時のトリガー

	//=============================
	// メンバ変数
	//=============================

};

#endif
#endif