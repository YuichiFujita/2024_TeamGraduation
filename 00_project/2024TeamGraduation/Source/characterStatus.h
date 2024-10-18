//=============================================================================
// 
//  キャラクターステータスヘッダー [characterStatus.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _CHARACTERSTATUS_H_
#define _CHARACTERSTATUS_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "gamemanager.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// キャラクターステータスクラス
//==========================================================================
class CCharacterStatus
{
public:

	// コンストラクタ
	CCharacterStatus();

	// コンストラクタ
	CCharacterStatus(float _fVelocityNormal, float _fVelocityDash, float _fVelocityBlink,
		float _fVelocityJump, float _fRadius, float _fHeight, int _nLife) :
		m_fVelocityNormal(_fVelocityNormal),	// 通常移動速度
		m_fVelocityDash(_fVelocityDash),		// ダッシュ移動速度
		m_fVelocityBlink(_fVelocityBlink),		// ブリンク移動速度
		m_fVelocityJump(_fVelocityJump),		// ジャンプ量
		m_fRadius(_fRadius),					// 半径
		m_fHeight(_fHeight),					// 身長
		m_nLife(_nLife),						// 体力
		m_typeTeam(CGameManager::SIDE_NONE) {}	// チームサイド

	// デストラクタ
	~CCharacterStatus();

	//=============================
	// メンバ関数
	//=============================
	void SetTeam(const CGameManager::TeamSide team) { m_typeTeam = team; }	// チームサイド設定
	CGameManager::TeamSide GetTeam()	{ return m_typeTeam; }				// チームサイド取得
	float GetVelocityNormal()			{ return m_fVelocityNormal; }		// 通常移動速度
	float GetVelocityDash()				{ return m_fVelocityDash; }			// ダッシュ移動速度
	float GetVelocityBlink()			{ return m_fVelocityBlink; }		// ブリンク移動速度
	float GetVelocityJump()				{ return m_fVelocityJump; }			// ジャンプ量
	float GetRadius()					{ return m_fRadius; }				// 半径
	float GetHeight()					{ return m_fHeight; }				// 身長
	int GetLife()						{ return m_nLife; }					// 体力

private:
	
	//=============================
	// メンバ変数
	//=============================
	float m_fVelocityNormal;	// 通常移動速度
	float m_fVelocityDash;		// ダッシュ移動速度
	float m_fVelocityBlink;		// ブリンク移動速度
	float m_fVelocityJump;		// ジャンプ量
	float m_fRadius;			// 半径
	float m_fHeight;			// 身長
	int m_nLife;				// 体力
	CGameManager::TeamSide m_typeTeam;	// チームサイド
};

#endif