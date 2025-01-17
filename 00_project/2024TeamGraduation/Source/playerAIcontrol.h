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
	enum EHeartMain				// 心
	{
		HEART_MAIN_NORMAL = 0,	// 通常
		HEART_MAIN_STRONG,		// 強気
		HEART_MAIN_TIMID,		// 弱気
		HEART_MAIN_MAX
	};

	enum EMode					// モード
	{
		MODE_IDLE = 0,			// 待機
		MODE_ATTACK,			// 攻め
		MODE_DEFENSE,			// 守り
		MODE_MAX
	};

	enum EMoveForcibly			// 強制行動
	{
		FORCIBLY_NONE = 0,		// なし
		FORCIBLY_STOP,			// 止まる
		FORCIBLY_RETURN,		// 戻る
		FORCIBLY_START,			// 初め
		FORCIBLY_MAX,
	};

	enum  EThrowType			// 投げタイプ
	{
		THROWTYPE_NONE = 0,		// なし
		THROWTYPE_NORMAL,		// 通常
		THROWTYPE_JUMP,			// ジャンプ
		THROWTYPE_SPECIAL,		// スペシャル
		//THROWTYPE_PASS,			// パス
		THROWTYPE_MAX
	};

	enum EActionStatus
	{
		ACTIONSTATUS_IDLE = 0,
		ACTIONSTATUS_ACTION,
		ACTIONSTATUS_COOLDOWN,
		ACTIONSTATUS_MAX
	};

	enum EAction				// アクション
	{
		IDLE = 0,
		DODGE,					// 回避行動
		SUPPORT,				// 味方をサポート
		CHASE_BALL,				// ボールを追いかける
		RETREAT,				// 後退（安全地帯に移動）
		RNDOM,					// ランダム
		LEAVE,					// 離れる
		MAX
	};

	enum ESupport
	{
		SUPPORT_NONE = 0,		// 
		SUPPORT_REBOUND,		// 
		SUPPORT_COVER,			// 
		SUPPORT_
	};

	// フラグ関連
	enum EThrowFlag				// 投げフラグ
	{
		THROW_NONE = 0,			// なし
		THROW_NORMAL,			// 投げ
		THROW_PASS,				// パス
		THROW_SPECIAL,			// スペシャル
	};

	enum EMoveFlag				// 行動フラグ
	{
		MOVEFLAG_IDLE = 0,		// 止まる
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
	struct SMove {
		CPlayer* pDefenseTarget;	// 守る対象
		MyLib::Vector3 pos;			// 位置
		float fTimer;				// タイマー
		bool bSet;					// 設定フラグ
		bool bCooldown;
		bool bCancel;				// キャンセル
	};

	struct SAction {
		float fTimer;				// タイマー
		bool bSet;					// 設定フラグ
		bool bCooldown;				// クールダウン
		bool bCancel;				// キャンセル
	};

private:
	//=============================
	// 構造体定義
	//=============================
	struct SParameter {			// パラメータ
		EHeartMain eHeartMain;	// 心(メイン)
		EHeartMain eHearDSub;	// 心(サブ)
		int nMotivation;		// モチベーション
		float fMove;			// 行動
		bool bSet;				// 設定ON/OFF
	};

	struct SAI
	{
		float fDistance;		// 距離感
		float fDistanceDefault;	// defaultの距離感
		float fDistanceValue;	// 距離感の増減
		bool bTypeAction;		// アクション種類
		bool bSet;				// 設定ON/OFF
		bool bRot;				// 向きは正しいか
		bool bDistance;			// 離れられているか
	};

public:

	// コンストラクタ
	CPlayerAIControl();

	// 生成
	static CPlayerAIControl* Create(CPlayer* player);

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//-----------------------------
	// 設定,取得
	//-----------------------------
	void SetPlayer(CPlayer* player) { m_pAI = player; }						// 自分の設定
	CPlayer* GetPlayer() { return m_pAI; }									// 取得
	void SetAction(EAction action) { m_eAction = action; }
	EAction GetAction() { return m_eAction; }

	void SetMode(EMode mode) { m_eMode = mode; }							// モード設定
	EMode GetMode() { return m_eMode; }										// 取得
	void SetForcibly(EMoveForcibly forcibly) { m_eForcibly = forcibly; }	// 強制行動設定
	EMoveForcibly GetForcibly() { return m_eForcibly; }						// 取得

	void SetActionStatus(EActionStatus status) { m_eActionStatus = status; }
	EActionStatus GetActionStatus() { return m_eActionStatus; }

	void SetMoveFlag(EMoveFlag move) { m_eMoveFlag = move; }				// 行動設定
	EMoveFlag GetMoveFlag() { return m_eMoveFlag; }							// 取得
	void SetActionFlag(EActionFlag action) { m_eActionFlag = action; }		// アクション設定
	EActionFlag GetActionFlag() { return m_eActionFlag; }					// 取得
	void SetThrowFlag(EThrowFlag Throw) { m_eThrowFlag = Throw; }			// 投げ設定
	EThrowFlag GetThrowFlag() { return m_eThrowFlag; }						// 取得

	void SetMoveInfo(SMove move) { m_sMove = move; }
	SMove GetMoveInfo() { return m_sMove; }
	void SetActionInfo(SAction action) { m_sAction = action; }
	SAction GetActionInfo() { return m_sAction; }

	void SetIsDistance(bool flag) { m_sAI.bDistance = flag; }

	void SetMoveFlagLandom(int nRate, int nMax, int nMin);

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

	// 行動
	void MoveIdle();		// 何もしない
	void MoveUp();			// 上移動
	void MoveDown();		// 下移動
	void MoveLeft();		// 左移動
	void MoveRight();		// 右移動

	//-----------------------------
	// 設定関数
	//-----------------------------
	CPlayer* GetThrowTarget();				// 投げるターゲット
	CPlayer* GetBallOwner();				// ボール持ち主
	float GetDistance() { return m_sAI.fDistance; }

private:
	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayerAIControl::* MODE_FUNC)(const float, const float, const float);
	static MODE_FUNC m_ModeFunc[];					// モード関数

	typedef void(CPlayerAIControl::* MOVEFORCIBLY_FUNC)();
	static MOVEFORCIBLY_FUNC m_MoveForciblyFunc[];	// 強制行動関数

	typedef void(CPlayerAIControl::* THROWTYPE_FUNC)();
	static THROWTYPE_FUNC m_ThrowTypeFunc[];		// 投げるタイプ関数

	typedef void(CPlayerAIControl::* MOVETYPE_FUNC)(const float, const float, const float);
	static MOVETYPE_FUNC m_MoveTypeFunc[];			// 行動関数

	typedef void(CPlayerAIControl::* MOVESTATE_FUNC)(const float, const float, const float);
	static MOVESTATE_FUNC m_MoveStateFunc[];				// キャッチ関数


	typedef void(CPlayerAIControl::* ACTION_FUNC)();
	static ACTION_FUNC m_ActionFunc[];				// アクション関数


	// フラグ関連の関数
	typedef void(CPlayerAIControl::* ACTIONFLAG_FUNC)();
	static ACTIONFLAG_FUNC m_ActionFlagFunc[];				// アクションフラグ関数

	typedef void(CPlayerAIControl::* THROWFLAG_FUNC)();
	static THROWFLAG_FUNC m_ThrowFlagFunc[];				// 投げフラグ関数

	typedef void(CPlayerAIControl::* MOVEFLAG_FUNC)();
	static MOVEFLAG_FUNC m_MoveFlagFunc[];				// 行動フラグ関数

	//-----------------------------
	// 状態関数
	//-----------------------------
	// モード
	void ModeIdle(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 待機
	void ModeAttack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 攻め
	void ModeDefense(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 守り

	// 強制行動
	void ForciblyNone() {};				// なし
	void ForciblyStop();				// 止まる
	virtual void ForciblyReturn() = 0;	// 戻る
	void ForciblyStart();				// 初め

	// 投げタイプ
	void ThrowTypeNone() {};		// なし
	void ThrowTypeNormal();			// 通常
	void ThrowTypeJump();			// ジャンプ
	void ThrowTypeSpecial();		// スペシャル

	// アクション
	void MoveDodge();				// 回避
	void MoveSupport();				// サポート
	void MoveChaseBall();			// ボールを追いかける
	virtual void MoveRetreat() = 0;	// 後退
	virtual void MoveRandom() = 0;	// ランダム
	void MoveLeave();				// 離れる

	// 行動フラグ
	void MoveFlagStop();			// なし
	void MoveFlagWalk();			// 歩く
	void MoveFlagBlink();			// ブリンク
	void MoveFlagDash();			// 走る

	// アクションフラグ
	void ActionFlagNone();			// なし
	void ActionFlagJump();			// ジャンプ

	// 投げフラグ
	void ThrowFlagNone() {};		// なし
	void ThrowFlag();				// 投げ
	void ThrowFlagPass();			// パス
	void ThrowFlagSpecial();		// スペシャル

	//=============================
	// メンバ関数
	//=============================
	void ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// モード管理
	void UpdateMode(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// モード更新
	void UpdateForcibly();																			// 強制行動
	void UpdateThrowType();																			// 投げ種類

	void UpdateDefense(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 守り
	void Action();
	void UpdateActionTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void MoveCover(CPlayer* pPlayer);

	// フラグ更新
	void UpdateMoveFlag();			// 行動
	void UpdateActionFlag();		// アクション
	void UpdateThrowFlag();			// 投げフラグ

	void BallSteal();		// ボールを奪う
	void BallChase();		// ぼるを追う

	void UpdateSee();						// 更新
	void SeeTarget(MyLib::Vector3 pos);		// ターゲットをみる
	void SeeBall();							// ボールを見る

	void InitHeart();		// 心の初期化
	void UpdateParameter();	// パラメータ

	//-----------------------------
	// 設定,取得
	//-----------------------------
	void SetMoveTimer(int nMin, int nMax);
	void SetActionTimer(int nMin, int nMax);
	void UpdateMoveTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	float GetDistanceBall();				// ボールとの距離
	float GetDistanceEnemy();				// 敵との距離
	float GetDistanceBallowner();			// ボール持ち主との距離

	//-----------------------------
	// 判定
	//-----------------------------
	bool IsDistanceBall();					// 距離：ボール
	bool IsPassTarget();					// パスする相手がいるか判定

public:
	bool IsPicksUpBall();				// ボールを拾う判断
	private:

	//=============================
	// メンバ変数
	//=============================
	CPlayer* m_pAI;					// 自分情報

	//-----------------------------
	// 列挙
	//-----------------------------
	EMode m_eMode;					// モード
	EMoveForcibly m_eForcibly;		// 強制行動
	EThrowType m_eThrowType;		// 投げ種類
	EAction m_eAction;				// アクション
	EActionStatus m_eActionStatus;	// アクション状態

	EMoveFlag m_eMoveFlag;			// 行動フラグ
	EActionFlag m_eActionFlag;		// アクションフラグ
	EThrowFlag m_eThrowFlag;		// 投げフラグ

	//-----------------------------
	// 構造体
	//-----------------------------
	SMove m_sMove;					// 行動
	SAction m_sAction;				// アクション
	SParameter m_sParameter;		// パラメータ

	SAI m_sAI;
};

#endif
