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
#include "playerStatus.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float TAPTIME = 0.125f;		// タップの入力時間
	const float TAPRATE_MIN = 0.6f;	// タップの最小割合
	const float TAPRATE_MAX = 1.0f;	// タップの最大割合
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

	// タップ取得
	CInputGamepad::STapInfo tapInfo = pPad->GetTap(CInputGamepad::BUTTON_B, player->GetMyPlayerIdx(), TAPTIME);

	if (pKey->GetTrigger(DIK_K) ||
		tapInfo.bInput)
	{// タップ範囲はパス
		SetPattern(player, CPlayer::EMotion::MOTION_THROW_PASS, CPlayer::EAction::ACTION_THROW);
	}

	if (pKey->GetTrigger(DIK_RETURN) ||
		tapInfo.bComplete)
	{// タップ範囲を超えていた場合は投げ
		ThrowSetting(player);
	}
}

//==========================================================================
// ジャンプ
//==========================================================================
void CPlayerUserControlAction::Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	
	if (!player->IsJump() &&
		(pKey->GetTrigger(DIK_SPACE) || 
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, player->GetMyPlayerIdx())))
	{// ジャンプしてない && ジャンプボタン

		// ジャンプトリガーON
		SetEnableJumpTrigger(true);

		// ジャンプ設定
		JumpSetting(player);
	}

	if (pKey->GetPress(DIK_SPACE) ||
		pPad->GetPress(CInputGamepad::BUTTON::BUTTON_A, player->GetMyPlayerIdx()))
	{// ジャンプボタンホールドで上昇
		JumpFloat(player);
	}

	if ((!pKey->GetPress(DIK_SPACE) && !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_A, player->GetMyPlayerIdx())))
	{// ジャンプボタン離した

		// ジャンプトリガーOFF
		SetEnableJumpTrigger(false);
	}
}

//==========================================================================
// スペシャル
//==========================================================================
void CPlayerUserControlAction::Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = player->GetBall();
	CGameManager* pGameManager = CGameManager::GetInstance();
	CGameManager::ETeamSide playerTeam = player->GetTeam();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(playerTeam);
	
	// スペシャルゲージMAX＋ボール所持か
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
