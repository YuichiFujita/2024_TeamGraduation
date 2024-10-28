//=============================================================================
// 
//  AIプレイヤーヘッダー [playerAI.h]
//  Author : 藤田勇一
// 
//=============================================================================

#ifndef _PLAYER_AI_
#define _PLAYER_AI_		// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "player.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerAIControlMove;		// 移動(AI)
class CPlayerAIControlAction;	// アクション(AI)

//==========================================================================
// クラス定義
//==========================================================================
// AIプレイヤークラス
class CPlayerAI : public CPlayer
{
public:
	
	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerAI();
	~CPlayerAI();

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
	void ChangeMoveControl(CPlayerAIControlMove* control);		// 移動の操作変更
	void ChangeActionControl(CPlayerAIControlAction* control);	// アクションの操作変更

	//=============================
	// 静的関数
	//=============================
	/*
		@brief		生成処理
		@details	プレイヤーインデックス
		@details	チームサイド
	*/
	static CPlayerAI *Create(const CGameManager::TeamSide team, const MyLib::Vector3& rPos);

protected:

	virtual void Debug() override;	// デバッグ処理

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
