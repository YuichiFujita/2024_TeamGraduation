//=============================================================================
// 
//  外野プレイヤーコントロール処理 [playerAIOut_controlMove.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerAIOut_controlMove.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "playerAction.h"
#include "playerBase.h"
#include "playerAIOut.h"

#include "playerManager.h"

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
CPlayerAIOutControlMove::CPlayerAIOutControlMove()
{
	m_bBlink = false;
	m_bDash = false;
	m_bWalk = false;
	m_fRot = 0.0f;
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAIOutControlMove::~CPlayerAIOutControlMove()
{

}

//==========================================================================
// ブリンク
//==========================================================================
void CPlayerAIOutControlMove::Blink(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
		// コントロール系の取得
	CPlayer::EDashAngle holdDashAngle = GetHoldDashAngle();	// ダッシュ方向
	float fInputInterval = GetInputInterval();	// 入力の受け付け猶予
	int* pCntTrigger = GetCntTrigger();			// トリガーカウント

	//----------------------------------------------------------------------
	//	第一入力の受付
	//----------------------------------------------------------------------
	if (fInputInterval <= 0.0f)
	{ // 猶予受け付けが終了中の場合

		bool bInput = false;
		if (false)
		{ // 左移動キーが押された場合

			// 割り当てられたキーの方向を取得
			//holdDashAngle = m_pLeftKey->GetAngle();
			bInput = true;
		}
		else if (false)
		{ // 右移動キーが押された場合

			// 割り当てられたキーの方向を取得
			//holdDashAngle = m_pRightKey->GetAngle();
			bInput = true;
		}

		if (bInput)
		{ // 移動入力が検知された場合

			// 入力のインターバルを設定
			fInputInterval = INTERVAL_INPUT;
		}
	}

	// ダッシュ方向の反映
	SetHoldDashAngle(holdDashAngle);

	//----------------------------------------------------------------------
	//	受付猶予時間の経過
	//----------------------------------------------------------------------
	CPlayer::SDashInfo infoDash;		// ダッシュ情報
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
// 走り
//==========================================================================
void CPlayerAIOutControlMove::Dash(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayer::SMotionFrag flagMotion = player->GetMotionFrag();	// モーションフラグ
	bool bDash = IsBlink();	// ダッシュフラグ
	if (!bDash) { return; }	// ダッシュしていない場合抜ける

	bool bKeyInput = false;	// キー入力検知
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
void CPlayerAIOutControlMove::Walk(CPlayer* player, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!m_bWalk) return;

	CPlayerBase* pBase = player->GetBase();						// プレイヤーベース情報
	CPlayerOut* pPlayerOut = pBase->GetPlayerOut();				// プレイヤー外野情報
	MyLib::Vector3 posLeft = pPlayerOut->GetPosLeft();			// 移動可能な左位置
	MyLib::Vector3 posRight = pPlayerOut->GetPosRight();		// 移動可能な右位置
	MyLib::Vector3 playerMove = player->GetMove();				// プレイヤー移動ベクトル
	CPlayer::SMotionFrag flagMotion = player->GetMotionFrag();	// モーションフラグ

	if (!flagMotion.bMove)
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
	if (m_fRot > 0)
	{ // 左移動が検知された場合

		// ベクトル逆方向に移動量を与える
		playerMove += -vecMove * fMoveValue;
	}
	else if (m_fRot < 0)
	{ // 右移動が検知された場合

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
CPlayer::SDashInfo CPlayerAIOutControlMove::Trigger(CPlayer* player, CPlayer::EDashAngle eAngle)
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
