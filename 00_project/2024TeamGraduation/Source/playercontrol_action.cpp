//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playercontrol.h"
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
CPlayerControlMove::CPlayerControlMove()
{
	memset(m_nCntTrigger, 0, sizeof(m_nCntTrigger));	// トリガーのカウント
	m_HoldDashAngle = CPlayer::DashAngle::ANGLE_UP;		// 保持してるダッシュの移動方向
	m_fInputInterval = 0.0f;							// 入力の受け付け猶予
	m_fTriggerInterval = 0.0f;							// トリガーのインターバル
}

//==========================================================================
// 通常移動
//==========================================================================
void CPlayerControlMove::Move(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// ダッシュ判定
	bool bDash = false;

	// 移動量取得
	float fMove = player->GetVelocity();
	if (bDash){
		// ダッシュ倍率掛ける
		fMove *= MULTIPLIY_DASH;
	}


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
	CPlayer::STATE state = player->GetState();

	// アクション取得
	CPlayerAction* pPlayerAction = player->GetActionPattern();
	if (pPlayerAction == nullptr) return;
	CPlayer::Action action = pPlayerAction->GetAction();


	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		player->IsPossibleMove())
	{// 移動可能モーションの時

		//--------------------------
		// ブリンク操作
		//--------------------------
		if (action != CPlayer::Action::ACTION_BLINK)
		{
			Blink(player, fDeltaTime, fDeltaRate, fSlowRate);
		}

		// 移動中にする
		motionFrag.bMove = true;

		if (pPad->IsTipStick())
		{// 左スティックが倒れてる場合

		}

		// ジャンプ状況取得
		bool bJump = player->IsJump();

		if (motionFrag.bMove &&
			!bJump)
		{// キャンセル可能 && 移動中

			// モーション終了判定
			pMotion->ToggleFinish(true);

			// 移動モーション
			if (bDash)
			{
				//pMotion->Set(CPlayer::MOTION::MOTION_DASH);
			}
			else
			{
				pMotion->Set(CPlayer::MOTION::MOTION_WALK);
			}
		}


		if (!bJump &&
			(pKey->GetTrigger(DIK_SPACE) || pPad->GetTrigger(CInputGamepad::BUTTON_A, player->GetMyPlayerIdx())))
		{// ジャンプ

			//bJump = true;
			//motionFrag.bJump = true;
			//move.y += 17.0f;

			//pMotion->Set(CPlayer::MOTION::MOTION_JUMP);

			//// サウンド再生
			//CSound::GetInstance()->PlaySound(CSound::LABEL_SE_JUMP);
		}

		// ジャンプ判定設定
		player->SetEnableJump(bJump);

	}
	else if (
		pMotion->IsGetMove(nMotionType) == 0 &&	// 移動可能なモーションか取得
		player->IsPossibleMove())
	{
		if (pKey->GetPress(DIK_A))
		{//←キーが押された,左移動

			if (pKey->GetPress(DIK_W))
			{//A+W,左上移動
				fRotDest = D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pKey->GetPress(DIK_S))
			{//A+S,左下移動
				fRotDest = D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//A,左移動
				fRotDest = D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pKey->GetPress(DIK_D))
		{//Dキーが押された,右移動

			if (pKey->GetPress(DIK_W))
			{//D+W,右上移動
				fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pKey->GetPress(DIK_S))
			{//D+S,右下移動
				fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//D,右移動
				fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pKey->GetPress(DIK_W))
		{//Wが押された、上移動
			fRotDest = D3DX_PI * 1.0f + Camerarot.y;
		}
		else if (pKey->GetPress(DIK_S))
		{//Sが押された、下移動
			fRotDest = D3DX_PI * 0.0f + Camerarot.y;
		}

		if (pPad->IsTipStick())
		{// 左スティックが倒れてる場合
			fRotDest = pPad->GetStickRotL(player->GetMyPlayerIdx());
		}
	}

	// モーションフラグ設定
	player->SetMotionFrag(motionFrag);

}

//==========================================================================
// ダッシュ
//==========================================================================
void CPlayerControlMove::Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 入力フラグ
	bool bInput = false;

	// ダッシュ判定
	CPlayer::SDashInfo info;

	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (m_fInputInterval <= 0.0f)
	{// 猶予受け付け終了中

		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y > 0))
		{// 上
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				pPad->GetStickMoveL(0).x < 0)
			{// 左上
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				pPad->GetStickMoveL(0).x > 0)
			{// 右上
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTUP;
			}
			else
			{// 上
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_UP;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y < 0))
		{// 下
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				pPad->GetStickMoveL(0).x < 0)
			{// 左下
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTDW;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				pPad->GetStickMoveL(0).x > 0)
			{// 右下
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTDW;
			}
			else
			{// 下
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_DOWN;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x > 0))
		{// 右
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				pPad->GetStickMoveL(0).y < 0)
			{// 右上
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				pPad->GetStickMoveL(0).y > 0)
			{// 右下
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTDW;
			}
			else
			{// 右
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHT;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x < 0))
		{// 左
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				pPad->GetStickMoveL(0).y < 0)
			{// 左上
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				pPad->GetStickMoveL(0).y > 0)
			{// 左下
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTDW;
			}
			else
			{// 左
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFT;
			}
		}
	}


	if (bInput)
	{// 入力された

		// 入力のインターバル
		m_fInputInterval = INTERVAL_INPUT;
	}

	if (m_fInputInterval > 0.0f)
	{// 受け付け猶予中

		// 次の入力判定
		bool bNextInput = false;
		switch (m_HoldDashAngle)
		{
		case CPlayer::DashAngle::ANGLE_LEFT:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y < 0))
			{// 上
				info = Trigger(player, CPlayer::DashAngle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y > 0))
			{// 下
				info = Trigger(player, CPlayer::DashAngle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			break;

		case CPlayer::DashAngle::ANGLE_RIGHT:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y < 0))
			{// 上
				info = Trigger(player, CPlayer::DashAngle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(0).y > 0))
			{// 下
				info = Trigger(player, CPlayer::DashAngle::ANGLE_RIGHTDW);
				bNextInput = true;
			}
			break;

		case CPlayer::DashAngle::ANGLE_UP:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x < 0))
			{// 左
				info = Trigger(player, CPlayer::DashAngle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x > 0))
			{// 右
				info = Trigger(player, CPlayer::DashAngle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			break;

		case CPlayer::DashAngle::ANGLE_DOWN:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x < 0))
			{// 左
				info = Trigger(player, CPlayer::DashAngle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(0).x > 0))
			{// 右
				info = Trigger(player, CPlayer::DashAngle::ANGLE_RIGHTDW);
				bNextInput = true;
			}
			break;

		default:
			info = Trigger(player, m_HoldDashAngle);
			bNextInput = true;
			break;
		}

		// 受け付けされてるよ
		if (bNextInput)
		{
			// 受け付け猶予終了
			m_fInputInterval = 0.0f;
		}
	}

	// 受け付け猶予減算
	float oldTime = m_fInputInterval;
	m_fInputInterval -= fDeltaTime * fDeltaRate * fSlowRate;
	m_fInputInterval = UtilFunc::Transformation::Clamp(m_fInputInterval, 0.0f, INTERVAL_INPUT);

	if (oldTime > 0.0f &&
		m_fInputInterval <= 0.0f)
	{// 前回まだ猶予中 && 今は終了
		info = Trigger(player, m_HoldDashAngle);
	}

	// ダッシュする
	if (info.bDash)
	{
		MyLib::Vector3 move;
		float division = (D3DX_PI * 2.0f) / CPlayer::DashAngle::ANGLE_MAX;	// 向き
		move.x += sinf((D3DX_PI * 0.0f) + division * info.angle) * 25.0f;
		move.z += cosf((D3DX_PI * 0.0f) + division * info.angle) * 25.0f;

		// 移動量設定
		player->SetMove(move);

		// トリガーのカウントリセット
		memset(m_nCntTrigger, 0, sizeof(m_nCntTrigger));
	}

	// トリガーの猶予減らしていく
	m_fTriggerInterval -= fDeltaTime * fDeltaRate * fSlowRate;
	if (m_fTriggerInterval <= 0.0f)
	{
		// トリガーのカウントリセット
		memset(m_nCntTrigger, 0, sizeof(m_nCntTrigger));
	}
}

//==========================================================================
// トリガー
//==========================================================================
CPlayer::SDashInfo CPlayerControlMove::Trigger(CPlayer* player, CPlayer::DashAngle angle)
{
	CPlayer::SDashInfo info;
	info.bDash = false;

	if (m_nCntTrigger[angle] == 1)
	{// ダッシュ
		info.bDash = true;
		info.angle = angle;

		// アクションパターン変更
		player->GetActionPattern()->SetAction(CPlayer::Action::ACTION_BLINK);
	}

	// トリガーのインターバルリセット
	m_fTriggerInterval = TIME_INTERVAL;

	// トリガーのカウント
	m_nCntTrigger[angle] = (m_nCntTrigger[angle] + 1) % 2;

	return info;
}