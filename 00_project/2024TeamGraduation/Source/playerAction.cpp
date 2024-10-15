//=============================================================================
// 
//  プレイヤーコントロール処理 [playerAction.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerAction.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"

namespace
{
	const float TIME_BLINK = 0.2f;		// ブリンク時間
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerAction::ACTION_FUNC CPlayerAction::m_ActionFunc[] =	// 行動関数
{
	&CPlayerAction::ActionNone,			// なし
	&CPlayerAction::ActionBlink,		// ブリンク
	&CPlayerAction::ActionRun,			// 走り
	&CPlayerAction::ActionJump,			// ジャンプ
	&CPlayerAction::ActionCatch,		// キャッチ
	&CPlayerAction::ActionThrow,		// 投げ
	&CPlayerAction::ActionThrowJump,	// 投げ(ジャンプ)
	&CPlayerAction::ActionSpecial,		// スペシャル
};

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAction::CPlayerAction(CPlayer* player)
{
	m_Action = CPlayer::Action::ACTION_NONE;	// アクション
	m_fActionTime = 0.0f;						// アクション時間
	m_pPlayer = player;							// プレイヤーのポインタ
}

//==========================================================================
// 更新
//==========================================================================
void CPlayerAction::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// アクション時間
	m_fActionTime += fDeltaTime * fDeltaRate * fSlowRate;

	// 行動更新
	(this->*(m_ActionFunc[m_Action]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// なし
//==========================================================================
void CPlayerAction::ActionNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ブリンク
//==========================================================================
void CPlayerAction::ActionBlink(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fActionTime >= TIME_BLINK)
	{// ブリンク経過
		SetAction(CPlayer::Action::ACTION_NONE);
	}
}

//==========================================================================
// 走り
//==========================================================================
void CPlayerAction::ActionRun(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ジャンプ
//==========================================================================
void CPlayerAction::ActionJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
}

//==========================================================================
// キャッチ
//==========================================================================
void CPlayerAction::ActionCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
}

//==========================================================================
// 投げ
//==========================================================================
void CPlayerAction::ActionThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
}

//==========================================================================
// ジャンプ投げ
//==========================================================================
void CPlayerAction::ActionThrowJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
}

//==========================================================================
// スペシャル
//==========================================================================
void CPlayerAction::ActionSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
}

//==========================================================================
// アクション設定
//==========================================================================
void CPlayerAction::SetAction(CPlayer::Action action)
{
	m_Action = action;
	m_fActionTime = 0.0f;		// アクション時間
}
