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
// 定数定義
//==========================================================================
namespace playerAIcontrol
{
	// 投げ関連
	const float THROW_JUMP_END = 130.0f;			// ジャンプ投げの最大位置(ジャンプ力MAX：150)

	// 線越え判定(中心(x)からの距離)
	const float LINE_DISTANCE_OVER = 10.0f;		// 線超え判定の距離
	const float RETURN_POS = 300.0f;			// 戻る位置
	const float OK_LENGTH = 50.0f;				// 判定の範囲(目的との距離)
}

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
		HEART_CRAYZY,			// 狂人
		HEART_MAX
	};

	enum EMode					// モード
	{
		MODE_NONE = 0,			// なし
		MODE_THROW,				// 投げ
		MODE_CATCH,				// キャッチ
		MODE_MAX
	};

	enum EThrowTiming			// タイミング
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

	enum EMoveTypeChatch
	{
		MOVETYPE_NONE = 0,		// なし
		MOVETYPE_DISTANCE,		// 距離を取る
		MOVETYPE_RANDOM,		// ランダム
		MOVETYPE_LEFT,			// 左
		MOVETYPE_RIGHT,			// 右
		MOVETYPE_FORWARD,		// 前方
		MOVETYPE_BACK,			// 後方
		MOVETYPE_MAX
	};

	enum EMoveForcibly			// 強制行動
	{
		FORCIBLY_NONE = 0,		// なし
		FORCIBLY_STOP,			// 止まる
		FORCIBLY_RETURN,		// 戻る
		FORCIBLY_START,			// 初め
		FORCIBLY_MAX,
	};

	enum ESee
	{
		SEE_NONE = 0,
		SEE_BALL,
		SEE_TARGET,
		SEE_MAX
	};

	enum EThrowFlag				// 投げフラグ
	{
		THROW_NONE = 0,			// なし
		THROW_NORMAL,			// 投げ
		THROW_PASS,				// パス
		THROW_SPECIAL,			// スペシャル
	};

	enum EMoveFlag				// 行動フラグ
	{
		MOVEFLAG_STOP = 0,		// 止まる
		MOVEFLAG_WALK,			// 歩く
		MOVEFLAG_BLINK,			// ブリンク
		MOVEFLAG_DASH,			// 走る
		MOVEFLAG_MAX
	};

	enum EActionFlag			// アクションフラグ
	{
		ACTION_NONE = 0,		// なし
		ACTION_JUMP,			// ジャンプ
		ACTION_MAX
	};

	//=============================
	// 構造体定義
	//=============================
	struct SThrow			// 投げ関連
	{
		float fTiming;		// タイミングカウント
		float fTimingRate;	// タイミングの割合
		float fJumpEnd;		// ジャンプの終了位置
		bool bTiming;		// タイミングフラグ
		bool bFoldJump;		// ジャンプの折り返しフラグ
	};

	struct SMove
	{
		float fRot;		// 向きのランド
		float fTimer;		// 行動タイマー
		bool bSet;			// 設定完了いてるか
	};

	struct SDistance		// 距離
	{
		float fInPair;		// 内野：相手
		float fInAlly;		// 内野：味方
		float fOut;			// 外野
		float fTarget;		// ターゲット
	};

public:

	// コンストラクタ
	CPlayerAIControl();

	static CPlayerAIControl* Create(CPlayer* player);

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//-----------------------------
	// 設定,取得
	//-----------------------------
	CPlayer* GetThrowTarget();												// 投げるターゲット取得
	CPlayer* GetBallOwner();												// ボール持ち主取得
	void SetMode(EMode mode) { m_eMode = mode; }							// モード設定
	EMode GetMode() { return m_eMode; }										// 取得
	void SetForcibly(EMoveForcibly forcibly) { m_eForcibly = forcibly; }	// 強制行動設定
	EMoveForcibly GetForcibly() { return m_eForcibly; }						// 取得
	void SetMove(EMoveFlag move) { m_eMoveFlag = move; }					// 行動設定
	EMoveFlag GetMove() { return m_eMoveFlag; }								// 取得
	void SetAction(EActionFlag action) { m_eActionFlag = action; }			// アクション設定
	EActionFlag GetAction() { return m_eActionFlag; }						// 取得
	void SetThrow(EThrowFlag Throw) { m_eThrow = Throw; }					// 投げ設定
	EThrowFlag GetThrow() { return m_eThrow; }								// 取得
	void SetPlayer(CPlayer* player) { m_pAI = player; }						// 自分の設定
	CPlayer* GetPlayer() { return m_pAI; }									// 取得


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

