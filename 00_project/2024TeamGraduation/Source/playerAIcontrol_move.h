//=============================================================================
// 
//  プレイヤーAIコントロール_移動ヘッダー [playerAIcontrol_move.h]
//  Author : 藤田勇一
// 
//=============================================================================

#ifndef _PLAYERAI_CONTROL_MOVE_H_
#define _PLAYERAI_CONTROL_MOVE_H_	// 二重インクルード防止

#define BRESSRANGE (0)

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playercontrol_move.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CSuffocation;
class CEffekseerObj;

//==========================================================================
// プレイヤーAIコントロール_移動クラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerAIControlMove : public CPlayerControlMove
{
public:

	// コンストラクタ
	CPlayerAIControlMove();

	void Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 移動

private:
	
	//=============================
	// メンバ関数
	//=============================
	void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// ブリンク
	void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// ブリンク
	void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// ウォーク
	CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::DashAngle angle) override;	// ダッシュ時のトリガー

	//=============================
	// メンバ変数
	//=============================
};


#endif