//==========================================================================
// 
//  ユーザー内野プレイヤーヘッダー [playerUserIn.h]
//  Author : 藤田勇一
//  Adder  : 髙田佳依
// 
//==========================================================================

#ifndef _PLAYER_USER_IN_
#define _PLAYER_USER_IN_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerIn.h"

//==========================================================================
// クラス定義
//==========================================================================
// ユーザー内野プレイヤークラス
class CPlayerUserIn : public CPlayerIn
{
public:
	
	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerUserIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea);
	virtual ~CPlayerUserIn() override;

	//=============================
	// オーバーライド関数
	//=============================
	virtual void MotionCrab(int nStartKey) override;	// カニ歩き変化
	virtual CPlayerUserIn* GetPlayerUserIn() override { return nullptr; }	// ユーザー内野プレイヤー取得
};

#endif
