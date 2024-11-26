//==========================================================================
// 
//  プレイヤーベース処理 [playerBase.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "playerBase.h"
#include "playerStatus.h"
#include "playercontrol_action.h"
#include "playercontrol_move.h"
#include "playerAction.h"
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
CPlayerBase::CPlayerBase(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase) :
	m_curTypeBase	 (typeBase),	// 現在のベースタイプ
	m_newTypeBase	 (typeBase),	// 新しいベースタイプ
	m_pPlayer		 (pPlayer),		// プレイヤー情報
	m_pControlMove	 (nullptr),		// 移動操作
	m_pControlAction (nullptr)		// アクション操作
{
	// 位置補正の割当
	pPlayer->ChangePosAdjuster(typeTeam, typeArea);
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
// ヒット
//==========================================================================
CPlayer::SHitInfo CPlayerBase::Hit(CBall* pBall)
{
	CGameManager::ETeamSide sideBall = pBall->GetTypeTeam();	// ボールチームサイド
	CBall::EAttack atkBall	= pBall->GetTypeAtk();		// ボール攻撃種類
	CBall::EState stateBall = pBall->GetState();		// ボール状態
	MyLib::Vector3 posB = pBall->GetPosition();			// ボール位置
	MyLib::HitResult_Character hitresult = {};			// 衝突情報
	CPlayerStatus* pStatus = m_pPlayer->GetStatus();	// ステータス情報

	// ヒット情報の初期化
	CPlayer::SHitInfo hitInfo;
	hitInfo.eHit = CPlayer::HIT_NONE;
	hitInfo.bHit = false;

	if (m_pPlayer->GetMotionFrag().bDead)
	{ // 死亡状態ならすり抜け

		return hitInfo;
	}

	if (stateBall == CBall::STATE_LAND																			// ボールが着地している
	||  stateBall == CBall::STATE_FREE && pBall->GetTypeTeam() != m_pPlayer->GetTeam()							// フリーボール且つ自チームのボールじゃない
	||  pBall->IsPass() && (pBall->GetTarget() == m_pPlayer || pBall->GetTypeTeam() != m_pPlayer->GetTeam()))	// パス状態且つターゲットが自分自身か敵チーム
	{ // 上記の条件の場合

		// ボールをキャッチ
		pBall->CatchLand(m_pPlayer);

		if (pBall->IsLanding())
		{ // ボールが着地している場合

			// 落ちてるのキャッチ
			m_pPlayer->SetMotion(CPlayer::EMotion::MOTION_DROPCATCH_WALK);

			// 投げの猶予設定
			m_pPlayer->GetBase()->GetPlayerControlAction()->SetThrowDrop();
		}

		// キャッチ状態
		hitInfo.eHit = CPlayer::EHit::HIT_CATCH;
		return hitInfo;
	}

	// リバウンドボールの場合キャッチする
	if (stateBall == CBall::STATE_REBOUND &&
		m_pPlayer->GetState() != CPlayer::EState::STATE_DMG)
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
	if (m_pPlayer->GetTeam() == sideBall) { return hitInfo; }

	// ダメージを受け付けないならすり抜ける
	if (!m_pPlayer->GetDamageInfo().bReceived)
	{
		hitInfo.bHit = true;
		return hitInfo;
	}

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

	if (m_pPlayer->GetState() == CPlayer::EState::STATE_INVADE_RETURN)
	{// コートから戻っているとき
			
		// モテ減少
		CGameManager* pGameMgr = CGameManager::GetInstance();
		pGameMgr->SubCharmValue(m_pPlayer->GetTeam(), CCharmValueManager::ETypeSub::SUB_INVADE_RUN);
	}

	return hitInfo;
}

//==========================================================================
// 位置の初期化
//==========================================================================
void CPlayerBase::InitPosition(const MyLib::Vector3& rPos)
{
	// プレイヤー位置の設定
	m_pPlayer->SetPosition(rPos);
}

//==========================================================================
// デバッグ
//==========================================================================
void CPlayerBase::Debug()
{

}

//==========================================================================
// カニ歩き判定
//==========================================================================
bool CPlayerBase::IsCrab()
{
	// ボールを持っていないか
	if (m_pPlayer->GetBall() != nullptr) { return false; }

	// 世界にボールがあるか
	CBall* pBall = CGameManager::GetInstance()->GetBall();	// ボール情報
	if (pBall == nullptr) { return false; }

	// ボールの状態：敵側であるか
	if (pBall->GetTypeTeam() == m_pPlayer->GetTeam())	{ return false; }

	// 敵チームで攻撃判定がある場合はカニ歩き
	if (pBall->IsAttack()) { return true; }

	if (pBall->GetTypeTeam() == CGameManager::ETeamSide::SIDE_NONE)	{ return false; }

	// フリーボールはカニ歩きしない
	if (pBall->GetState() == CBall::EState::STATE_FREE) { return false; }

	// 自身の状態：ブリンク＆走りでない
	CPlayer::EAction action = m_pPlayer->GetActionPattern()->GetAction();	// アクションパターン
	if (action == CPlayer::EAction::ACTION_BLINK) { return false; }
	if (m_pPlayer->IsDash()) { return false; }

	return true;
}

//==========================================================================
//	ベース変更の更新
//==========================================================================
void CPlayerBase::UpdateChangeBase()
{
	if (m_curTypeBase != m_newTypeBase)
	{ // 現在のベースタイプから変更があった場合

		// ベースの変更
		m_pPlayer->ChangeBase(m_newTypeBase);
	}
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
// 操作の設定 (移動)
//==========================================================================
void CPlayerBase::SetPlayerControlMove(CPlayerControlMove* pControlMove)
{
	// 操作クラスの設定
	m_pControlMove = pControlMove;
}

//==========================================================================
// 操作の設定 (アクション)
//==========================================================================
void CPlayerBase::SetPlayerControlAction(CPlayerControlAction* pControlAction)
{
	// 操作クラスの設定
	m_pControlAction = pControlAction;
}
