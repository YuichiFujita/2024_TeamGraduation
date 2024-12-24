//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerAIcontrol_left.h"
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

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControlLeft::CPlayerAIControlLeft()
{

}

#if 0
//==========================================================================
// 生成
//==========================================================================
CPlayerAIControlLeft* CPlayerAIControlLeft::Create(CPlayer* player)
{
	// メモリの確保
	CPlayerAIControlLeft* pObj = DEBUG_NEW CPlayerAIControlLeft;
	if (pObj != nullptr)
	{
		pObj->m_pAI = player;

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
HRESULT CPlayerAIControlLeft::Init()
{
	// 基底クラスの初期処理
	CPlayerAIControl::Init();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAIControlLeft::Uninit()
{
	// 基底クラスの終了処理
	CPlayerAIControl::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIControlLeft::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 基底クラスの更新
	CPlayerAIControl::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// モーション取得
	CPlayer* pPlayer = GetPlayer();
	if (!pPlayer) return;

	CMotion* pMotion = pPlayer->GetMotion();

	// モーションタイプの取得
	int nMotionType = pMotion->GetType();

	if (nMotionType == CPlayer::EMotion::MOTION_TOSS)
	{
		pPlayer->SetRotDest(-D3DX_PI * 0.5f);
	}
}

//==========================================================================
// 走り投げ
//==========================================================================
void CPlayerAIControlLeft::AttackDash(CPlayer* pTarget)
{
	// ターゲットの取得
	if (!pTarget) return;
	MyLib::Vector3 posTarget = pTarget->GetPosition();

	// 自分の情報
	CPlayer* pMy = GetPlayer();
	if (!pMy) return;
	MyLib::Vector3 posMy = pMy->GetPosition();

	// ラインの位置
	MyLib::Vector3 linePos = { 0.0f, posMy.y, posMy.z };

	// ラインとの距離
	float distanceLine = 0.0f;
	float distanceTarget = 0.0f;
	float JUMP_LENGTH_TARGET = 500.0f;
	float JUMP_LENGTH_LINE = 300.0f;

	// ターゲットのエリアの取得
	CGameManager::ETeamSide side = pMy->GetTeam();

	if (pTarget)
	{// ターゲットがいた場合
		distanceTarget = posMy.DistanceXZ(pTarget->GetPosition());	// 自分と相手の距離
		distanceLine = posMy.DistanceXZ(linePos);	// 自分と中心線との距離
	}
	else
	{
		return;
	}

	if (distanceTarget > JUMP_LENGTH_TARGET && distanceLine > JUMP_LENGTH_LINE)
	{// 自分とターゲットの距離が500.0f以上&&中央線との距離が範囲以上の場合

		// 走る
		SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

		// 相手の位置に近づく
		if (Approatch(posTarget, JUMP_LENGTH_LINE))
		{// 範囲内の場合
			//SetForcibly(EMoveForcibly::FORCIBLY_NONE);	// 強制行動：なし
			SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);			// 行動：止まる
		}

		return;
	}

	if (distanceTarget > JUMP_LENGTH_TARGET)
	{// ターゲットとの距離が範囲以上&&中央線との距離が範囲内の場合
		SetActionFlag(EActionFlag::ACTION_JUMP);	// アクション：跳ぶ
	}
	else
	{
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);		// 行動：止まる
		SetThrowFlag(EThrowFlag::THROW_NORMAL);		// 投げ：投げる
	}

	if (pMy->GetPosition().y >= playerAIcontrol::THROW_JUMP_END)	// 高さによって変わる
	{
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);		// 行動：止まる
		SetThrowFlag(EThrowFlag::THROW_NORMAL);		// 投げ：投げる
	}
}

//==========================================================================
// 戻る
//==========================================================================
void CPlayerAIControlLeft::ForciblyReturn()
{
	// プレイヤー情報取得
	CPlayer* pMy = GetPlayer();
	if (!pMy) return;

	MyLib::Vector3 myPos = pMy->GetPosition();
	CGameManager::ETeamSide typeTeam = pMy->GetTeam();

	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = pMy->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 歩く
	SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

	// 近づく
	if (Approatch({ -playerAIcontrol::RETURN_POS, myPos.y, myPos.z }, playerAIcontrol::OK_LENGTH))
	{
		SetForcibly(EMoveForcibly::FORCIBLY_NONE);
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
	}
}

//==========================================================================
// 安全地帯へ
//==========================================================================
void CPlayerAIControlLeft::MoveRetreat()
{
	CPlayer* pPlayer = GetPlayer();
	if (!pPlayer) return;

	// 自分の位置
	MyLib::Vector3 posMy = pPlayer->GetPosition();

	// 安全地帯
	float posSafeX = GetDistance();

	if (posMy.x < -posSafeX) {// 移動タイプ：無
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
		SetAction(EAction::IDLE);
		SetIsDistance(true);
		return;
	}

	SetIsDistance(false);

	// 行動フラグ：歩く
	SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);

	// 左移動
	MoveLeft();
}

//==========================================================================
// ランダム移動
//==========================================================================
void CPlayerAIControlLeft::MoveRandom()
{
	// x950 z560
	MyLib::Vector3 posSafeMax = { -950.0f, 0.0f, 560.0f };
	MyLib::Vector3 posSafeMin = { -GetDistance(), 0.0f, -560.0f };

	SMove move = GetMoveInfo();

	if (!move.bSet) {
		// 位置の設定
		// x座標
		float fRand = (float)UtilFunc::Transformation::Random(posSafeMax.x, posSafeMin.x);
		move.pos.x = fRand;
		// z座標
		fRand = (float)UtilFunc::Transformation::Random(posSafeMin.z, posSafeMax.z);
		move.pos.z = fRand;

		move.bSet = true;
	}

	// 行動：歩き
	SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);

	// 近づく
	if (Approatch(move.pos, 10.0f)) {
		move.bSet = false;
	}

	// 行動情報の設定
	SetMoveInfo(move);
}

//==========================================================================
// プレイヤーは線を超えていますか？
//==========================================================================
bool CPlayerAIControlLeft::IsLineOverPlayer()
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

	if (myPos.x > -playerAIcontrol::LINE_DISTANCE_OVER && GetAction() != EActionFlag::ACTION_JUMP)
	{// 位置が超えていた&&ジャンプしてない場合
		bOver = true;
	}

	return bOver;
}

//==========================================================================
// ボールは線を超えていますか？
//==========================================================================
bool CPlayerAIControlLeft::IsLineOverBall()
{
	bool bOver = false;	// 超えた判定用

	// ボール情報取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) { return bOver; }

	// プレイヤー情報取得
	CPlayer* pMy = GetPlayer();
	if (!pMy) return bOver;

	CGameManager::ETeamSide typeTeam = pMy->GetTeam();

	if (pBall->GetPosition().x > 5.0f)
	{
		bOver = true;
	}

	return bOver;
}