//=============================================================================
// 
//  プレイヤーコントロール処理 [playerUsercontrol_move.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerUsercontrol_move.h"
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
	const float RATIO_STICKBLINK = 0.75f;	// スティックブリンク時の割合
	const float TIME_INTERVAL = 0.3f;	// ダッシュ猶予
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerUserControlMove::CPlayerUserControlMove()
{

}

//==========================================================================
// 通常移動
//==========================================================================
void CPlayerUserControlMove::Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
		if (action != CPlayer::EAction::ACTION_BLINK &&
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

		if (player->GetMotionFrag().bMove && 
			pMotion->IsGetCancelable() &&
			!bJump)
		{// キャンセル可能 && 移動中

			// モーションキャンセル
			pMotion->ToggleFinish(true);

		 //TODO: 投げの余白キャンセルとか用 ToggleFinishは必要(モーション出来たら)
		
		}
	}
}

//==========================================================================
// ブリンク
//==========================================================================
void CPlayerUserControlMove::Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 入力フラグ
	bool bInput = false;

	// ダッシュ判定
	CPlayer::SDashInfo info;

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	//コントロール系取得
	int* nCntTrigger = GetCntTrigger();						// トリガーのカウント取得
	CPlayer::EDashAngle HoldDashAngle = GetHoldDashAngle();	// ダッシュ方向取得
	float fInputInterval = GetInputInterval();				// 入力の受け付け猶予取得
	bool bDash = IsBlink();									// 走るフラグ取得

	// プレイヤー番号取得
	int playerIdx = player->GetMyPlayerIdx();


	// Lスティックの割合
	MyLib::Vector3 stickRatio = pPad->GetStickPositionRatioL(playerIdx);

	ImGui::Text("stickRatio.x: %.2f", stickRatio.x);
	ImGui::Text("stickRatio.y: %.2f", stickRatio.y);

	if (fInputInterval <= 0.0f)
	{// 猶予受け付け終了中


		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
			(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_Y) && stickRatio.y >= RATIO_STICKBLINK) ||
				pKey->GetTrigger(DIK_W))
		{// 上
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
				stickRatio.x <= -RATIO_STICKBLINK ||
				pKey->GetPress(DIK_A))
			{// 左上
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
				stickRatio.x >= RATIO_STICKBLINK ||
				pKey->GetPress(DIK_D))
			{// 右上
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
			}
			else
			{// 上
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_UP;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, playerIdx) ||
			(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_Y) && stickRatio.y <= -RATIO_STICKBLINK) ||
				pKey->GetTrigger(DIK_S))
		{// 下
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
				stickRatio.x <= -RATIO_STICKBLINK ||
				pKey->GetPress(DIK_A))
			{// 左下
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTDW;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
				stickRatio.x >= RATIO_STICKBLINK ||
				pKey->GetPress(DIK_D))
			{// 右下
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
			}
			else
			{// 下
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_DOWN;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
			(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_X) && stickRatio.x >= RATIO_STICKBLINK) ||
				pKey->GetTrigger(DIK_D))
		{// 右
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
				stickRatio.y >= RATIO_STICKBLINK ||
				pKey->GetPress(DIK_W))
			{// 右上
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				stickRatio.y <= -RATIO_STICKBLINK ||
				pKey->GetPress(DIK_S))
			{// 右下
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
			}
			else
			{// 右
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHT;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
			(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_X) && stickRatio.x <= -RATIO_STICKBLINK) ||
				pKey->GetTrigger(DIK_A))
		{// 左
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
				stickRatio.y >= RATIO_STICKBLINK ||
				pKey->GetPress(DIK_W))
			{// 左上
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, playerIdx) ||
				stickRatio.y <= -RATIO_STICKBLINK ||
				pKey->GetPress(DIK_S))
			{// 左下
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTDW;
			}
			else
			{// 左
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFT;
			}
		}
	}


	if (bInput)
	{// 入力された

		// 入力のインターバル
		fInputInterval = INTERVAL_INPUT;
	}

	if (fInputInterval > 0.0f && fInputInterval != INTERVAL_INPUT)
	{// 受け付け猶予中

		// 次の入力判定
		bool bNextInput = false;
		switch (HoldDashAngle)
		{
		case CPlayer::EDashAngle::ANGLE_LEFT:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
				(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_Y) && stickRatio.y >= RATIO_STICKBLINK))
			{// 上
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, playerIdx) ||
				(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_Y) && stickRatio.y <= -RATIO_STICKBLINK))
			{// 下
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			break;

		case CPlayer::EDashAngle::ANGLE_RIGHT:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, playerIdx) ||
				(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_Y) && stickRatio.y >= RATIO_STICKBLINK))
			{// 上
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, playerIdx) ||
				(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_Y) && stickRatio.y <= -RATIO_STICKBLINK))
			{// 下
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_RIGHTDW);
				bNextInput = true;
			}
			break;

		case CPlayer::EDashAngle::ANGLE_UP:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
				(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_X) && stickRatio.x <= -RATIO_STICKBLINK))
			{// 左
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
				(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_X) && stickRatio.x >= RATIO_STICKBLINK))
			{// 右
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			break;

		case CPlayer::EDashAngle::ANGLE_DOWN:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, playerIdx) ||
				(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_X) && stickRatio.x <= -RATIO_STICKBLINK))
			{// 左
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, playerIdx) ||
				(pPad->GetLStickTrigger(playerIdx, CInputGamepad::STICK_X) && stickRatio.x >= RATIO_STICKBLINK))
			{// 右
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_RIGHTDW);
				bNextInput = true;
			}
			break;

		default:
			info = Trigger(player, HoldDashAngle);
			bNextInput = true;
			break;
		}

		// 受け付けされてるよ
		if (bNextInput)
		{
			// 受け付け猶予終了
			fInputInterval = 0.0f;
		}
	}
	SetHoldDashAngle(HoldDashAngle);		// ダッシュ方向設定

	// 受け付け猶予減算
	float oldTime = fInputInterval;
	fInputInterval -= fDeltaTime * fDeltaRate * fSlowRate;
	fInputInterval = UtilFunc::Transformation::Clamp(fInputInterval, 0.0f, INTERVAL_INPUT);

	// 入力の受け付け猶予設定
	SetInputInterval(fInputInterval);

	if (oldTime > 0.0f &&
		fInputInterval <= 0.0f)
	{// 前回まだ猶予中 && 今は終了
		info = Trigger(player, HoldDashAngle);
	}

	// ダッシュする
	if (info.bDash && !bDash)
	{
		MyLib::Vector3 move = player->GetMove();
		float velocityBlink = player->GetParameter().fVelocityBlink;
		float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// 向き
		MyLib::Vector3 rot = player->GetRotation();

		move.x += sinf(division * info.eAngle + Camerarot.y) * velocityBlink;
		move.z += cosf(division * info.eAngle + Camerarot.y) * velocityBlink;

		// 移動量設定
		player->SetMove(move);

		// 向き設定
		player->SetRotDest(info.eAngle * division + D3DX_PI + Camerarot.y);

		// トリガーのカウントリセット
		memset(nCntTrigger, 0, sizeof(nCntTrigger) * CPlayer::EDashAngle::ANGLE_MAX);

		// トリガーのカウント設定
		SetCntTrigger(nCntTrigger);

		// ダッシュフラグ
		bDash = true;

		// モーション設定
		player->SetMotion(CPlayer::EMotion::MOTION_BLINK);
	}

	// トリガーのインターバル取得
	float fTriggerInterval = GetTriggerInterval();

	// トリガーの猶予減らしていく
	fTriggerInterval -= fDeltaTime * fDeltaRate * fSlowRate;
	if (fTriggerInterval <= 0.0f)
	{
		// トリガーのカウントリセット
		memset(nCntTrigger, 0, sizeof(nCntTrigger) * CPlayer::EDashAngle::ANGLE_MAX);

		// トリガーのカウント設定
		SetCntTrigger(nCntTrigger);
	}

	// トリガーのインターバル設定
	SetTriggerInterval(fTriggerInterval);

	// コントロール系
	SetBlink(bDash);							// 走るフラグ設定
}

