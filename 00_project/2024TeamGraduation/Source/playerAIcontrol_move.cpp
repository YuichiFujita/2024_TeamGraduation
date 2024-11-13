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
	m_bBlink = false;
	m_bDash = false;
	m_bWalk = false;
	m_fClabDirection = 0.0f;
}

//==========================================================================
// ダッシュ
//==========================================================================
void CPlayerAIControlMove::Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!m_bBlink) { return; }

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

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	SetBlink(bBlink);	//走るフラグ設定
}

//==========================================================================
// ウォーク
//==========================================================================
void CPlayerAIControlMove::Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{	
	// モーションフラグ取得
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	if (!m_bWalk) {
		motionFrag.bMove = false;
		return; 
	}

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	motionFrag.bMove = true;	// モーションフラグ(歩く)

	bool bDash = IsBlink();	//走るフラグ

	// 移動量取得
	float fMove = player->GetParameter().fVelocityNormal;
	if (bDash)
	{// ダッシュは変更
		fMove = player->GetParameter().fVelocityDash;
	}
	fMove *= fDeltaRate;
	fMove *= fSlowRate;

	// 移動量更新
	MyLib::Vector3 move = {};
	move = player->GetMove();

	// 向き取得
	MyLib::Vector3 rot = player->GetRotation();
	
	float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// 向き

	// TODO : 方向に応じてカニ歩き

	if (player->IsCrab())
	{// カニ歩き
		move.x += sinf(m_fClabDirection + (D3DX_PI * 1.0f)) * fMove;
		move.z += cosf(m_fClabDirection + (D3DX_PI * 1.0f)) * fMove;
	}
	else
	{
		move.x += sinf(rot.y + (D3DX_PI * 1.0f)) * fMove;
		move.z += cosf(rot.y + (D3DX_PI * 1.0f)) * fMove;
	}

	// 移動量設定
	player->SetMove(move);

	// モーションフラグ設定
	player->SetMotionFrag(motionFrag);
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