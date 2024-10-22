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
// 統括
//==========================================================================
void CPlayerUserControlAction::Action(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// 目標の向き取得
	float fRotDest = player->GetRotDest();

	// 移動量取得
	MyLib::Vector3 move = player->GetMove();

	// モーション情報取得
	CMotion* pMotion = player->GetMotion();
	int nMotionType = pMotion->GetType();
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	// 状態取得
	CPlayer::STATE state = player->GetState();

	// アクション取得
	CPlayerAction* pPlayerAction = player->GetActionPattern();
	if (pPlayerAction == nullptr) return;
	CPlayer::Action action = pPlayerAction->GetAction();


	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		player->IsPossibleMove())
	{// 移動可能モーションの時

		//--------------------------
		// アクション操作
		//--------------------------
		if (action != CPlayer::Action::ACTION_BLINK ||
			action != CPlayer::Action::ACTION_DODGE)
		{
			Catch(player, fDeltaTime, fDeltaRate, fSlowRate);
			Throw(player, fDeltaTime, fDeltaRate, fSlowRate);
			Jump(player, fDeltaTime, fDeltaRate, fSlowRate);
		}

	}

	Special(player, fDeltaTime, fDeltaRate, fSlowRate);
	Charm(player, fDeltaTime, fDeltaRate, fSlowRate);

	// モーションフラグ設定
	player->SetMotionFrag(motionFrag);
}

//==========================================================================
// キャッチ
//==========================================================================
void CPlayerUserControlAction::Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (player->GetBall() != nullptr)
	{
		return;
	}

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	CMotion* pMotion = player->GetMotion();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_B, player->GetMyPlayerIdx()))
	{
		// アクションパターン変更
		SetPattern(player, CPlayer::MOTION::MOTION_CATCH, CPlayer::Action::ACTION_CATCH);
	}
}

//==========================================================================
// 投げ
//==========================================================================
void CPlayerUserControlAction::Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = player->GetBall();

	if (pBall == nullptr)
	{
		return;
	}

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	CMotion* pMotion = player->GetMotion();

	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_B, player->GetMyPlayerIdx()))
	{
		// アクションパターン変更
		if (player->IsJump())
		{
			SetPattern(player, CPlayer::MOTION::MOTION_THROW_JUMP, CPlayer::Action::ACTION_THROW_JUMP);
		}
		else
		{
			SetPattern(player, CPlayer::MOTION::MOTION_THROW, CPlayer::Action::ACTION_THROW);
		}
	}
}

//==========================================================================
// ジャンプ
//==========================================================================
void CPlayerUserControlAction::Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	bool bJump = player->IsJump();

	if (bJump)
	{
		return;
	}

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	//ジャンプ処理
	if (pKey->GetTrigger(DIK_SPACE) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_A, player->GetMyPlayerIdx()))
	{
		SetJump(player);
	}
}

//==========================================================================
// スペシャル
//==========================================================================
void CPlayerUserControlAction::Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = player->GetBall();

	if (pBall == nullptr)
	{//スペシャルゲージMAX＋ボール所持か
		return;
	}

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetTrigger(DIK_X) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_LB, player->GetMyPlayerIdx()))
	{
		pBall->ThrowSpecial(player);

		// アクションパターン変更
		SetPattern(player, CPlayer::MOTION::MOTION_SPECIAL, CPlayer::Action::ACTION_SPECIAL);
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
