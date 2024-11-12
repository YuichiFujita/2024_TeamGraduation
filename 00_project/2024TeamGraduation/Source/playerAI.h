//==========================================================================
// 
//  AIプレイヤーヘッダー [playerAI.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_AI_
#define _PLAYER_AI_		// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerBase.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerAIControlMove;		// 移動(AI)
class CPlayerAIControlAction;	// アクション(AI)

//==========================================================================
// クラス定義
//==========================================================================
// AIプレイヤークラス
class CPlayerAI : public CPlayerBase
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
		MOVE_NORMAL = 0,	// 通常
		MOVE_WALK,			// 歩き
		MOVE_DASH,			// 走り
		MOVE_MAX
	};

	enum EThrowTiming	// 投げのタイミング
	{
		TIMING_NORMAL = 0,	// 通常
		TIMING_FEINT,		// フェイント
		TIMING_JUMP_NORMAL,	// 通常(ジャンプ)
		TIMING_JUMP_QUICK,	// 速く(ジャンプ)
		TIMING_JUMP_DELAY,	// 遅く(ジャンプ)
		TIMING_JUMP_FEINT,	// フェイント(ジャンプ)
		TIMING_MAX
	};

	enum ECatchType
	{
		CATCH_TYPE_NONE = 0,	// なし
		CATCH_TYPE_NORMAL,		// 通常
		CATCH_TYPE_JUST,		// ジャスト
		CATCH_TYPE_DASH,		// ダッシュ
		CATCH_TYPE_FIND,		// 取りに行く
		CATCH_TYPE_MAX
	};

public:
	
	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerAI(CPlayer* pPlayer, const CPlayer::EFieldArea typeArea);
	~CPlayerAI();

	//=============================
	// オーバーライド関数
	//=============================
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	virtual CPlayer::SHitInfo Hit(CBall* pBall) override;	// ヒット
	virtual void Debug() override;	// デバッグ

	//=============================
	// パターン
	//=============================
	void ChangeMoveControl(CPlayerAIControlMove* control);		// 移動の操作変更
	void ChangeActionControl(CPlayerAIControlAction* control);	// アクションの操作変更

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayerAI::* MODE_FUNC)(const float , const float, const float);
	static MODE_FUNC m_ModeFunc[];			// モード関数

	typedef void(CPlayerAI::* TYPE_FUNC)(const float, const float, const float);
	static TYPE_FUNC m_ThrowTypeFunc[];		// 投げ種類関数

	typedef void(CPlayerAI::* MOVE_FUNC)(const float, const float, const float);
	static MOVE_FUNC m_ThrowMoveFunc[];		// 投げ状態関数

	typedef void(CPlayerAI::* TIMING_FUNC)(const float, const float, const float);
	static TIMING_FUNC m_ThrowTimingFunc[];	// 投げタイミング関数

	typedef void(CPlayerAI::* CATCH_FUNC)(const float, const float, const float);
	static CATCH_FUNC m_CatchFunc[];		// 投げタイミング関数

	//=============================
	// メンバ関数
	//=============================
	//-----------------------------
	// 状態関数
	//-----------------------------
	void UpdateMode(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 状態更新
	void ModeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// なし
	void ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 投げ統括
	void ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// キャッチ統括

	// 投げの種類関数
	void TypeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// なし
	void TypeThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 通常投げ
	void TypeJumpThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ジャンプ投げ
	void TypeSpecialThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// スペシャル投げ
	
	// 移動の状態
	void MoveNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// その場投げ
	void MoveWalk(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 歩いて投げ
	void MoveDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 走って投げ

	// 投げのタイミング
	void TimingManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 
	void TimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 
	void TimingFeint(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 

	void TimingJumpNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void TimingJumpQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void TimingJumpDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void TimingJumpFeint(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// キャッチ
	void CatchNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void FindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//-----------------------------
	// その他関数
	//-----------------------------
	void Reset();	// 列挙リセット
	void Target();	// ターゲット
	bool IsWait();	// 待て

	//=============================
	// メンバ変数
	//=============================
	EMode m_eMode;					// モード
	EThrowType m_eThrowType;		// 投げ種類
	EThrowMove m_eThrowMove;		// 投げ行動
	EThrowTiming m_eThrowTiming;	// 投げタイミング
	ECatchType m_eCatchType;		// キャッチ種類
	float m_fTiming;		// タイミングカウント
	float m_fTimingRate;	// タイミングの割合
	bool m_bTiming;			// タイミングフラグ
	bool m_bFoldJump;		// ジャンプの折り返しフラグ
	float m_fJumpEnd;		// ジャンプの終了位置
};

#endif
