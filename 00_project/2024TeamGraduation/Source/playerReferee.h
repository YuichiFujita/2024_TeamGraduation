//=============================================================================
// 
//  審判用プレイヤーヘッダー [playerReferee.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYER_REFEREE_H_
#define _PLAYER_REFEREE_H_	// 二重インクルード防止

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
class CPlayerReferee : public CPlayer
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerReferee(
		const CGameManager::ETeamSide typeTeam = CGameManager::ETeamSide::SIDE_NONE,
		const CPlayer::EFieldArea typeArea = CPlayer::EFieldArea::FIELD_NONE,
		const CPlayer::EBaseType typeBase = CPlayer::EBaseType::TYPE_USER,
		int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CPlayerReferee();

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
