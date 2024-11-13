//==========================================================================
// 
//  プレイヤーベースヘッダー [playerBase.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _PLAYER_BASE_H_
#define _PLAYER_BASE_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "player.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerControlMove;
class CPlayerControlAction;
class CBall;

//==========================================================================
// プレイヤーベース定義
//==========================================================================
class CPlayerBase
{
public:
	// コンストラクタ
	CPlayerBase(CPlayer* pPlayer, const CPlayer::EFieldArea typeArea);

	// デストラクタ
	virtual ~CPlayerBase() = 0;

	// 純粋仮想関数
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// 更新

	// 仮想関数
	virtual CPlayer::SHitInfo Hit(CBall* pBall);	// ヒット
	virtual void Debug();	// デバッグ

	// メンバ関数
	void DeleteControl();	// 操作破棄
	void SetPlayerControlMove(CPlayerControlMove* pControlMove);		// 操作設定 (移動)
	void SetPlayerControlAction(CPlayerControlAction* pControlAction);	// 操作設定 (アクション)
	CPlayerControlMove* GetPlayerControlMove()		{ return m_pControlMove; }		// 操作取得 (移動)
	CPlayerControlAction* GetPlayerControlAction()	{ return m_pControlAction; }	// 操作取得 (アクション)

protected:
	// メンバ関数
	CPlayer* GetPlayer() { return m_pPlayer; }	// プレイヤー情報取得

private:
	// メンバ変数
	CPlayer* m_pPlayer;	// プレイヤー情報
	CPlayerControlMove* m_pControlMove;		// 移動操作
	CPlayerControlAction* m_pControlAction;	// アクション操作
};

#endif
