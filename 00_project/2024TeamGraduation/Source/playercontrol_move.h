//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playercontrol_move.h]
//  Author : Kai Takada
// 
//=============================================================================

#ifndef _PLAYER_CONTROL_MOVE_H_
#define _PLAYER_CONTROL_MOVE_H_	// 二重インクルード防止

#include "playercontrol.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerAIControlMove;
class CPlayerAIOutControlMove;
class CPlayerUserControlMove;
class CPlayerUserOutControlMove;

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerControlMove : public CPlayerControl
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerControlMove();
	virtual ~CPlayerControlMove() override;

	//=============================
	// 純粋仮想関数
	//=============================
	virtual CPlayerAIControlMove* GetAI()			{ return nullptr; }
	virtual CPlayerAIOutControlMove* GetAIOut()		{ return nullptr; }
	virtual CPlayerUserControlMove* GetUser()		{ return nullptr; }
	virtual CPlayerUserOutControlMove* GetUserOut()	{ return nullptr; }
	virtual void Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 移動

	void SetCntTrigger(int nIdx, int nCnt) { m_nCntTrigger[nIdx] = nCnt; }						// トリガーのカウント設定
	void SetCntTrigger(int* nTrigger);															// トリガーのカウント設定
	int GetCntTrigger(int nIdx) { return m_nCntTrigger[nIdx]; }									// トリガーのカウント取得
	int* GetCntTrigger() { return &m_nCntTrigger[0]; }											// トリガーのカウント取得
	void SetHoldDashAngle(CPlayer::EDashAngle eAngle) { m_HoldDashAngle = eAngle; }				// ダッシュ方向設定
	CPlayer::EDashAngle GetHoldDashAngle() { return m_HoldDashAngle; }							// ダッシュ方向取得
	void SetInputInterval(float fInputInterval) { m_fInputInterval = fInputInterval; }			// 入力の受け付け猶予設定
	float GetInputInterval() { return m_fInputInterval; }										// 入力の受け付け猶予取得
	void SetTriggerInterval(float fTriggerInterval) { m_fTriggerInterval = fTriggerInterval; }	// トリガーのインターバル設定
	float GetTriggerInterval() { return m_fTriggerInterval; }									// トリガーのインターバル取得
	void SetBlink(bool bDash) { m_bBlink = bDash; }												// ブリンクフラグ設定
	bool IsBlink() { return m_bBlink; }															// ブリンクフラグ取得
	void SetEnableInputAngle(CPlayer::EDashAngle inputAngle) { m_InputAngle = inputAngle; }		// 現在の入力方向設定
	CPlayer::EDashAngle GetInputAngle() { return m_InputAngle; }								// 現在の入力方向取得
	void SetInputAngleCtr(float fInputAngleCtr) { m_fInputAngleCtr = fInputAngleCtr; }			// 現在の入力方向カウンター設定
	float GetInputAngleCtr() { return m_fInputAngleCtr; }										// 現在の入力方向カウンター取得
	void SetCrabMoveEasingTime(float fCrabMoveEasingTime) { m_fCrabMoveEasingTime = fCrabMoveEasingTime; }		// 現在のカニ歩き移動補正値設定
	float GetCrabMoveEasingTime() { return m_fCrabMoveEasingTime; }												// 現在のカニ歩き移動補正値取得

	void UpdateInputAngle(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 現在の入力方向更新
	virtual void CrabSetting(CPlayer* player);				// カニ歩き状態

private:
	
	//=============================
	// メンバ関数
	//=============================
	virtual void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// ブリンク
	virtual void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// 走る
	virtual void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// ウォーク
	virtual CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::EDashAngle eAngle) = 0;	// ダッシュ時のトリガー

	//=============================
	// メンバ変数
	//=============================
	int m_nCntTrigger[CPlayer::EDashAngle::ANGLE_MAX];	// トリガーのカウント
	CPlayer::EDashAngle m_HoldDashAngle;				// 保持してるダッシュの移動方向
	float m_fInputInterval;								// 入力の受け付け猶予
	float m_fTriggerInterval;							// トリガーのインターバル
	bool m_bBlink;										// 走るフラグ
	CPlayer::EDashAngle m_InputAngle;					// 現在の入力方向
	float m_fInputAngleCtr;								// 現在の入力方向の保持カウンター
	
	float m_fCrabMoveEasingTime;		// 現在のカニ歩き移動補正値
};

#endif