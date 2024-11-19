//=============================================================================
// 
//  ボールステータスヘッダー [ballStatus.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _BALLSTATUS_H_
#define _BALLSTATUS_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "gamemanager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CBall;

//==========================================================================
// ボールステータスクラス
//==========================================================================
class CBallStatus
{
public:

	//=============================
	// 構造体定義
	//=============================
	/**
	@brief	パラメーター
	*/
	struct SBallParameter
	{
		float fThrowMoveNormal;		// 投げ速度(通常)
		float fThrowMoveJump;		// 投げ速度(ジャンプ)
		float fThrowMoveSpecial;	// 投げ速度(スペシャル)
		float fCatchRange;			// キャッチ範囲
		int nDamageNormal;			// ダメージ(通常)
		int nDamageJump;			// ダメージ(ジャンプ)
		int nDamageSpecial;			// ダメージ(スペシャル)
		float fKnockbackNormal;		// ノックバック(通常)
		float fKnockbackJump;		// ノックバック(ジャンプ)
		float fKnockbackSpecial;	// ノックバック(スペシャル)

		// コンストラクタ
		SBallParameter() :
			fThrowMoveNormal(0.0f),		// 投げ速度(通常)
			fThrowMoveJump(0.0f),		// 投げ速度(ジャンプ)
			fThrowMoveSpecial(0.0f),	// 投げ速度(スペシャル)
			fCatchRange(0.0f),			// キャッチ範囲
			nDamageNormal(0),			// ダメージ(通常)
			nDamageJump(0),				// ダメージ(ジャンプ)
			nDamageSpecial(0),			// ダメージ(スペシャル)
			fKnockbackNormal(0),		// ノックバック(通常)
			fKnockbackJump(0),			// ノックバック(ジャンプ)
			fKnockbackSpecial(0)		// ノックバック(スペシャル)
		{}

		// パラメーター付きコンストラクタ
		SBallParameter
		(
			float _fThrowMoveNormal,	// 投げ速度(通常)
			float _fThrowMoveJump,		// 投げ速度(ジャンプ)
			float _fThrowMoveSpecial,	// 投げ速度(スペシャル)
			float _fCatchRange,			// キャッチ範囲
			int _nDamageNormal,			// ダメージ(通常)
			int _nDamageJump,			// ダメージ(ジャンプ)
			int _nDamageSpecial,		// ダメージ(スペシャル)
			float _fKnockbackNormal,	// ノックバック(通常)
			float _fKnockbackJump,		// ノックバック(ジャンプ)
			float _fKnockbackSpecial	// ノックバック(スペシャル)
		) :
			fThrowMoveNormal(_fThrowMoveNormal),	// 投げ速度(通常)
			fThrowMoveJump(_fThrowMoveJump),		// 投げ速度(ジャンプ)
			fThrowMoveSpecial(_fThrowMoveSpecial),	// 投げ速度(スペシャル)
			fCatchRange(_fCatchRange),				// キャッチ範囲
			nDamageNormal(_nDamageNormal),			// ダメージ(通常)
			nDamageJump(_nDamageJump),				// ダメージ(ジャンプ)
			nDamageSpecial(_nDamageSpecial),		// ダメージ(スペシャル)
			fKnockbackNormal(_fKnockbackNormal),	// ノックバック(通常)
			fKnockbackJump(_fKnockbackJump),		// ノックバック(ジャンプ)
			fKnockbackSpecial(_fKnockbackSpecial)	// ノックバック(スペシャル)
		{}
	};

	// コンストラクタ
	CBallStatus(const SBallParameter& parameter);

	// デストラクタ
	~CBallStatus();

	//=============================
	// メンバ関数
	//=============================
	void BindBall(CBall* pBall)	{ m_pBall = pBall; }	// プレイヤー割当


#if _DEBUG
	void SetParameter(const SBallParameter& parameter) { m_Parameter = parameter; };	// パラメーター設定
#endif
	SBallParameter GetParameter() { return m_Parameter; }	// パラメーター取得

private:
	
	//=============================
	// メンバ変数
	//=============================
	CBall* m_pBall;				// ボールのポインタ

#if _DEBUG
	SBallParameter m_Parameter;	// パラメーター
#else
	const SBallParameter m_Parameter;	// パラメーター
#endif
};

#endif