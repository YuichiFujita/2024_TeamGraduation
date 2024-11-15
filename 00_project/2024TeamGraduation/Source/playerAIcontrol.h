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
	enum EHeart	// 心
	{
		HEART_NONE = 0,	// 抜け殻人
		HEART_NORMAL,	// 通常
		HEART_STRONG,	// 強気
		HEART_TIMID,	// 弱気
		HEART_MAX
	};

	enum ETiming	// タイミング
	{
		TIMING_NORMAL = 0,	// 通常
		TIMING_QUICK,		// 速
		TIMING_DELAY,		// 遅
		TIMING_MAX
	};

	enum  EThrowType	// 投げタイプ
	{
		THROWTYPE_NORMAL = 0,
		THROWTYPE_JUMP,
		THROWTYPE_SPECIAL,
		THROWTYPE_MAX
	};

	enum ECatchType	// キャッチ種類
	{
		CATCH_TYPE_NONE = 0,	// なし
		CATCH_TYPE_NORMAL,		// 通常
		CATCH_TYPE_JUST,		// ジャスト
		CATCH_TYPE_DASH,		// ダッシュ
		CATCH_TYPE_FIND,		// 取りに行く
		CATCH_TYPE_MAX
	};

	enum EMoveType	// 行動種類
	{
		MOVETYPE_NONE = 0,
		MOVETYPE_WALK,			// 歩く
		MOVETYPE_DASH,			// 走る
		MOVETYPE_LEAVE,			// 離れる
		MOVETYPE_APPROATCH,		// 近づく
		MOVETYPE_MAX
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

	struct SCatchInfo	// キャッチ情報
	{
		ECatchType eCatchType;		// キャッチ種類
	};

	struct SMoveInfo	// 行動
	{
		EMoveType eType;	// タイプ
		bool bJump;			// ジャンプフラグ
	};

	struct SMode	// モード
	{
		EMode eMode;				// モード
	};

	struct SInfo
	{
		SMode sMode;				// モード
		EHeart eHeart;				// 心
		SThrowInfo sThrowInfo;		// 投げ情報
		SCatchInfo sCatchInfo;		// キャッチ情報
		SMoveInfo sMoveInfo;		// 行動情報
	};

public:

	// コンストラクタ
	CPlayerAIControl();

	static CPlayerAIControl* Create(CPlayer* player);

	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetMode(EMode mode) { m_sInfo.sMode.eMode = mode; }	// モード設定
	void SetPlayerInfo(CPlayer* player) { m_pAI = player; }

private:
	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayerAIControl::* MODE_FUNC)(const float, const float, const float);
	static MODE_FUNC m_ModeFunc[];			// モード関数

	typedef void(CPlayerAIControl::* THOWTYPE_FUNC)(CPlayer*, const float, const float, const float);
	static THOWTYPE_FUNC m_ThrowTypeFunc[];			// 投げるタイプ関数

	typedef void(CPlayerAIControl::* THOWMOVE_FUNC)(CPlayer*, const float, const float, const float);
	static THOWMOVE_FUNC m_ThrowMoveFunc[];			// 投げるまでの行動関数

	typedef void(CPlayerAIControl::* THOWTIMING_FUNC)(CPlayer*, const float, const float, const float);
	static THOWTIMING_FUNC m_ThrowTimingFunc[];			// 投げタイプ関数

	typedef void(CPlayerAIControl::* CATCH_FUNC)(const float, const float, const float);
	static CATCH_FUNC m_CatchFunc[];		// キャッチ関数

	//=============================
	// メンバ関数
	//=============================
	void ModeManager();
	//-----------------------------
	// 状態関数
	//-----------------------------
	void ModeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// なし
	void ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 投げ統括
	void ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// キャッチ統括

	// 投げタイプ
	void ThrowTypeNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTypeJump(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowTypeSpecial(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	// 投げるまでの行動
	void ThrowMoveStop(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowMoveWalk(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowMoveDash(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// 投げタイミング
	void ThrowJumpTimingNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowJumpTimingQuick(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void ThrowJumpTimingDelay(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	// キャッチ
	void CatchNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void FindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//-----------------------------
	// その他関数
	//-----------------------------
	void SetThrowTarget(CPlayer** ppTarget = nullptr);		// 投げるターゲット
	void DistanceLeaveCatch();		// キャッチ距離(離れる)
	void DistanceApproachCatch();	// キャッチ距離(近づく)

	void PlanThrowFlow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 投げる流れ
	void PlanHeart();		// 心のプラン
	void PlanThrow(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 投げるプラン
	void PlanThrowDistance(CPlayer* pTarget);	// 投げる距離プラン
	void PlanIsJump(CPlayer* pTarget);	// 跳ぶかどうか
	void PlanMove(CPlayer* pTarget);	// 行動プラン

	bool IsLineOverBall();	// 線超え判定(ボール)
	void JumpThrowTiming(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void StrategyTiming(CPlayer* pTarget);	// タイミングの思考
	void StrategyLine(CPlayer* pTarget);
	void LineLeftTeam(CPlayer* pTarget);
	void LineRightTeam(CPlayer* pTarget);

	void Reset();			// 変数リセット

	bool IsWhoPicksUpTheBall();

	void RunUp(CPlayer* pTarget);

	void Debug();

	//=============================
	// メンバ変数
	//=============================
	SInfo m_sInfo;		// モード情報
	CPlayer* m_pAI;			// 自分自身

	float m_fDistance;			// ターゲットとの距離
	bool m_bStart;
};

#endif
