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
	&CPlayerAction::ActionNone,	// なし
	&CPlayerAction::ActionBlink,	// ブリンク
	&CPlayerAction::ActionRun,	// 走り
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
// アクション設定
//==========================================================================
void CPlayerAction::SetAction(CPlayer::Action action)
{
	m_Action = action;
	m_fActionTime = 0.0f;		// アクション時間
}
