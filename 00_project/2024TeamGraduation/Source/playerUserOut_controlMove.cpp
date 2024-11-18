//=============================================================================
// 
//  外野プレイヤーコントロール処理 [playerUserOut_controlMove.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerUserOut_controlMove.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "playerAction.h"
#include "playerBase.h"
#include "playerUserOut.h"

// TODO:いらない
#include "3D_effect.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float INTERVAL_INPUT = (2.0f / 60.0f);	// 入力
	const float RATIO_STICKBLINK = 0.75f;	// スティックブリンク時の割合
	const float TIME_INTERVAL = 0.3f;	// ダッシュ猶予
	const float BLINK_JUMP_COR = 0.5f;	// 空中時ブリンク補正値
	const float INPUT_COUNTER = (4.0f / 60.0f);	// 入力カウンター
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerUserOutControlMove::CPlayerUserOutControlMove()
{

}

//==========================================================================
// ブリンク
//==========================================================================
void CPlayerUserOutControlMove::Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// キー入力のブリンク
	BilnkKey(player, fDeltaTime, fDeltaRate, fSlowRate);

	// スティック入力のブリンク
	BilnkStick(player, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// キー入力のブリンク
//==========================================================================
void CPlayerUserOutControlMove::BilnkKey(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// インプット情報の取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// キーボード情報
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// パッド情報

	// コントロール系の取得
	CPlayer::EDashAngle holdDashAngle = GetHoldDashAngle();	// ダッシュ方向
	float fInputInterval = GetInputInterval();	// 入力の受け付け猶予
	int* pCntTrigger = GetCntTrigger();			// トリガーカウント

	//----------------------------------------------------------------------
	//	第一入力の受付
	//----------------------------------------------------------------------
	if (fInputInterval <= 0.0f)
	{ // 猶予受け付けが終了中の場合

		// TODO：プレイヤーから見た左右に変更
		bool bInput = false;
		if (pKey->GetTrigger(DIK_A))
		{
			holdDashAngle = CPlayer::EDashAngle::ANGLE_LEFT;
			bInput = true;
		}
		else if (pKey->GetTrigger(DIK_D))
		{
			holdDashAngle = CPlayer::EDashAngle::ANGLE_RIGHT;
			bInput = true;
		}

		if (bInput)
		{ // 移動入力が検知された場合

			// 入力のインターバルを設定
			fInputInterval = INTERVAL_INPUT;
		}
	}

	//----------------------------------------------------------------------
	//	第二入力の受付
	//----------------------------------------------------------------------
	CPlayer::SDashInfo infoDash;	// ダッシュ情報
	if (fInputInterval > 0.0f && fInputInterval != INTERVAL_INPUT)
	{ // 受け付け猶予中の場合

		// ブリンクかの判定
		infoDash = Trigger(player, holdDashAngle);
			
		// 受け付け猶予の終了
		fInputInterval = 0.0f;
	}

	// ダッシュ方向の反映
	SetHoldDashAngle(holdDashAngle);

	//----------------------------------------------------------------------
	//	受付猶予時間の経過
	//----------------------------------------------------------------------
	float fOldTime = fInputInterval;	// 過去の受付猶予時間 

	// 受け付け猶予の減算
	fInputInterval -= fDeltaTime * fSlowRate;
	fInputInterval = UtilFunc::Transformation::Clamp(fInputInterval, 0.0f, INTERVAL_INPUT);

	// 入力受け付け猶予の反映
	SetInputInterval(fInputInterval);

	if (fOldTime > 0.0f && fInputInterval <= 0.0f)
	{ // 前回がまだ猶予中且つ、現在は経過終了している場合

		// ブリンクかの判定
		infoDash = Trigger(player, holdDashAngle);	// ダッシュ情報の取得
	}

	//----------------------------------------------------------------------
	//	ブリンクの発動
	//----------------------------------------------------------------------
	if (infoDash.bDash && !IsBlink())
	{ // ブリンクを開始できる場合

		const float fDivision = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// 向き
		if (player->GetBall() == nullptr)
		{ // ボール所持では使用不可

			MyLib::Vector3 move = player->GetMove();	// 移動量
			const float fVeloBlink = player->GetParameter().fVelocityBlink;	// ブリンク速度

			// 移動量の更新
			move.x += sinf(fDivision * infoDash.eAngle) * fVeloBlink;
			move.z += cosf(fDivision * infoDash.eAngle) * fVeloBlink;

			if (player->IsJump())
			{ // ジャンプ中の場合

				// 移動量を補正
				move.x *= BLINK_JUMP_COR;
				move.z *= BLINK_JUMP_COR;
			}

			// 移動量の反映
			player->SetMove(move);

			// ブリンクモーションを再生
			player->SetMotion(CPlayer::EMotion::MOTION_BLINK);
		}

		// 目標向きの設定
		player->SetRotDest(infoDash.eAngle * fDivision + D3DX_PI);

		// トリガーカウントのリセット
		memset(pCntTrigger, 0, sizeof(pCntTrigger) * CPlayer::EDashAngle::ANGLE_MAX);

		// トリガーカウントの設定
		SetCntTrigger(pCntTrigger);

		// ダッシュしている状態にする
		SetBlink(true);
	}

	//----------------------------------------------------------------------
	//	トリガーインターバルの経過
	//----------------------------------------------------------------------
	float fTriggerInterval = GetTriggerInterval();	// トリガーインターバル

	// トリガーインターバルの減算
	fTriggerInterval -= fDeltaTime * fSlowRate;
	if (fTriggerInterval <= 0.0f)
	{ // トリガーインターバルが終了した場合

		// トリガーカウントのリセット
		memset(pCntTrigger, 0, sizeof(pCntTrigger) * CPlayer::EDashAngle::ANGLE_MAX);

		// トリガーカウントの設定
		SetCntTrigger(pCntTrigger);
	}

	// トリガーインターバルの反映
	SetTriggerInterval(fTriggerInterval);
}

//==========================================================================
// スティック入力のブリンク
//==========================================================================
void CPlayerUserOutControlMove::BilnkStick(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#if 0
	// 入力フラグ
	bool bInput = false;

	// ダッシュ判定
	CPlayer::SDashInfo info;

	// インプット情報取得
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// コントロール系取得
	int* nCntTrigger = GetCntTrigger();						// トリガーのカウント取得
	CPlayer::EDashAngle HoldDashAngle = GetHoldDashAngle();	// ダッシュ方向取得
	bool bBlink = IsBlink();								// ブリンクフラグ取得

	// プレイヤー番号取得
	int playerIdx = player->GetMyPlayerIdx();

	// Lスティックの割合
	MyLib::Vector3 stickRatio = pPad->GetStickPositionRatioL(playerIdx);

	//--------------------------
	// スティック
	//--------------------------
	bool bTipStick = false;
	if (pPad->GetStickMoveL(playerIdx).y > 0)
	{// 上
		bTipStick = true;

		if (pPad->GetStickMoveL(playerIdx).x < 0)
		{// 左上
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
		}
		else if (pPad->GetStickMoveL(playerIdx).x > 0)
		{// 右上
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
		}
		else
		{// 上
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_UP;
		}
	}
	else if (pPad->GetStickMoveL(playerIdx).y < 0)
	{// 下
		bTipStick = true;

		if (pPad->GetStickMoveL(playerIdx).x < 0)
		{// 左下
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTDW;
		}
		else if (pPad->GetStickMoveL(playerIdx).x > 0)
		{// 右下
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
		}
		else
		{// 下
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_DOWN;
		}
	}
	else if (pPad->GetStickMoveL(playerIdx).x > 0)
	{// 右
		bTipStick = true;

		if (pPad->GetStickMoveL(playerIdx).y < 0)
		{// 右上
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTUP;
		}
		else if (pPad->GetStickMoveL(playerIdx).y > 0)
		{// 右下
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHTDW;
		}
		else
		{// 右
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_RIGHT;
		}
	}
	else if (pPad->GetStickMoveL(playerIdx).x < 0)
	{// 左
		bTipStick = true;

		if (pPad->GetStickMoveL(playerIdx).y < 0)
		{// 左上
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTUP;
		}
		else if (pPad->GetStickMoveL(playerIdx).y > 0)
		{// 左下
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFTDW;
		}
		else
		{// 左
			HoldDashAngle = CPlayer::EDashAngle::ANGLE_LEFT;
		}
	}

	if (bTipStick && pPad->GetTrigger(CInputGamepad::BUTTON_LSTICKPUSH, playerIdx))
	{// スティック倒し && 押し込み
		bInput = true;
		info = Trigger(player, HoldDashAngle);
		info = Trigger(player, HoldDashAngle);
	}
	SetHoldDashAngle(HoldDashAngle);	// ダッシュ方向設定

	// ダッシュする
	if (info.bDash && !bBlink)
	{
		float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// 向き
		MyLib::Vector3 rot = player->GetRotation();
		if (player->GetBall() == nullptr)
		{//ボール所持では使用不可

			// 移動量取得
			MyLib::Vector3 move = player->GetMove();
			float velocityBlink = player->GetParameter().fVelocityBlink;

			// 移動量更新
			move.x += sinf(division * info.eAngle + Camerarot.y) * velocityBlink;
			move.z += cosf(division * info.eAngle + Camerarot.y) * velocityBlink;

			if (player->IsJump())
			{//ジャンプ時補正
				move.x *= BLINK_JUMP_COR;
				move.z *= BLINK_JUMP_COR;
			}

			// 移動量設定
			player->SetMove(move);

			// モーション設定
			player->SetMotion(CPlayer::EMotion::MOTION_BLINK);
		}

		// 向き設定
		player->SetRotDest(info.eAngle * division + D3DX_PI + Camerarot.y);

		// トリガーのカウントリセット
		memset(nCntTrigger, 0, sizeof(nCntTrigger) * CPlayer::EDashAngle::ANGLE_MAX);

		// トリガーのカウント設定
		SetCntTrigger(nCntTrigger);

		// ダッシュフラグ
		bBlink = true;
	}

	// コントロール系
	SetBlink(bBlink);	// 走るフラグ設定
#endif
}

//==========================================================================
// 走り
//==========================================================================
void CPlayerUserOutControlMove::Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayer::SMotionFrag flagMotion = player->GetMotionFrag();	// モーションフラグ
	bool bDash = IsBlink();	// ダッシュフラグ
	if (!bDash) { return; }	// ダッシュしていない場合抜ける

	// インプット情報の取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// キーボード情報
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// パッド情報

	bool bKeyInput = (pKey->GetPress(DIK_A) || pKey->GetPress(DIK_D));	// キー入力検知
	if (!bKeyInput)
	{ // 移動入力が検知されなかった場合

		// ダッシュを解除する
		bDash = false;
	}
	else
	{ // 移動入力が検知された場合

		// ダッシュを継続する
		flagMotion.bMove = true;
		player->SetEnableDash(true);
	}

	// モーションフラグ反映
	player->SetMotionFrag(flagMotion);
	
	// ダッシュフラグ反映
	SetBlink(bDash);
}

