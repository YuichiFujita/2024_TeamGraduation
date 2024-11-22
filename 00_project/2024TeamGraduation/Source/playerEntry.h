//==========================================================================
// 
//  エントリー用プレイヤーヘッダー [playerEntry.h]
//  Author : 相馬靜雅
// 
//==========================================================================

#ifndef _PLAYER_ENTRY_
#define _PLAYER_ENTRY_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerBase.h"

//==========================================================================
// クラス定義
//==========================================================================
// エントリー用プレイヤークラス
class CPlayerEntry : public CPlayerBase
{
public:
	
	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerEntry(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase);
	virtual ~CPlayerEntry() override;

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
