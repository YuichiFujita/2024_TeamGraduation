//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerAIcontrol_attack.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "teamStatus.h"

// デバッグ用
#include "3D_effect.h"

// player関連
#include "playerBase.h"
#include "playerStatus.h"
#include "playerAIcontrol.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol_move.h"
#include "playerManager.h"
#include "playerAIcontrol_rightAttack.h"
#include "playerAIcontrol_leftAttack.h"

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerAIControlAttack::ATTACKMODE_FUNC CPlayerAIControlAttack::m_AttackModeFunc[] =	// 攻撃モード関数
{
	&CPlayerAIControlAttack::AttackModePreparation,		// 準備
	&CPlayerAIControlAttack::AttackModeAttack,			// 攻撃
};

CPlayerAIControlAttack::PREPARATION_FUNC CPlayerAIControlAttack::m_PreparationFunc[] =	// 攻撃モード関数
{
	&CPlayerAIControlAttack::PreparationNone,			// なし
	&CPlayerAIControlAttack::PreparationWait,			// 待つ
	&CPlayerAIControlAttack::PreparationGo,				// 行く
	&CPlayerAIControlAttack::PreparationLeave,			// 離れる
};

CPlayerAIControlAttack::THROWTYPE_FUNC CPlayerAIControlAttack::m_ThrowTypeFunc[] =	// 投げフラグ関数
{
	&CPlayerAIControlAttack::ThrowTypeNone,				// なし
	&CPlayerAIControlAttack::ThrowTypeNormal,			// 通常
	&CPlayerAIControlAttack::ThrowTypeJump,				// ジャンプ
	&CPlayerAIControlAttack::ThrowTypeSpecial,			// スペシャル
	&CPlayerAIControlAttack::ThrowTypePass,				// パス
};

CPlayerAIControlAttack::THROWFLAG_FUNC CPlayerAIControlAttack::m_ThrowFlagFunc[] =	// 投げフラグ関数
{
	&CPlayerAIControlAttack::ThrowFlagNone,				// なし
	&CPlayerAIControlAttack::ThrowFlag,					// 投げ
	&CPlayerAIControlAttack::ThrowFlagPass,				// パス
	&CPlayerAIControlAttack::ThrowFlagSpecial,			// スペシャル
};

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	// 距離
	const float DISTANSE_LINE = 400.0f;
	const float DISTANSE_TARGET = 100.0f;

	// 終了半径
	const float END_RADIUS = 30.0f;
};

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControlAttack::CPlayerAIControlAttack()
{

}

