//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playerUsercontrol_move.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYERUSER_CONTROL_MOVE_H_
#define _PLAYERUSER_CONTROL_MOVE_H_	// 二重インクルード防止

#define BRESSRANGE (0)

#include "playercontrol_move.h"

class CSuffocation;
class CEffekseerObj;

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