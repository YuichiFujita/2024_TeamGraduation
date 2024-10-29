//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "playercontrol_action.h"
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
CPlayerControlAction::CPlayerControlAction()
{

}

//==========================================================================
// 統括
//==========================================================================
void CPlayerControlAction::Action(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モーション情報取得
	CMotion* pMotion = player->GetMotion();
	int nMotionType = pMotion->GetType();

	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		player->IsPossibleMove())
	{// 移動可能モーションの時

		//--------------------------
		// アクション操作
		//--------------------------
		ConditionalAction(player, fDeltaTime, fDeltaRate, fSlowRate);
	}

	Special(player, fDeltaTime, fDeltaRate, fSlowRate);
	Charm(player, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 基本アクション操作
//==========================================================================
void CPlayerControlAction::ConditionalAction(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// アクション取得
	CPlayerAction* pPlayerAction = player->GetActionPattern();
	if (pPlayerAction == nullptr) return; 
	CPlayer::EAction action = pPlayerAction->GetAction();

	if (action != CPlayer::EAction::ACTION_BLINK ||
		action != CPlayer::EAction::ACTION_DODGE)
	{
		Catch(player, fDeltaTime, fDeltaRate, fSlowRate);
		Throw(player, fDeltaTime, fDeltaRate, fSlowRate);
		Jump(player, fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// スペシャル発動
//==========================================================================
void CPlayerControlAction::SpecialSetting(CPlayer* player, CBall* pBall, CTeamStatus* pTeamStatus)
{
	pBall->Special(player);

	//スペシャルゲージ消費
	pTeamStatus->ZeroSpecialValue();

	// アクションパターン変更
	SetPattern(player, CPlayer::EMotion::MOTION_WIN, CPlayer::EAction::ACTION_SPECIAL);	// TAKADA：SPECIAL_THROWが出来たらそれにしてくれ
}

//==========================================================================
// 投げ設定
//==========================================================================
void CPlayerControlAction::ThrowSetting(CPlayer* player)
{
	// アクションパターン変更
	if (player->IsJump())
	{
		SetPattern(player, CPlayer::EMotion::MOTION_THROW_JUMP, CPlayer::EAction::ACTION_THROW_JUMP);
	}
	else if (player->IsDash())
	{
		SetPattern(player, CPlayer::EMotion::MOTION_THROW_RUN, CPlayer::EAction::ACTION_THROW);
	}
	else
	{
		SetPattern(player, CPlayer::EMotion::MOTION_THROW, CPlayer::EAction::ACTION_THROW);
	}
}

//==========================================================================
// ジャンプ設定
//==========================================================================
void CPlayerControlAction::JumpSetting(CPlayer* player)
{
	// ジャンプ判定取得
	bool bJump = player->IsJump();
	if (bJump)
	{
		return;
	}

	// 移動量取得
	MyLib::Vector3 move = player->GetMove();

	// モーション情報取得
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	bJump = true;
	motionFrag.bJump = true;
	move.y += 12.0f;

	player->SetMove(move);
	player->SetMotionFrag(motionFrag);

	// ジャンプ判定設定
	player->SetEnableJump(bJump);

	//ボール所持判定
	CPlayer::EMotion motion = CPlayer::EMotion::MOTION_JUMP;

	if (player->GetBall() != nullptr)
	{
		motion = CPlayer::EMotion::MOTION_JUMP_BALL;
	}

	// アクションパターン変更
	SetPattern(player, motion, CPlayer::EAction::ACTION_JUMP);

	// サウンド再生
	//CSound::GetInstance()->PlaySound(CSound::LABEL_SE_JUMP);
}