//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_H_
#define _PLAYERAICONTROL_H_	// 二重インクルード防止

#include "player.h"
#include "playerAIcontrol_mode.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerAIControlMove;		// 移動(AI)
class CPlayerAIControlAction;	// アクション(AI)
class CPlayerAIControlMode;		// モード(AI)
class CPlayer;

//==========================================================================
// 定数定義
//==========================================================================

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
class CPlayerAIControl
{
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

	void SetMode(CPlayerAIControlMode::EMode mode) { m_pAIControlMode->SetMode(mode); }	// モード設定
	CPlayerAIControlMode::EMode GetMode() { return  m_pAIControlMode->GetMode(); }		// 取得

private:
	//=============================
	// メンバ関数
	//=============================
	void ModeStart();
	void ModeManager();
	void ChangeMode(CPlayer* pPlayer, CPlayerAIControlMode::EMode mode);

	//=============================
	// メンバ変数
	//=============================
	CPlayer* m_pAI;							// 自分情報
	CPlayerAIControlMode* m_pAIControlMode;	// モード

	bool m_bStart;
};

#endif
