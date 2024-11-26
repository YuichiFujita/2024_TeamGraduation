//==========================================================================
// 
//  指定なしプレイヤーヘッダー [playerNone.h]
//  Author : 相馬靜雅
//  Adder  : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_NONE_
#define _PLAYER_NONE_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerBase.h"

//==========================================================================
// クラス定義
//==========================================================================
// 指定なしプレイヤークラス
class CPlayerNone : public CPlayerBase
{
public:
	
	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerNone(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea);
	virtual ~CPlayerNone() override;

	//=============================
	// オーバーライド関数
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	virtual CPlayer::SHitInfo Hit(CBall* pBall) override { return {}; }	// ヒット
	virtual void MotionCrab(int nStartKey) override {}					// カニ歩き変化
};

#endif
