//==========================================================================
// 
//  内野プレイヤーヘッダー [playerIn.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_IN_
#define _PLAYER_IN_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerBase.h"

//==========================================================================
// クラス定義
//==========================================================================
// 内野プレイヤークラス
class CPlayerIn : public CPlayerBase
{
public:
	
	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase);
	virtual ~CPlayerIn() override = 0;

	//=============================
	// オーバーライド関数
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	virtual CPlayer::SHitInfo Hit(CBall* pBall) override;		// ヒット
	virtual CPlayerIn* GetPlayerIn() override { return this; }	// 外野プレイヤー取得

	//=============================
	// パターン
	//=============================
	void ChangeMoveControl(CPlayerControlMove* control);		// 移動の操作変更
	void ChangeActionControl(CPlayerControlAction* control);	// アクションの操作変更
};

#endif
