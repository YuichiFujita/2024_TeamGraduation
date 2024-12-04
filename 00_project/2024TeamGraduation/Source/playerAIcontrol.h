//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_H_
#define _PLAYERAICONTROL_H_	// 二重インクルード防止

#include "player.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerAIControlMove;		// 移動(AI)
class CPlayerAIControlAction;	// アクション(AI)
class CPlayer;

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
class CPlayerAIControl
{
public:
	//=============================
	// 列挙型定義
	//=============================
	enum EHeart					// 心
	{
		HEART_NONE = 0,			// 抜け殻人
		HEART_NORMAL,			// 通常
		HEART_STRONG,			// 強気
		HEART_TIMID,			// 弱気
		HEART_MAX
	};

	enum EMode					// モード
	{
		MODE_NONE = 0,			// なし
		MODE_THROW,				// 投げ
		MODE_CATCH,				// キャッチ
		MODE_MAX
	};

	enum EThrowTiming				// タイミング
	{
		TIMING_NONE = 0,
		TIMING_NORMAL,			// 通常
		TIMING_QUICK,			// 速
		TIMING_DELAY,			// 遅
		TIMING_JUMP_NORMAL,		// ジャンプ通常
		TIMING_JUMP_QUICK,		// ジャンプ速
		TIMING_JUMP_DELAY,		// ジャンプ遅
		TIMING_MAX
	};

	enum  EThrowType			// 投げタイプ
	{
		THROWTYPE_NONE = 0,		// なし
		THROWTYPE_NORMAL,		// 通常
		THROWTYPE_JUMP,			// ジャンプ
		THROWTYPE_SPECIAL,		// スペシャル
		THROWTYPE_PASS,			// パス
		THROWTYPE_MAX
	};

	enum EThrow					// 投げ種類
	{
		THROW_NONE = 0,			// なし
		THROW_NORMAL,			// 投げ
		THROW_PASS,				// パス
		THROW_SPECIAL,			// スペシャル
	};

	enum ECatchType				// キャッチ種類
	{
		CATCH_TYPE_NONE = 0,	// なし
		CATCH_TYPE_NORMAL,		// 通常
		CATCH_TYPE_JUST,		// ジャスト
		CATCH_TYPE_DASH,		// ダッシュ
		CATCH_TYPE_PASS_GIVE,	// パスを貰う
		CATCH_TYPE_PASS_STEAL,	// パスを奪う
		CATCH_TYPE_FIND,		// 取りに行く
		CATCH_TYPE_MAX
	};

	enum EMoveForcibly			// 強制行動
	{
		FORCIBLY_NONE = 0,		// なし
		FORCIBLY_STOP,			// 止まる
		FORCIBLY_RETURN,		// 戻る
		FORCIBLY_START,			// 初め
		FORCIBLY_MAX,
	};

	enum EMoveType				// 行動種類
	{
		MOVETYPE_STOP = 0,		// 止まる
		MOVETYPE_WALK,			// 歩く
		MOVETYPE_DASH,			// 走る
		MOVETYPE_MAX
	};

	enum EAction				// アクション
	{
		ACTION_NONE = 0,		// なし
		ACTION_JUMP,			// ジャンプ
		ACTION_MAX
	};

	//=============================
	// 構造体定義
	//=============================
	struct SThrow
	{
		float fTiming;		// タイミングカウント
		float fTimingRate;	// タイミングの割合
		float fJumpEnd;		// ジャンプの終了位置
		bool bTiming;		// タイミングフラグ
		bool bFoldJump;		// ジャンプの折り返しフラグ
	};

public:

	// コンストラクタ
	CPlayerAIControl();

	static CPlayerAIControl* Create(CPlayer* player);

	virtual HRESULT Init() = 0;
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

protected:
	//=============================
	// 仮想・純粋関数
	//=============================
	virtual bool IsLineOverBall() = 0;				// 線超え判定(ボール)
	virtual bool IsLineOverPlayer() = 0;			// 線越え判定(プレイヤー)
	virtual void AttackDash(CPlayer* pTarget) = 0;	// 走り投げ

	//=============================
	// メンバ関数
	//=============================
	bool Leave(MyLib::Vector3 targetPos, float distance);		// 離れる
	bool Approatch(MyLib::Vector3 targetPos, float distance);	// 近づく

	//-----------------------------
	// 設定。取得
	//-----------------------------
	CPlayer* GetThrowTarget();	// 投げるターゲット
	CPlayer* GetCatchTarget();	// キャッチターゲット

	void SetMode(EMode mode) { m_eMode = mode; }	// モード設定
	EMode GetMode() { return m_eMode; }
	void SetForcibly(EMoveForcibly forcibly) { m_eForcibly = forcibly; }
	EMoveForcibly GetForcibly() { return m_eForcibly; }
	void SetMove(EMoveType move) { m_eMove = move; }
	EMoveType GetMove() { return m_eMove; }
	void SetAction(EAction action) { m_eAction = action; }
	void SetThrow(EThrow a) { m_eThrow = a; }

