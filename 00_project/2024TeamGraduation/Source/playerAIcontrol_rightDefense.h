//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_RIGHTDEFENSE_H_
#define _PLAYERAICONTROL_RIGHTDEFENSE_H_	// 二重インクルード防止

#include "player.h"
#include "playerAIcontrol_defense.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerAIControlMove;		// 移動(AI)
class CPlayerAIControlAction;	// アクション(AI)
class CPlayerAIControl;
class CPlayer;

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
class CPlayerAIControlRightDefense : public CPlayerAIControlDefense
{
public:

	// コンストラクタ
	CPlayerAIControlRightDefense();

	static CPlayerAIControlRightDefense* Create(CPlayer* player);


	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

private:
	//=============================
	// 仮想・純粋関数
	//=============================
	bool IsLineOverBall() override;				// 線超え判定(ボール)
	bool IsLineOverPlayer() override;			// 線越え判定(プレイヤー)
	void MoveRetreat() override;				// 後退
	void MoveRandom() override;					// ランダム
	void BallSteal() override;

	//=============================
	// メンバ関数
	//=============================

	//=============================
	// メンバ変数
	//=============================
};

#endif