//==========================================================================
// 生成
//==========================================================================
CPlayerAIControlAttack* CPlayerAIControlAttack::Create(CPlayer* player)
{
	// メモリの確保
	CPlayerAIControlAttack* pObj = nullptr;

	if (!player) {
		assert(("プレイヤー情報が無いよ", false));
	}

	CGameManager::ETeamSide team = player->GetTeam();
	CGameManager::GetInstance()->AssertCheckTeam(team);

	switch (team)
	{// チーム別
	case CGameManager::ETeamSide::SIDE_LEFT:
		pObj = DEBUG_NEW CPlayerAIControlLeftAttack;
		break;

	case CGameManager::ETeamSide::SIDE_RIGHT:
		pObj = DEBUG_NEW CPlayerAIControlRightAttack;
		break;

	default:
		assert(("チームが無いよ", false));
		break;
	}

	if (pObj != nullptr)
	{
		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerAIControlAttack::Init()
{
	// 親クラスの初期化
	CPlayerAIControlMode::Init();

	// 値のクリア
	m_pTarget = nullptr;

	ZeroMemory(&m_sTimig, sizeof(m_sTimig));

	m_eAttackMode = EAttackMode::ATTACKMODE_PREPARATION;
	m_ePreparation = EAttackPrepatarion::ATTACKPREPATARION_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
	m_eThrowFlag = EThrowFlag::THROW_NONE;
	m_eAttackNormal = EAttackNormal::ATTACK_NONE;

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAIControlAttack::Uninit()
{
	// 親クラスの終了処理
	CPlayerAIControlMode::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIControlAttack::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsStop()) return;

	if (!m_pTarget)
	{// ターゲットが設定されていない場合
		m_pTarget = GetThrowTarget();		// ターゲットの取得
	}
	else
	{
		if (m_ePreparation != EAttackPrepatarion::ATTACKPREPATARION_LEAVE)
		{// 離れる状態以外の場合
			// 見る
			SeeTarget(m_pTarget->GetPosition());
		}

		// 投げ種類の更新
		(this->*(m_AttackModeFunc[m_eAttackMode]))(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 親クラスの更新（最後尾に設置）
	CPlayerAIControlMode::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// 攻撃モード：準備
//================================================================================
void CPlayerAIControlAttack::AttackModePreparation(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_ePreparation == EAttackPrepatarion::ATTACKPREPATARION_NONE)
	{// 準備状態が設定されていない場合

		// 自分とターゲットとの距離を算出
		float fDistance = GetPlayer()->GetPosition().DistanceXZ(m_pTarget->GetPosition());

		/*if (fDistance < 100.0f)
		{
			m_ePreparation = EAttackPrepatarion::ATTACKPREPATARION_LEAVE;
		}
		else*/
		{
			m_ePreparation = EAttackPrepatarion::ATTACKPREPATARION_GO;
		}

		//m_ePreparation = EAttackPrepatarion::ATTACKPREPATARION_WAIT;
	}

	// 準備の更新
	(this->*(m_PreparationFunc[m_ePreparation]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// 準備：待つ
//================================================================================
void CPlayerAIControlAttack::PreparationWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// タイマーセット
	SetAttackTimer(1, 2);

	// タイマー更新
	if (IsUpdateAttackTimer(fDeltaTime, fDeltaRate, fSlowRate))
	{// タイマーが終わったら投げる
		m_eAttackMode = EAttackMode::ATTACKMODE_ATTACK;
	}
}

//================================================================================
// 準備：直ぐ
//================================================================================
void CPlayerAIControlAttack::PreparationGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_eAttackMode = EAttackMode::ATTACKMODE_ATTACK;
}

#if 0	// オーバーライド
//================================================================================
// 準備：離れる
//================================================================================
void CPlayerAIControlAttack::PreparationLeave()
{

}
#endif

//================================================================================
// 攻撃モード：攻撃
//================================================================================
void CPlayerAIControlAttack::AttackModeAttack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 攻撃更新
	UpdateAttack(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// 攻撃更新
//================================================================================
void CPlayerAIControlAttack::UpdateAttack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	if (m_eThrowType == EThrowType::THROWTYPE_NONE)
	{
		// スペシャルゲージの取得
		CGameManager* pGameManager = CGameManager::GetInstance();						// ゲームマネージャ
		CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(pAI->GetTeam());			// 自分チームのスペシャル情報
		CTeamStatus* pTeamStatus1 = pGameManager->GetTeamStatus(m_pTarget->GetTeam());	// 相手チームのスペシャル情報

		float fSpecialValue = pTeamStatus->GetSpecialValue();
		float fSpecialValue1 = pTeamStatus1->GetSpecialValue();

		if (pTeamStatus->IsMaxSpecial())
		{// ゲージが溜まっていたら
			// スペシャル投げ
			m_eThrowType = EThrowType::THROWTYPE_SPECIAL;
		}
		else
		{
			// ターゲットとの距離を取得
			float distanse = GetPlayer()->GetPosition().DistanceXZ(m_pTarget->GetPosition());

			// 投げタイプの設定
			m_eThrowType = (EThrowType)UtilFunc::Transformation::Random(EThrowType::THROWTYPE_NORMAL, EThrowType::THROWTYPE_JUMP);
		}
	}

	// 投げの更新
	UpdateThrow(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// 投げ更新
//================================================================================
void CPlayerAIControlAttack::UpdateThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 投げ種類の更新
	(this->*(m_ThrowTypeFunc[m_eThrowType]))(fDeltaTime, fDeltaRate, fSlowRate);

	// 投げ更新
	(this->*(m_ThrowFlagFunc[m_eThrowFlag]))();
}

//--------------------------------------------------------------------------
// 通常投げ
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowTypeNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ターゲットとの距離を取得
	float distanse = GetPlayer()->GetPosition().DistanceXZ(m_pTarget->GetPosition());

	if (m_eAttackNormal == EAttackNormal::ATTACK_NONE)
	{// 設定されていない場合
		m_eAttackNormal = (EAttackNormal)UtilFunc::Transformation::Random(EAttackNormal::ATTACK_THROW, EAttackNormal::ATTACK_DASH);
	}

	switch (m_eAttackNormal)
	{
	case EAttackNormal::ATTACK_THROW:	// 通常

		if (distanse > 400.0f)
		{
			// 走り投げ
			AttackDash(m_pTarget, DISTANSE_TARGET, DISTANSE_LINE);
			return;
		}
		// 通常投げ
		AttackNormal(m_pTarget, fDeltaTime, fDeltaRate, fSlowRate);
		break;

	case EAttackNormal::ATTACK_DASH:	// ジャンプ
		// 走り投げ
		AttackDash(m_pTarget, DISTANSE_TARGET, DISTANSE_LINE);
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------------------------------
// ジャンプ投げ
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowTypeJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ターゲットとの距離を取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;
	MyLib::Vector3 pos = pAI->GetPosition();

	float distanse = pAI->GetPosition().DistanceXZ(m_pTarget->GetPosition());

	if (distanse > 600.0f)
	{
		m_eThrowType = EThrowType::THROWTYPE_NORMAL;
		return;
	}

	// 走りジャンプ投げ
	AttackDashJump(m_pTarget, DISTANSE_TARGET, DISTANSE_LINE, false);
}

//--------------------------------------------------------------------------
// スペシャル
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowTypeSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_eThrowFlag = EThrowFlag::THROW_SPECIAL;
}

//--------------------------------------------------------------------------
// パス
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowTypePass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_eThrowFlag = EThrowFlag::THROW_PASS;
}

//--------------------------------------------------------------------------
// 投げる
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowFlag()
{
	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// 投げる
	pControlAIAction->SetIsThrow(true);
}

//--------------------------------------------------------------------------
// パス
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowFlagPass()
{
	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// パス
	pControlAIAction->SetIsPass(true);
}

//--------------------------------------------------------------------------
// スペシャル
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowFlagSpecial()
{
	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// スペシャル投げ
	pControlAIAction->SetIsSpecial(true);
}

//================================================================================
// キャンセル判断
//================================================================================
bool CPlayerAIControlAttack::IsCancelJumpAttack()
{
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	// モーション取得
	CMotion* pMotion = pAI->GetMotion();
	int nType = pMotion->GetType();

	if (nType == CPlayer::EMotion::MOTION_JUMP ||			// ジャンプ状態
		nType == CPlayer::EMotion::MOTION_JUMP_BALL)		// ジャンプ状態(ボール所持)
	{
		// 自分とターゲットとの距離を算出
		float fDistance = GetPlayer()->GetPosition().DistanceXZ(m_pTarget->GetPosition());
		
		if (fDistance < 500.0f)
		{
			int n = rand() % 2;

			//if (n == 0)
			{
				// 離れる
				m_ePreparation = EAttackPrepatarion::ATTACKPREPATARION_LEAVE;

				return true;
			}
		}
	}

	return false;
}

//================================================================================
// ノーマル投げ
//================================================================================
void CPlayerAIControlAttack::AttackNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 攻撃までの時間を設定
	SetAttackTimer(0, 2);

	// タイマー更新
	if (IsUpdateAttackTimer(fDeltaTime, fDeltaRate, fSlowRate))
	{// タイマーが終わったら投げる
		m_eThrowFlag = EThrowFlag::THROW_NORMAL;
	}
}

//================================================================================
// ジャンプ投げ
//================================================================================
void CPlayerAIControlAttack::AttackJump(CPlayer* pTarget)
{
	// 自分の情報
	CPlayer* pMy = GetPlayer();
	if (!pMy) return;
	MyLib::Vector3 posMy = pMy->GetPosition();

	// アクション：跳ぶ
	SetActionFlag(EActionFlag::ACTION_JUMP);

	// 行動：止まる
	SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);

	CMotion* pMotion = pMy->GetMotion();
	int nType = pMotion->GetType();
	if (nType == CPlayer::EMotion::MOTION_JUMP_BALL)
	{
		// ジャンプ割合の取得
		float rate = GetJumpRate();

		if (rate >= 0.0f)
		{
			m_eThrowFlag = EThrowFlag::THROW_NORMAL;
		}
	}
}

//================================================================================
// ダッシュ投げ
//================================================================================
void CPlayerAIControlAttack::AttackDash(CPlayer* pTarget, float fTargetDis, float fLineDis)
{
	// ターゲットの取得
	if (!pTarget) return;
	MyLib::Vector3 posTarget = pTarget->GetPosition();

	// 自分の情報
	CPlayer* pMy = GetPlayer();
	if (!pMy) return;
	MyLib::Vector3 posMy = pMy->GetPosition();

	// ラインの位置
	MyLib::Vector3 linePos = { 0.0f, posMy.y, posMy.z };

	// 距離計算
	float distanceTarget = posMy.DistanceXZ(pTarget->GetPosition());	// 自分と相手の距離
	float distanceLine = posMy.DistanceXZ(linePos);						// 自分と中心線との距離

	MyLib::Vector3 pos = { posTarget.x, posMy.y, posMy.z };
	
	if (distanceTarget > fTargetDis &&	// ターゲットとの距離が近いかどうか
		distanceLine > fLineDis)		// 中央線との距離が近いかどうか
	{
		// 走る
		SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

		// 相手の位置に近づく
		if (Approatch(pos, END_RADIUS))
		{// 範囲内の場合
			SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);		// 行動：止まる
		}

		return;
	}

	SetThrowFlag(EThrowFlag::THROW_NORMAL);		// 投げフラグ：投げる
}

//================================================================================
// ダッシュジャンプ投げ
//================================================================================
void CPlayerAIControlAttack::AttackDashJump(CPlayer* pTarget, float fTargetDistanse, float LineDistanse, bool bOver)
{
	// ターゲットの取得
	if (!pTarget) return;
	MyLib::Vector3 posTarget = pTarget->GetPosition();

	// 自分の情報
	CPlayer* pMy = GetPlayer();
	if (!pMy) return;
	MyLib::Vector3 posMy = pMy->GetPosition();

	// ラインの位置
	MyLib::Vector3 linePos = { 0.0f, posMy.y, posMy.z };

	// ターゲットのエリアの取得
	CGameManager::ETeamSide side = pMy->GetTeam();
	CGameManager::GetInstance()->AssertCheckTeam(side);

	float distanceTarget = posMy.DistanceXZ(pTarget->GetPosition());	// 自分と相手の距離
	float distanceLine = posMy.DistanceXZ(linePos);						// 自分と中心線との距離
	
	if (distanceTarget > fTargetDistanse &&		// ターゲットとの距離
		distanceLine > LineDistanse)			// 線との距離
	{
		// 走る
		SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

		// 相手の位置に近づく
		if (Approatch(posTarget, END_RADIUS))
		{// 範囲内の場合

			//if (!bOver)
			//{
			//	// 止まる
			//	SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
			//}
		}

		return;
	}

	SetActionFlag(EActionFlag::ACTION_JUMP);	// アクション：跳ぶ

	CMotion* pMotion = pMy->GetMotion();
	int nType = pMotion->GetType();
	if (nType == CPlayer::EMotion::MOTION_JUMP_BALL)
	{
		// ジャンプ割合の取得
		float rate = GetJumpRate();

		if (rate >= 0.0f)
		{
			m_eThrowFlag = EThrowFlag::THROW_NORMAL;
		}
	}
}

//==========================================================================
// フェイント
//==========================================================================
void CPlayerAIControlAttack::AttackFeint()
{
	// 準備：なし
	m_ePreparation = EAttackPrepatarion::ATTACKPREPATARION_NONE;

	// 攻撃モード：準備
	m_eAttackMode = EAttackMode::ATTACKMODE_PREPARATION;
}

//==========================================================================
// 離れる : Leave(離れる相手、離れる距離)
//==========================================================================
bool CPlayerAIControlAttack::Leave(MyLib::Vector3 targetPos, float distance)
{
	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 自分の位置情報
	MyLib::Vector3 posMy = pAI->GetPosition();

	// 相手と自分の距離
	float length = posMy.DistanceXZ(targetPos);	// 距離：相手と自分

	if (distance < length) {// 移動タイプ：無
		return true;
	}

	// ターゲットから自分の角度
	float direction = targetPos.AngleXZ(posMy);

	// ボール持ち主の取得
	CPlayer* pPlayerBall = GetBallOwner();

	if (pPlayerBall)
	{// ボールが存在しない
		if (pPlayerBall->GetTeam() != pAI->GetTeam() &&
			pPlayerBall != pAI) {// 違うチームが持っている && 持ち主が自分以外場合

			if (GetMoveFlag() == EMoveFlag::MOVEFLAG_DASH)
			{// 行動：走る
				// 角度設定
				pAI->SetRotDest(direction);
				return false;
			}

			// カニ進行方向の設定
			pControlAIMove->SetClabDirection(direction);
			return false;
		}
	}

	// 角度設定
	pAI->SetRotDest(direction);

	return false;
}

//==========================================================================
// 近づく : Approatch(近づく相手、近づく距離)
//==========================================================================
bool CPlayerAIControlAttack::Approatch(MyLib::Vector3 targetPos, float distance)
{
	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ボール情報の取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	// 自分の位置情報
	MyLib::Vector3 posMy = pAI->GetPosition();

	// 相手と自分の距離
	float length = posMy.DistanceXZ(targetPos);	// 距離：相手と自分

	if (distance > length) {// 移動タイプ：無
		return true;
	}

	// 自分からターゲットの角度
	float direction = posMy.AngleXZ(targetPos);

	// ボール持ち主の取得
	CPlayer* pPlayerBall = GetBallOwner();

	if (pPlayerBall)
	{// ボールが存在しない
		if (pPlayerBall->GetTeam() != pAI->GetTeam() &&
			pPlayerBall != pAI) {// 違うチームが持っている && 持ち主が自分以外場合

			if (GetMoveFlag() == EMoveFlag::MOVEFLAG_DASH)
			{// 行動：走る
				// 角度設定
				pAI->SetRotDest(direction);
				return false;
			}

			// カニ進行方向の設定
			pControlAIMove->SetClabDirection(direction);
			return false;
		}
	}

	// 角度設定
	pAI->SetRotDest(direction);

	return false;
}

//==========================================================================
// ボール持ち主の取得（敵の場合）
//==========================================================================
CPlayer* CPlayerAIControlAttack::GetBallOwner()
{
	CPlayer* pTarget = nullptr;	// 目標ターゲット

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return pTarget;

	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) { return pTarget; }
	CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// チームタイプの取得

	// ボールを持っている奴と自分が同じチームの場合
	if (typeTeam == pAI->GetTeam()) return pTarget;

	// ターゲットの設定
	pTarget = pBall->GetPlayer();
	if (!pTarget) return nullptr;

	return pTarget;
}

