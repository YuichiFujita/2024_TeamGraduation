//==========================================================================
// 
//  内野プレイヤー処理 [playerIn.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerIn.h"
#include "playerStatus.h"

// 使用クラス
#include "playercontrol_move.h"
#include "playercontrol_action.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerIn::CPlayerIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerBase(pPlayer, typeTeam, typeArea)
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerIn::~CPlayerIn()
{

}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerIn::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// プレイヤーの取得
	CPlayer* pPlayer = GetPlayer();

	// 操作クラスの取得
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// 操作の更新
	pControlMove->Move(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);
	pControlAction->Action(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);

	// 操作クラスの反映
	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// ヒット
//==========================================================================
CPlayer::SHitInfo CPlayerIn::Hit(CBall* pBall)
{
	// 基底クラスのヒット
	CPlayer::SHitInfo hitInfo = CPlayerBase::Hit(pBall);	// ヒット情報

	// ダメージを受けていない場合は抜ける
	if (!hitInfo.bHit) { return hitInfo; }

	CGameManager::ETeamSide sideBall = pBall->GetTypeTeam();	// ボールチームサイド
	CPlayer* pPlayer = GetPlayer();								// プレイヤー情報
	CPlayerStatus* pStatus = pPlayer->GetStatus();				// ステータス情報
	CGameManager* pGameManager = CGameManager::GetInstance();	// ゲームマネージャー情報
	MyLib::HitResult_Character hitresult = {};					// 衝突情報

	// 無敵状態なら抜ける
	if (!pPlayer->GetDamageInfo().bReceived) { return hitInfo; }

	// ダメージを与える
	pStatus->LifeDamage(pBall->GetDamage());

	if (pPlayer->GetLife() <= 0)
	{
		// 終活
		pPlayer->DeadSetting(&hitresult, pBall);
	}
	else
	{
		// ダメージ状態にする
		pPlayer->DamageSetting(pBall);
	}

	// モテ加算(ボール投げた側)
	pGameManager->AddCharmValue(sideBall, CCharmManager::ETypeAdd::ADD_HIT);

	// スペシャル加算
	pGameManager->AddSpecialValue(sideBall, CSpecialValueManager::ETypeAdd::ADD_HIT);

	return hitInfo;
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayerIn::ChangeMoveControl(CPlayerControlMove* control)
{
	// 操作クラスの取得
	CPlayerControlMove* pControlMove = GetPlayerControlMove();

	// 操作クラスの入替
	delete pControlMove;
	pControlMove = control;

	// 操作クラスの反映
	SetPlayerControlMove(pControlMove);
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayerIn::ChangeActionControl(CPlayerControlAction* control)
{
	// 操作クラスの取得
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// 操作クラスの入替
	delete pControlAction;
	pControlAction = control;

	// 操作クラスの反映
	SetPlayerControlAction(pControlAction);
}
