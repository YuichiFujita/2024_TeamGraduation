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

void CPlayerAIControlLeftDefense::MoveRetreat()
{

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

	// 行動：歩き
	SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);

	// 近づく
	if (Approatch(action.pos, 10.0f)) {
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