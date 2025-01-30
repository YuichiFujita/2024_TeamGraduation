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
class CPlayerAIOutControl
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum EMode	// モード
	{
		MODE_NONE = 0,	// なし
		MODE_THROW,		// 投げ
		MODE_MOVE,		// 行動
		MODE_MAX
	};

private:

	//=============================
	// 列挙型定義
	//=============================
	enum EThrow
	{// 投げ
		THROW_NONE = 0,
		THROW_NORMAL,
		THROW_PASS,
		THROW_MAX
	};

	enum EMove
	{// 行動
		MOVE_NONE = 0,
		MOVE_FIND,
		MOVE_MEETING,
		MOVE_MAX
	};

public:

	// コンストラクタ
	CPlayerAIOutControl();

	static CPlayerAIOutControl* Create(CPlayer* player);

	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetMode(EMode mode) { m_eMode = mode; }
	void SetPlayerInfo(CPlayer* player) { m_pAIOut = player; }

private:
	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayerAIOutControl::* MODE_FUNC)(const float, const float, const float);
	static MODE_FUNC m_ModeFunc[];			// モード関数

	typedef void(CPlayerAIOutControl::* THROW_FUNC)(const float, const float, const float);
	static THROW_FUNC m_ThrowFunc[];			// モード関数

	typedef void(CPlayerAIOutControl::* MOVE_FUNC)(const float, const float, const float);
	static MOVE_FUNC m_MoveFunc[];			// 行動関数

	//-----------------------------
	// 状態関数
	//-----------------------------
	void ModeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// なし
	void ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 投げ統括
	void ModeMoveManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// キャッチ統括

	// 投げ
	void ThrowNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// なし
	void ThrowNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 通常
	void ThrowPass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// パス

	// 行動
	void MoveNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ない
	void MoveRetrieve(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 回収
	void MoveMeeting(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 対面


	//=============================
	// メンバ関数
	//=============================
	void ModeManager();				// モード管理
	CPlayer* GetThrowTarget();		// 投げるターゲット
	void Pass();					// パス
	bool IsPass();					// パス判定
	void Throw();					// 投げる
	void LookBall();				// ボールを見る
	void AreaCheck();				// ボールのエリア

	//-----------------------------
	// その他関数
	//-----------------------------

	//=============================
	// メンバ変数
	//=============================
	CPlayer* m_pAIOut;			// 自分自身
	CPlayer* m_pTarget;
	EMode m_eMode;
	EThrow m_eThrow;
	EMove m_eMove;

	bool m_bStart;
	bool m_bEnd;
};

#endif
