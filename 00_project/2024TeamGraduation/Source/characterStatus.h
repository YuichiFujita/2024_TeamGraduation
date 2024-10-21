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

	//=============================
	// 構造体定義
	//=============================
	/**
	@brief	ステータス
	*/
	struct CharParameter
	{
		float fVelocityNormal;	// 通常移動速度
		float fVelocityDash;	// ダッシュ移動速度
		float fVelocityBlink;	// ブリンク移動速度
		float fVelocityJump;	// ジャンプ量
		float fRadius;			// 半径
		float fHeight;			// 身長
		int nLife;				// 体力

		// コンストラクタ
		CharParameter() :
			fVelocityNormal(0.0f),	// 通常移動速度
			fVelocityDash(0.0f),	// ダッシュ移動速度
			fVelocityBlink(0.0f),	// ブリンク移動速度
			fVelocityJump(0.0f),	// ジャンプ量
			fRadius(0.0f),			// 半径
			fHeight(0.0f),			// 身長
			nLife(0)				// 体力
		{}

		// パラメーター付きコンストラクタ
		CharParameter(float _fVelocityNormal, float _fVelocityDash, float _fVelocityBlink,
			float _fVelocityJump, float _fRadius, float _fHeight, int _nLife) :
			fVelocityNormal(_fVelocityNormal),	// 通常移動速度
			fVelocityDash(_fVelocityDash),		// ダッシュ移動速度
			fVelocityBlink(_fVelocityBlink),	// ブリンク移動速度
			fVelocityJump(_fVelocityJump),		// ジャンプ量
			fRadius(_fRadius),					// 半径
			fHeight(_fHeight),					// 身長
			nLife(_nLife)						// 体力
		{}
	};

	// コンストラクタ
	CCharacterStatus();

	// コンストラクタ
	CCharacterStatus(CharParameter status) :
		m_Parameter(status),					// パラメーター
		m_typeTeam(CGameManager::SIDE_NONE) {}	// チームサイド

	// デストラクタ
	~CCharacterStatus();

	//=============================
	// メンバ関数
	//=============================
	void SetTeam(const CGameManager::TeamSide team) { m_typeTeam = team; }	// チームサイド設定
	CGameManager::TeamSide GetTeam()	{ return m_typeTeam; }				// チームサイド取得
	CharParameter GetParameter() { return m_Parameter; }					// パラメーター取得

#if _DEBUG
	void SetParameter(const CharParameter& parameter) { m_Parameter = parameter; }	// パラメーター設定
#endif

private:
	
	//=============================
	// メンバ変数
	//=============================
	CharParameter m_Parameter;			// パラメーター
	CGameManager::TeamSide m_typeTeam;	// チームサイド
};

#endif