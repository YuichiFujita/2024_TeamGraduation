//=============================================================================
// 
//  プレイヤーAIコントロール_アクション処理 [playerAIcontrol_action.cpp]
//  Author : 藤田勇一
// 
//=============================================================================
#include "playerAIcontrol_action.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "ball.h"
#include "playerAction.h"
#include "teamStatus.h"
#include "playerStatus.h"

//==========================================================================
// 定数定義
//==========================================================================
//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float TAPTIME = 0.125f;	// タップの入力時間
	const float TAPRATE_MIN = 0.6f;	// タップの最小割合
	const float TAPRATE_MAX = 1.0f;	// タップの最大割合
}

//==========================================================================
// 静的メンバ変数
//==========================================================================
bool CPlayerAIControlAction::m_bAutoThrow = true;

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControlAction::CPlayerAIControlAction()
{
	ZeroMemory(&m_sFlag, sizeof(m_sFlag));
	m_fJumpTimer = 0.0f;
	m_fJumpRate = 0.0f;
	m_fMaxJumpRate = 0.0f;
	m_fCatchRadius = 0.0f;
}

//==========================================================================
// キャッチ
//==========================================================================
void CPlayerAIControlAction::Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (player->GetBall() != nullptr) return;
	//if (!m_sFlag.bCatch) return;

	// TODO：全自動キャッチ機構
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall->IsAttack() || pBall->GetTypeTeam() == player->GetTeam()) return;

	if (UtilFunc::Collision::CircleRange3D(pBall->GetPosition(), player->GetPosition(), pBall->GetRadius(), m_fCatchRadius))
	{
		// アクションパターン変更
		SetPattern(player, CPlayer::EMotion::MOTION_CATCH_STANCE, CPlayer::EAction::ACTION_CATCH);
	}

	m_sFlag.bCatch = false;
}

//==========================================================================
// 投げ
//==========================================================================
void CPlayerAIControlAction::Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#if _DEBUG
	if (!m_bAutoThrow) { return; }
#endif

	CBall* pBall = player->GetBall();

	if (m_sFlag.bThrow)
	{
		if (pBall == nullptr)
		{
			m_sFlag.bThrow = false;

			return;
		}

		// 投げる
		ThrowSetting(player);
	}
	else if (m_sFlag.bPass)
	{
		m_sFlag.bPass = false;

		// パス
		SetPattern(player, CPlayer::EMotion::MOTION_THROW_PASS, CPlayer::EAction::ACTION_THROW);
	}
}

//==========================================================================
// ジャンプ
//==========================================================================
void CPlayerAIControlAction::Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//ジャンプ処理
	if (!player->IsJump() &&		// ジャンプしてない
		m_sFlag.bJump)				// ジャンプON
	{
		// ジャンプOFF
		m_sFlag.bJump = false;

		// ジャンプ上昇ON
		m_sFlag.bJumpFloat = true;

		// ジャンプトリガーON
		SetEnableJumpTrigger(true);

		// ジャンプ設定
		JumpSetting(player);
	}

	if (m_sFlag.bJumpFloat)
	{// ジャンプボタンホールドで上昇
		JumpFloat(player, fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// ジャンプ上昇
//==========================================================================
void CPlayerAIControlAction::JumpFloat(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ジャンプ判定取得
	bool bJump = player->IsJump();

	if (bJump && IsJumpTrigger())
	{// ジャンプ中は押してる間距離伸びていく

		//ジャンプ処理
		m_fJumpTimer += fDeltaTime * fSlowRate;
		float rate = m_fJumpTimer / TAPTIME;
		
		if (m_fMaxJumpRate > rate)
		{
			// 移動量取得
			MyLib::Vector3 move = player->GetMove();

			float jumpRate = TAPRATE_MIN + (TAPRATE_MAX - TAPRATE_MIN) * rate;
			m_fJumpRate = jumpRate;

			move.y = player->GetParameter().fVelocityJump * jumpRate;
		
			// 移動量設定
			player->SetMove(move);
		}
		else
		{// 終了
			// フラグリセット
			m_sFlag.bJumpFloat = false;
			m_fJumpTimer = 0.0f;
			m_fJumpRate = 0.0f;

			// ジャンプトリガーOFF
			SetEnableJumpTrigger(false);
		}

		return;
	}

	// タイマーリセット
	m_fJumpTimer = 0.0f;
}

//==========================================================================
// スペシャル
//==========================================================================
void CPlayerAIControlAction::Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = player->GetBall();
	CGameManager* pGameManager = CGameManager::GetInstance();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(player->GetTeam());

	//スペシャルゲージMAX＋ボール所持か
	if (pBall == nullptr || !pTeamStatus->IsMaxSpecial()) return;

	//TODO: フラグか何かで発動操作
	if (m_sFlag.bSpecial)
	{
		// フラグリセット
		m_sFlag.bSpecial = false;

		//発動
		SpecialSetting(player, pBall, pTeamStatus);
	}
}

//==========================================================================
// モテボタン
//==========================================================================
void CPlayerAIControlAction::Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	player->GetActionPattern()->SetEnableCharm(false);

	//TODO: フラグか何かで操作
	if (false)
	{
		//モテアクション発動準備状態
		player->GetActionPattern()->SetEnableCharm(true);
	}
}