//==========================================================================
// 走り
//==========================================================================
void CPlayerUserControlMove::Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	bool bDash = IsBlink();	//走るフラグ取得
	
	if (!bDash)
	{
		return;
	}

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// モーションフラグ取得
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	// プレイヤー番号取得
	int playerIdx = player->GetMyPlayerIdx();

	// 全ての方向入力チェック
	bool bUP = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) &&
		!pKey->GetPress(DIK_W);

	bool bDown = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) &&
		!pKey->GetPress(DIK_S);

	bool bRight = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) &&
		!pKey->GetPress(DIK_D);

	bool bLeft = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) &&
		!pKey->GetPress(DIK_A);

	// 左スティック判定
	bool bStick = pPad->IsTipStickL(playerIdx, CInputGamepad::STICK_AXIS::STICK_X) || pPad->IsTipStickL(playerIdx, CInputGamepad::STICK_AXIS::STICK_Y);

	if (bUP && bDown && bRight && bLeft && !bStick)
	{// ダッシュ解除
		bDash = false;

		if (player->GetMotion()->GetType() == CPlayer::EMotion::MOTION_RUN)
		{// ダッシュからは派生
			player->SetMotion(CPlayer::EMotion::MOTION_GRIP_FRONT);
		}
	}
	else
	{
		// ダッシュ継続
		motionFrag.bMove = true;
		player->SetEnableDash(true);
	}

	// モーションフラグ設定
	player->SetMotionFrag(motionFrag);
	
	SetBlink(bDash);	//走るフラグ設定
}

