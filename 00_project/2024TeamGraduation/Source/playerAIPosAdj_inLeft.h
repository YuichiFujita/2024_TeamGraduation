//==========================================================================
// 
//  プレイヤー位置補正_左内野コートヘッダー [playerPosAdj_inLeft.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_AI_POSADJ_INLEFT_H_
#define _PLAYER_AI_POSADJ_INLEFT_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerPosAdj_inLeft.h"
#include "playerPosAdj_in.h"

class CPlayer;

//==========================================================================
// プレイヤー位置補正_左内野コート定義
//==========================================================================
class CPlayerAIPosAdjInLeft : public CPlayerPosAdjInLeft
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerAIPosAdjInLeft();
	virtual ~CPlayerAIPosAdjInLeft() override;

private:
	//=============================
	// オーバーライド関数
	//=============================
	virtual bool IsLineOut(CPlayer* pPlayer) override;								// ライン越えフラグ取得
	virtual bool IsUnstable(CPlayer* pPlayer) override;								// おっとっとフラグ取得
	virtual CPlayerPosAdjIn::EInputUnstable IsInputLine(CPlayer* pPlayer) override;	// おっとっと時入力フラグ取得
};

#endif