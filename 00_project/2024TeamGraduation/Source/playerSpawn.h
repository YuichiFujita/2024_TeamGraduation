//==========================================================================
// 
//  登場用プレイヤーヘッダー [playerSpawn.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_SPAWN_
#define _PLAYER_SPAWN_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerBase.h"

//==========================================================================
// クラス定義
//==========================================================================
// 登場用プレイヤークラス
class CPlayerSpawn : public CPlayerBase
{
public:
	
	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerSpawn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase);
	virtual ~CPlayerSpawn() override;

	//=============================
	// オーバーライド関数
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	virtual CPlayer::SHitInfo Hit(CBall* pBall) override;	// ヒット
	virtual void MotionCrab(int nStartKey) override {}		// カニ歩き変化

	//=============================
	// パターン
	//=============================
	void ChangeMoveControl(CPlayerControlMove* control);		// 移動の操作変更
	void ChangeActionControl(CPlayerControlAction* control);	// アクションの操作変更
};

#endif
