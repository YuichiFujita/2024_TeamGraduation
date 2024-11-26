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
class CPlayerIn;
class CPlayerOut;
class CPlayerUserIn;
class CPlayerUserOut;
class CPlayerAIIn;
class CPlayerAIOut;
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
	CPlayerBase(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase);

	// デストラクタ
	virtual ~CPlayerBase() = 0;

	// 純粋仮想関数
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// 更新
	virtual void MotionCrab(int nStartKey) = 0;	// カニ歩き変化

	// 仮想関数
	virtual CPlayer::SHitInfo Hit(CBall* pBall);			// ヒット
	virtual void InitPosition(const MyLib::Vector3& rPos);	// 位置初期化
	virtual bool IsCrab();	// カニ歩き判定
	virtual void Debug();	// デバッグ
	virtual CPlayerIn* GetPlayerIn()			{ return nullptr; }	// 内野プレイヤー取得
	virtual CPlayerOut* GetPlayerOut()			{ return nullptr; }	// 外野プレイヤー取得
	virtual CPlayerUserIn* GetPlayerUserIn()	{ return nullptr; }	// ユーザー内野プレイヤー取得
	virtual CPlayerUserOut* GetPlayerUserOut()	{ return nullptr; }	// ユーザー外野プレイヤー取得
	virtual CPlayerAIIn* GetPlayerAIIn()		{ return nullptr; }	// AI内野プレイヤー取得
	virtual CPlayerAIOut* GetPlayerAIOut()		{ return nullptr; }	// AI外野プレイヤー取得

	// メンバ関数
	void UpdateChangeBase();	// ベース変更更新
	void DeleteControl();		// 操作破棄
	void ChangeMoveControl(CPlayerControlMove* control);				// 移動の操作変更
	void ChangeActionControl(CPlayerControlAction* control);			// アクションの操作変更
	void SetPlayerControlMove(CPlayerControlMove* pControlMove);		// 操作設定 (移動)
	void SetPlayerControlAction(CPlayerControlAction* pControlAction);	// 操作設定 (アクション)
	void SetNewBase(const CPlayer::EBaseType typeBase)	{ m_newTypeBase = typeBase; }	// 新ベース設定
	CPlayerControlMove* GetPlayerControlMove()			{ return m_pControlMove; }		// 操作取得 (移動)
	CPlayerControlAction* GetPlayerControlAction()		{ return m_pControlAction; }	// 操作取得 (アクション)

protected:
	// メンバ関数
	CPlayer* GetPlayer() { return m_pPlayer; }	// プレイヤー情報取得

private:
	// メンバ変数
	const CPlayer::EBaseType m_curTypeBase;	// 現在のベースタイプ
	CPlayer::EBaseType m_newTypeBase;		// 新しいベースタイプ
	CPlayer* m_pPlayer;						// プレイヤー情報
	CPlayerControlMove* m_pControlMove;		// 移動操作
	CPlayerControlAction* m_pControlAction;	// アクション操作
};

#endif
