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
	const float TAPTIME = 0.15f;	// タップの入力時間
	const float TAPRATE_MIN = 0.7f;	// タップの最小割合
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
}

//==========================================================================
// キャッチ
//==========================================================================
void CPlayerAIControlAction::Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
}

//==========================================================================
// ジャンプ
//==========================================================================
void CPlayerAIControlAction::Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	bool bJump = player->IsJump();

	if (bJump) return;

	//ジャンプ処理	//TODO: AIでの行動フラグとか使う？
	if (m_sFlag.bJump)
	{
		// フラグリセット
		m_sFlag.bJump = false;

		JumpSetting(player);
	}
}

//==========================================================================
// ジャンプ上昇
//==========================================================================
void CPlayerAIControlAction::JumpFloat(CPlayer* player)
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
