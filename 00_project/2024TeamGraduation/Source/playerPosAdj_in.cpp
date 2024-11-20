//==========================================================================
// 
//  プレイヤー位置補正_内野コート処理 [playerPosAdj_in.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerPosAdj_in.h"
#include "player.h"
#include "playerStatus.h"
#include "playerAction.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerPosAdjIn::CPlayerPosAdjIn()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerPosAdjIn::~CPlayerPosAdjIn()
{

}

//==========================================================================
// 調整
//==========================================================================
void CPlayerPosAdjIn::UpdateAdjuster(CPlayer* pPlayer)
{
	CPlayer::SMotionFrag flagMotion = pPlayer->GetMotionFrag();	// モーションフラグ
	MyLib::Vector3 pos = pPlayer->GetPosition();	// 位置
	CPlayer::EState state = pPlayer->GetState();	// 状態
	bool bJump = pPlayer->IsJump();					// ジャンプフラグ

	if (state != CPlayer::EState::STATE_OUTCOURT
	&&  state != CPlayer::EState::STATE_OUTCOURT_RETURN)
	{ // コート越え状態以外はコート内に補正

		// コート内に補正
		CGameManager::GetInstance()->SetPosLimit(pos);
	}

	if (pos.y <= CGameManager::FIELD_LIMIT)
	{ // 地面より下の場合

		// 地面に着地させる
		pos.y = CGameManager::FIELD_LIMIT;

		if (bJump && !flagMotion.bDead)
		{ // ジャンプ中着地

			// 着地モーションの再生
			pPlayer->SetMotion(CPlayer::EMotion::MOTION_LAND);
		}

		// 重力の初期化
		MyLib::Vector3 move = pPlayer->GetMove();
		move.y = 0.0f;
		pPlayer->SetMove(move);

		// ジャンプしていない状態にする
		pPlayer->SetEnableJump(false);
	}

	if (!pPlayer->IsJump() && !flagMotion.bDead)
	{ // 相手コートに侵入したときはコート内に補正

		// チームコートに戻す
		CheckReturn(pPlayer);

#if 0
		// おっとっとする
		CheckUnstable(pPlayer);
#endif
	}

	// 位置を反映
	pPlayer->SetPosition(pos);
}

//==========================================================================
// チームコート復帰
//==========================================================================
void CPlayerPosAdjIn::CheckReturn(CPlayer* pPlayer)
{
	MyLib::Vector3 pos = pPlayer->GetPosition();	// 位置
	CPlayer::EState state = pPlayer->GetState();	// 状態
	CBall* pBall = pPlayer->GetBall();				// ボール情報

	// もう戻ってる場合は抜ける
	if (state == CPlayer::EState::STATE_INVADE_RETURN
	||  state == CPlayer::EState::STATE_INVADE_TOSS) { return; }

	if (IsLineOut(pPlayer))
	{ // ラインを超えていた場合

		// ノックバックの開始位置を設定
		CPlayer::SKnockbackInfo infoKnock = pPlayer->GetKnockBackInfo();	// 取得
		infoKnock.posStart = pos;				// 位置設定
		pPlayer->SetKnockBackInfo(infoKnock);	// 反映

		if (pBall == nullptr)
		{ // ボールを持っていない場合

			// 侵入から戻る状態にする
			pPlayer->SetState(CPlayer::EState::STATE_INVADE_RETURN);
		}
		else
		{ // ボールを持ってる場合

			// トスモーションの再生
			pPlayer->SetMotion(CPlayer::EMotion::MOTION_TOSS);

			// トス状態にする
			pPlayer->SetState(CPlayer::EState::STATE_INVADE_TOSS);
		}
	}
}

//==========================================================================
// おっとっとする
//==========================================================================
void CPlayerPosAdjIn::CheckUnstable(CPlayer* pPlayer)
{
	// 侵入から戻る状態
	if (pPlayer->GetState() == CPlayer::EState::STATE_INVADE_RETURN) return;

	CMotion* motion = pPlayer->GetMotion();
	CPlayerAction* pAction = pPlayer->GetActionPattern();
	CPlayer::SMotionFrag motionFrag = pPlayer->GetMotionFrag();	// モーションフラグ

	CPlayer::EAction action = CPlayer::EAction::ACTION_NONE;	// アクション種類

	bool bBrake = pPlayer->IsBrake();						// ブレーキフラグ
	MyLib::Vector3 move = pPlayer->GetMove();	// 移動量

	if (IsUnstable(pPlayer))
	{ // おっとっとラインを超えていた場合

		// ブレーキフラグ
		if (!bBrake)
		{
			bBrake = true;
		}
		else
		{
			move.x = 0.0f;
			move.z = 0.0f;
			pPlayer->SetEnableMove(false);
		}

		if (motionFrag.bMove)
		{// 移動中なら

			// 設定
			pPlayer->SetMove(move);
			pPlayer->SetEnableBrake(bBrake);
			pAction->SetAction(action);
			return;
		}

		// 敵側に入力された
		if (IsInputLine(pPlayer))
		{
			// チームコートに戻す
			CheckReturn(pPlayer);
		}

		// 上書き防止
		if (motion->GetType() == CPlayer::EMotion::MOTION_UNSTABLE)	return;

		// おっとっとモーションの再生
		pPlayer->SetMotion(CPlayer::EMotion::MOTION_UNSTABLE);

		// アクション設定
		action = CPlayer::EAction::ACTION_UNSTABLE;
		pAction->SetAction(action);
	}
	else
	{
		bBrake = false;
	}

	// 設定
	pPlayer->SetMove(move);
	pPlayer->SetEnableBrake(bBrake);
}
