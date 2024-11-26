//=============================================================================
// 
//  ドレスアップ用プレイヤーヘッダー [playerDressup.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYER_DRESSUP_H_
#define _PLAYER_DRESSUP_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "player.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// プレイヤークラス定義
class CPlayerDressUP : public CPlayer
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerDressUP(const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase, int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CPlayerDressUP();

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
