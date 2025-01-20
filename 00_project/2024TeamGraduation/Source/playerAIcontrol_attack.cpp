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
	& CPlayerAIControlAttack::PreparationGo,			// なし
	&CPlayerAIControlAttack::PreparationLeave,			// 離れる
};

CPlayerAIControlAttack::THROWTYPE_FUNC CPlayerAIControlAttack::m_ThrowTypeFunc[] =	// 投げフラグ関数
{
	&CPlayerAIControlAttack::ThrowTypeNone,
	&CPlayerAIControlAttack::ThrowTypeNormal,
	&CPlayerAIControlAttack::ThrowTypeJump,
	&CPlayerAIControlAttack::ThrowTypeSpecial,
};

CPlayerAIControlAttack::THROWFLAG_FUNC CPlayerAIControlAttack::m_ThrowFlagFunc[] =	// 投げフラグ関数
{
	&CPlayerAIControlAttack::ThrowFlagNone,			// なし
	&CPlayerAIControlAttack::ThrowFlag,				// 投げ
	&CPlayerAIControlAttack::ThrowFlagPass,			// パス
	&CPlayerAIControlAttack::ThrowFlagSpecial,		// スペシャル
};

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

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
	switch (player->GetTeam())
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
	m_eAttackMode = EATTACKMODE::ATTACKMODE_PREPARATION;
	m_ePreparation = EATTACKPREPATARION::ATTACKPREPATARION_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
	m_eThrowFlag = EThrowFlag::THROW_NONE;

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAIControlAttack::Uninit()
{
	CPlayerAIControlMode::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIControlAttack::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//SeeTarget();

	// 投げ種類の更新
	(this->*(m_AttackModeFunc[m_eAttackMode]))();

	// 親クラスの更新（最後尾に設置）
	CPlayerAIControlMode::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// 攻撃モード：準備
//================================================================================
void CPlayerAIControlAttack::AttackModePreparation()
{
	if (m_ePreparation == EATTACKPREPATARION::ATTACKPREPATARION_NONE)
	{
		int n = rand() % 2;

		switch (n)
		{
		case 0:	// 通常
			m_ePreparation = EATTACKPREPATARION::ATTACKPREPATARION_GO;
			break;

		case 1:	// ジャンプ
			m_ePreparation = EATTACKPREPATARION::ATTACKPREPATARION_LEAVE;
			break;

		default:
			assert(false);
			break;
		}
	}

	// 準備の更新
	(this->*(m_PreparationFunc[m_ePreparation]))();
}

//================================================================================
// 準備：直ぐ
//================================================================================
void CPlayerAIControlAttack::PreparationGo()
{
	m_eAttackMode = EATTACKMODE::ATTACKMODE_ATTACK;
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
void CPlayerAIControlAttack::AttackModeAttack()
{
	// 攻撃更新
	UpdateAttack();
}

//================================================================================
// 攻撃更新
//================================================================================
void CPlayerAIControlAttack::UpdateAttack()
{
	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// 取得
	CGameManager* pGameManager = CGameManager::GetInstance();	// ゲームマネージャ
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(pAI->GetTeam());	// 自分のチーム情報

	if (m_eThrowType == EThrowType::THROWTYPE_NONE)
	{
		if (pTeamStatus->IsMaxSpecial())
		{// ゲージが溜まっていたら
			// スペシャル投げ
			m_eThrowType = EThrowType::THROWTYPE_SPECIAL;
		}
		else
		{// 投げろフラグがオンだったら
			//int n = 1;
			// 今はランダムで決定
			int n = rand() % 2;

			switch (n)
			{
			case 0:	// 通常
				m_eThrowType = EThrowType::THROWTYPE_NORMAL;
				break;

			case 1:	// ジャンプ
				m_eThrowType = EThrowType::THROWTYPE_JUMP;
				break;

			default:
				assert(false);
				break;
			}
		}
	}

	// 投げの更新
	UpdateThrow();
}

//================================================================================
// 投げ更新
//================================================================================
void CPlayerAIControlAttack::UpdateThrow()
{
	// 投げ種類の更新
	(this->*(m_ThrowTypeFunc[m_eThrowType]))();

	// 投げ更新
	(this->*(m_ThrowFlagFunc[m_eThrowFlag]))();
}

//================================================================================
// ダッシュ投げ
//================================================================================
void CPlayerAIControlAttack::AttackDash(CPlayer* pTarget)
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

	// ラインとの距離
	float distanceLine = 0.0f;
	float distanceTarget = 0.0f;
	float JUMP_LENGTH_TARGET = 100.0f;
	float JUMP_LENGTH_LINE = 200.0f;

	// ターゲットのエリアの取得
	CGameManager::ETeamSide side = pMy->GetTeam();

	if (pTarget)
	{// ターゲットがいた場合
		distanceTarget = posMy.DistanceXZ(pTarget->GetPosition());	// 自分と相手の距離
		distanceLine = posMy.DistanceXZ(linePos);	// 自分と中心線との距離
	}
	else
	{
		return;
	}

	if (distanceTarget > JUMP_LENGTH_TARGET && distanceLine > JUMP_LENGTH_LINE)
	{// 自分とターゲットの距離が700.0f以上&&中央線との距離が範囲以上の場合

		// 走る
		SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

		// 相手の位置に近づく
		if (Approatch(posTarget, JUMP_LENGTH_LINE))
		{// 範囲内の場合
			SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);			// 行動：止まる
		}

		return;
	}

	SetThrowFlag(EThrowFlag::THROW_NORMAL);		// 投げ：投げる
}

