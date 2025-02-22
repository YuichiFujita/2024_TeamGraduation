//==========================================================================
// 
//  プレイヤー位置補正_右内野コートヘッダー [playerPosAdj_inRight.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_POSADJ_INRIGHT_H_
#define _PLAYER_POSADJ_INRIGHT_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerPosAdj_in.h"

//==========================================================================
// プレイヤー位置補正_右内野コート定義
//==========================================================================
class CPlayerPosAdjInRight : public CPlayerPosAdjIn
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerPosAdjInRight();
	virtual ~CPlayerPosAdjInRight() override;

private:
	//=============================
	// オーバーライド関数
	//=============================
	virtual bool IsLineOut(CPlayer* pPlayer) override;								// ライン越えフラグ取得
	virtual bool IsUnstable(CPlayer* pPlayer) override;								// おっとっとフラグ取得
	virtual CPlayerPosAdjIn::EInputUnstable IsInputLine(CPlayer* pPlayer) override;	// おっとっと時入力フラグ取得
};

#endif