//==========================================================================
// ターゲット設定
//==========================================================================
CPlayer* CPlayerAIControlAttack::GetThrowTarget()
{
	CPlayer* pTarget = nullptr;			// 最終的なターゲット
	CPlayer* pTargetLength = nullptr;	// 距離ターゲットの保存用
	CPlayer* pTargetLife = nullptr;		// 体力ターゲットの保存用
	float fMinDis = 1000000.0f;	// 近いプレイヤー
	int nMinLife = 1000000;

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return pTarget;

	MyLib::Vector3 Mypos = pAI->GetPosition();	// 位置情報の取得

	// ボールの取得
	CBall* pBall = pAI->GetBall();
	if (pBall == nullptr) { return pTarget; }
	CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// チームタイプの取得

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す
		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// プレイヤー位置

		if (typeTeam == pPlayer->GetTeam() ||
			pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT||
			pPlayer->GetMotionFrag().bDead)
		{// 同じチーム||外野||死亡している場合
			continue;
		}

		// 敵との距離を求める
		float fLength = Mypos.DistanceXZ(posPlayer);

		// 体力
		int nLife = pPlayer->GetLife();

		if (nLife < nMinLife)
		{// より体力の少ないプレイヤーがいた場合

			// 最小体力の更新
			nMinLife = nLife;

			// ターゲットを更新
			pTargetLife = pPlayer;
		}

		if (fLength < fMinDis)
		{ // より近い相手プレイヤーがいた場合

			// 最小距離の更新
			fMinDis = fLength;

			// ターゲットを更新
			pTargetLength = pPlayer;
		}
	}

	int i = rand() % 2;

	if (i == 0)
	{
		pTarget = pTargetLife;
	}
	else
	{
		pTarget = pTargetLength;
	}

	return pTarget;
}

