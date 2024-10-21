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
