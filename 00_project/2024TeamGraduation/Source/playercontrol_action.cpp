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
	const float TIME_THROWDROP = 10.0f / 60.0f;	// 投げ(ドロップボール)の猶予

	const float TAPTIME = 0.10f;		// タップの入力時間
	const float TAPRATE_MIN = 0.7f;	// タップの最小割合
	const float TAPRATE_MAX = 1.0f;	// タップの最大割合
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerControlAction::CPlayerControlAction()
{
	m_fThrowDropTime = 0.0f;	// 投げ(ドロップボール)の猶予
	m_bThrowDrop = false;		// 投げ(ドロップボール)可能判定
	m_bJumpTrigger = false;		// ジャンプトリガー
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
		(player->IsPossibleMove() || player->IsPossibleAction()))
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
	// 投げ(ドロップボール)の猶予
	UpdateThrowDrop(fDeltaTime, fDeltaRate, fSlowRate);

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
		UserChange(player, fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// スペシャル発動
//==========================================================================
void CPlayerControlAction::SpecialSetting(CPlayer* player, CBall* pBall, CTeamStatus* pTeamStatus)
{
	pBall->Special(player);

	// スペシャルゲージ消費
	pTeamStatus->ZeroSpecialValue();

	// アクションパターン変更
	SetPattern(player, CPlayer::EMotion::MOTION_HYPE, CPlayer::EAction::ACTION_SPECIAL);
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
	else if (player->GetBase()->GetPlayerControlAction()->IsThrowDrop())
	{
		SetPattern(player, CPlayer::EMotion::MOTION_THROW_DROP, CPlayer::EAction::ACTION_THROW);
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

	// 移動量取得
	MyLib::Vector3 move = player->GetMove();

	// モーション情報取得
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	// ジャンプ設定
	bJump = true;
	motionFrag.bJump = true;
	move.y = player->GetParameter().fVelocityJump * TAPRATE_MIN;

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

//==========================================================================
// ジャンプ上昇
//==========================================================================
void CPlayerControlAction::JumpFloat(CPlayer* player)
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
// 投げ(ドロップボール)可能
//==========================================================================
void CPlayerControlAction::SetThrowDrop()
{
	// 投げ(ドロップボール)の猶予設定
	m_fThrowDropTime = TIME_THROWDROP;

	// 投げの判定
	m_bThrowDrop = true;
}

//==========================================================================
// 投げ(ドロップボール)の猶予
//==========================================================================
void CPlayerControlAction::UpdateThrowDrop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 投げ(ドロップボール)の猶予減算
	m_fThrowDropTime -= fDeltaTime * fDeltaRate * fSlowRate;
	m_fThrowDropTime = UtilFunc::Transformation::Clamp(m_fThrowDropTime - fDeltaTime * fDeltaRate * fSlowRate, 0.0f, TIME_THROWDROP);

	// 投げの判定
	m_bThrowDrop = m_fThrowDropTime > 0.0f;
}
