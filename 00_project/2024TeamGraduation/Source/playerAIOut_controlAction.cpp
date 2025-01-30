//=============================================================================
// 
//  プレイヤーAIコントロール_アクション処理 [playerAIcontrol_action.cpp]
//  Author : 藤田勇一
// 
//=============================================================================
#include "playerAIOut_controlAction.h"
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
	const float TAPTIME = 0.15f;	// タップの入力時間
	const float TAPRATE_MIN = 0.7f;	// タップの最小割合
	const float TAPRATE_MAX = 1.0f;	// タップの最大割合
}

//==========================================================================
// 静的メンバ変数
//==========================================================================
bool CPlayerAIOutControlAction::m_bAutoThrow = true;

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIOutControlAction::CPlayerAIOutControlAction()
{
	ZeroMemory(&m_sFlag, sizeof(m_sFlag));
	m_fJumpTimer = 0.0f;
}

//==========================================================================
// キャッチ
//==========================================================================
void CPlayerAIOutControlAction::Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (player->GetBall() != nullptr) return;

	CMotion* pMotion = player->GetMotion();

	// TODO：全自動キャッチ機構
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall->IsAttack() || pBall->GetTypeTeam() == player->GetTeam()) return;

	if (UtilFunc::Collision::CircleRange3D(pBall->GetPosition(), player->GetPosition(), pBall->GetRadius(), 300.0f))
	{
		// アクションパターン変更
		SetPattern(player, CPlayer::EMotion::MOTION_CATCH_STANCE, CPlayer::EAction::ACTION_CATCH);
	}
}

//==========================================================================
// 投げ
//==========================================================================
void CPlayerAIOutControlAction::Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#if _DEBUG
	if (!m_bAutoThrow) { return; }
#endif

	if (!player->GetBall()) return;


	CBall* pBall = player->GetBall();

	if (m_sFlag.bThrow)
	{
		m_sFlag.bThrow = false;

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
void CPlayerAIOutControlAction::Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIOutControlAction::JumpFloat(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ジャンプ判定取得
	bool bJump = player->IsJump();

	if (bJump && IsJumpTrigger())
	{// ジャンプ中は押してる間距離伸びていく

		//ジャンプ処理
		m_fJumpTimer += fDeltaTime * fSlowRate;
		float rate = m_fJumpTimer / TAPTIME;

		if (TAPRATE_MAX > rate)
		{
			// 移動量取得
			MyLib::Vector3 move = player->GetMove();

			float jumpRate = TAPRATE_MIN + (TAPRATE_MAX - TAPRATE_MIN) * rate;

			move.y = player->GetParameter().fVelocityJump * jumpRate;

			// 移動量設定
			player->SetMove(move);
		}
		else
		{// 終了
			// フラグリセット
			m_sFlag.bJumpFloat = false;
			m_fJumpTimer = 0.0f;

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
void CPlayerAIOutControlAction::Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
}

//==========================================================================
// モテボタン
//==========================================================================
void CPlayerAIOutControlAction::Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	player->GetActionPattern()->SetEnableCharm(false);

	//TODO: フラグか何かで操作
	if (false)
	{
		//モテアクション発動準備状態
		player->GetActionPattern()->SetEnableCharm(true);
	}
}
