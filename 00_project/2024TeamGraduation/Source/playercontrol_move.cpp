//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol_move.cpp]
//  Author : Kai Takada
// 
//=============================================================================
#include "playercontrol_move.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "playerAction.h"
#include "playerStatus.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float INTERVAL_INPUT = (2.0f / 60.0f);	// 入力
	const float TIME_INTERVAL = 0.3f;	// ダッシュ猶予
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerControlMove::CPlayerControlMove()
{
	memset(m_nCntTrigger, 0, sizeof(m_nCntTrigger));	// トリガーのカウント
	m_HoldDashAngle = CPlayer::EDashAngle::ANGLE_UP;		// 保持してるダッシュの移動方向
	m_fInputInterval = 0.0f;							// 入力の受け付け猶予
	m_fTriggerInterval = 0.0f;							// トリガーのインターバル
	m_bBlink = false;
	m_pInputAngle = nullptr;					// 現在の入力方向
	m_fInputAngleCtr = 0.0f;					// 現在の入力方向の保持カウンター
}

//==========================================================================
// 通常移動
//==========================================================================
void CPlayerControlMove::Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モーション情報取得
	CMotion* pMotion = player->GetMotion();
	int nMotionType = pMotion->GetType();
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	// アクション取得
	CPlayerAction* pPlayerAction = player->GetActionPattern();
	if (pPlayerAction == nullptr) return;
	CPlayer::EAction action = pPlayerAction->GetAction();

	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		player->IsPossibleMove())
	{// 移動可能モーションの時

		//--------------------------
		// 移動操作
		//--------------------------
		if (action != CPlayer::EAction::ACTION_BLINK ||
			action != CPlayer::EAction::ACTION_DODGE)
		{
			// ブリンク
			Blink(player, fDeltaTime, fDeltaRate, fSlowRate);

			// 歩き
			Walk(player, fDeltaTime, fDeltaRate, fSlowRate);

			// ダッシュ
			Dash(player, fDeltaTime, fDeltaRate, fSlowRate);
		}

		// ジャンプ状況取得
		bool bJump = player->IsJump();

		if (motionFrag.bMove &&
			pMotion->IsGetCancelable() && 
			!bJump)
		{// キャンセル可能 && 移動中

			// モーションキャンセル
			pMotion->ToggleFinish(true);
			
			//TODO: 投げの余白キャンセルとか用 ToggleFinishは必要(モーション出来たら)
		}
	}
	
#if 0
	// カニ歩き判定
	if (player->IsCrab())
	{
		CrabSetting(player);
	}
#endif
}

//==========================================================================
// トリガーのカウント設定
//==========================================================================
void CPlayerControlMove::SetCntTrigger(int* nTrigger)
{
	if (nTrigger == nullptr) return;

	for (int i = 0; i < CPlayer::EDashAngle::ANGLE_MAX; i++)
	{
		m_nCntTrigger[i] = nTrigger[i];
	}
}

//==========================================================================
// カニ歩き状態
//==========================================================================
void CPlayerControlMove::CrabSetting(CPlayer* player)
{
	// 目標の向き取得
	float fRotDest = player->GetRotDest();
	MyLib::Vector3 sizeCourt = CGame::GetInstance()->GetGameManager()->GetCourtSize();

	MyLib::Vector3 posCourt = MyLib::Vector3();
	MyLib::Vector3 pos = player->GetPosition();
	posCourt.z = pos.z;

	// 相手コート奥を見る
	switch (player->GetStatus()->GetTeam())
	{
	case CGameManager::TeamSide::SIDE_LEFT:
	
		posCourt.x = sizeCourt.x;
		
		break;

	case CGameManager::TeamSide::SIDE_RIGHT:
	
		posCourt.x = -sizeCourt.x;
		
		break;

	default:
		break;
	}

	D3DXVECTOR3 vecDiff = D3DXVECTOR3(pos.x - posCourt.x,
		0.0f,
		pos.z - posCourt.z);
	float fAngle = atan2f(vecDiff.x, vecDiff.z);		//目標の向き
	UtilFunc::Transformation::RotNormalize(fAngle);

	//pos取得ー線に対する角度
	fRotDest = fAngle;

	player->SetRotDest(fRotDest);
}
