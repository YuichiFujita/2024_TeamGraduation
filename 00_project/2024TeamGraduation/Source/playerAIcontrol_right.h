//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_RIGHT_H_
#define _PLAYERAICONTROL_RIGHT_H_	// 二重インクルード防止

#include "player.h"
#include "playerAIcontrol.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerAIControlMove;		// 移動(AI)
class CPlayerAIControlAction;	// アクション(AI)
class CPlayer;

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
class CPlayerAIControlRight : public CPlayerAIControl
{
public:

	// コンストラクタ
	CPlayerAIControlRight();

	static CPlayerAIControlRight* Create(CPlayer* player);

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

private:
	//=============================
	// メンバ関数
	//=============================

	// オーバーライド
	bool IsLineOverBall() override;				// 線超え判定(ボール)
	bool IsLineOverPlayer() override;			// 線越え判定(プレイヤー)
	void AttackDash(CPlayer* pTarget) override;	// 走り投げ
	void ForciblyReturn() override;				// 強制行動：戻る
	void MoveRetreat() override;				// 後退
	void MoveRandom() override;					// ランダム移動

	//=============================
	// メンバ変数
	//=============================
};

#endif
