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

	if (UtilFunc::Collision::CircleRange3D(pBall->GetPosition(), player->GetPosition(), pBall->GetRadius(), 100.0f))
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
	bool bJump = player->IsJump();

	if (bJump) return;

	//ジャンプ処理	//TODO: AIでの行動フラグとか使う？
	if (m_sFlag.bJump)
	{
		// フラグリセット
		m_sFlag.bJump = false;

		// ジャンプセット
		JumpSetting(player);
	}

	if (m_sFlag.bJumpFloat)
	{// ジャンプボタンホールドで上昇
		JumpFloat(player, fDeltaTime, fDeltaRate, fSlowRate);
	}
	if (!m_sFlag.bJumpFloat)
	{// ジャンプボタン離した

		// ジャンプトリガーOFF
		SetEnableJumpTrigger(false);
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

		// インプット情報取得
		CInputGamepad* pPad = CInputGamepad::GetInstance();

		//ジャンプ処理
		CInputGamepad::STapInfo tapInfo = pPad->GetTap(CInputGamepad::BUTTON_A, player->GetMyPlayerIdx(), TAPTIME);

		if (tapInfo.fRatio < 1.0f && pPad->GetPress(CInputGamepad::BUTTON_A, player->GetMyPlayerIdx()))
		{// タップ範囲 && 入力継続

			// 移動量取得
			MyLib::Vector3 move = player->GetMove();

			float jumpRatio = TAPRATE_MIN + (TAPRATE_MAX - TAPRATE_MIN) * tapInfo.fRatio;
			move.y = player->GetParameter().fVelocityJump * jumpRatio;

			// 移動量設定
			player->SetMove(move);
		}
	}
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
