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
	// 列挙型定義
	//=============================
	enum EThrowMode
	{
		MODE_NONE = 0,	// 通常
		MODE_WALK,		// 歩き
		MODE_DASH,		// 走り
		MODE_QUICK,		// すぐに
		MODE_DELAY,		// 遅らせて
		MODE_MAX
	};
	
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
	// 関数リスト
	//=============================
	typedef void(CPlayerAI::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// 状態関数

	//=============================
	// メンバ関数
	//=============================
	SHitInfo Hit(CBall* pBall) override;

	//-----------------------------
	// 状態関数
	//-----------------------------
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 状態更新
	void StateNone();				// なし
	void StateThrowManager(CBall* pBall);	// 
	void StateCatchManager();

	void Throw();	// 通常投げ
	void JumpThrow();	// ジャンプ投げ
	void SpecialThrow();	// スペシャル投げ

	//-----------------------------
	// その他関数
	//-----------------------------
	void Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 操作
	void DeleteControl() override;	// 操作削除

	//=============================
	// メンバ変数
	//=============================
	EThrowMode m_throwMode;
};

#endif
