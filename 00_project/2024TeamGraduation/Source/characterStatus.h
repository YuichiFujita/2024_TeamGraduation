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
		float fVelocityNormal;		// 通常移動速度
		float fVelocityDash;		// ダッシュ移動速度
		float fVelocityBlink;		// ブリンク移動速度
		float fVelocityJump;		// ジャンプ量
		float fJumpStartMove;		// ジャンプ投げ時の開始上移動量
		float fJumpUpdateMove;		// ジャンプ投げ時の常時上移動量
		int nBallPartsIdx;			// ボールを持つパーツインデックス
		MyLib::Vector3 ballOffset;	// ボールを持つ位置オフセット
		float fRadius;				// 半径
		float fHeight;				// 身長
		int nLife;					// 体力

		// コンストラクタ
		CharParameter() :
			fVelocityNormal(0.0f),	// 通常移動速度
			fVelocityDash(0.0f),	// ダッシュ移動速度
			fVelocityBlink(0.0f),	// ブリンク移動速度
			fVelocityJump(0.0f),	// ジャンプ量
			fJumpStartMove(0.0f),	// ジャンプ投げ時の開始上移動量
			fJumpUpdateMove(0.0f),	// ジャンプ投げ時の常時上移動量
			nBallPartsIdx(0),		// ボールを持つパーツインデックス
			ballOffset(VEC3_ZERO),	// ボールを持つ位置オフセット
			fRadius(0.0f),			// 半径
			fHeight(0.0f),			// 身長
			nLife(0)				// 体力
		{}

		// パラメーター付きコンストラクタ
		CharParameter
		(
			float _fVelocityNormal,
			float _fVelocityDash,
			float _fVelocityBlink,
			float _fVelocityJump,
			float _fJumpStartMove,
			float _fJumpUpdateMove,
			int _nBallPartsIdx,
			MyLib::Vector3 _ballOffset,
			float _fRadius,
			float _fHeight,
			int _nLife
		) :
			fVelocityNormal(_fVelocityNormal),	// 通常移動速度
			fVelocityDash(_fVelocityDash),		// ダッシュ移動速度
			fVelocityBlink(_fVelocityBlink),	// ブリンク移動速度
			fVelocityJump(_fVelocityJump),		// ジャンプ量
			fJumpStartMove(_fJumpStartMove),	// ジャンプ投げ時の開始上移動量
			fJumpUpdateMove(_fJumpUpdateMove),	// ジャンプ投げ時の常時上移動量
			nBallPartsIdx(_nBallPartsIdx),		// ボールを持つパーツインデックス
			ballOffset(_ballOffset),			// ボールを持つ位置オフセット
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