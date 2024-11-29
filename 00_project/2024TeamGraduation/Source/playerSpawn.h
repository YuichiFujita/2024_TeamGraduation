//==========================================================================
// 
//  登場演出用プレイヤーヘッダー [playerSpawn.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_SPAWN_
#define _PLAYER_SPAWN_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "player.h"

//==========================================================================
// クラス定義
//==========================================================================
// 登場演出用プレイヤークラス
class CPlayerSpawn : public CPlayer
{
public:
	
	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerSpawn(const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase, int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CPlayerSpawn();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;	// 動的削除
};

#endif
