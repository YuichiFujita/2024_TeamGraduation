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
}

//==========================================================================
// 通常移動
//==========================================================================
void CPlayerControlMove::Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// 目標の向き取得
	float fRotDest = player->GetRotDest();

	// 移動量取得
	MyLib::Vector3 move = player->GetMove();

	// モーション情報取得
	CMotion* pMotion = player->GetMotion();
	int nMotionType = pMotion->GetType();
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	// 状態取得
	CPlayer::EState state = player->GetState();

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
			!bJump)
		{// キャンセル可能 && 移動中

		 //TODO: 投げの余白キャンセルとか用 ToggleFinishは必要(モーション出来たら)
		
		}
	}
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