//==========================================================================
// ウォーク
//==========================================================================
void CPlayerUserOutControlMove::Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerBase* pBase = player->GetBase();						// プレイヤーベース情報
	CPlayerUserOut* pPlayerOut = pBase->GetPlayerUserOut();		// プレイヤー外野情報
	MyLib::Vector3 posLeft = pPlayerOut->GetPosLeft();			// 移動可能な左位置
	MyLib::Vector3 posRight = pPlayerOut->GetPosRight();		// 移動可能な右位置
	MyLib::Vector3 playerMove = player->GetMove();				// プレイヤー移動ベクトル
	CPlayer::SMotionFrag flagMotion = player->GetMotionFrag();	// モーションフラグ

	// 現在の入力方向カウンター
	float fInputAngleCtr = GetInputAngleCtr();
	fInputAngleCtr -= fDeltaTime * fSlowRate;
	UtilFunc::Transformation::Clamp(fInputAngleCtr, 0.0f, INPUT_COUNTER);
	SetInputAngleCtr(fInputAngleCtr);

	// 現在の入力方向
	CPlayer::EDashAngle eAngle;
	CPlayer::EDashAngle* pInputAngle = GetInputAngle();
	if (pInputAngle != nullptr && fInputAngleCtr <= 0.0f)
	{
		delete pInputAngle;
		pInputAngle = nullptr;
	}
	SetInputAngle(pInputAngle);

	// インプット情報の取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// キーボード情報
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// パッド情報

	bool bKeyInput = (pKey->GetTrigger(DIK_A) || pKey->GetTrigger(DIK_D));	// キー入力検知
	if (!flagMotion.bMove && bKeyInput)
	{ // 移動してない且つ、入力を検知した場合

		// 左右フラグ反転
		player->InverseFootLR();
	}

	// 移動方向ベクトルの作成
	MyLib::Vector3 vecMove = posRight - posLeft;	// ベクトルの作成
	vecMove = vecMove.Normal();						// ベクトルの正規化

	// 移動量の作成
	CCharacterStatus::CharParameter param = player->GetParameter();					// プレイヤーパラメータ
	float fMoveValue = (IsBlink()) ? param.fVelocityDash : param.fVelocityNormal;	// 移動量
	fMoveValue *= fDeltaRate * fSlowRate;	// 経過時間乗算

	// 移動操作
	if (pKey->GetPress(DIK_A))
	{ // 左移動が検知された場合

		eAngle = CPlayer::EDashAngle::ANGLE_LEFT;

		// ベクトル逆方向に移動量を与える
		playerMove += -vecMove * fMoveValue;
	}
	else if (pKey->GetPress(DIK_D))
	{ // 右移動が検知された場合

		eAngle = CPlayer::EDashAngle::ANGLE_RIGHT;

		// ベクトル方向に移動量を与える
		playerMove += vecMove * fMoveValue;
	}
	else
	{ // 操作が検知された場合

		// 移動しない状態にする
		flagMotion.bMove = false;

		// モーションフラグ反映
		player->SetMotionFrag(flagMotion);
		return;
	}

	// 移動中にする
	flagMotion.bMove = true;

	// 現在の入力方向設定
	if (pInputAngle == nullptr)
	{
		pInputAngle = DEBUG_NEW CPlayer::EDashAngle;
	}
	*pInputAngle = eAngle;
	SetInputAngle(pInputAngle);

	// モーションフラグ反映
	player->SetMotionFrag(flagMotion);

	// 移動量反映
	player->SetMove(playerMove);

	// 目標向き設定
	player->SetRotDest(posLeft.AngleXZ(posRight) + D3DX_PI * 0.5f);
}

//==========================================================================
// トリガー
//==========================================================================
CPlayer::SDashInfo CPlayerUserOutControlMove::Trigger(CPlayer* player, CPlayer::EDashAngle eAngle)
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
		if (player->GetBall() == nullptr)
		{
			player->GetActionPattern()->SetAction(CPlayer::EAction::ACTION_BLINK);
		}
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