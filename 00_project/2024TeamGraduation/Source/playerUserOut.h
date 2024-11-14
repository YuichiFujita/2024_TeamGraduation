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
#include "playerUser.h"

//==========================================================================
// クラス定義
//==========================================================================
// ユーザー外野プレイヤークラス
class CPlayerUserOut : public CPlayerUser
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
	virtual bool IsCrab() override { return false; }	// カニ歩き判定

	//=============================
	// メンバ関数
	//=============================
	MyLib::Vector3 GetPosLeft()	 { return m_posLeft; }	// 移動可能な左位置の取得
	MyLib::Vector3 GetPosRight() { return m_posRight; }	// 移動可能な右位置の取得

private:

	//=============================
	// メンバ変数
	//=============================
	MyLib::Vector3 m_posLeft;	// 移動可能左位置
	MyLib::Vector3 m_posRight;	// 移動可能右位置
};

#endif
