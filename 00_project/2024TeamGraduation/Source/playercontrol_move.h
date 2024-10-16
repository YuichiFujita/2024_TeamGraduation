//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playercontrol_move.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYERCONTROL_MOVE_H_
#define _PLAYERCONTROL_MOVE_H_	// 二重インクルード防止

#define BRESSRANGE (0)

#include "player.h"

class CSuffocation;
class CEffekseerObj;

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerControlMove
{
public:

	// コンストラクタ
	CPlayerControlMove();

	virtual void Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 移動

private:
	
	//=============================
	// メンバ関数
	//=============================
	void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ブリンク
	void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ブリンク
	void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ウォーク
	CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::DashAngle angle);	// ダッシュ時のトリガー

	//=============================
	// メンバ変数
	//=============================
	int m_nCntTrigger[CPlayer::DashAngle::ANGLE_MAX];	// トリガーのカウント
	CPlayer::DashAngle m_HoldDashAngle;					// 保持してるダッシュの移動方向
	float m_fInputInterval;								// 入力の受け付け猶予
	float m_fTriggerInterval;							// トリガーのインターバル
	bool m_bDash;										// 走るフラグ
};


#endif