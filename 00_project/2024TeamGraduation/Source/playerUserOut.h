//==========================================================================
// 
//  ユーザー外野プレイヤーヘッダー [playerUserOut.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_USER_OUT_
#define _PLAYER_USER_OUT_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerOut.h"
#include "playerUserOut_controlMove.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CBindKey;

//==========================================================================
// クラス定義
//==========================================================================
// ユーザー外野プレイヤークラス
class CPlayerUserOut : public CPlayerOut
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerUserOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea);
	virtual ~CPlayerUserOut() override;

	//=============================
	// オーバーライド関数
	//=============================
	virtual CPlayerUserOut* GetPlayerUserOut() override { return this; }	// ユーザー外野プレイヤー取得

	//=============================
	// メンバ関数
	//=============================
	void BindLeftKey(CBindKey* pKey) { GetPlayerControlMove()->GetUserOut()->BindLeftKey(pKey); }	// 左移動キー割当
	void BindRightKey(CBindKey* pKey) { GetPlayerControlMove()->GetUserOut()->BindRightKey(pKey); }	// 右移動キー割当
};

#endif
