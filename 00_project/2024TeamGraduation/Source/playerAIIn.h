//==========================================================================
// 
//  AI内野プレイヤーヘッダー [playerAIIn.h]
//  Author : 藤田勇一
//  Adder  : 小笠原彪
// 
//==========================================================================

#ifndef _PLAYER_AI_IN_
#define _PLAYER_AI_IN_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerIn.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerAIControl;	// コントロール(AI)

//==========================================================================
// クラス定義
//==========================================================================
// AI内野プレイヤークラス
class CPlayerAIIn : public CPlayerIn
{
public:
	
	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerAIIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea);
	virtual ~CPlayerAIIn() override;

	//=============================
	// オーバーライド関数
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	virtual void MotionCrab(int nStartKey) override;		// カニ歩き変化
	virtual CPlayer::SHitInfo Hit(CBall* pBall) override;	// ヒット
	virtual void Debug() override;							// デバッグ
	virtual CPlayerAIIn* GetPlayerAIIn() override { return nullptr; }	// AI内野プレイヤー取得

private:

	//=============================
	// メンバ変数
	//=============================
	CPlayerAIControl* m_pAIControl;
};

#endif
