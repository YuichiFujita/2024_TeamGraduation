//=============================================================================
// 
//  プレイヤーコントロール処理 [playerUsercontrol_action.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerUsercontrol_action.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "ball.h"
#include "playerAction.h"
#include "teamStatus.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerUserControlAction::CPlayerUserControlAction()
{

}

//==========================================================================
// キャッチ
//==========================================================================
void CPlayerUserControlAction::Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (player->GetBall() != nullptr) return;

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	CMotion* pMotion = player->GetMotion();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_B, player->GetMyPlayerIdx()))
	{
		// アクションパターン変更
		CPlayer::EMotion motion = CPlayer::EMotion::MOTION_CATCH_STANCE;
		if (player->IsJump())
		{
			motion = CPlayer::EMotion::MOTION_CATCH_STANCE_JUMP;
		}
		SetPattern(player, motion, CPlayer::EAction::ACTION_CATCH);
	}
}

//==========================================================================
// 投げ
//==========================================================================
void CPlayerUserControlAction::Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = player->GetBall();

	if (pBall == nullptr) return;

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	CMotion* pMotion = player->GetMotion();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_B, player->GetMyPlayerIdx()))
	{
		ThrowSetting(player);
	}
}

//==========================================================================
// ジャンプ
//==========================================================================
void CPlayerUserControlAction::Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	bool bJump = player->IsJump();

	if (bJump) return;

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	//ジャンプ処理
	if (pKey->GetTrigger(DIK_SPACE) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_A, player->GetMyPlayerIdx()))
	{
		JumpSetting(player);
	}
}

//==========================================================================
// スペシャル
//==========================================================================
void CPlayerUserControlAction::Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = player->GetBall();
	CGameManager* pGameManager = CGameManager::GetInstance();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(player->GetCharStatus()->GetTeam());
	
	//スペシャルゲージMAX＋ボール所持か
	if (pBall == nullptr ||	!pTeamStatus->IsMaxSpecial()) return;

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_X) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_LB, player->GetMyPlayerIdx()))
	{
		//発動
		SpecialSetting(player, pBall, pTeamStatus);
	}
}

//==========================================================================
// モテボタン
//==========================================================================
void CPlayerUserControlAction::Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	player->GetActionPattern()->SetEnableCharm(false);

	if (pKey->GetPress(DIK_C) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_RB, player->GetMyPlayerIdx()))
	{
		//モテアクション発動準備状態
		player->GetActionPattern()->SetEnableCharm(true);
	}
}