//==========================================================================
// ターゲットを見る
//==========================================================================
void CPlayerAIControlAttack::SeeTarget(MyLib::Vector3 pos)
{
	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// 向き設定
	float angle = pAI->GetPosition().AngleXZ(pos);
	pAI->SetRotDest(angle);
}

//================================================================================
// 行動してはいけない判定
//================================================================================
bool CPlayerAIControlAttack::IsStop()
{
	CPlayer* pPlayer = GetPlayer();
	CPlayer::EState state = pPlayer->GetState();

	if (state == CPlayer::EState::STATE_CATCH_NORMAL ||
		state == CPlayer::EState::STATE_CATCH_JUST ||
		state == CPlayer::EState::STATE_CATCH_SPECIAL)
	{
		return true;
	}

	return false;
}

//==========================================================================
// 更新：アクションタイマー
//==========================================================================
bool CPlayerAIControlAttack::IsUpdateAttackTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sTimig.bSet)
	{
		// タイマーのカウントダウン
		m_sTimig.fTimer -= fDeltaTime * fDeltaRate * fSlowRate;

		if (m_sTimig.fTimer <= 0.0f)
		{
			// 行動の構造体初期化
			ZeroMemory(&m_sTimig, sizeof(m_sTimig));

			// 投げる
			m_eThrowFlag = EThrowFlag::THROW_NORMAL;

			return true;
		}
	}

	return false;
}

//==========================================================================
// アクション時間の設定
//==========================================================================
void CPlayerAIControlAttack::SetAttackTimer(int nMin, int nMax)
{
	// 角度の設定
	if (!m_sTimig.bSet)
	{// 設定されていない場合

		// 行動時間の設定
		float fRand = (float)UtilFunc::Transformation::Random(nMin, nMax);
		m_sTimig.fTimer = fRand;

		// 時間設定ON
		m_sTimig.bSet = true;
	}
}
