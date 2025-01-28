//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerAIcontrol_leftDefense.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "teamStatus.h"

// デバッグ用
#include "3D_effect.h"

// player関連
#include "playerBase.h"
#include "playerStatus.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol_move.h"
#include "playerManager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

};

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControlLeftDefense::CPlayerAIControlLeftDefense()
{

}

#if 1
//==========================================================================
// 生成
//==========================================================================
CPlayerAIControlLeftDefense* CPlayerAIControlLeftDefense::Create()
{
	// メモリの確保
	CPlayerAIControlLeftDefense* pObj = DEBUG_NEW CPlayerAIControlLeftDefense;
	if (pObj != nullptr)
	{
		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}
#endif

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerAIControlLeftDefense::Init()
{
	CPlayerAIControlDefense::Init();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAIControlLeftDefense::Uninit()
{
	CPlayerAIControlDefense::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIControlLeftDefense::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerAIControlDefense::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ボールを奪う
//==========================================================================
void CPlayerAIControlLeftDefense::MoveRetreat()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// 位置の取得
	MyLib::Vector3 posBall = pBall->GetPosition();		// ボール
	MyLib::Vector3 posEnd = pBall->GetPosPassEnd();		// ボールのパス終了位置
	MyLib::Vector3 posMy = pAI->GetPosition();		// 自分の位置

	// 終了位置のx,zを参照した位置の設定
	MyLib::Vector3 pos = { posEnd.x, posMy.y, posEnd.z };

	if (pos.x > 0)
	{
		SetActionStatus(EActionStatus::ACTIONSTATUS_IDLE);
		return;
	}

	// パス相手の取得
	CPlayer* pTarget = pBall->GetTarget();
	if (pTarget)
	{
		// ターゲットとボールの位置
		float distanth0 = pTarget->GetPosition().DistanceXZ(posBall);

		if (distanth0 < 100.0f)
		{// ボールとパス先の距離が範囲内ならあきらめる
			SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
			SetActionStatus(EActionStatus::ACTIONSTATUS_IDLE);
			return;
		}
	}

	// ボールとの距離
	float distance = posMy.DistanceXZ(posBall);

	// 行動状態：走る
	SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

	// ボールの方へ行く
	if (Approatch(pos, 100.0f) || distance < 100.0f)
	{// 終了位置に近づけた||ボールとの距離が範囲内の場合

		if (posBall.y < 140.0f)
		{// 取れそうな高さに来た！
			SetActionFlag(EActionFlag::ACTION_JUMP);
		}

		SetActionStatus(EActionStatus::ACTIONSTATUS_IDLE);
	}
}

//==========================================================================
// ランダム移動
//==========================================================================
void CPlayerAIControlLeftDefense::MoveRandom()
{
	// x-950 z560
	MyLib::Vector3 posSafeMax = { -950.0f, 0.0f, 560.0f };
	MyLib::Vector3 posSafeMin = { -GetDistance(), 0.0f, -560.0f };

	CPlayerAIControlDefense::SAction action = GetActionInfo();
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	if (!action.bSet) {
		// 位置の設定
		// x座標
		float fRand = (float)UtilFunc::Transformation::Random((int)posSafeMax.x, (int)posSafeMin.x);
		action.pos.x = fRand;
		// z座標
		fRand = (float)UtilFunc::Transformation::Random((int)posSafeMin.z, (int)posSafeMax.z);
		action.pos.z = fRand;

		action.bSet = true;
	}

	// もくてき位置との距離
	float distanse = pAI->GetPosition().DistanceXZ(action.pos);
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	CMotion* pMotion = pAI->GetMotion();
	int nType = pMotion->GetType();

	if (distanse > 300.0f)
	{
		if (pBall)
		{
			if (!pBall->GetPlayer() &&						// ボール所持者がいない
				nType != CPlayer::EMotion::MOTION_JUMP)		// ジャンプ中じゃない
			{
				// 行動：走り
				SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);
			}
			else
			{
				// 行動：歩き
				SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);
			}
		}
		else
		{
			// 行動：歩き
			SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);
		}
	}
	else {
		// 行動：歩き
		SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);
	}

	// 近づく
	if (Approatch(action.pos, 30.0f)) {
		action.bSet = false;

		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
		SetActionStatus(EActionStatus::ACTIONSTATUS_COOLDOWN);
	}

	// 行動情報の設定
	SetActionInfo(action);
}

//==========================================================================
// プレイヤーは線を超えていますか？
//==========================================================================
bool CPlayerAIControlLeftDefense::IsLineOverPlayer()
{
	bool bOver = false;	// 超えた判定用

	// プレイヤー情報取得
	CPlayer* pMy = GetPlayer();
	if (!pMy) return bOver;

	MyLib::Vector3 myPos = pMy->GetPosition();

	if (myPos.y > 0)
	{
		return bOver;
	}

	//if (myPos.x > -10.0f && GetAction() != EActionFlag::ACTION_JUMP)
	{// 位置が超えていた&&ジャンプしてない場合
		bOver = true;
	}

	return bOver;
}

//==========================================================================
// ボールは線を超えていますか？
//==========================================================================
bool CPlayerAIControlLeftDefense::IsLineOverBall()
{
	bool bOver = false;	// 超えた判定用

	// ボール情報取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) { return bOver; }

	if (pBall->GetPosition().x >= 0.0f)
	{
		bOver = true;
	}

	return bOver;
}