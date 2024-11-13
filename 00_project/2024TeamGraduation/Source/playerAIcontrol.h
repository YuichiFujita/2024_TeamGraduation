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
	enum EHeart
	{
		HEART_NONE = 0,	// 抜け殻人
		HEART_NORMAL,	// 通常
		HEART_STRONG,	// 強気
		HEART_TIMID,	// 弱気
		HEART_MAX
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
		MOVETYPE_DISTANCE,
		MOVETYPE_MAX
	};

public:

	// コンストラクタ
	CPlayerAIControl();

	static CPlayerAIControl* Create(CPlayer* player);

	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetMode(EMode mode) { m_eMode = mode; }	// モード設定
	void SetPlayerInfo(CPlayer* player) { m_pAI = player; }

private:
	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayerAIControl::* MODE_FUNC)(const float, const float, const float);
	static MODE_FUNC m_ModeFunc[];			// モード関数

	typedef void(CPlayerAIControl::* CATCH_FUNC)(const float, const float, const float);
	static CATCH_FUNC m_CatchFunc[];		// 投げタイミング関数

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

	// キャッチ
	void CatchNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void FindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//-----------------------------
	// その他関数
	//-----------------------------
	void Reset();			// 変数リセット
	void SetThrowTarget(CPlayer** ppTarget = nullptr);		// 投げるターゲット
	void DistanceThrow();	// 投げる距離
	void DistanceCatch();	// キャッチ距離

	void PlanThrowFlow();	// 投げる流れ
	void PlanHeart();		// 心のプラン
	void PlanThrow(CPlayer* pTarget);		// 投げるプラン
	void PlanThrowDistance(CPlayer* pTarget);	// 投げる距離プラン
	void PlanIsJump(CPlayer* pTarget);	// 跳ぶかどうか
	void PlanMove(CPlayer* pTarget);	// 行動プラン

	void LineDistance();	// 線との距離
	void LineOverPlayer();	// 線超え
	bool IsLineOverBall();	// 線超え判定(ボール)


	//=============================
	// メンバ変数
	//=============================
	EMode m_eMode;					// モード
	EHeart m_eHeart;				// 心の種類
	ECatchType m_eCatchType;		// キャッチ種類
	EMoveType m_eMoveType;			// 行動種類
	float m_fTiming;		// タイミングカウント
	float m_fTimingRate;	// タイミングの割合
	bool m_bTiming;			// タイミングフラグ
	bool m_bFoldJump;		// ジャンプの折り返しフラグ
	float m_fJumpEnd;		// ジャンプの終了位置

	CPlayer* m_pAI;

	bool m_bGOThrow;		// 投げてよし！
};

#endif
