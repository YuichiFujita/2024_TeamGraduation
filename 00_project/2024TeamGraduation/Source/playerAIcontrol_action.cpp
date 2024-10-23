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

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// 静的メンバ変数
//==========================================================================
#ifdef _DEBUG
bool CPlayerAIControlAction::m_bAutoThrow = true;
#endif

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControlAction::CPlayerAIControlAction()
{

}

//==========================================================================
// 統括
//==========================================================================
void CPlayerAIControlAction::Action(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
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

	// TODO：操作はAIにいるのか問題
#if 0
	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		player->IsPossibleMove())
	{// 移動可能モーションの時

		//--------------------------
		// アクション操作
		//--------------------------
		if (action != CPlayer::Action::ACTION_BLINK)
		{
			Catch(player, fDeltaTime, fDeltaRate, fSlowRate);
			Throw(player, fDeltaTime, fDeltaRate, fSlowRate);
			Jump(player, fDeltaTime, fDeltaRate, fSlowRate);
		}

	}

	Special(player, fDeltaTime, fDeltaRate, fSlowRate);
	Charm(player, fDeltaTime, fDeltaRate, fSlowRate);
#else

	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		player->IsPossibleMove())
	{// 移動可能モーションの時

		//--------------------------
		// アクション操作
		//--------------------------
		if (action != CPlayer::Action::ACTION_BLINK)
		{
			Catch(player, fDeltaTime, fDeltaRate, fSlowRate);
			Throw(player, fDeltaTime, fDeltaRate, fSlowRate);
		}
	}

#endif

	// モーションフラグ設定
	player->SetMotionFrag(motionFrag);
}

//==========================================================================
// キャッチ
//==========================================================================
void CPlayerAIControlAction::Catch(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (player->GetBall() != nullptr)
	{
		return;
	}

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	CMotion* pMotion = player->GetMotion();

#if 0
	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_B, player->GetMyPlayerIdx()))
#else
	// TODO：全自動キャッチ機構
	CBall* pBall = CGame::GetInstance()->GetGameManager()->GetBall();
	if (UtilFunc::Collision::CircleRange3D(pBall->GetPosition(), player->GetPosition(), pBall->GetRadius(), 100.0f))
#endif
	{
		// アクションパターン変更
		pMotion->Set(CPlayer::MOTION::MOTION_CATCH);
		player->GetActionPattern()->SetAction(CPlayer::Action::ACTION_CATCH);
	}
}

//==========================================================================
// 投げ
//==========================================================================
void CPlayerAIControlAction::Throw(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = player->GetBall();
	if (pBall == nullptr)
	{
		return;
	}

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

#if 0
	if (pKey->GetTrigger(DIK_RETURN) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_B, player->GetMyPlayerIdx()))
#else
	if (m_bAutoThrow)
	{
		fThrowTime += fDeltaTime;
	}

	// ボタン
	if (ImGui::Button("PlayerAI : ThrowBall") || fThrowTime > 1.0f)
#endif
	{
#ifdef _DEBUG
	fThrowTime = 0.0f;
#endif

		// アクションパターン変更
		if (player->IsJump())
		{
			player->GetActionPattern()->SetAction(CPlayer::Action::ACTION_THROW_JUMP);
		}
		else
		{
			pBall->ThrowNormal(player);

			player->GetActionPattern()->SetAction(CPlayer::Action::ACTION_THROW);
		}
	}
}

//==========================================================================
// ジャンプ
//==========================================================================
void CPlayerAIControlAction::Jump(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControlAction::Special(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
		player->GetActionPattern()->SetAction(CPlayer::Action::ACTION_SPECIAL);
	}
}

//==========================================================================
// モテボタン
//==========================================================================
void CPlayerAIControlAction::Charm(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (pKey->GetPress(DIK_C) ||
		pPad->GetTrigger(CInputGamepad::BUTTON_RB, player->GetMyPlayerIdx()))
	{
		//モテアクション発動準備状態
	}
}
