//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_RIGHTATTACK_H_
#define _PLAYERAICONTROL_RIGHTATTACK_H_	// 二重インクルード防止

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
class CPlayerAIControlRightAttack : public CPlayerAIControlAttack
{
public:

	enum  EThrowType			// 投げタイプ
	{
		THROWTYPE_NONE = 0,		// なし
		THROWTYPE_NORMAL,		// 通常
		THROWTYPE_JUMP,			// ジャンプ
		THROWTYPE_SPECIAL,		// スペシャル
		//THROWTYPE_PASS,			// パス
		THROWTYPE_MAX
	};

public:

	// コンストラクタ
	CPlayerAIControlRightAttack();
	
	// 生成
	static CPlayerAIControlRightAttack* Create();

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

private:
	//=============================
	// メンバ関数
	//=============================
	void PreparationLeave(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Preparation() override;

	//=============================
	// メンバ変数
	//=============================

};

#endif
