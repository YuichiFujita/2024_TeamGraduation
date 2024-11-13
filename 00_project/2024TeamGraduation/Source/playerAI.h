//==========================================================================
// 
//  AIプレイヤーヘッダー [playerAI.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_AI_
#define _PLAYER_AI_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerBase.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerAIControlMove;		// 移動(AI)
class CPlayerAIControlAction;	// アクション(AI)

//==========================================================================
// クラス定義
//==========================================================================
// AIプレイヤークラス
class CPlayerAI : public CPlayerBase
{
public:
	
	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerAI(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea);
	virtual ~CPlayerAI() = 0;

	//=============================
	// オーバーライド関数
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新

	//=============================
	// パターン
	//=============================
	void ChangeMoveControl(CPlayerControlMove* control);		// 移動の操作変更
	void ChangeActionControl(CPlayerControlAction* control);	// アクションの操作変更
};

#endif
