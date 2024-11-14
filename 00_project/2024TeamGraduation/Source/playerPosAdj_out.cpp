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

// TODO
#include "3D_effect.h"

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

	// TODO：位置を補正
#if 0
	MyLib::Vector3 posSize = (posRight.Absolute() - posLeft.Absolute()) * 0.5f;
	posSize.z = posSize.y = 0.0f;

	MyLib::Vector3 posOrigin;
	D3DXVec3Lerp(&posOrigin, &posLeft, &posRight, 0.5f);

	UtilFunc::Collision::InBoxPillar(pos, posOrigin, posSize, posSize, VEC3_ZERO, VEC3_ZERO);

	CEffect3D::Create(pos,					VEC3_ZERO, MyLib::color::White(),  10.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);	// 
	CEffect3D::Create(posOrigin,			VEC3_ZERO, MyLib::color::Purple(), 10.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);	// 
	CEffect3D::Create(posOrigin + posSize,	VEC3_ZERO, MyLib::color::Green(),  10.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);	// 
	CEffect3D::Create(posOrigin - posSize,	VEC3_ZERO, MyLib::color::Green(),  10.0f, 0.1f, 1, CEffect3D::TYPE::TYPE_NORMAL);	// 
#else
	// 奥の補正
	MyLib::Vector3 pposLeft = posLeft + MyLib::Vector3(1000.0f, 0.0f, 0.5f);
	MyLib::Vector3 pposRight = posRight + MyLib::Vector3(1000.0f, 0.0f, 0.5f);
	UtilFunc::Collision::CollisionLimitLine(pposLeft, pposRight, pos, pPlayer->GetOldPosition());

	// 手前の補正
	MyLib::Vector3 ppposLeft = posRight + MyLib::Vector3(-1000.0f, 0.0f, 0.0f);
	MyLib::Vector3 ppposRight = posLeft + MyLib::Vector3(-1000.0f, 0.0f, 0.0f);
	UtilFunc::Collision::CollisionLimitLine(ppposLeft, ppposRight, pos, pPlayer->GetOldPosition());

	// 左の補正
	UtilFunc::Collision::CollisionLimitLine(posLeft - MyLib::Vector3(0.0f, 0.0f, 1000.0f), posLeft + MyLib::Vector3(0.0f, 0.0f, 1000.0f), pos, pPlayer->GetOldPosition());

	// 右の補正
	UtilFunc::Collision::CollisionLimitLine(posRight + MyLib::Vector3(0.0f, 0.0f, 1000.0f), posRight - MyLib::Vector3(0.0f, 0.0f, 1000.0f), pos, pPlayer->GetOldPosition());
#endif

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
