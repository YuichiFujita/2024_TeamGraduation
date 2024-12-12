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
	const float INTERVAL_INPUT = (2.0f / 60.0f);	// 入力
	const float RATIO_STICKBLINK = 0.75f;	// スティックブリンク時の割合
	const float TIME_INTERVAL = 0.3f;	// ダッシュ猶予
	const float BLINK_JUMP_COR = 0.5f;	// 空中時ブリンク補正値
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
// デストラクタ
//==========================================================================
CPlayerAIControlMove::~CPlayerAIControlMove()
{

}

//==========================================================================
// ダッシュ
//==========================================================================
void CPlayerAIControlMove::Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 入力フラグ
	bool bInput = false;
	// ダッシュフラグ取得
	bool bBlink = IsBlink();

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// ダッシュする
	if (!bBlink && m_bBlink)
	{
		float division = (D3DX_PI * 2.0f) / CPlayer::EDashAngle::ANGLE_MAX;	// 向き
		MyLib::Vector3 rot = player->GetRotation();
		if (player->GetBall() == nullptr)
		{//ボール所持では使用不可

			// 移動量取得
			MyLib::Vector3 move = player->GetMove();
			float velocityBlink = player->GetParameter().fVelocityBlink;

			// 移動量更新
			move.x += sinf(rot.y + (D3DX_PI * 1.0f)) * velocityBlink;
			move.z += cosf(rot.y + (D3DX_PI * 1.0f)) * velocityBlink;

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
		player->SetRotDest(rot.y + /*D3DX_PI +*/ Camerarot.y);

		// ダッシュフラグ
		bBlink = true;
	}

	// コントロール系
	SetBlink(bBlink);	// 走るフラグ設定
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

	// モーションフラグ取得
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	if (!m_bDash)
	{// ダッシュ解除
		bBlink = false;

		// 前グリップ
		int motionType = player->GetMotion()->GetType();
		if (motionType == CPlayer::EMotion::MOTION_RUN ||
			motionType == CPlayer::EMotion::MOTION_RUN_BALL)
		{// ダッシュからは派生
			player->SetMotion(CPlayer::EMotion::MOTION_GRIP_FRONT);
		}

		// 走るフラグ設定
		player->SetEnableDash(false);
	}
	else
	{
		// ダッシュ継続
		motionFrag.bMove = true;
		player->SetEnableDash(true);
	}

	// モーションフラグ設定
	player->SetMotionFrag(motionFrag);

	//走るフラグ設定
	SetBlink(bBlink);
}

//==========================================================================
// ウォーク
//==========================================================================
void CPlayerAIControlMove::Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{	
	// モーションフラグ取得
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	if (!m_bWalk) {// 歩くがオフの時モーションもオフ
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
	
	// TODO : 方向に応じてカニ歩き

	if (player->GetBase()->IsCrab())
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