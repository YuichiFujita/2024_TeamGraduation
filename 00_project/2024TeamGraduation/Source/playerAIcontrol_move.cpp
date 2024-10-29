//=============================================================================
// 
//  プレイヤーAIコントロール_移動処理 [playerAIcontrol_move.cpp]
//  Author : 藤田勇一
// 
//=============================================================================
#include "playerAIcontrol_move.h"
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
	const float MULTIPLIY_DASH = 1.5f;		// ダッシュの倍率
	const float INTERVAL_INPUT = (2.0f / 60.0f);	// 入力
	const float TIME_INTERVAL = 0.3f;	// ダッシュ猶予
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControlMove::CPlayerAIControlMove()
{

}

//==========================================================================
// 通常移動
//==========================================================================
void CPlayerAIControlMove::Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
		// ブリンク操作
		//--------------------------
		if (action != CPlayer::EAction::ACTION_BLINK)
		{
			Blink(player, fDeltaTime, fDeltaRate, fSlowRate);
		}

		// 歩き
		Walk(player, fDeltaTime, fDeltaRate, fSlowRate);

		Dash(player, fDeltaTime, fDeltaRate, fSlowRate);

		// 移動中にする
		motionFrag.bMove = true;


		// ジャンプ状況取得
		bool bJump = player->IsJump();

		if (motionFrag.bMove &&
			!bJump)
		{// キャンセル可能 && 移動中

			// モーション終了判定
			pMotion->ToggleFinish(true);

			// 移動モーション
			if (player->IsDash())
			{
				player->SetMotion(CPlayer::EMotion::MOTION_RUN);
			}
			else
			{
				player->SetMotion(CPlayer::EMotion::MOTION_WALK);
			}
		}

	}

	// モーションフラグ設定
	player->SetMotionFrag(motionFrag);
}

