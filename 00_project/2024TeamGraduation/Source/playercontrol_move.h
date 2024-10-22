//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playercontrol_move.h]
//  Author : Kai Takada
// 
//=============================================================================

#ifndef _PLAYER_CONTROL_MOVE_H_
#define _PLAYER_CONTROL_MOVE_H_	// 二重インクルード防止

#define BRESSRANGE (0)

#include "playercontrol.h"

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerControlMove : public CPlayerControl
{
public:

	// コンストラクタ
	CPlayerControlMove();

	virtual void Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 移動

	void SetCntTrigger(int nIdx, int nCnt) { m_nCntTrigger[nIdx] = nCnt; }						// トリガーのカウント設定
	void SetCntTrigger(int* nTrigger);															// トリガーのカウント設定
	int GetCntTrigger(int nIdx) { return m_nCntTrigger[nIdx]; }									// トリガーのカウント取得
	int* GetCntTrigger() { return &m_nCntTrigger[0]; }											// トリガーのカウント取得
	void SetHoldDashAngle(CPlayer::DashAngle angle) { m_HoldDashAngle = angle; }				// ダッシュ方向設定
	CPlayer::DashAngle GetHoldDashAngle() { return m_HoldDashAngle; }							// ダッシュ方向取得
	void SetInputInterval(float fInputInterval) { m_fInputInterval = fInputInterval; }			// 入力の受け付け猶予設定
	float GetInputInterval() { return m_fInputInterval; }										// 入力の受け付け猶予取得
	void SetTriggerInterval(float fTriggerInterval) { m_fTriggerInterval = fTriggerInterval; }	// トリガーのインターバル設定
	float GetTriggerInterval() { return m_fTriggerInterval; }									// トリガーのインターバル取得
	void SetBlink(bool bDash) { m_bBlink = bDash; }												// ブリンクフラグ設定
	bool IsBlink() { return m_bBlink; }															// ブリンクフラグ取得

private:
	
	//=============================
	// メンバ関数
	//=============================
	virtual void Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ブリンク
	virtual void Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 走る
	virtual void Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ウォーク
	virtual CPlayer::SDashInfo Trigger(CPlayer* player, CPlayer::DashAngle angle);	// ダッシュ時のトリガー

	//=============================
	// メンバ変数
	//=============================
	int m_nCntTrigger[CPlayer::DashAngle::ANGLE_MAX];	// トリガーのカウント
	CPlayer::DashAngle m_HoldDashAngle;					// 保持してるダッシュの移動方向
	float m_fInputInterval;								// 入力の受け付け猶予
	float m_fTriggerInterval;							// トリガーのインターバル
	bool m_bBlink;										// 走るフラグ
};

#endif