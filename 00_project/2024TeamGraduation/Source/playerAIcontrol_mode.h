//=============================================================================
// 
//  プレイヤーAIコントロール_移動ヘッダー [playerAIcontrol_move.h]
//  Author :Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAI_CONTROL_MODE_H_
#define _PLAYERAI_CONTROL_MODE_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playercontrol_move.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CSuffocation;
class CEffekseerObj;
class CPlayerUserControlMove;

//==========================================================================
// プレイヤーAIコントロール_移動クラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerAIControlMode
{
public:

	enum EMode					// モード
	{
		MODE_START = 0,			// 初め
		MODE_IDLE,				// 待機
		MODE_ATTACK,			// 攻め
		MODE_DEFENCE,			// 守り
		MODE_MAX
	};

	// フラグ関連
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

private:
	struct SParameter
	{
		float fDistanse;		// 距離
		float fRadius;			// 半径

		float fJumpLength;
		float fLineLength;
		float fJumpEnd;			// ジャンプ終了位置
	};

public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerAIControlMode();
	~CPlayerAIControlMode();

	static CPlayerAIControlMode* Create(CPlayer* pPlayer, EMode mode);

	virtual HRESULT Init();
	void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//=============================
	// 設定/取得関数
	//=============================
	void SetPlayer(CPlayer* player) { m_pAI = player; }						// 自分のポインタ設定
	CPlayer* GetPlayer() { return m_pAI; }									// 取得

	void SetMode(EMode mode) { m_eMode = mode; }							// モードの設定
	EMode GetMode() { return m_eMode; }										// 取得
	void SetMoveFlag(EMoveFlag move) { m_eMoveFlag = move; }				// 行動設定
	EMoveFlag GetMoveFlag() { return m_eMoveFlag; }							// 取得
	void SetActionFlag(EActionFlag action) { m_eActionFlag = action; }		// アクションフラグ設定
	EActionFlag GetActionFlag() { return m_eActionFlag; }					// 取得

	void SetParameter(SParameter parameter) { m_sParameter = parameter; }	// パラメータ設定
	SParameter GetParameter() { return m_sParameter; }						// 取得

	void SetMaxjumpRate(float rate);										// ジャンプ力割合の設定
	float GetMaxjumpRate() { return m_fMaxJumpRate; }						// 取得
	float GetJumpRate();													// ジャンプ力割合の取得

private:
	//=============================
	// 関数リスト
	//=============================
	// フラグ関連の関数
	typedef void(CPlayerAIControlMode::* ACTIONFLAG_FUNC)();
	static ACTIONFLAG_FUNC m_ActionFlagFunc[];				// アクションフラグ関数

	typedef void(CPlayerAIControlMode::* MOVEFLAG_FUNC)();
	static MOVEFLAG_FUNC m_MoveFlagFunc[];					// 行動フラグ関数

	//-----------------------------
	// 状態関数
	//-----------------------------
	// 行動フラグ
	void MoveFlagStop();			// なし
	void MoveFlagWalk();			// 歩く
	void MoveFlagBlink();			// ブリンク
	void MoveFlagDash();			// 走る

	// アクションフラグ
	void ActionFlagNone();			// なし
	void ActionFlagJump();			// ジャンプ

	//=============================
	// メンバ関数
	//=============================
	void UpdateMoveFlag();			// 行動フラグ更新
	void UpdateActionFlag();		// アクションフラグ更新

	//=============================
	// メンバ変数
	//=============================
	CPlayer* m_pAI;					// 自分のポインタ

	float m_fMaxJumpRate;

	// 列挙
	EMode m_eMode;					// モードの列挙
	EMoveFlag m_eMoveFlag;			// 行動の列挙
	EActionFlag m_eActionFlag;		// アクションの列挙

	// 構造体
	SParameter m_sParameter;
};

#endif