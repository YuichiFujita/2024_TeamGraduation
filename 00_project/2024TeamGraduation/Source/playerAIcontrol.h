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

private:
	//=============================
	// メンバ関数
	//=============================
	void ModeManager();
	void ChangeMode(CPlayer* pPlayer, CPlayerAIControlMode::EMode mode);

	//=============================
	// メンバ変数
	//=============================
	CPlayer* m_pAI;							// 自分情報
	CPlayerAIControlMode* m_pAIControlMode;	// モード
};

#endif