//==========================================================================
// ダッシュ
//==========================================================================
void CPlayerAIControlMove::Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
	int* nCntTrigger = GetCntTrigger();						//トリガーのカウント取得
	CPlayer::EDashAngle HoldDashAngle = GetHoldDashAngle();	//ダッシュ方向取得
	float fInputInterval = GetInputInterval();				//入力の受け付け猶予取得
	float fTriggerInterval = GetTriggerInterval();			//トリガーのインターバル取得
	bool bDash = IsBlink();									//走るフラグ取得

	if (fInputInterval <= 0.0f)
	{// 猶予受け付け終了中

		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
			(pPad->GetLStickTrigger(0, CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y > 0 ||
				pKey->GetTrigger(DIK_W)))
		{// 上
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				pPad->GetStickMoveL(0).x < 0 ||
				pKey->GetPress(DIK_A))
			{// 左上
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				pPad->GetStickMoveL(0).x > 0 ||
				pKey->GetPress(DIK_D))
			{// 右上
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
			}
			else
			{// 上
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_UP;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
			(pPad->GetLStickTrigger(0, CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y < 0 ||
				pKey->GetTrigger(DIK_S)))
		{// 下
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				pPad->GetStickMoveL(0).x < 0 ||
				pKey->GetPress(DIK_A))
			{// 左下
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTDW;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				pPad->GetStickMoveL(0).x > 0 ||
				pKey->GetPress(DIK_D))
			{// 右下
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
			}
			else
			{// 下
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_DOWN;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
			(pPad->GetLStickTrigger(0, CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x > 0 ||
				pKey->GetTrigger(DIK_D)))
		{// 右
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				pPad->GetStickMoveL(0).y < 0 ||
				pKey->GetPress(DIK_W))
			{// 右上
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				pPad->GetStickMoveL(0).y > 0 ||
				pKey->GetPress(DIK_S))
			{// 右下
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
			}
			else
			{// 右
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHT;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
			(pPad->GetLStickTrigger(0, CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x < 0 ||
				pKey->GetTrigger(DIK_A)))
		{// 左
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				pPad->GetStickMoveL(0).y < 0 ||
				pKey->GetPress(DIK_W))
			{// 左上
				HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				pPad->GetStickMoveL(0).y > 0 ||
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

	if (fInputInterval > 0.0f)
	{// 受け付け猶予中

		// 次の入力判定
		bool bNextInput = false;
		switch (HoldDashAngle)
		{
		case CPlayer::EDashAngle::ANGLE_LEFT:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				(pPad->GetLStickTrigger(0, CInputGamepad::STICK_Y) && pPad->IsTipStickL(0, CInputGamepad::STICK_AXIS::STICK_Y)))
			{// 上
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				(pPad->GetLStickTrigger(0, CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y > 0))
			{// 下
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			break;

		case CPlayer::EDashAngle::ANGLE_RIGHT:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				(pPad->GetLStickTrigger(0, CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y < 0))
			{// 上
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				(pPad->GetLStickTrigger(0, CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y > 0))
			{// 下
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_RIGHTDW);
				bNextInput = true;
			}
			break;

		case CPlayer::EDashAngle::ANGLE_UP:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				(pPad->GetLStickTrigger(0, CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x < 0))
			{// 左
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				(pPad->GetLStickTrigger(0, CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x > 0))
			{// 右
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			break;

		case CPlayer::EDashAngle::ANGLE_DOWN:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				(pPad->GetLStickTrigger(0, CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x < 0))
			{// 左
				info = Trigger(player, CPlayer::EDashAngle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				(pPad->GetLStickTrigger(0, CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x > 0))
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

	// 受け付け猶予減算
	float oldTime = fInputInterval;
	fInputInterval -= fDeltaTime * fDeltaRate * fSlowRate;
	fInputInterval = UtilFunc::Transformation::Clamp(fInputInterval, 0.0f, INTERVAL_INPUT);

	if (oldTime > 0.0f &&
		fInputInterval <= 0.0f)
	{// 前回まだ猶予中 && 今は終了
		info = Trigger(player, HoldDashAngle);
	}

	// ダッシュする
	if (info.bDash && !bDash)
	{
		MyLib::Vector3 move = player->GetMove();
		float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// 向き
		move.x += sinf((D3DX_PI * 0.0f) + division * info.eAngle + Camerarot.y) * 15.0f;
		move.z += cosf((D3DX_PI * 0.0f) + division * info.eAngle + Camerarot.y) * 15.0f;

		// 移動量設定
		player->SetMove(move);

		// トリガーのカウントリセット
		memset(nCntTrigger, 0, sizeof(nCntTrigger));

		bDash = true;
	}

	// トリガーの猶予減らしていく
	fTriggerInterval -= fDeltaTime * fDeltaRate * fSlowRate;
	if (fTriggerInterval <= 0.0f)
	{
		// トリガーのカウントリセット
		memset(nCntTrigger, 0, sizeof(nCntTrigger));
	}

	//コントロール系
	SetCntTrigger(nCntTrigger);				//トリガーのカウント設定
	SetHoldDashAngle(HoldDashAngle);		//ダッシュ方向設定
	SetInputInterval(fInputInterval);		//入力の受け付け猶予設定
	SetTriggerInterval(fTriggerInterval);	//トリガーのインターバル設定
	SetBlink(bDash);							//走るフラグ設定
}

//==========================================================================
// 走り
//==========================================================================
void CPlayerAIControlMove::Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	bool bBlink = IsBlink();	//走るフラグ取得
	if (!bBlink)
	{
		return;
	}

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	bool bUP = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) &&
		!pKey->GetPress(DIK_W);

	bool bDown = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) &&
		!pKey->GetPress(DIK_S);

	bool bRight = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) &&
		!pKey->GetPress(DIK_D);

	bool bLeft = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) &&
		!pKey->GetPress(DIK_A);

	// 左スティック判定
	bool bStick = pPad->IsTipStickL(0, CInputGamepad::STICK_AXIS::STICK_X) && pPad->IsTipStickL(0, CInputGamepad::STICK_AXIS::STICK_Y);

	if (bUP && bDown && bRight && bLeft && !bStick)
	{
		bBlink = false;
	}

	SetBlink(bBlink);	//走るフラグ設定

	ImGui::Checkbox("UP", &bUP);
	ImGui::Checkbox("Down", &bDown);
	ImGui::Checkbox("Right", &bRight);
	ImGui::Checkbox("Left", &bLeft);
}

//==========================================================================
// ウォーク
//==========================================================================
void CPlayerAIControlMove::Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	bool bDash = IsBlink();	//走るフラグ

	// 現在の入力方向
	CPlayer::EDashAngle* pInputAngle = GetInputAngle();
	if (pInputAngle != nullptr) delete pInputAngle;
	pInputAngle = nullptr;
	SetInputAngle(pInputAngle);

	CPlayer::EDashAngle eAngle;
	bool bInput = false;

	if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
		pPad->GetStickMoveL(0).y > 0 ||
		pKey->GetPress(DIK_W))
	{// 上
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
			pPad->GetStickMoveL(0).x < 0 ||
			pKey->GetPress(DIK_A))
		{// 左上
			eAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
			pPad->GetStickMoveL(0).x > 0 ||
			pKey->GetPress(DIK_D))
		{// 右上
			eAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
		}
		else
		{// 上
			eAngle = CPlayer::EDashAngle::ANGLE_UP;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
		pPad->GetStickMoveL(0).y < 0 ||
		pKey->GetPress(DIK_S))
	{// 下
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
			pPad->GetStickMoveL(0).x < 0 ||
			pKey->GetPress(DIK_A))
		{// 左下
			eAngle = CPlayer::EDashAngle::ANGLE_LEFTDW;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
			pPad->GetStickMoveL(0).x > 0 ||
			pKey->GetPress(DIK_D))
		{// 右下
			eAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
		}
		else
		{// 下
			eAngle = CPlayer::EDashAngle::ANGLE_DOWN;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
		pPad->GetStickMoveL(0).x > 0 ||
		pKey->GetPress(DIK_D))
	{// 右
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
			pPad->GetStickMoveL(0).y < 0 ||
			pKey->GetPress(DIK_W))
		{// 右上
			eAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
			pPad->GetStickMoveL(0).y > 0 ||
			pKey->GetPress(DIK_S))
		{// 右下
			eAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
		}
		else
		{// 右
			eAngle = CPlayer::EDashAngle::ANGLE_RIGHT;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
		pPad->GetStickMoveL(0).x < 0 ||
		pKey->GetPress(DIK_A))
	{// 左
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
			pPad->GetStickMoveL(0).y < 0 ||
			pKey->GetPress(DIK_W))
		{// 左上
			eAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
			pPad->GetStickMoveL(0).y > 0 ||
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
		return;
	}

	// 移動量取得
	float fMove = player->GetParameter().fVelocityNormal;
	if (bDash)
	{// ダッシュは変更
		fMove = player->GetParameter().fVelocityDash;
	}
	fMove *= fDeltaRate;
	fMove *= fSlowRate;

	MyLib::Vector3 move = player->GetMove();
	float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// 向き
	move.x += sinf((D3DX_PI * 0.0f) + division * eAngle + Camerarot.y) * fMove;
	move.z += cosf((D3DX_PI * 0.0f) + division * eAngle + Camerarot.y) * fMove;

	// 移動量設定
	player->SetMove(move);

	// 向き設定
	player->SetRotDest(eAngle * division + D3DX_PI + Camerarot.y);

	// 現在の入力方向設定
	pInputAngle = new CPlayer::EDashAngle;
	*pInputAngle = eAngle;
	SetInputAngle(pInputAngle); 
}

//==========================================================================
// トリガー
//==========================================================================
CPlayer::SDashInfo CPlayerAIControlMove::Trigger(CPlayer* player, CPlayer::EDashAngle eAngle)
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
	SetCntTrigger(nCntTrigger);				//トリガーのカウント
	SetTriggerInterval(fTriggerInterval);	//トリガーのインターバル

	return info;
}