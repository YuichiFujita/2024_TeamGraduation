//=============================================================================
// 
//  ユーザープレイヤーヘッダー [playerUser.h]
//  Author : 藤田勇一
// 
//=============================================================================

#ifndef _PLAYER_USER_
#define _PLAYER_USER_		// 二重インクルード防止

//==========================================================================
// クラス定義
//==========================================================================
#include "player.h"

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
	void ChangeMoveControl(CPlayerControlMove* control);		// 移動の操作変更
	void ChangeActionControl(CPlayerControlAction* control);	// アクションの操作変更

	//=============================
	// 静的関数
	//=============================
	/*
		@brief		生成処理
		@details	必要があれば引数追加
	*/
	static CPlayerUser *Create(const int nIdx);

private:

	//=============================
	// メンバ関数
	//=============================
	//-----------------------------
	// その他関数
	//-----------------------------
	void Move(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 移動
	void DeleteControl() override;	// 操作削除

	//=============================
	// メンバ変数
	//=============================
	//-----------------------------
	// パターン用インスタンス
	//-----------------------------
	CPlayerControlMove* m_pControlMove;		// 移動操作
	CPlayerControlAction* m_pControlAction;	// アクション操作
};

#endif
