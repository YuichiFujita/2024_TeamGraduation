//==========================================================================
// 
//  ユーザープレイヤーヘッダー [playerUser.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_USER_
#define _PLAYER_USER_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerBase.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerUserControlMove;	// 移動(操作)
class CPlayerUserControlAction;	// アクション(操作)

//==========================================================================
// クラス定義
//==========================================================================
// ユーザープレイヤークラス
class CPlayerUser : public CPlayerBase
{
public:
	
	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerUser(CPlayer* pPlayer, const CPlayer::EFieldArea typeArea);
	~CPlayerUser() override;

	//=============================
	// オーバーライド関数
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新

	//=============================
	// パターン
	//=============================
	void ChangeMoveControl(CPlayerUserControlMove* control);		// 移動の操作変更
	void ChangeActionControl(CPlayerUserControlAction* control);	// アクションの操作変更
};

#endif
