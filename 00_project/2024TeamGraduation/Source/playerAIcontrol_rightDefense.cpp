//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : Takeru Ogasawara
// 
//=============================================================================
#include "playerAIcontrol_rightDefense.h"
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
CPlayerAIControlRightDefense::CPlayerAIControlRightDefense()
{
	
}

#if 1
//==========================================================================
// 生成
//==========================================================================
CPlayerAIControlRightDefense* CPlayerAIControlRightDefense::Create(CPlayer* player)
{
	// メモリの確保
	CPlayerAIControlRightDefense* pObj = DEBUG_NEW CPlayerAIControlRightDefense;
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
HRESULT CPlayerAIControlRightDefense::Init()
{
	// 親クラスの初期化処理
	CPlayerAIControlDefense::Init();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAIControlRightDefense::Uninit()
{
	// 親クラスの終了
	CPlayerAIControlDefense::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIControlRightDefense::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親クラスの更新
	CPlayerAIControlDefense::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 後退
//==========================================================================
void CPlayerAIControlRightDefense::MoveRetreat()
{
	CPlayer* pPlayer = GetPlayer();
	if (!pPlayer) return;

	// 自分の位置
	MyLib::Vector3 posMy = pPlayer->GetPosition();

	// 安全地帯
	float posSafeX = GetDistance();

	if (posMy.x > posSafeX) {// 移動タイプ：無
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
		SetAction(EAction::IDLE);
		return;
	}

	// 行動フラグ：歩く
	SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);

	// 右移動
	MoveRight();
}

//==========================================================================
// ランダム移動
//==========================================================================
void CPlayerAIControlRightDefense::MoveRandom()
{
	// x950 z560
	MyLib::Vector3 posSafeMax = { 950.0f, 0.0f, 560.0f };
	MyLib::Vector3 posSafeMin = { GetDistance(), 0.0f, -560.0f };

	CPlayerAIControlDefense::SAction action = GetActionInfo();
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	if (!action.bSet) {
		// 位置の設定
		// x座標
		float fRand = (float)UtilFunc::Transformation::Random((int)posSafeMin.x, (int)posSafeMax.x);
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
bool CPlayerAIControlRightDefense::IsLineOverPlayer()
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

	if (myPos.x < 10.0f && GetAction() != GetActionFlag() != EActionFlag::ACTION_JUMP)
	{// 位置が超えていた&&ジャンプしてない場合
		bOver = true;
	}

	return bOver;
}

//==========================================================================
// ボールは線を超えていますか？
//==========================================================================
bool CPlayerAIControlRightDefense::IsLineOverBall()
{
	// ボール情報取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) { return false; }

	if (pBall->GetPosition().x <= 0.0f)
	{
		return true;
	}

	return false;
}