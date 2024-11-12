//==========================================================================
// 
//  プレイヤーベース処理 [playerBase.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerBase.h"
#include "playerStatus.h"
#include "ball.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerBase::CPlayerBase(CPlayer* pPlayer, const CPlayer::EFieldArea typeArea) :
	m_pPlayer		 (pPlayer),	// プレイヤー情報
	m_pControlMove	 (nullptr),	// 移動操作
	m_pControlAction (nullptr)	// アクション操作
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerBase::~CPlayerBase()
{
	// 操作の破棄
	DeleteControl();
}

//==========================================================================
// 操作の破棄
//==========================================================================
void CPlayerBase::DeleteControl()
{
	// 移動操作の破棄
	SAFE_DELETE(m_pControlMove);

	// アクション操作の破棄
	SAFE_DELETE(m_pControlAction);
}

//==========================================================================
// ヒット
//==========================================================================
CPlayer::SHitInfo CPlayerBase::Hit(CBall* pBall)
{
	CGameManager::TeamSide sideBall = pBall->GetTypeTeam();	// ボールチームサイド
	CBall::EAttack atkBall	= pBall->GetTypeAtk();			// ボール攻撃種類
	CBall::EState stateBall = pBall->GetState();			// ボール状態
	MyLib::Vector3 posB = pBall->GetPosition();				// ボール位置
	MyLib::HitResult_Character hitresult = {};				// 衝突情報
	CPlayerStatus* pStatus = m_pPlayer->GetStatus();		// ステータス情報

	// ヒット情報の初期化
	CPlayer::SHitInfo hitInfo;
	hitInfo.eHit = CPlayer::HIT_NONE;
	hitInfo.bHit = false;

	if (m_pPlayer->GetMotionFrag().bDead)
	{ // 死亡状態ならすり抜け

		return hitInfo;
	}

	if (stateBall == CBall::STATE_LAND
	||  stateBall == CBall::STATE_FREE && pBall->GetTypeTeam() != pStatus->GetTeam())
	{ // ボールが着地している、またはフリーボール且つ自分のチームボールではない場合

		// ボールをキャッチ
		pBall->CatchLand(m_pPlayer);

		if (pBall->IsLanding())
		{ // ボールが着地している場合

			// 落ちてるのキャッチ
			m_pPlayer->SetMotion(CPlayer::EMotion::MOTION_DROPCATCH_WALK);
		}

		// キャッチ状態
		hitInfo.eHit = CPlayer::EHit::HIT_CATCH;
		return hitInfo;
	}

	// ダメージを受け付けないならすり抜ける
	if (!m_pPlayer->GetDamageInfo().bReceived) { return hitInfo; }

	// リバウンドボールの場合キャッチする
	if (stateBall == CBall::STATE_REBOUND)
	{
		// キャッチ状態
		hitInfo.eHit = CPlayer::EHit::HIT_CATCH;

		// カバーキャッチ時の設定
		m_pPlayer->CoverCatchSetting(pBall);

		return hitInfo;
	}

	// 攻撃状態以外ならすり抜ける
	if (atkBall == CBall::EAttack::ATK_NONE) { return hitInfo; }
	
	// 味方のボールならすり抜ける
	if (pStatus->GetTeam() == sideBall) { return hitInfo; }

	if (m_pPlayer->GetMotionFrag().bCatch
	&&  UtilFunc::Collision::CollisionViewRange3D(m_pPlayer->GetPosition(), posB, m_pPlayer->GetRotation().y, 160))	// TODO：160はボールステータスに変更
	{ // キャッチアクション中だった中でも受け付け中の場合

		// キャッチ時処理
		m_pPlayer->CatchSetting(pBall);

		// キャッチ状態
		hitInfo.eHit = CPlayer::EHit::HIT_CATCH;

		return hitInfo;
	}

	// ダメージを受ける場合はフラグをONにする
	hitInfo.bHit = true;

	// ダメージを与える
	//pStatus->LifeDamage(pBall->GetDamage());	// TODO : 後からBall内の攻撃演出をストラテジーにして、GetDamageを作成
	pStatus->LifeDamage(10);

	if (m_pPlayer->GetLife() <= 0)
	{
		// 終活
		m_pPlayer->DeadSetting(&hitresult, pBall);
	}
	else
	{
		// ダメージ状態にする
		m_pPlayer->DamageSetting(pBall);
	}

	// ゲームマネージャ取得
	CGameManager* pGameMgr = CGameManager::GetInstance();
	if (pGameMgr == nullptr) return hitInfo;

	// モテ加算(ボール投げた側)
	pGameMgr->AddCharmValue(sideBall, CCharmManager::EType::TYPE_HIT);

	return hitInfo;
}

//==========================================================================
// デバッグ
//==========================================================================
void CPlayerBase::Debug()
{

}
