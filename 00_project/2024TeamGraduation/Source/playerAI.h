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
private:
	//=============================
	// 列挙型定義
	//=============================
	enum EMode	// モード
	{
		MODE_NONE = 0,	// なし
		MODE_THROW,		// 投げ
		MODE_CATCH,		// キャッチ
		MODE_MAX
	};

	enum EThrowType	// 投げの種類
	{
		TYPE_NONE = 0,	// なし
		TYPE_NORMAL,	// 通常投げ
		TYPE_JUMP,		// ジャンプ投げ
		TYPE_SPECIAL,	// スペシャル投げ
		TYPE_MAX
	};

	enum EThrowMove	// 投げの状態
	{
		MOVE_NORMAL = 0,		// 通常
		MOVE_WALK,			// 歩き
		MOVE_DASH,			// 走り
		MOVE_MAX
	};

	enum EThrowTiming	// 投げのタイミング
	{
		TIMING_NORMAL = 0,	// 通常
		TIMING_QUICK,		// 速く
		TIMING_DELAY,		// 遅く
		TIMING_FEINT,		// フェイント
		TIMING_MAX
	};

	//=============================
	// 構造体定義
	//=============================

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
	// 関数リスト
	//=============================
	typedef void(CPlayerAI::* MODE_FUNC)(const float , const float, const float);
	static MODE_FUNC m_ModeFunc[];	// モード関数

	typedef void(CPlayerAI::* TYPE_FUNC)(const float, const float, const float);
	static TYPE_FUNC m_ThrowTypeFunc[];	// 投げ種類関数

	typedef void(CPlayerAI::* MOVE_FUNC)(const float, const float, const float);
	static MOVE_FUNC m_ThrowMoveFunc[];	// 投げ状態関数

	typedef void(CPlayerAI::* TIMING_FUNC)(const float, const float, const float);
	static TIMING_FUNC m_ThrowTimingFunc[];	// 投げタイミング関数


	//=============================
	// メンバ関数
	//=============================
	SHitInfo Hit(CBall* pBall) override;

	//-----------------------------
	// 状態関数
	//-----------------------------
	void UpdateMode(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);				// 状態更新
	void ModeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};			// なし
	void ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 投げ統括
	void ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// キャッチ統括

	// 投げの種類関数
	void TypeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// なし
	void TypeThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 通常投げ
	void TypeJumpThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ジャンプ投げ
	void TypeSpecialThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// スペシャル投げ
	
	// 投げの状態
	void MoveNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void MoveWalk(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 歩いて投げる
	void MoveDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 走って投げる

	// 投げのタイミング
	void TimingManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void TimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void TimingQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void TimingDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void TimingFeint(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//-----------------------------
	// その他関数
	//-----------------------------
	void Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 操作
	void DeleteControl() override;	// 操作削除

	//=============================
	// メンバ変数
	//=============================
	EMode m_eMode;
	EThrowType m_eThrowType;
	EThrowMove m_eThrowMove;
	EThrowTiming m_eThrowTiming;

	float m_fTimingCount;	// タイミングカウント
	bool m_bTiming;			// タイミングフラグ
};

#endif
