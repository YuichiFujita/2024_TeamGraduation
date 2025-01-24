//==========================================================================
// 
//  カットイン用プレイヤーヘッダー [playerCutIn.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_CUTIN_H_
#define _PLAYER_CUTIN_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "player.h"

//==========================================================================
// クラス定義
//==========================================================================
// プレイヤークラス定義
class CPlayerCutIn : public CPlayer
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerCutIn(const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase, int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CPlayerCutIn();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Kill() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
};

#endif
