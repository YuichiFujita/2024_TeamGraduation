//==========================================================================
// 
//  外野プレイヤー処理 [playerOut.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerOut.h"

// 使用クラス
#include "playercontrol_move.h"
#include "playercontrol_action.h"
#include "playerAction.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerOut::CPlayerOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase) : CPlayerBase(pPlayer, typeTeam, typeArea, typeBase),
	m_posLeft	(VEC3_ZERO),	// 移動可能左位置
	m_posRight	(VEC3_ZERO),	// 移動可能右位置
	m_bClab(false)
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerOut::~CPlayerOut()
{

}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerOut::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// プレイヤーの取得
	CPlayer* pPlayer = GetPlayer();

	// 操作クラスの取得
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	if (!pPlayer->IsDeathState())
	{ // 死んでいない場合

		// 操作の更新
		pControlMove->Move(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);
		pControlAction->Action(pPlayer, fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 操作クラスの反映
	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// ヒット
//==========================================================================
CPlayer::SHitInfo CPlayerOut::Hit(CBall* pBall)
{
	CGameManager::ETeamSide sideBall = pBall->GetTypeTeam();	// ボールチームサイド
	CBall::EAttack atkBall	= pBall->GetTypeAtk();				// ボール攻撃種類
	CBall::EState stateBall = pBall->GetState();				// ボール状態
	MyLib::Vector3 posBall = pBall->GetPosition();				// ボール位置
	MyLib::HitResult_Character hitresult = {};					// 衝突情報
	CPlayer* pPlayer = GetPlayer();								// プレイヤー情報
	CPlayerStatus* pStatus = pPlayer->GetStatus();				// ステータス情報
	CPlayer::EState state = pPlayer->GetState();				// プレイヤー状態
	CPlayer::EAction action = pPlayer->GetActionPattern()->GetAction();	// プレイヤー行動状態
	CBallStatus::SBallParameter ballParameter = pPlayer->GetBallParameter();	// ボールステータス
	float fCatchRange = UtilFunc::Transformation::RadianChangeToDegree(ballParameter.fCatchRange);	// キャッチ範囲

	// ヒット情報の初期化
	CPlayer::SHitInfo hitInfo;
	hitInfo.eHit = CPlayer::HIT_NONE;
	hitInfo.bHit = false;

	if (IsAutoBallCatch(pBall))
	{ // 自動ボールキャッチが可能な場合

		// ボールをキャッチ
		pBall->CatchLand(pPlayer);

		if (pBall->IsLanding())
		{ // ボールが着地している場合

			// 落ちてるのキャッチ
			pPlayer->SetMotion(CPlayer::EMotion::MOTION_DROPCATCH_WALK);

			// 投げの猶予設定
			pPlayer->GetBase()->GetPlayerControlAction()->SetThrowDrop();
		}

		// キャッチ状態
		hitInfo.eHit = CPlayer::EHit::HIT_CATCH;
		return hitInfo;
	}

	// 攻撃状態以外ならすり抜ける
	if (atkBall == CBall::EAttack::ATK_NONE) { return hitInfo; }

	if (pPlayer->GetMotionFrag().bCatch
	&&  UtilFunc::Collision::CollisionViewRange3D(pPlayer->GetPosition(), posBall, pPlayer->GetRotation().y, fCatchRange))
	{ // キャッチアクション中だった中でも受け付け中の場合

		// キャッチ時処理
		pPlayer->CatchSetting(pBall);

		// キャッチ状態
		hitInfo.eHit = CPlayer::EHit::HIT_CATCH;
		return hitInfo;
	}

	return hitInfo;
}

//==========================================================================
// 位置の初期化
//==========================================================================
void CPlayerOut::InitPosition(const MyLib::Vector3& /*rPos*/)
{
	// 左右位置の中央座標を計算
	MyLib::Vector3 posCenter;	// 中央座標
	D3DXVec3Lerp(&posCenter, &m_posLeft, &m_posRight, 0.5f);

	// プレイヤー位置の設定
	GetPlayer()->SetPosition(posCenter);
}

//==========================================================================
// カニ歩き判定
//==========================================================================
bool CPlayerOut::IsCrab()
{
	// プレイヤー情報の取得
	CPlayer* pPlayer = GetPlayer();

	if (!pPlayer || !m_bClab) return false;

	// 世界にボールがあるか
	CBall* pBall = CGameManager::GetInstance()->GetBall();	// ボール情報
	if (pBall == nullptr) { return false; }

	// 自身の状態：ブリンク＆走りでない
	CPlayer::EAction action = pPlayer->GetActionPattern()->GetAction();	// アクションパターン
	if (action == CPlayer::EAction::ACTION_BLINK) { return false; }
	if (pPlayer->IsDash()) { return false; }

	return true;

}

//==========================================================================
// カニ歩きモーション判定
//==========================================================================
bool CPlayerOut::IsCrabMotion()
{
	// 全部カニ歩き
	return true;
}