//==========================================================================
// ウォーク
//==========================================================================
void CPlayerUserControlMove::Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// モーションフラグ取得
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	bool bDash = IsBlink();	//走るフラグ

	CPlayer::EDashAngle eAngle;
	bool bInput = false;


	// プレイヤー番号取得
	int playerIdx = player->GetMyPlayerIdx();

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

	if (!bInput)
	{
		// 移動中にする
		motionFrag.bMove = false;

		// モーションフラグ設定
		player->SetMotionFrag(motionFrag);
		return;
	}
	
	// 移動中にする
	motionFrag.bMove = true;

	// 移動量取得
	float fMove = player->GetParameter().fVelocityNormal;
	if (bDash)
	{// ダッシュは変更
		fMove = player->GetParameter().fVelocityDash;
	}
	fMove *= fDeltaRate;
	fMove *= fSlowRate;

	// 移動量更新
	MyLib::Vector3 move = player->GetMove();
	MyLib::Vector3 rot = player->GetRotation();

	float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// 向き
	move.x += sinf(rot.y + (D3DX_PI * 1.0f)) * fMove;
	move.z += cosf(rot.y + (D3DX_PI * 1.0f)) * fMove;

	// 移動量設定
	player->SetMove(move);

	// 向き設定
	player->SetRotDest(eAngle * division + D3DX_PI + Camerarot.y);

	// モーションフラグ設定
	player->SetMotionFrag(motionFrag);

	SetBlink(bDash);	//走るフラグ設定
}

//==========================================================================
// トリガー
//==========================================================================
CPlayer::SDashInfo CPlayerUserControlMove::Trigger(CPlayer* player, CPlayer::EDashAngle eAngle)
{
	CPlayer::SDashInfo info;
	info.bDash = false;
	
	float fTriggerInterval = GetTriggerInterval();	//トリガーのインターバル
	int* nCntTrigger = GetCntTrigger();				//トリガーのカウント

	if (nCntTrigger[eAngle] == 1)
	{// ダッシュ
		info.bDash = true;
		info.eAngle = eAngle;

		// アクションパターン変更
		player->GetActionPattern()->SetAction(CPlayer::EAction::ACTION_BLINK);
	}

	// トリガーのインターバルリセット
	fTriggerInterval = TIME_INTERVAL;

	// トリガーのカウント
	nCntTrigger[eAngle] = (nCntTrigger[eAngle] + 1) % 2;

	//コントロール系
	SetCntTrigger(nCntTrigger);				// トリガーのカウント
	SetTriggerInterval(fTriggerInterval);	// トリガーのインターバル

	return info;
}