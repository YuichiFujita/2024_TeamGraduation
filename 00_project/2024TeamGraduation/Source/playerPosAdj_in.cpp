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
#include "EffekseerObj.h"

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

		// 位置を反映
		pPlayer->SetPosition(pos);
	}

	if (pos.y <= CGameManager::FIELD_LIMIT)
	{ // 地面より下の場合

		// 地面に着地させる
		pos.y = CGameManager::FIELD_LIMIT;

		// 位置を反映
		pPlayer->SetPosition(pos);

		if (bJump && !flagMotion.bDead)
		{ // ジャンプ中着地

			// 着地モーションの再生
			pPlayer->SetMotion(CPlayer::EMotion::MOTION_LAND);

			// エフェクト
			CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_LAND,
				pos,
				MyLib::Vector3(),	// 向き
				MyLib::Vector3(),
				15.0f, true);
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

#if 1
		// おっとっとする
		CheckUnstable(pPlayer);
#endif

		// チームコートに戻す
		if (pPlayer->GetActionPattern()->GetAction() != CPlayer::EAction::ACTION_UNSTABLE)
		{// おっとっと中じゃない
			CheckReturn(pPlayer);
		}

	}
}

//==========================================================================
// チームコート復帰
//==========================================================================
void CPlayerPosAdjIn::CheckReturn(CPlayer* pPlayer)
{
	CPlayer::EState state = pPlayer->GetState();	// 状態

	// もう戻ってる場合は抜ける
	if (state == CPlayer::EState::STATE_INVADE_RETURN
	||  state == CPlayer::EState::STATE_INVADE_TOSS
	||	state == CPlayer::EState::STATE_DMG) { return; }

	if (IsLineOut(pPlayer))
	{ // ラインを超えていた場合

		// 復帰設定
		ReturnSetting(pPlayer);
	}
}

//==========================================================================
// おっとっとする
//==========================================================================
void CPlayerPosAdjIn::CheckUnstable(CPlayer* pPlayer)
{
	CMotion* motion = pPlayer->GetMotion();
	CPlayerAction* pAction = pPlayer->GetActionPattern();
	CPlayer::SMotionFrag motionFrag = pPlayer->GetMotionFrag();	// モーションフラグ

	CPlayer::EAction action = pAction->GetAction();	// アクション種類

	// 侵入から戻る状態
	if (pPlayer->GetState() == CPlayer::EState::STATE_INVADE_RETURN ||
		pPlayer->GetState() == CPlayer::EState::STATE_INVADE_TOSS)
	{
		// アクション設定
		action = CPlayer::EAction::ACTION_NONE;
		pAction->SetAction(action);
		return;
	}

	bool bBrake = pPlayer->IsBrake();							// ブレーキフラグ
	MyLib::Vector3 move = pPlayer->GetMove();					// 移動量

	// 別アクション中
	if (pAction->GetAction() != CPlayer::EAction::ACTION_NONE &&
		pAction->GetAction() != CPlayer::EAction::ACTION_UNSTABLE)
	{
		pPlayer->SetEnableAction(true);
		return;
	}

	if (pPlayer->GetState() != CPlayer::EState::STATE_DMG &&
		(IsUnstable(pPlayer) || action == CPlayer::EAction::ACTION_UNSTABLE))
	{ // おっとっとラインを超えていた場合

		// アクション設定
		action = CPlayer::EAction::ACTION_UNSTABLE;

		// ブレーキフラグ
		if (!bBrake)
		{
			move.x = 0.0f;
			bBrake = true;
		}
		else
		{
			move.x = 0.0f;
			pPlayer->SetEnableAction(true);
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

		// 入力状況
		EInputUnstable inputUnstable = IsInputLine(pPlayer);

		if (inputUnstable == EInputUnstable::INPUT_ENEMY)
		{// 敵側に入力された

			// 復帰設定
			ReturnSetting(pPlayer);

			// アクション設定
			action = CPlayer::EAction::ACTION_NONE;
			pAction->SetAction(action);
		}
		else if (inputUnstable == EInputUnstable::INPUT_FRIEND)
		{// 味方側に入力された

			// フラグ再設定
			bBrake = false;
			pPlayer->SetEnableAction(true);
			pPlayer->SetEnableMove(true);

			// アクション設定
			action = CPlayer::EAction::ACTION_NONE;
			pAction->SetAction(action);

			return;
		}

		// 上書き防止
		if (motion->GetType() == CPlayer::EMotion::MOTION_UNSTABLE)	return;

		// おっとっとモーションの再生
		if (motion->GetType() != CPlayer::EMotion::MOTION_TOSS)
		{
			pPlayer->SetMotion(CPlayer::EMotion::MOTION_UNSTABLE);
		}

		// アクション設定
		action = CPlayer::EAction::ACTION_UNSTABLE;
	}
	else
	{
		bBrake = false;

		// アクション設定
		action = CPlayer::EAction::ACTION_NONE;
		pAction->SetAction(action);
	}

	// 設定
	pPlayer->SetMove(move);
	pPlayer->SetEnableBrake(bBrake);
	
	// アクション設定
	pAction->SetAction(action);
}

//==========================================================================
// 復帰設定
//==========================================================================
void CPlayerPosAdjIn::ReturnSetting(CPlayer* pPlayer)
{
	MyLib::Vector3 pos = pPlayer->GetPosition();	// 位置
	CBall* pBall = pPlayer->GetBall();				// ボール情報

	// ノックバックの開始位置を設定
	CPlayer::SKnockbackInfo infoKnock = pPlayer->GetKnockBackInfo();	// 取得
	infoKnock.posStart = pos;				// 位置設定
	pPlayer->SetKnockBackInfo(infoKnock);	// 反映

	CGameManager::ETeamSide team = pPlayer->GetTeam();		// チーム
	
	CGameManager* pGameMgr = CGameManager::GetInstance();

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

		// モテ減少
		pGameMgr->SubCharmValue(team, CCharmValueManager::ETypeSub::SUB_INVADE_RUN);
	}
}
