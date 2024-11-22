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

#if 1
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
	CPlayer::EState state = pPlayer->GetState();	// 状態

	// もう戻ってる場合は抜ける
	if (state == CPlayer::EState::STATE_INVADE_RETURN
	||  state == CPlayer::EState::STATE_INVADE_TOSS) { return; }

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
	// 侵入から戻る状態
	if (pPlayer->GetState() == CPlayer::EState::STATE_INVADE_RETURN ||
		pPlayer->GetState() == CPlayer::EState::STATE_INVADE_TOSS)
		return;

	CMotion* motion = pPlayer->GetMotion();
	CPlayerAction* pAction = pPlayer->GetActionPattern();
	CPlayer::SMotionFrag motionFrag = pPlayer->GetMotionFrag();	// モーションフラグ

	CPlayer::EAction action = CPlayer::EAction::ACTION_NONE;	// アクション種類

	bool bBrake = pPlayer->IsBrake();							// ブレーキフラグ
	MyLib::Vector3 move = pPlayer->GetMove();					// 移動量

	// 別アクション中
	if (pAction->GetAction() != CPlayer::EAction::ACTION_NONE &&
		pAction->GetAction() != CPlayer::EAction::ACTION_UNSTABLE)
	{
		pPlayer->SetEnableAction(true);
		return;
	}

	if (IsUnstable(pPlayer))
	{ // おっとっとラインを超えていた場合

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
		}
		else if (inputUnstable == EInputUnstable::INPUT_FRIEND)
		{// 味方側に入力された

			bBrake = false;
			pPlayer->SetEnableAction(true);
			pPlayer->SetEnableMove(true);
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
	}

	// 設定
	pPlayer->SetMove(move);
	pPlayer->SetEnableBrake(bBrake);
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
		pGameMgr->SubCharmValue(team, CCharmManager::ETypeSub::SUB_INVADE_RUN);
	}
}

//==========================================================================
// 端に逃げ続ける
//==========================================================================
bool CPlayerPosAdjIn::CheckEdgeEscape(CPlayer* pPlayer)
{
	MyLib::Vector3 pos = pPlayer->GetPosition();
	CGameManager* pGmMgr = CGameManager::GetInstance();
	MyLib::Vector3 posCourt = MyLib::Vector3();											// 自陣中央
	MyLib::Vector3 sizeCourt = pGmMgr->GetCourtSize(pPlayer->GetTeam(), posCourt);		// 自陣サイズ

	// ボール情報
	CBall* pBall = pGmMgr->GetBall();
	CPlayer* pBallPlayer = pBall->GetPlayer();
	MyLib::Vector3 posBP = pBallPlayer->GetPosition();

	// ボール所持プレイヤーと自陣中央のベクトル
	MyLib::Vector3 vecDiff = (posBP - posCourt).Normal();

	// 端エリアをベクトルから交差判定
	// 自陣4点(上下左右)
	MyLib::Vector3 posEdge[4] =
	{
		MyLib::Vector3(posCourt.x + sizeCourt.x, 0.0f, posCourt.z + sizeCourt.x),	// 右奥
		MyLib::Vector3(posCourt.x + sizeCourt.x, 0.0f, posCourt.z - sizeCourt.x),	// 右手前
		MyLib::Vector3(posCourt.x - sizeCourt.x, 0.0f, posCourt.z - sizeCourt.x),	// 左手前
		MyLib::Vector3(posCourt.x - sizeCourt.x, 0.0f, posCourt.z + sizeCourt.x),	// 左奥
	};

	MyLib::Vector3 lineMid = MyLib::Vector3();	// 交点
	MyLib::Vector2 areaSize = MyLib::Vector2(sizeCourt.x, sizeCourt.z);		// エリアサイズ
	areaSize.x *= 0.2f;							//z長x短

	// 判定
	for (int i = 0; i < 4; i++)
	{
		MyLib::Vector3 pos1 = posEdge[0];
		if (i < 3)
		{
			pos1 = posEdge[i + 1];
		}

		if (UtilFunc::Collision::IsVecToLine(posEdge[i], pos1, posCourt, vecDiff))
		{// ライン交差

			lineMid = UtilFunc::Calculation::GetCenterPosition3D(posEdge[i], pos1);

			areaSize += lineMid;
			// エリアサイズ設定
			if (lineMid.z == posCourt.z)
			{
				float f = areaSize.x;
				areaSize.x = areaSize.y;
				areaSize.y = f;
			}
		
			break;
		}
		
	}

	if (UtilFunc::Collision::CollisionSquare(lineMid, areaSize, 0.0f, pos))
	{// 端エリア内にいるか

		return true;
	}

	return false;
}
