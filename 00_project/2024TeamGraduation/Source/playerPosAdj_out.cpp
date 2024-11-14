//==========================================================================
// 
//  プレイヤー位置補正_外野コート処理 [playerPosAdj_out.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerPosAdj_out.h"
#include "player.h"
#include "playerStatus.h"
#include "playerBase.h"
#include "playerUserOut.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerPosAdjOut::CPlayerPosAdjOut()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerPosAdjOut::~CPlayerPosAdjOut()
{

}

//==========================================================================
// 調整
//==========================================================================
void CPlayerPosAdjOut::UpdateAdjuster(CPlayer* pPlayer)
{
	CPlayer::SMotionFrag flagMotion = pPlayer->GetMotionFrag();	// モーションフラグ
	MyLib::Vector3 pos = pPlayer->GetPosition();	// 位置
	CPlayer::EState state = pPlayer->GetState();	// 状態
	bool bJump = pPlayer->IsJump();					// ジャンプフラグ

	CPlayerBase* pBase = pPlayer->GetBase();				// プレイヤーベース情報
	CPlayerUserOut* pPlayerOut = pBase->GetPlayerUserOut();	// プレイヤー外野情報
	MyLib::Vector3 posLeft = pPlayerOut->GetPosLeft();		// 移動可能な左位置
	MyLib::Vector3 posRight = pPlayerOut->GetPosRight();	// 移動可能な右位置

	// Z座標を補正
	pos.z = posLeft.z;

	if (pos.y <= CGameManager::FIELD_LIMIT)
	{ // 地面より下の場合

		// 地面に着地させる
		pos.y = CGameManager::FIELD_LIMIT;

		if (bJump && !flagMotion.bDead)
		{ // ジャンプ中着地

			// 着地モーションの再生
			pPlayer->SetMotion(CPlayer::EMotion::MOTION_LAND);
		}

		// 重力の初期化
		MyLib::Vector3 move = pPlayer->GetMove();
		move.y = 0.0f;
		pPlayer->SetMove(move);

		// ジャンプしていない状態にする
		pPlayer->SetEnableJump(false);
	}

	// 位置を反映
	pPlayer->SetPosition(pos);
}
