//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_DEFENSE_H_
#define _PLAYERAICONTROL_DEFENSE_H_	// 二重インクルード防止

#include "player.h"
#include "playerAIcontrol_mode.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerAIControlMove;		// 移動(AI)
class CPlayerAIControlAction;	// アクション(AI)
class CPlayer;

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
class CPlayerAIControlDefense : public CPlayerAIControlMode
{
public: 
	enum EFlow				// 流れ
	{
		FLOW_START = 0,
		FLOW_GAME,
		FLOW_MAX
	};

	enum EActionStatus		// アクション状態
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

	struct SAction {
		MyLib::Vector3 pos;			// 位置
		float fTimer;				// タイマー
		bool bSet;					// 設定フラグ
		bool bCooldown;				// クールダウン
		bool bCancel;				// キャンセル
	};

public:

	// コンストラクタ
	CPlayerAIControlDefense();

	static CPlayerAIControlDefense* Create(CPlayer* player);


	HRESULT Init() override;
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

	// 設定・取得
	void SetActionStatus(EActionStatus status) { m_eActionStatus = status; }
	EActionStatus GetActionStatus() { return m_eActionStatus; }
	void SetAction(EAction action) { m_eAction = action; }
	EAction GetAction() { return m_eAction; }
	void SetActionInfo(SAction action) { m_sAction = action; }
	SAction GetActionInfo() { return m_sAction; }

protected:
	//=============================
	// 仮想・純粋関数
	//=============================
	virtual bool IsLineOverBall() { return false; }				// 線超え判定(ボール)
	virtual bool IsLineOverPlayer() { return false; };			// 線越え判定(プレイヤー)
	virtual void BallSteal() = 0;		// ボールを奪う

	//=============================
	// メンバ関数
	//=============================
	bool Leave(MyLib::Vector3 targetPos, float distance);		// 離れる
	bool Approatch(MyLib::Vector3 targetPos, float distance);	// 近づく
	void MoveUp();			// 上移動
	void MoveDown();		// 下移動
	void MoveLeft();		// 左移動
	void MoveRight();		// 右移動

	void UpdateSee();						// 更新
	void SeeTarget(MyLib::Vector3 pos);		// ターゲットをみる
	void SeeBall();							// ボールを見る

	//-----------------------------
	// 設定関数
	//-----------------------------
	CPlayer* GetBallOwner();				// ボール持ち主
	float GetDistance() { return m_fDistanse; }

	float GetDistanceBall();				// ボールとの距離
	float GetDistanceEnemy();				// 敵との距離
	float GetDistanceBallowner();			// ボール持ち主との距離

private:

	//-----------------------------
	// 判定
	//-----------------------------
	bool IsDistanceBall();					// 距離：ボール
	bool IsPassTarget();					// パスする相手がいるか判定
	bool IsPicksUpBall();					// ボールを拾う判断

	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayerAIControlDefense::* ACTION_FUNC)();
	static ACTION_FUNC m_ActionFunc[];				// アクション関数

	//-----------------------------
	// 状態関数
	//-----------------------------
	void MoveIdle();
	void MoveDodge();				// 回避
	void MoveSupport();				// サポート
	void MoveChaseBall();			// ボールを追いかける
	virtual void MoveRetreat() = 0;	// 後退
	virtual void MoveRandom() = 0;	// ランダム
	void MoveLeave();				// 離れる

	//=============================
	// メンバ関数
	//=============================
	void UpdateDefense(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 守り
	void Action0();			// アクション
	void SelectAction();			// アクション
	void TeammateBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// チームメイトボール
	void TeamEnemyBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 相手チームボール

	void BallChase();		// ボールを追う

	void UpdateActionTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void SetActionTimer(int nMin, int nMax);	// 行動タイマー

	bool IsCancel();

	//=============================
	// メンバ変数
	//=============================
	float m_fDistanse;

	//-----------------------------
	// 列挙型
	//-----------------------------
	EFlow m_eFlow;					// 流れ
	EAction m_eAction;				// アクション
	EActionStatus m_eActionStatus;	// アクション状態

	//-----------------------------
	// 構造体
	//-----------------------------
	SAction m_sAction;				// アクション
};

#endif
