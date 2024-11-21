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
	const float INPUT_COUNTER = (60.0f / 60.0f);	// 入力カウンター
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
	m_fCrabMoveEasingTime = 0.0f;				// 現在のカニ歩き移動補正値
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerControlMove::~CPlayerControlMove()
{
	SAFE_DELETE(m_pInputAngle);
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

		motionFrag = player->GetMotionFrag();
	
		if (motionFrag.bMove &&
			pMotion->IsGetCancelable() && 
			!bJump)
		{// キャンセル可能 && 移動中

			// モーションキャンセル
			pMotion->ToggleFinish(true);
			
			//TODO: 投げの余白キャンセルとか用 ToggleFinishは必要(モーション出来たら)
		}
	}

	// 入力方向
	UpdateInputAngle(player, fDeltaTime, fDeltaRate, fSlowRate);

#if 1
	// カニ歩き判定
	if (player->GetBase()->IsCrab())
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
	MyLib::Vector3 pos = player->GetPosition();

	// コートサイズ取得
	MyLib::Vector3 sizeCourt = CGameManager::GetInstance()->GetCourtSize();
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	// カニ歩きじゃないときは抜ける
	if (!player->GetBase()->IsCrab()) return;

	// ボールの位置
	MyLib::Vector3 posBall = pBall->GetPosition();

	// 目標の向き
	float fAngle = pos.AngleXZ(posBall);
	UtilFunc::Transformation::RotNormalize(fAngle);

	// プレイヤーの目標の向き設定
	player->SetRotDest(fAngle);
}

//==========================================================================
// 現在の入力方向更新
//==========================================================================
void CPlayerControlMove::UpdateInputAngle(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// プレイヤー番号取得
	int playerIdx = player->GetMyPlayerIdx();

	CPlayer::EDashAngle eAngle = CPlayer::EDashAngle::ANGLE_UP;
	bool bInput = false;

	if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
		pPad->GetStickMoveL(playerIdx).y > 0 ||
		pKey->GetPress(DIK_W))
	{// 上
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
			pPad->GetStickMoveL(playerIdx).x < 0 ||
			pKey->GetPress(DIK_A))
		{// 左上
			eAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
			pPad->GetStickMoveL(playerIdx).x > 0 ||
			pKey->GetPress(DIK_D))
		{// 右上
			eAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
		}
		else
		{// 上
			eAngle = CPlayer::EDashAngle::ANGLE_UP;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, playerIdx) ||
		pPad->GetStickMoveL(playerIdx).y < 0 ||
		pKey->GetPress(DIK_S))
	{// 下
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
			pPad->GetStickMoveL(playerIdx).x < 0 ||
			pKey->GetPress(DIK_A))
		{// 左下
			eAngle = CPlayer::EDashAngle::ANGLE_LEFTDW;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
			pPad->GetStickMoveL(playerIdx).x > 0 ||
			pKey->GetPress(DIK_D))
		{// 右下
			eAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
		}
		else
		{// 下
			eAngle = CPlayer::EDashAngle::ANGLE_DOWN;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
		pPad->GetStickMoveL(playerIdx).x > 0 ||
		pKey->GetPress(DIK_D))
	{// 右
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
			pPad->GetStickMoveL(playerIdx).y < 0 ||
			pKey->GetPress(DIK_W))
		{// 右上
			eAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, playerIdx) ||
			pPad->GetStickMoveL(playerIdx).y > 0 ||
			pKey->GetPress(DIK_S))
		{// 右下
			eAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
		}
		else
		{// 右
			eAngle = CPlayer::EDashAngle::ANGLE_RIGHT;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
		pPad->GetStickMoveL(playerIdx).x < 0 ||
		pKey->GetPress(DIK_A))
	{// 左
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
			pPad->GetStickMoveL(playerIdx).y < 0 ||
			pKey->GetPress(DIK_W))
		{// 左上
			eAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, playerIdx) ||
			pPad->GetStickMoveL(playerIdx).y > 0 ||
			pKey->GetPress(DIK_S))
		{// 左下
			eAngle = CPlayer::EDashAngle::ANGLE_LEFTDW;
		}
		else
		{// 左
			eAngle = CPlayer::EDashAngle::ANGLE_LEFT;
		}
	}

	CPlayer::EDashAngle eInputOld = CPlayer::EDashAngle::ANGLE_UP;
	bool bOld = false;

	// 現在の入力方向設定
	if (m_pInputAngle != nullptr)
	{
		eInputOld = *m_pInputAngle;
		bOld = true;

		delete m_pInputAngle;
		m_pInputAngle = nullptr;
	}

	if (bInput)
	{
		// 現在の入力方向設定
		if (m_pInputAngle == nullptr)
		{
			m_pInputAngle = DEBUG_NEW CPlayer::EDashAngle;
			MyAssert::CustomAssert(m_pInputAngle != nullptr, "なんでnew出来てねぇんだよ");
		}
		*m_pInputAngle = eAngle;

		if (eInputOld != eAngle || !bOld)
		{// 前回無入力 or 違う入力

			// 入力方向カウンター設定
			SetInputAngleCtr(INPUT_COUNTER);
		}
	}

	// 現在の入力方向カウンター
	if (m_fInputAngleCtr >= 0.0f)
	{
		m_fInputAngleCtr -= fDeltaTime * fSlowRate;
		m_fInputAngleCtr = UtilFunc::Transformation::Clamp(m_fInputAngleCtr, 0.0f, INPUT_COUNTER);
	}
}
