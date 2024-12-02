//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_RIGHT_H_
#define _PLAYERAICONTROL_RIGHT_H_	// 二重インクルード防止

#include "player.h"
#include "playerAIcontrol.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerAIControlMove;		// 移動(AI)
class CPlayerAIControlAction;	// アクション(AI)
class CPlayer;

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
class CPlayerAIControlRight : public CPlayerAIControl
{
public:

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

private:
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

	enum ETiming				// タイミング
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
		THROWTYPE_MAX
	};

	enum ECatchType				// キャッチ種類
	{
		CATCH_TYPE_NONE = 0,	// なし
		CATCH_TYPE_NORMAL,		// 通常
		CATCH_TYPE_JUST,		// ジャスト
		CATCH_TYPE_DASH,		// ダッシュ
		CATCH_TYPE_PASS,		// パス
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

	enum EThrow				// 投げ種類
	{
		THROW_NONE = 0,		// なし
		THROW_NORMAL,		// 投げ
		THROW_PASS,			// パス
		THROW_SPECIAL,		// スペシャル
	};

	enum EAction	// アクション
	{
		ACTION_NONE = 0,
		ACTION_JUMP,
		ACTION_MAX
	};

	enum ELine
	{
		LINE_IN = 0,	// 線内
		LINE_OVER,		// 線外
	};

	//=============================
	// 構造体定義
	//=============================
	struct SThrowInfo	// 投げ情報
	{
		EThrowType eType;	// タイプ
		ETiming eTiming;	// タイミング

		float fTiming;		// タイミングカウント
		float fTimingRate;	// タイミングの割合
		float fJumpEnd;		// ジャンプの終了位置
		bool bTiming;		// タイミングフラグ
		bool bFoldJump;		// ジャンプの折り返しフラグ
		bool bThrow;		// 投げてよし！
	};

	struct SInfo
	{
		SThrowInfo sThrowInfo;		// 投げ情報
		EMode eMode;				// モード
		ECatchType eCatchType;		// キャッチ種類
	};

	struct STarget	// 学習
	{
		float fDistanceIN;		// 内野との距離
		float fDistanceOUT;		// 外野との距離
	};

public:

	// コンストラクタ
	CPlayerAIControlRight();

	static CPlayerAIControlRight* Create(CPlayer* player);

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

	void SetMode(EMode mode) { m_sInfo.eMode = mode; }	// モード設定
	void SetPlayerInfo(CPlayer* player) { m_pAI = player; }

private:
	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayerAIControlRight::* AREA_FUNC)();
	static AREA_FUNC m_AreaFunc[];			// モード関数

	typedef void(CPlayerAIControlRight::* MODE_FUNC)(const float, const float, const float);
	static MODE_FUNC m_ModeFunc[];			// モード関数

	typedef void(CPlayerAIControlRight::* THROWTYPE_FUNC)(CPlayer*, const float, const float, const float);
	static THROWTYPE_FUNC m_ThrowTypeFunc[];			// 投げるタイプ関数

	typedef void(CPlayerAIControlRight::* THROWMOVE_FUNC)(CPlayer*, const float, const float, const float);
	static THROWMOVE_FUNC m_ThrowMoveFunc[];			// 投げるまでの行動関数

	typedef void(CPlayerAIControlRight::* THROWTIMING_FUNC)(CPlayer*, const float, const float, const float);
	static THROWTIMING_FUNC m_ThrowTimingFunc[];			// 投げタイミング関数

	typedef void(CPlayerAIControlRight::* THROW_FUNC)();
	static THROW_FUNC m_ThrowFunc[];			// 投げタイミング関数

	typedef void(CPlayerAIControlRight::* CATCH_FUNC)(const float, const float, const float);
	static CATCH_FUNC m_CatchFunc[];			// キャッチ関数

	typedef void(CPlayerAIControlRight::* MOVEFORCIBLY_FUNC)();
	static MOVEFORCIBLY_FUNC m_MoveForciblyFunc[];			// 行動関数

	typedef void(CPlayerAIControlRight::* MOVE_FUNC)();
	static MOVE_FUNC m_MoveFunc[];			// 行動関数

	typedef void(CPlayerAIControlRight::* ACTION_FUNC)();
	static ACTION_FUNC m_ActionFunc[];			// 行動関数

	//=============================
	// メンバ関数
	//=============================
	void ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// モード管理
	void MoveManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 行動管理
	void ActionManager();
	void ThrowManager();

	//-----------------------------
	// 状態関数
	//-----------------------------
	void AreaNone();
	void AreaLeft();
	void AreaRight();

	void ModeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// なし
	void ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 投げ統括
	void ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// キャッチ統括

	// 投げタイプ
	void ThrowTypeNone(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void ThrowTypeNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTypeJump(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTypeSpecial(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// 投げるまでの行動
	void ThrowMoveNone(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowMoveWalk(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowMoveDash(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// 投げタイミング
	void ThrowTimingNone(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void ThrowTimingNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingQuick(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingDelay(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpQuick(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTimingJumpDelay(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// キャッチ
	void CatchNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchPass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchFindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void ThrowNone() {};
	void Throw();
	void ThrowPass();
	void ThrowSpecial();
	//void ActionJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// 強制行動
	void ForciblyNone() {};			// なし
	void ForciblyStop();			// 止まる
	void ForciblyReturn();			// 戻る
	void ForciblyStart();			// 初め

	// 行動
	void MoveStop();			// なし
	void MoveWalk();			// 歩く
	void MoveDash();			// 走る

	// アクション
	void ActionNone();
	void ActionJump();

	//-----------------------------
	// その他関数
	//-----------------------------
	CPlayer* GetThrowTarget();		// 投げるターゲット
	CPlayer* GetCatchTarget();		// キャッチターゲット

	void PlanHeart();		// 心のプラン
	void PlanThrow(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 投げるプラン

	void CatchDistance(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 距離：キャッチ状態
	void CatchOutDistance();				// 距離：外野
	void CatchLineLeftDistance();			// パスする相手
	void PlanIsJump(CPlayer* pTarget);		// 跳ぶかどうか
	void JumpThrowTiming(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Timing(CPlayer* pTarget);			// タイミングの思考

	bool IsPassTarget();					// パスする相手がいるか判定
	bool IsWhoPicksUpTheBall();				// ボールを拾う判断
	bool IsLineOverBall();					// 線超え判定(ボール)
	bool IsLineOverPlayer();				// 線越え判定(プレイヤー)

	bool Leave(MyLib::Vector3 targetPos, float distance);		// 離れる
	void LeaveOut(float distance);								// 離れる
	bool Approatch(MyLib::Vector3 targetPos, float distance);	// 近づく

	void AttackDash();

	void IsJumpCatch();

	//=============================
	// メンバ変数
	//=============================
	CPlayer* m_pAI;				// 自分情報
	CPlayer* m_pTarget;			// ターゲット情報

	SInfo m_sInfo;				// モード情報
	STarget m_sTarget;			// ターゲット情報
	EMoveForcibly m_eForcibly;	// 強制行動
	EMoveType m_eMove;			// 行動タイプ
	ELine m_eLine;				// 線
	EHeart m_eHeart;			// 心
	EAction m_eAction;
	EThrow m_eThrow;

	bool m_bStart;
	bool m_bEnd;
};

#endif