	void SetPlayer(CPlayer* player) { m_pAI = player; }
	CPlayer* GetPlayer() { return m_pAI; }

private:
	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayerAIControl::* MODE_FUNC)(const float, const float, const float);
	static MODE_FUNC m_ModeFunc[];			// モード関数

	typedef void(CPlayerAIControl::* MOVEFORCIBLY_FUNC)();
	static MOVEFORCIBLY_FUNC m_MoveForciblyFunc[];	// 強制行動関数

	typedef void(CPlayerAIControl::* MOVE_FUNC)();
	static MOVE_FUNC m_MoveFunc[];					// 行動関数

	typedef void(CPlayerAIControl::* ACTION_FUNC)();
	static ACTION_FUNC m_ActionFunc[];				// アクション関数

	typedef void(CPlayerAIControl::* THROWTYPE_FUNC)();
	static THROWTYPE_FUNC m_ThrowTypeFunc[];			// 投げるタイプ関数

	typedef void(CPlayerAIControl::* THROWMOVE_FUNC)();
	static THROWMOVE_FUNC m_ThrowMoveFunc[];		// 投げるまでの行動関数

	typedef void(CPlayerAIControl::* THROW_FUNC)();
	static THROW_FUNC m_ThrowFunc[];				// 投げタイミング関数

	typedef void(CPlayerAIControl::* THROWTIMING_FUNC)(const float, const float, const float);
	static THROWTIMING_FUNC m_ThrowTimingFunc[];	// 投げタイミング関数

	typedef void(CPlayerAIControl::* CATCH_FUNC)();
	static CATCH_FUNC m_CatchFunc[];				// キャッチ関数

	//-----------------------------
	// 状態関数
	//-----------------------------
	// モード
	void ModeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// なし
	void ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 投げ統括
	void ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// キャッチ統括

	// 強制行動
	void ForciblyNone() {};			// なし
	void ForciblyStop();			// 止まる
	virtual void ForciblyReturn() = 0;			// 戻る
	void ForciblyStart();			// 初め

	// 行動
	void MoveStop();			// なし
	void MoveWalk();			// 歩く
	void MoveDash();			// 走る

	// アクション
	void ActionNone();
	void ActionJump();

	// 投げタイプ
	void ThrowTypeNone() {};
	void ThrowTypeNormal();
	void ThrowTypeJump();
	void ThrowTypeSpecial();

	// 投げ
	void ThrowNone() {};
	void Throw();
	void ThrowPass();
	void ThrowSpecial();

	// 投げるまでの行動
	void ThrowMoveNone();
	void ThrowMoveWalk();
	void ThrowMoveDash();

	// 投げタイミング
	void ThrowTimingNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void ThrowTimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// キャッチ
	void CatchNone() {};
	void CatchNormal();
	void CatchJust();
	void CatchDash();
	void CatchPassGive();
	void CatchPassSteal();
	void CatchFindBall();

	//=============================
	// メンバ関数
	//=============================
	void ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void UpdateMode();			// モード
	void UpdateForcibly();		// 強制行動
	void UpdateMove();			// 行動
	void UpdateAction();		// アクション
	void UpdateThrowType();		// 投げ種類
	void UpdateThrowMove();		// 投げ行動
	void UpdateThrow();			// 投げ
	void UpdateThrowTiming(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 投げタイミング
	void UpdateCatch();			// キャッチ

	void UpdateSee();

	void PlanThrow();
	void PlanHeart();		// 心のプラン

	void CatchDistance(CPlayer* pTarget);	// 距離：キャッチ状態
	bool IsPassTarget();					// パスする相手がいるか判定
	bool IsWhoPicksUpTheBall();				// ボールを拾う判断

	void SeeTarget(MyLib::Vector3 pos);		// ターゲットをみる
	void SeeBall();							// ボールを見る

	//=============================
	// メンバ変数
	//=============================
	CPlayer* m_pAI;					// 自分情報

	//-----------------------------
	// 列挙
	//-----------------------------
	EMode m_eMode;					// モード
	EMoveForcibly m_eForcibly;		// 強制行動
	EMoveType m_eMove;				// 行動タイプ
	EHeart m_eHeart;				// 心
	EAction m_eAction;				// アクション
	EThrowType m_eThrowType;		// 投げ種類
	EThrow m_eThrow;				// 投げ
	EThrowTiming m_eThrowTiming;	// 投げタイミング
	ECatchType m_eCatchType;		// キャッチ

	//-----------------------------
	// 構造体
	//-----------------------------
	SThrow m_sThrow;				// 投げ
};

#endif
