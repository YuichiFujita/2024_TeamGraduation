//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_LEFTATTACK_H_
#define _PLAYERAICONTROL_LEFTATTACK_H_	// 二重インクルード防止

#include "player.h"
#include "playerAIcontrol_attack.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerAIControlMove;		// 移動(AI)
class CPlayerAIControlAction;	// アクション(AI)
class CPlayer;

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
class CPlayerAIControlLeftAttack : public CPlayerAIControlAttack
{
public:

	// コンストラクタ
	CPlayerAIControlLeftAttack();

	static CPlayerAIControlLeftAttack* Create(CPlayer* player);

	HRESULT Init() ;
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

private:
	//=============================
	// メンバ関数
	//=============================
	void PreparationLeave(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

	//=============================
	// メンバ変数
	//=============================
};

#endif
