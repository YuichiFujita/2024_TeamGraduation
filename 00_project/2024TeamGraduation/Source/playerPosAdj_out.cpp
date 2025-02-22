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
#if 1
	CPlayerBase* pBase = pPlayer->GetBase();				// プレイヤーベース情報
	CPlayerOut* pPlayerOut = pBase->GetPlayerOut();			// プレイヤー外野情報
	MyLib::Vector3 posCur = pPlayer->GetPosition();			// プレイヤー現在位置
	MyLib::Vector3 posOld = pPlayer->GetOldPosition();		// プレイヤー過去位置
	MyLib::Vector3 posLeft = pPlayerOut->GetPosLeft();		// 移動可能な左位置
	MyLib::Vector3 posRight = pPlayerOut->GetPosRight();	// 移動可能な右位置
	MyLib::Vector3 vecMove = posRight - posLeft;			// プレイヤー移動ベクトル
	float fAngleMove = vecMove.AngleXZ(VEC3_ZERO);			// プレイヤー移動角度

	// プレイヤーから見て奥側の位置補正
	{
		MyLib::Vector3 posFarLeft = posLeft;
		posFarLeft.x += sinf(fAngleMove - HALF_PI) * 0.5f;
		posFarLeft.z += cosf(fAngleMove - HALF_PI) * 0.5f;
		posFarLeft.x += sinf(fAngleMove) * 1000.0f;
		posFarLeft.z += cosf(fAngleMove) * 1000.0f;

		MyLib::Vector3 posFarRight = posRight;
		posFarRight.x += sinf(fAngleMove - HALF_PI) * 0.5f;
		posFarRight.z += cosf(fAngleMove - HALF_PI) * 0.5f;
		posFarRight.x += sinf(fAngleMove) * 1000.0f;
		posFarRight.z += cosf(fAngleMove) * 1000.0f;

		// 奥の補正
		UtilFunc::Collision::CollisionLimitLine(posFarLeft, posFarRight, posCur, posOld);
	}

	// プレイヤーから見て手前側の位置補正
	{
		MyLib::Vector3 posNearLeft = posRight;
		posNearLeft.x -= sinf(fAngleMove - HALF_PI) * 0.5f;
		posNearLeft.z -= cosf(fAngleMove - HALF_PI) * 0.5f;
		posNearLeft.x -= sinf(fAngleMove) * 1000.0f;
		posNearLeft.z -= cosf(fAngleMove) * 1000.0f;

		MyLib::Vector3 posNearRight = posLeft;
		posNearRight.x -= sinf(fAngleMove - HALF_PI) * 0.5f;
		posNearRight.z -= cosf(fAngleMove - HALF_PI) * 0.5f;
		posNearRight.x -= sinf(fAngleMove) * 1000.0f;
		posNearRight.z -= cosf(fAngleMove) * 1000.0f;

		// 手前の補正
		UtilFunc::Collision::CollisionLimitLine(posNearLeft, posNearRight, posCur, posOld);
	}

	// プレイヤーから見て左側の位置補正
	{
		MyLib::Vector3 posLeftLeft = posLeft;
		posLeftLeft.x += sinf(fAngleMove + HALF_PI) * 1000.0f;
		posLeftLeft.z += cosf(fAngleMove + HALF_PI) * 1000.0f;

		MyLib::Vector3 posLeftRight = posLeft;
		posLeftRight.x += sinf(fAngleMove - HALF_PI) * 1000.0f;
		posLeftRight.z += cosf(fAngleMove - HALF_PI) * 1000.0f;

		// 左の補正
		UtilFunc::Collision::CollisionLimitLine(posLeftLeft, posLeftRight, posCur, pPlayer->GetOldPosition());
	}

	// プレイヤーから見て右側の位置補正
	{
		MyLib::Vector3 posRightLeft = posRight;
		posRightLeft.x += sinf(fAngleMove - HALF_PI) * 1000.0f;
		posRightLeft.z += cosf(fAngleMove - HALF_PI) * 1000.0f;

		MyLib::Vector3 posRightRight = posRight;
		posRightRight.x += sinf(fAngleMove + HALF_PI) * 1000.0f;
		posRightRight.z += cosf(fAngleMove + HALF_PI) * 1000.0f;

		// 右の補正
		UtilFunc::Collision::CollisionLimitLine(posRightLeft, posRightRight, posCur, pPlayer->GetOldPosition());
	}
#else
	// ベース情報の取得
	CPlayerBase* pBase = pPlayer->GetBase();		// プレイヤーベース情報
	CPlayerOut* pPlayerOut = pBase->GetPlayerOut();	// プレイヤー外野情報

	// 座標情報の取得
	MyLib::Vector3 posCur = pPlayer->GetPosition();			// プレイヤー現在位置
	MyLib::Vector3 posLeft = pPlayerOut->GetPosLeft();		// 移動可能な左位置
	MyLib::Vector3 posRight = pPlayerOut->GetPosRight();	// 移動可能な右位置

	// 補正座標の計算
	MyLib::Vector3 vecToLeft = posCur - posLeft;		// プレイヤーと左位置間のベクトル
	MyLib::Vector3 vecMove = posRight - posLeft;		// プレイヤー移動ベクトル
	//float fRate = D3DXVec3Dot(&vecMove, &vecLine);	// 移動ベクトルの補正割合
	float fRate = (vecMove.x * vecToLeft.x) + (vecMove.z * vecToLeft.z);	// TODO：ん〜間違ってなくね？内積の位置補正
	if (fRate > 1.0f)
	{
		posCur.x = posRight.x;
		posCur.z = posRight.z;
	}
	else if (fRate < 0.0f)
	{
		posCur.x = posLeft.x;
		posCur.z = posLeft.z;
	}
	else
	{
		posCur.x = vecMove.x * fRate;
		posCur.z = vecMove.z * fRate;
	}
#endif

	if (posCur.y <= CGameManager::FIELD_LIMIT)
	{ // 地面より下の場合

		// 地面に着地させる
		posCur.y = CGameManager::FIELD_LIMIT;

		if (pPlayer->IsJump() && !pPlayer->GetMotionFrag().bDead)
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
	pPlayer->SetPosition(posCur);
}
