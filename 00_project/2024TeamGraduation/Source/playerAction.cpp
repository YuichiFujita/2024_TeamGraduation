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
#include "ball.h"
#include "playerStatus.h"

namespace
{
	const float DODGE_SLOW = 0.8f;		//回避時スロー値
}

namespace ActionTime
{
	const float BLINK = 0.35f;		// ブリンク時間
	const float DODGE = 0.5f;		// 回避時間
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerAction::ACTION_FUNC CPlayerAction::m_ActionFunc[] =	// 行動関数
{
	&CPlayerAction::ActionNone,			// なし
	&CPlayerAction::ActionBlink,		// ブリンク
	&CPlayerAction::ActionDodge,		// 回避
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
	m_bCharm = false;
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
	if (m_fActionTime >= ActionTime::BLINK)
	{// ブリンク経過
		
		CPlayer::sDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
		DmgInfo.reciveTime = 0.0f;
		m_pPlayer->SetDamageInfo(DmgInfo);

		SetAction(CPlayer::Action::ACTION_NONE);
	}

	if (m_pPlayer->GetBall() != nullptr)
	{
		return;
	}

	//ダメージ受付しない時間設定
	CPlayer::sDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
	DmgInfo.reciveTime = 0.1f;
	m_pPlayer->SetDamageInfo(DmgInfo);

	//回避判定
	CListManager<CBall> sampleList = CBall::GetListObj();
	std::list<CBall*>::iterator itr = sampleList.GetEnd();
	CBall* pObj = nullptr;

	while (sampleList.ListLoop(itr))
	{
		pObj = (*itr);

		if (!pObj->IsAttack() ||
			pObj->GetTypeTeam() == m_pPlayer->GetStatus()->GetTeam())
		{
			return;
		}

		if (UtilFunc::Collision::CollisionCircleCylinder(
			pObj->GetPosition(), m_pPlayer->GetPosition(), pObj->GetRadius(), m_pPlayer->GetRadius(), m_pPlayer->GetParameter().fHeight))
		{
			//ダメージ受付しない時間設定
			CPlayer::sDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
			DmgInfo.reciveTime = 0.5f;
			m_pPlayer->SetDamageInfo(DmgInfo);

			// 回避状態に移行
			// 操作不能状態
			m_pPlayer->SetEnableMove(false);
			SetAction(CPlayer::Action::ACTION_DODGE);
			m_pPlayer->SetState(CPlayer::STATE_DODGE);
			//m_pPlayer->SetMotion(CPlayer::MOTION_DODGE);
		}
	}
}

//==========================================================================
// 回避
//==========================================================================
void CPlayerAction::ActionDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayer::sDamageInfo DmgInfo = m_pPlayer->GetDamageInfo();
	DmgInfo.reciveTime = 0.1f;
	m_pPlayer->SetDamageInfo(DmgInfo);

	if (m_pPlayer->GetMotion()->IsFinish())
	//if (m_fActionTime >= ActionTime::DODGE)
	{// 終了

		//スロー
		float fRate = GET_MANAGER->GetSlowRate();
		fRate += DODGE_SLOW;
		UtilFunc::Transformation::ValueNormalize(fRate, 1.0f, 0.0f);
		GET_MANAGER->SetSlowRate(fRate);

		m_pPlayer->SetEnableMove(true);
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
	if (!m_pPlayer->IsJump())
	{// キャッチ猶予
		SetAction(CPlayer::Action::ACTION_NONE);
	}
}

//==========================================================================
// キャッチ
//==========================================================================
void CPlayerAction::ActionCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pPlayer->GetMotion()->IsFinish())
	{// キャッチ猶予
		SetAction(CPlayer::Action::ACTION_NONE);
	}
}

//==========================================================================
// 投げ
//==========================================================================
void CPlayerAction::ActionThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pPlayer->GetMotion()->IsFinish())
	{// 終了
		SetAction(CPlayer::Action::ACTION_NONE);
	}
}

//==========================================================================
// ジャンプ投げ
//==========================================================================
void CPlayerAction::ActionThrowJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_pPlayer->GetMotion()->IsFinish())
	{// ジャンプ移行
		SetAction(CPlayer::Action::ACTION_JUMP);
	}
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
