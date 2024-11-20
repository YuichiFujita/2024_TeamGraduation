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
		MODE_CATCH,		// キャッチ
		MODE_MAX
	};

private:

	//=============================
	// 列挙型定義
	//=============================
	enum ECatch
	{
		CATCH_NONE = 0,
		CATCH_FIND,
		CATCH_MAX
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


	typedef void(CPlayerAIOutControl::* CATCH_FUNC)(const float, const float, const float);
	static CATCH_FUNC m_CatchFunc[];			// キャッチ関数


	//-----------------------------
	// 状態関数
	//-----------------------------
	void ModeNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};		// なし
	void ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 投げ統括
	void ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// キャッチ統括

	// キャッチ
	void CatchNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {};
	void RetrieveBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);


	//=============================
	// メンバ関数
	//=============================
	void ModeManager();
	CPlayer* GetThrowTarget();		// 投げるターゲット
	void Pass();
	bool IsPass();
	void Throw();

	void LookBall();

	void AreaCheck();

	//-----------------------------
	// その他関数
	//-----------------------------

	//=============================
	// メンバ変数
	//=============================
	CPlayer* m_pAIOut;			// 自分自身

	EMode m_eMode;
	ECatch m_eCatch;

	bool m_bStart;
	bool m_bEnd;
};

#endif
