//==========================================================================
// 
//  外野プレイヤーコントロールヘッダー [playerUserOut_controlMove.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYERUSER_OUT_CONTROL_MOVE_H_
#define _PLAYERUSER_OUT_CONTROL_MOVE_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playercontrol_move.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerUserOutControlMove;
class CPlayerAIControlMove;

//==========================================================================
// 外野プレイヤーコントロールクラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerUserOutControlMove : public CPlayerControlMove
{
public:

	// コンストラクタ
	CPlayerUserOutControlMove();

	//=============================
	// オーバーライド関数
	//=============================
	virtual CPlayerAIControlMove* GetAI() override { return nullptr; }
	virtual CPlayerUserControlMove* GetUser() override { return nullptr; }

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
	MyLib::Vector3 m_posLeft;	// 移動可能左位置
	MyLib::Vector3 m_posRight;	// 移動可能右位置
};

#endif