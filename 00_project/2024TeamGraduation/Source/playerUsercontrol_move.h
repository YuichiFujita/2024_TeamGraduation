//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playerUsercontrol_move.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYERUSER_CONTROL_MOVE_H_
#define _PLAYERUSER_CONTROL_MOVE_H_	// 二重インクルード防止

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

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerUserControlMove();
	~CPlayerUserControlMove();

	//=============================
	// 仮想関数
	//=============================
	virtual CPlayerUserControlMove* GetUser() override { return this; }

private:
	
	//=============================
	// メンバ関数
	//=============================
	void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// ブリンク
	void BilnkKey(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// キー入力のブリンク
	void BilnkStick(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// スティック入力のブリンク
	void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// ダッシュ
	void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// ウォーク
	CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::EDashAngle eAngle) override;	// ダッシュ時のトリガー
	bool IsInverseTrigger(CPlayer::EDashAngle eAngle, int* nCntTrigger);	// 逆方向判定
};

#endif