private:
	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayerAIControl::* MODE_FUNC)(const float, const float, const float);
	static MODE_FUNC m_ModeFunc[];					// モード関数

	typedef void(CPlayerAIControl::* MOVEFORCIBLY_FUNC)();
	static MOVEFORCIBLY_FUNC m_MoveForciblyFunc[];	// 強制行動関数

	typedef void(CPlayerAIControl::* MOVEFLAG_FUNC)();
	static MOVEFLAG_FUNC m_MoveFlagFunc[];				// 行動関数

	typedef void(CPlayerAIControl::* MOVETYPE_FUNC)(const float, const float, const float);
	static MOVETYPE_FUNC m_MoveTypeFunc[];				// 行動関数

	typedef void(CPlayerAIControl::* ACTION_FUNC)();
	static ACTION_FUNC m_ActionFunc[];				// アクション関数

	typedef void(CPlayerAIControl::* THROWTYPE_FUNC)();
	static THROWTYPE_FUNC m_ThrowTypeFunc[];		// 投げるタイプ関数

	typedef void(CPlayerAIControl::* THROWMOVE_FUNC)();
	static THROWMOVE_FUNC m_ThrowMoveFunc[];		// 投げるまでの行動関数

	typedef void(CPlayerAIControl::* THROW_FUNC)();
	static THROW_FUNC m_ThrowFunc[];				// 投げタイミング関数

	typedef void(CPlayerAIControl::* THROWTIMING_FUNC)(const float, const float, const float);
	static THROWTIMING_FUNC m_ThrowTimingFunc[];	// 投げタイミング関数

	typedef void(CPlayerAIControl::* CATCH_FUNC)(const float, const float, const float);
	static CATCH_FUNC m_CatchFunc[];				// キャッチ関数

	//-----------------------------
	// 状態関数
	//-----------------------------
	// モード
	void ModeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// なし
	void ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 投げ統括
	void ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// キャッチ統括

	// 強制行動
	void ForciblyNone() {};				// なし
	void ForciblyStop();				// 止まる
	virtual void ForciblyReturn() = 0;	// 戻る
	void ForciblyStart();				// 初め

	// 行動
	void MoveFlagStop();			// なし
	void MoveFlagWalk();			// 歩く
	void MoveFlagBlink();			// ブリンク
	void MoveFlagDash();			// 走る

	// 行動タイプ
	void MoveTypeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};	// なし
	void MoveTypeDistance(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 距離を取る
	void MoveTypeAtyakotya(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// あっちゃこっちゃ
	void MoveTypeLeft(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 左右
	void MoveTypeRight(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 左右
	void MoveTypeUp(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 上下
	void MoveTypeDown(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 上下

	// アクション
	void ActionNone();			// なし
	void ActionJump();			// ジャンプ

	// 投げタイプ
	void ThrowTypeNone() {};	// なし
	void ThrowTypeNormal();		// 通常
	void ThrowTypeJump();		// ジャンプ
	void ThrowTypeSpecial();	// スペシャル

	// 投げ
	void ThrowNone() {};		// なし
	void Throw();				// 投げ
	void ThrowPass();			// パス
	void ThrowSpecial();		// スペシャル

	// 投げるまでの行動
	void ThrowMoveNone();		// なし
	void ThrowMoveWalk();		// 歩き
	void ThrowMoveDash();		// 走り

	// 投げタイミング
	void ThrowTimingNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void ThrowTimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// キャッチ
	void CatchNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchPassGive(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchPassSteal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchFindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//=============================
	// メンバ関数
	//=============================
	void ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void UpdateMode(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// モード
	void UpdateForcibly();		// 強制行動
	void UpdateMoveFlag(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 行動
	void UpdateMoveType(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 行動
	void UpdateActionFlag();		// アクション
	void UpdateThrowType();		// 投げ種類
	void UpdateThrowMove();		// 投げ行動
	void UpdateThrowFlag();			// 投げ
	void UpdateThrowTiming(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 投げタイミング
	void UpdateCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// キャッチ
	void UpdateSee();			// 見る

	void PlanThrow();			// 投げのプラン
	void PlanHeart();			// 心のプラン

	bool IsDistanceBall();					// 距離：ボール
	bool IsPassTarget();					// パスする相手がいるか判定
	bool IsWhoPicksUpTheBall();				// ボールを拾う判断
	void SeeTarget(MyLib::Vector3 pos);		// ターゲットをみる
	void SeeBall();							// ボールを見る
	float GetDistance(CPlayer::EFieldArea area, CGameManager::ETeamSide teamMy, CGameManager::ETeamSide teamPair);
	float GetDistanceBallowner();

	void SetMoveTimer(float timer);

	void Parameter();

	//=============================
	// メンバ変数
	//=============================
	CPlayer* m_pAI;					// 自分情報

	//-----------------------------
	// 列挙
	//-----------------------------
	EMode m_eMode;					// モード
	EMoveForcibly m_eForcibly;		// 強制行動
	EMoveFlag m_eMoveFlag;			// 行動フラグ
	EMoveTypeChatch m_eMoveType;			// 行動タイプ
	EHeart m_eHeart;				// 心
	EActionFlag m_eActionFlag;		// アクションフラグ
	EThrowType m_eThrowType;		// 投げ種類
	EThrowFlag m_eThrow;			// 投げ
	EThrowTiming m_eThrowTiming;	// 投げタイミング
	ECatchType m_eCatchType;		// キャッチ
	ESee m_eSee;					// 見る

	//-----------------------------
	// 構造体
	//-----------------------------
	SThrow m_sThrow;				// 投げ
	SMove m_sMove;					// 行動
	SDistance m_sDistance;			// 距離
};

#endif
