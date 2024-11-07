//=============================================================================
// 
//  ユーザープレイヤーヘッダー [playerUser.h]
//  Author : 藤田勇一
// 
//=============================================================================

#ifndef _PLAYER_USER_
#define _PLAYER_USER_		// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "player.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerUserControlMove;	// 移動(操作)
class CPlayerUserControlAction;	// アクション(操作)

//==========================================================================
// クラス定義
//==========================================================================
// ユーザープレイヤークラス
class CPlayerUser : public CPlayer
{
public:
	
	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerUser();
	~CPlayerUser();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;

	//=============================
	// メンバ関数
	//=============================
	void Kill();	// 削除

	//=============================
	// パターン
	//=============================
	void ChangeMoveControl(CPlayerUserControlMove* control);		// 移動の操作変更
	void ChangeActionControl(CPlayerUserControlAction* control);	// アクションの操作変更

private:

	//=============================
	// メンバ関数
	//=============================
	//-----------------------------
	// その他関数
	//-----------------------------
	void Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 操作
	void DeleteControl() override;	// 操作削除

	//=============================
	// メンバ変数
	//=============================
};

#endif
