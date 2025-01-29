//==========================================================================
// 
//  AI外野プレイヤーヘッダー [playerAIOut.h]
//  Author : 藤田勇一
//  Adder  : 小笠原彪
// 
//==========================================================================

#ifndef _PLAYER_AI_OUT_
#define _PLAYER_AI_OUT_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerOut.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerAIOutControl;	// コントロール(AI外野)

//==========================================================================
// クラス定義
//==========================================================================
// AI外野プレイヤークラス
class CPlayerAIOut : public CPlayerOut
{
public:

	enum EAIOutRange
	{
		RANGE_NONE = 0,
		RANGE_UP,
		RANGE_DOWN,
		RANGE_LEFT,
		RANGE_RIGHT,
		RANGE_MAX
	};

public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerAIOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea);
	virtual ~CPlayerAIOut() override;

	//=============================
	// オーバーライド関数
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	virtual void MotionCrab(int nStartKey) override;		// カニ歩き変化
	virtual CPlayer::SHitInfo Hit(CBall* pBall) override;	// ヒット
	virtual void Debug() override;	// デバッグ
	virtual CPlayerAIOut* GetPlayerAIOut() override { return this; }	// AI外野プレイヤー取得
	virtual bool IsCrab() override;					// カニ歩き判定


private:
	//=============================
	// メンバ変数
	//=============================
	CPlayerAIOutControl* m_pAIOutControl;	// AIコントロール(外野)
	EAIOutRange m_eAIRange;
};

#endif