//================================================================================
// ダッシュジャンプ投げ
//================================================================================
void CPlayerAIControlAttack::AttackDashJump(CPlayer* pTarget)
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

	// ラインとの距離
	float distanceLine = 0.0f;
	float distanceTarget = 0.0f;
	float JUMP_LENGTH_TARGET = 500.0f;
	float JUMP_LENGTH_LINE = 300.0f;

	// ターゲットのエリアの取得
	CGameManager::ETeamSide side = pMy->GetTeam();

	if (pTarget)
	{// ターゲットがいた場合
		distanceTarget = posMy.DistanceXZ(pTarget->GetPosition());	// 自分と相手の距離
		distanceLine = posMy.DistanceXZ(linePos);	// 自分と中心線との距離
	}
	else
	{
		return;
	}

	if (distanceTarget > JUMP_LENGTH_TARGET && distanceLine > JUMP_LENGTH_LINE)
	{// 自分とターゲットの距離が700.0f以上&&中央線との距離が範囲以上の場合

		// 走る
		SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

		// 相手の位置に近づく
		if (Approatch(posTarget, JUMP_LENGTH_LINE))
		{// 範囲内の場合
			SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);			// 行動：止まる
		}

		return;
	}

	if (distanceTarget > JUMP_LENGTH_TARGET/* && distanceLine > JUMP_LENGTH_LINE*/)
	{// ターゲットとの距離が範囲以上&&中央線との距離が範囲内の場合
		SetActionFlag(EActionFlag::ACTION_JUMP);	// アクション：跳ぶ
	}

	if (pMy->GetPosition().y >= playerAIcontrol::THROW_JUMP_END)	// 高さによって変わる
	{
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);		// 行動：止まる
		SetThrowFlag(EThrowFlag::THROW_NORMAL);		// 投げ：投げる
	}
}

//--------------------------------------------------------------------------
// 通常投げ
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowTypeNormal()
{
	m_eThrowFlag = EThrowFlag::THROW_NORMAL;
}

//--------------------------------------------------------------------------
// ジャンプ投げ
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowTypeJump()
{
	// その場なのか歩くのか走るのか
	//m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

	CPlayer* pTartget = GetThrowTarget();

	// 走り投げ
	AttackDashJump(pTartget);
}

//--------------------------------------------------------------------------
// スペシャル投げ
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowTypeSpecial()
{
	m_eThrowFlag = EThrowFlag::THROW_SPECIAL;
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

	// 投げ状態：無
	m_eThrowFlag = EThrowFlag::THROW_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
}

//--------------------------------------------------------------------------
// パス 処理書いてない
//--------------------------------------------------------------------------
void CPlayerAIControlAttack::ThrowFlagPass()
{

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

	// 投げなし
	m_eThrowFlag = EThrowFlag::THROW_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
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

	// 見る
	SeeTarget(pTarget->GetPosition());

	return pTarget;
}

//==========================================================================
// ターゲット設定
//==========================================================================
CPlayer* CPlayerAIControlAttack::GetThrowTarget()
{
	CPlayer* pTarget = nullptr;	// 目標ターゲット
	float fMinDis = 1000000.0f;	// 近いプレイヤー

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

		// 同じチーム||外野の場合
		if ((typeTeam == pPlayer->GetTeam()) ||
			(pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT))
		{
			continue;
		}

		// 敵との距離を求める
		float fLength = Mypos.DistanceXZ(posPlayer);

		if (fLength < fMinDis)
		{ // より近い相手プレイヤーがいた場合

			// 最小距離の更新
			fMinDis = fLength;

			// ターゲットを更新
			pTarget = pPlayer;

			// 見る
			SeeTarget(pTarget->GetPosition());
		}
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
