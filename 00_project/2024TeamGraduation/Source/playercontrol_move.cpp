//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol_move.cpp]
//  Author : 相馬靜雅
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
	m_bDash = false;
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
		// 移動操作
		//--------------------------
		if (action != CPlayer::Action::ACTION_BLINK)
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
// ブリンク
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

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	if (m_fInputInterval <= 0.0f)
	{// 猶予受け付け終了中

		if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).y > 0 ||
				pKey->GetTrigger(DIK_W)))
		{// 上
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) ||
				pPad->GetStickMoveL(player->GetMyPlayerIdx()).x < 0 ||
				pKey->GetPress(DIK_A))
			{// 左上
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) ||
				pPad->GetStickMoveL(player->GetMyPlayerIdx()).x > 0 ||
				pKey->GetPress(DIK_D))
			{// 右上
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTUP;
			}
			else
			{// 上
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_UP;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).y < 0 ||
				pKey->GetTrigger(DIK_S)))
		{// 下
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) ||
				pPad->GetStickMoveL(player->GetMyPlayerIdx()).x < 0 ||
				pKey->GetPress(DIK_A))
			{// 左下
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTDW;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) ||
				pPad->GetStickMoveL(player->GetMyPlayerIdx()).x > 0 ||
				pKey->GetPress(DIK_D))
			{// 右下
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTDW;
			}
			else
			{// 下
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_DOWN;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).x > 0 ||
				pKey->GetTrigger(DIK_D)))
		{// 右
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) ||
				pPad->GetStickMoveL(player->GetMyPlayerIdx()).y < 0 ||
				pKey->GetPress(DIK_W))
			{// 右上
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
				pPad->GetStickMoveL(player->GetMyPlayerIdx()).y > 0 ||
				pKey->GetPress(DIK_S))
			{// 右下
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHTDW;
			}
			else
			{// 右
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_RIGHT;
			}
		}
		else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) ||
			(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).x < 0 ||
				pKey->GetTrigger(DIK_A)))
		{// 左
			bInput = true;

			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) ||
				pPad->GetStickMoveL(player->GetMyPlayerIdx()).y < 0 ||
				pKey->GetPress(DIK_W))
			{// 左上
				m_HoldDashAngle = CPlayer::DashAngle::ANGLE_LEFTUP;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) ||
				pPad->GetStickMoveL(player->GetMyPlayerIdx()).y > 0 ||
				pKey->GetPress(DIK_S))
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
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).y < 0))
			{// 上
				info = Trigger(player, CPlayer::DashAngle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).y > 0))
			{// 下
				info = Trigger(player, CPlayer::DashAngle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			break;

		case CPlayer::DashAngle::ANGLE_RIGHT:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).y < 0))
			{// 上
				info = Trigger(player, CPlayer::DashAngle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_Y) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).y > 0))
			{// 下
				info = Trigger(player, CPlayer::DashAngle::ANGLE_RIGHTDW);
				bNextInput = true;
			}
			break;

		case CPlayer::DashAngle::ANGLE_UP:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).x < 0))
			{// 左
				info = Trigger(player, CPlayer::DashAngle::ANGLE_LEFTUP);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).x > 0))
			{// 右
				info = Trigger(player, CPlayer::DashAngle::ANGLE_RIGHTUP);
				bNextInput = true;
			}
			break;

		case CPlayer::DashAngle::ANGLE_DOWN:
			if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).x < 0))
			{// 左
				info = Trigger(player, CPlayer::DashAngle::ANGLE_LEFTDW);
				bNextInput = true;
			}
			else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) ||
				(pPad->GetLStickTrigger(CInputGamepad::STICK_X) && pPad->GetStickMoveL(player->GetMyPlayerIdx()).x > 0))
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
	if (info.bDash && !m_bDash)
	{
		MyLib::Vector3 move = player->GetMove();
		float division = (D3DX_PI * 2.0f) / CPlayer::DashAngle::ANGLE_MAX;	// 向き
		move.x += sinf((D3DX_PI * 0.0f) + division * info.angle + Camerarot.y) * 15.0f;
		move.z += cosf((D3DX_PI * 0.0f) + division * info.angle + Camerarot.y) * 15.0f;

		// 移動量設定
		player->SetMove(move);

		// トリガーのカウントリセット
		memset(m_nCntTrigger, 0, sizeof(m_nCntTrigger));

		// ダッシュフラグ
		m_bDash = true;

		// モーション設定
		player->SetMotion(CPlayer::MOTION::MOTION_BLINK);
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
// 走り
//==========================================================================
void CPlayerControlMove::Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!m_bDash)
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

	// 全ての方向入力チェック
	bool bUP = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) &&
		!pKey->GetPress(DIK_W);

	bool bDown = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) &&
		!pKey->GetPress(DIK_S);

	bool bRight = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) &&
		!pKey->GetPress(DIK_D);

	bool bLeft = !pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) &&
		!pKey->GetPress(DIK_A);

	// スティックの倒れ判定
	bool bStick = pPad->IsTipStick();

	if (bUP && bDown && bRight && bLeft && !bStick)
	{
		m_bDash = false;
	}
	else
	{
		// ダッシュ継続
		motionFrag.bMove = true;
		player->SetEnableDash(true);
	}

	ImGui::Checkbox("UP", &bUP);
	ImGui::Checkbox("Down", &bDown);
	ImGui::Checkbox("Right", &bRight);
	ImGui::Checkbox("Left", &bLeft);

	// モーションフラグ設定
	player->SetMotionFrag(motionFrag);
}

