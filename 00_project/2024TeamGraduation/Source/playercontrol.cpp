//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playercontrol.h"
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
CPlayerControl::CPlayerControl()
{

}

//==========================================================================
// アクション＆モーションの一括設定
//==========================================================================
void CPlayerControl::SetPattern(CPlayer* player, CPlayer::MOTION typeM, CPlayer::Action typeA)
{
	CMotion* pMotion = player->GetMotion();

	pMotion->Set(CPlayer::MOTION::MOTION_CATCH);
	player->GetActionPattern()->SetAction(CPlayer::Action::ACTION_CATCH);
}

//==========================================================================
// ジャンプ設定
//==========================================================================
void CPlayerControl::SetJump(CPlayer* player)
{
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

	// アクションパターン変更
	SetPattern(player, CPlayer::MOTION::MOTION_JUMP, CPlayer::Action::ACTION_JUMP);

	// サウンド再生
	//CSound::GetInstance()->PlaySound(CSound::LABEL_SE_JUMP);
}
