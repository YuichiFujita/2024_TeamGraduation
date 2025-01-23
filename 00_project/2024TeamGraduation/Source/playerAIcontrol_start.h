//=============================================================================
// 
//  プレイヤーAIコントロール_移動ヘッダー [playerAIcontrol_move.h]
//  Author :Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAI_CONTROL_START_H_
#define _PLAYERAI_CONTROL_START_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerAIcontrol_mode.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CSuffocation;
class CEffekseerObj;
class CPlayerUserControlMove;

//==========================================================================
// プレイヤーAIコントロール_移動クラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerAIControlStart : public CPlayerAIControlMode
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerAIControlStart();
	~CPlayerAIControlStart();

	static CPlayerAIControlStart* Create(CPlayer* pPlayer);

	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

private:
	//=============================
	// メンバ関数
	//=============================
	void Gooooo();
	void SeeBall();							// ボールを見る
	bool IsPicksUpBall();					// ボールを拾う判断

	//=============================
	// メンバ変数
	//=============================

};

#endif