//==========================================================================
// ウォーク
//==========================================================================
void CPlayerControlMove::Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// インプット情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// モーションフラグ取得
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	CPlayer::DashAngle angle;
	bool bInput = false;

	if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) ||
		pPad->GetStickMoveL(player->GetMyPlayerIdx()).y > 0 ||
		pKey->GetPress(DIK_W))
	{// 上
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) ||
			pPad->GetStickMoveL(player->GetMyPlayerIdx()).x < 0 ||
			pKey->GetPress(DIK_A))
		{// 左上
			angle = CPlayer::DashAngle::ANGLE_LEFTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) ||
			pPad->GetStickMoveL(player->GetMyPlayerIdx()).x > 0 ||
			pKey->GetPress(DIK_D))
		{// 右上
			angle = CPlayer::DashAngle::ANGLE_RIGHTUP;
		}
		else
		{// 上
			angle = CPlayer::DashAngle::ANGLE_UP;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) ||
		pPad->GetStickMoveL(player->GetMyPlayerIdx()).y < 0 ||
		pKey->GetPress(DIK_S))
	{// 下
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) ||
			pPad->GetStickMoveL(player->GetMyPlayerIdx()).x < 0 ||
			pKey->GetPress(DIK_A))
		{// 左下
			angle = CPlayer::DashAngle::ANGLE_LEFTDW;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) ||
			pPad->GetStickMoveL(player->GetMyPlayerIdx()).x > 0 ||
			pKey->GetPress(DIK_D))
		{// 右下
			angle = CPlayer::DashAngle::ANGLE_RIGHTDW;
		}
		else
		{// 下
			angle = CPlayer::DashAngle::ANGLE_DOWN;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_RIGHT, player->GetMyPlayerIdx()) ||
		pPad->GetStickMoveL(player->GetMyPlayerIdx()).x > 0 ||
		pKey->GetPress(DIK_D))
	{// 右
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) ||
			pPad->GetStickMoveL(player->GetMyPlayerIdx()).y < 0 ||
			pKey->GetPress(DIK_W))
		{// 右上
			angle = CPlayer::DashAngle::ANGLE_RIGHTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) ||
			pPad->GetStickMoveL(player->GetMyPlayerIdx()).y > 0 ||
			pKey->GetPress(DIK_S))
		{// 右下
			angle = CPlayer::DashAngle::ANGLE_RIGHTDW;
		}
		else
		{// 右
			angle = CPlayer::DashAngle::ANGLE_RIGHT;
		}
	}
	else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_LEFT, player->GetMyPlayerIdx()) ||
		pPad->GetStickMoveL(player->GetMyPlayerIdx()).x < 0 ||
		pKey->GetPress(DIK_A))
	{// 左
		bInput = true;

		if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_UP, player->GetMyPlayerIdx()) ||
			pPad->GetStickMoveL(player->GetMyPlayerIdx()).y < 0 ||
			pKey->GetPress(DIK_W))
		{// 左上
			angle = CPlayer::DashAngle::ANGLE_LEFTUP;
		}
		else if (pPad->GetPress(CInputGamepad::BUTTON::BUTTON_DOWN, player->GetMyPlayerIdx()) ||
			pPad->GetStickMoveL(player->GetMyPlayerIdx()).y > 0 ||
			pKey->GetPress(DIK_S))
		{// 左下
			angle = CPlayer::DashAngle::ANGLE_LEFTDW;
		}
		else
		{// 左
			angle = CPlayer::DashAngle::ANGLE_LEFT;
		}
	}

	if (!bInput)
	{
		return;
	}
	
	// 移動中にする
	motionFrag.bMove = true;

	// 移動量取得
	float fMove = player->GetVelocity();
	fMove *= fDeltaRate;
	fMove *= fSlowRate;

	if (m_bDash)
	{
		fMove *= 1.5f;
	}

	MyLib::Vector3 move = player->GetMove();
	float division = (D3DX_PI * 2.0f) / CPlayer::DashAngle::ANGLE_MAX;	// 向き
	move.x += sinf((D3DX_PI * 0.0f) + division * angle + Camerarot.y) * fMove ;
	move.z += cosf((D3DX_PI * 0.0f) + division * angle + Camerarot.y) * fMove;

	// 移動量設定
	player->SetMove(move);

	// 向き設定
	player->SetRotDest(angle * division + D3DX_PI + Camerarot.y);

	// モーションフラグ設定
	player->SetMotionFrag(motionFrag);
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