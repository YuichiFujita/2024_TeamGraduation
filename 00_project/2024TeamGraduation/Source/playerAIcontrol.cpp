//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerAIcontrol.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "teamStatus.h"


// player関連
#include "playerBase.h"
#include "playerStatus.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol_move.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace timing
{
	// タイミング
	const float TIMING_NORMAL = 1.0f;	// 通常
	const int TIMING_RAND_MAX = 100;	// randMAX値
	const int TIMING_RAND_MIN = -80;	// randMIN値
}

namespace
{
	// ジャンプ投げの最大位置
	const float JUMP_END_POS = 140.0f;

	// 距離を取る長さ
	const float LENGTH_MAX = 400.0f;
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerAIControl::MODE_FUNC CPlayerAIControl::m_ModeFunc[] =	// モード関数
{
	&CPlayerAIControl::ModeNone,				// なし
	&CPlayerAIControl::ModeThrowManager,		// 投げ
	&CPlayerAIControl::ModeCatchManager,		// キャッチ
};

CPlayerAIControl::THOWTYPE_FUNC CPlayerAIControl::m_ThrowTimingFunc[] =	// 投げタイプ関数
{
	&CPlayerAIControl::ThrowJumpTimingNormal,		// 通常
	&CPlayerAIControl::ThrowJumpTimingQuick,		// 速
	&CPlayerAIControl::ThrowJumpTimingDelay,		// 遅
};

CPlayerAIControl::CATCH_FUNC CPlayerAIControl::m_CatchFunc[] =	// キャッチ関数
{
	&CPlayerAIControl::CatchNone,
	&CPlayerAIControl::CatchNormal,
	&CPlayerAIControl::CatchJust,
	&CPlayerAIControl::CatchDash,
	&CPlayerAIControl::FindBall,
};

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControl::CPlayerAIControl()
{
	// メンバ変数の初期化
	ZeroMemory(&m_sInfo, sizeof(m_sInfo));
	m_pAI = nullptr;
}

//==========================================================================
// 生成
//==========================================================================
CPlayerAIControl* CPlayerAIControl::Create(CPlayer* player)
{
	// メモリの確保
	CPlayerAIControl* pObj = DEBUG_NEW CPlayerAIControl;
	if (pObj != nullptr)
	{
		pObj->m_pAI = player;

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
HRESULT CPlayerAIControl::Init()
{
	
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAIControl::Uninit()
{
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIControl::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モード管理
	ModeManager();

	// 状態更新
	(this->*(m_ModeFunc[m_sInfo.sMode.eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// モード管理
//==========================================================================
void CPlayerAIControl::ModeManager()
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() == nullptr ||
		pBall->GetTypeTeam() != m_pAI->GetStatus()->GetTeam())
	{// ボールが取得されていない場合||自分とボールを持っているチームが違う場合
		m_sInfo.sMode.eMode = EMode::MODE_CATCH;
	}
	else if (pBall->GetPlayer() == m_pAI)
	{// ボールを持っているのが自分だった場合
		m_sInfo.sMode.eMode = EMode::MODE_THROW;
	}
}

//==========================================================================
// 投げの管理
//==========================================================================
void CPlayerAIControl::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 投げの流れ
	PlanThrowFlow(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ターゲット設定
//==========================================================================
void CPlayerAIControl::SetThrowTarget(CPlayer** ppTarget)
{
	CPlayer* pTarget = nullptr;	// 目標ターゲット
	float fMinDis = 1000000.0f;	// 近いプレイヤー

	MyLib::Vector3 pos = m_pAI->GetPosition();	// 位置情報の取得

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// プレイヤー位置

		// ボールの取得
		CBall* pBall = m_pAI->GetBall();
		if (pBall == nullptr) { return; }
		CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// チームタイプの取得

		// 同じチームの場合次へ
		if (typeTeam == pPlayer->GetStatus()->GetTeam()) { continue; }

		// 敵との距離を求める
		float fLength = sqrtf(
			(pPlayer->GetPosition().x - pos.x) * (pPlayer->GetPosition().x - pos.x) +
			(pPlayer->GetPosition().z - pos.z) * (pPlayer->GetPosition().z - pos.z));

		if (fLength < fMinDis)
		{ // より近い相手プレイヤーがいた場合

			// ターゲットを更新
			pTarget = pPlayer;
			*ppTarget = pTarget;

			// 方向設定
			m_pAI->SetRotDest(pos.AngleXZ(pTarget->GetPosition()));
		}
	}
}

//==========================================================================
// 投げる距離
//==========================================================================
void CPlayerAIControl::DistanceThrow()
{

}

//==========================================================================
// 何投げるまでの流れを考える
//==========================================================================
void CPlayerAIControl::PlanThrowFlow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ターゲット
	CPlayer* pTarget = nullptr;

	// ターゲットを決める関数	// 常
	SetThrowTarget(&pTarget);

	// 距離を決める関数			// 常
	PlanThrowDistance(pTarget);

	// ジャンプするorｓしない関数	// 一瞬
	PlanIsJump(pTarget);

	// 歩かないor歩くor走る関数		// 常

	// ライン越え
	LineOverPlayer();	// 常

	// 何を投げるか考える関数	// 常
	PlanThrow(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 心のプラン
//==========================================================================
void CPlayerAIControl::PlanHeart()
{
	// 体力の取得
	int nLife = m_pAI->GetParameter().nLife;

	// 残り体力で心持を決める

}

//==========================================================================
// 何を投げるか
//==========================================================================
void CPlayerAIControl::PlanThrow(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 投げてよし！&&タイミング設定済なら
	if (!m_sInfo.sThrowInfo.bThrow && m_sInfo.sThrowInfo.bTiming) return;

	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	CGameManager* pGameManager = CGameManager::GetInstance();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(m_pAI->GetStatus()->GetTeam());

	if (pTeamStatus->IsMaxSpecial())
	{// ゲージが溜まっていたら
		// スペシャル投げ
		pControlAIAction->SetIsSpecial(true);
	}
	else
	{// 投げろフラグがオンだったら
		int n = 0;
		// 今はランダムで決定
		n = rand() % 2;
		//n = 1;

		switch (n)
		{
		case 0:
			pControlAIAction->SetIsThrow(true);
			break;

		case 1:
			JumpThrowTiming(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
			break;

		default:
			break;
		}

		m_sInfo.sThrowInfo.bTiming = true;
	}

}

//==========================================================================
// 距離プラン
//==========================================================================
void CPlayerAIControl::PlanThrowDistance(CPlayer* pTarget)
{
	if (pTarget == nullptr) return;

	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 距離を見る
	MyLib::Vector3 posTarget = pTarget->GetPosition();
	MyLib::Vector3 posMy = m_pAI->GetPosition();

	// 自分から相手の距離
	float fDistance = posMy.DistanceXZ(posTarget);

	if (fDistance < LENGTH_MAX - 50.0f)
	{// 離れろ！
		m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_LEAVE;
		
		// 相手から自分の方向
		m_pAI->SetRotDest(posTarget.AngleXZ(posMy));

		// 歩け！
		pControlAIMove->SetIsWalk(true);

		return;
	}
	else if(fDistance > LENGTH_MAX + 50.0f)
	{// 近づけ！
		m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_APPROATCH;

		// 相手から自分の方向
		m_pAI->SetRotDest(posMy.AngleXZ(posTarget));

		// 歩け！
		pControlAIMove->SetIsWalk(true);

		return;
	}

	// 動くんじゃない！
	m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_NONE;

	// 歩くな！
	pControlAIMove->SetIsWalk(false);

	// 投げてよし！
	m_sInfo.sThrowInfo.bThrow = true;

	// 変数リセットしろ！
	//Reset();
}

//==========================================================================
// 跳ぶかどうか まだ何を参考にするかは未定
//==========================================================================
void CPlayerAIControl::PlanIsJump(CPlayer* pTarget)
{
	if (pTarget == nullptr && !m_sInfo.sMoveInfo.bJump) return;

	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// 距離を見る
	MyLib::Vector3 posTarget = pTarget->GetPosition();
	MyLib::Vector3 posMy = m_pAI->GetPosition();

	// 自分から相手の距離
	float fDistance = posMy.DistanceXZ(posTarget);
	
	if (fDistance > LENGTH_MAX + 50.0f && fDistance < LENGTH_MAX - 50.0f)
	{// 離れていたら
		pControlAIAction->SetIsJump(true);

		m_sInfo.sMoveInfo.bJump = false;
	}
}

//==========================================================================
// 行動プラン
//==========================================================================
void CPlayerAIControl::PlanMove(CPlayer* pTarget)
{
	if (pTarget == nullptr) return;


}

//==========================================================================
// 線から離れる
//==========================================================================
void CPlayerAIControl::LineDistance()
{

}

//==========================================================================
// 線超え(プレイヤー)
//==========================================================================
void CPlayerAIControl::LineOverPlayer()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// チームタイプの取得
	CGameManager::ETeamSide typeTeam = m_pAI->GetStatus()->GetTeam();

	// 距離を見る
	MyLib::Vector3 posTarget = {};
	MyLib::Vector3 posMy = m_pAI->GetPosition();

	// 自分から相手の距離
	float fDistance = posMy.DistanceXZ(posTarget);

	if (typeTeam == CGameManager::ETeamSide::SIDE_LEFT)
	{
		if (fDistance < 100.0f)
		{// 離れろ！
			m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_LEAVE;

			// 相手から自分の方向
			//m_pAI->SetRotDest(posTarget.AngleXZ(posMy));

			// 相手から自分の方向
			m_pAI->SetRotDest(posMy.AngleXZ(posTarget));

			// 歩け！
			pControlAIMove->SetIsWalk(true);

			return;
		}
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{
		if (m_pAI->GetPosition().x < 0.0f)
		{
			// 
		}
	}
}

//==========================================================================
// 線超え判定(ボール)
//==========================================================================
bool CPlayerAIControl::IsLineOverBall()
{
	bool bOver = false;	// 超えた判定用

	// ボール情報取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) { return bOver; }

	CGameManager::ETeamSide typeTeam = m_pAI->GetStatus()->GetTeam();

	if (typeTeam == CGameManager::ETeamSide::SIDE_LEFT)
	{
		if (pBall->GetPosition().x > 0.0f)
		{
			Reset();
			bOver = true;
		}
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{
		if (pBall->GetPosition().x < 0.0f)
		{
			Reset();
			bOver = true;
		}
	}

	return bOver;
}

//==========================================================================
// ジャンプ投げタイミング
//==========================================================================
void CPlayerAIControl::JumpThrowTiming(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 線に対しての思考
	StrategyLine(pTarget);

	// タイミング思考
	StrategyTiming(pTarget);

	// 状態更新
	(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// タイミングの思考 今はRAND
//==========================================================================
void CPlayerAIControl::StrategyTiming(CPlayer* pTarget)
{
	if (m_sInfo.sThrowInfo.bTiming) { return; }

	if (m_sInfo.sThrowInfo.eTiming == EThrowType::THROWTYPE_NORMAL)
	{// 通常投げ
		// ランダムでタイミングを決める
		m_sInfo.sThrowInfo.fTiming = timing::TIMING_NORMAL + UtilFunc::Transformation::Random(timing::TIMING_RAND_MIN, timing::TIMING_RAND_MAX) * 0.01f;

		m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_NORMAL;	// 通常投げ
		m_sInfo.sThrowInfo.bTiming = true;	// タイミングフラグオン
	}
	else if (m_sInfo.sThrowInfo.eTiming == EThrowType::THROWTYPE_JUMP)
	{// ジャンプ投げ

		// 自身からみたターゲットの位置
		float fDistance = m_pAI->GetPosition().DistanceXZ(pTarget->GetPosition());

		while (true)
		{
			// タイミングの設定
			int n = rand() % 3;
			//int n = 3;

			if (n == 0)
			{// 番号0
				m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_NORMAL;
				m_sInfo.sThrowInfo.bTiming = true;
				break;

			}
			else if (n == 1 && fDistance < LENGTH_MAX - 100.0f)
			{// 番号2&&ターゲットとの距離が以内の場合
				m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_QUICK;
				m_sInfo.sThrowInfo.bTiming = true;
				break;
			}
			else if (n == 2 && fDistance < LENGTH_MAX - 100.0f)
			{// 番号1&&ターゲットとの距離が以内の場合

				m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_DELAY;
				m_sInfo.sThrowInfo.bTiming = true;
				break;
			}
		}
	}
}

//==========================================================================
// 線に対しての思考
//==========================================================================
void CPlayerAIControl::StrategyLine(CPlayer* pTarget)
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// チームタイプの取得
	CGameManager::ETeamSide typeTeam = m_pAI->GetStatus()->GetTeam();

	// ターゲット距離(中央)
	MyLib::Vector3 posTarget = {};	// ターゲット位置
	MyLib::Vector3 posMy = m_pAI->GetPosition();	// 自身位置

	// 自分から相手(中央)の距離
	float fDistance = posMy.DistanceXZ(posTarget);

	if (typeTeam == CGameManager::ETeamSide::SIDE_LEFT)
	{// チームが左の場合
		if (fDistance < 100.0f)
		{// 離れろ！
			m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_LEAVE;

			// 相手から自分の方向
			//m_pAI->SetRotDest(posTarget.AngleXZ(posMy));

			// 相手から自分の方向
			m_pAI->SetRotDest(posMy.AngleXZ(posTarget));

			// 歩け！
			pControlAIMove->SetIsWalk(true);

			return;
		}
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{// 右の場合
		if (m_pAI->GetPosition().x < 0.0f)
		{
			// 
		}
	}
}

//--------------------------------------------------------------------------
// 跳び投げ通常タイミング
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowJumpTimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_sInfo.sThrowInfo.fTiming > 0.0f)
	{
		m_sInfo.sThrowInfo.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// 歩きリセット
	pControlAIAction->SetIsJump(true);	// ジャンプオン

	if (m_pAI->GetPosition().y >= JUMP_END_POS)	// 高さによって変わる
	{// カウント0以上
		// 投げる
		pControlAIAction->SetIsThrow(true);


		// 変数リセット
		Reset();
	}
}

//--------------------------------------------------------------------------
// 跳び投げ速いタイミング
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowJumpTimingQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_sInfo.sThrowInfo.fTiming > 0.0f)
	{// カウント0以上
		m_sInfo.sThrowInfo.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// 歩きリセット
	pControlAIAction->SetIsJump(true);	// ジャンプオン

	if (m_pAI->GetPosition().y >= JUMP_END_POS * 0.5f)
	{
		// 投げる
		pControlAIAction->SetIsThrow(true);

		// 変数リセット
		Reset();
	}
}

//--------------------------------------------------------------------------
// 跳び投げ遅いタイミング
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowJumpTimingDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_sInfo.sThrowInfo.fTiming > 0.0f)
	{// カウント0以上
		m_sInfo.sThrowInfo.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;
		return;
	}

	pControlAIMove->SetIsWalk(false);	// 歩きリセット
	pControlAIAction->SetIsJump(true);	// ジャンプオン

	// 位置情報取得
	MyLib::Vector3 pos = m_pAI->GetPosition();

	if (pos.y >= JUMP_END_POS)	// 高さによって変わる
	{
		m_sInfo.sThrowInfo.bFoldJump = true;	// 折り返しオン
	}

	if (!m_sInfo.sThrowInfo.bFoldJump) { return; }

	if (pos.y <= JUMP_END_POS * 0.5f)	// 高さによって変わる
	{
		// 投げる
		pControlAIAction->SetIsThrow(true);

		// それぞれの状態のリセット
		Reset();
	}
}





// キャッチ =========================================================================================================================================================
//==========================================================================
// キャッチの管理
//==========================================================================
void CPlayerAIControl::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() != nullptr) {	// キャッチ状態
		m_sInfo.sCatchInfo.eCatchType = ECatchType::CATCH_TYPE_NORMAL;
	}
	else if (pBall->GetPlayer() == nullptr)
	{
		// ボールを取りに行く
		m_sInfo.sCatchInfo.eCatchType = ECatchType::CATCH_TYPE_FIND;
	}

	// 投げる種類更新
	(this->*(m_CatchFunc[m_sInfo.sCatchInfo.eCatchType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 通常キャッチ
//==========================================================================
void CPlayerAIControl::CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sInfo.sMoveInfo.eType != EMoveType::MOVETYPE_APPROATCH)
	{// 行動タイプが近づく以外の場合
		// 距離を取る
		DistanceLeaveCatch();
	}
	else if (m_sInfo.sMoveInfo.eType != EMoveType::MOVETYPE_LEAVE)
	{// 行動タイプが離れる以外の場合
		// 距離を縮める
		DistanceApproachCatch();
	}
}

//==========================================================================
// ジャストキャッチ
//==========================================================================
void CPlayerAIControl::CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ダッシュキャッチ
//==========================================================================
void CPlayerAIControl::CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// キャッチの時距離を取る
//==========================================================================
void CPlayerAIControl::DistanceLeaveCatch()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	CPlayer* pTarget = nullptr;	// 目標ターゲット
	float fMinDis = LENGTH_MAX;	// 近いプレイヤー

	MyLib::Vector3 pos = m_pAI->GetPosition();	// 位置情報の取得
	CGameManager::ETeamSide myTeam = m_pAI->GetStatus()->GetTeam();

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// プレイヤー位置

		// ボールの取得
		CBall* pBall = CGameManager::GetInstance()->GetBall();
		if (pBall == nullptr) { return; }
		CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// チームタイプの取得

		// 同じチームまたはボールがどのサイドでも無い場合
		if ((typeTeam == m_pAI->GetStatus()->GetTeam()) ||
			(typeTeam == CGameManager::ETeamSide::SIDE_NONE))
		{
			continue;
		}

		// 味方は見ない
		if (pPlayer->GetStatus()->GetTeam() == myTeam) continue;

		// 相手との距離を求める
		float fLength = posPlayer.DistanceXZ(pos);

		if (fLength < fMinDis)
		{ // 数値より近い相手プレイヤーがいた場合

			// 離れる行動状態へ
			m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_LEAVE;

			// ターゲット設定
			pTarget = pPlayer;

			// カニ進行方向の設定
			float direction = pTarget->GetPosition().AngleXZ(pos);
			pControlAIMove->SetClabDirection(direction);

			// 歩きオン
			pControlAIMove->SetIsWalk(true);
		}
		else
		{
			m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_NONE;

			// 歩きオフ
			pControlAIMove->SetIsWalk(false);
		}
	}
}

//==========================================================================
// キャッチの時近づく
//==========================================================================
void CPlayerAIControl::DistanceApproachCatch()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	CPlayer* pTarget = nullptr;	// 目標ターゲット
	float fMinDis = LENGTH_MAX;	// 距離

	MyLib::Vector3 pos = m_pAI->GetPosition();	// 位置情報の取得
	CGameManager::ETeamSide myTeam = m_pAI->GetStatus()->GetTeam();

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// プレイヤー位置

		// ボールの取得
		CBall* pBall = CGameManager::GetInstance()->GetBall();
		if (pBall == nullptr) { return; }
		CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// チームタイプの取得

		// 同じチームまたはボールがどのサイドでも無い場合
		if ((typeTeam == m_pAI->GetStatus()->GetTeam()) ||
			(typeTeam == CGameManager::ETeamSide::SIDE_NONE))
		{
			continue;
		}

		// 味方は見ない
		if (pPlayer->GetStatus()->GetTeam() == myTeam) continue;

		// 相手との距離を求める
		float fLength = posPlayer.DistanceXZ(pos);

		if (fLength > fMinDis)
		{ // 数値より近い相手プレイヤーがいた場合

			// 離れる行動状態へ
			m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_APPROATCH;

			// ターゲット設定
			pTarget = pPlayer;

			// カニ進行方向の設定
			float direction = pos.AngleXZ(pTarget->GetPosition());
			pControlAIMove->SetClabDirection(direction);

			// 歩きオン
			pControlAIMove->SetIsWalk(true);
		}
		else
		{
			m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_NONE;

			// 歩きオフ
			pControlAIMove->SetIsWalk(false);
		}
	}
}

//==========================================================================
// 取りに行く
//==========================================================================
void CPlayerAIControl::FindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsLineOverBall()) { return; }

	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (pBall == nullptr || pBall->GetPlayer() != nullptr) 
	{// ボールがnullptr且つプレイヤーがボールを取っている場合
		Reset();	// 変数リセット
		return;
	}

	// 角度を求める(playerからみたボール)
	float fAngle = m_pAI->GetPosition().AngleXZ(pBall->GetPosition());

	// 歩きオン!
	pControlAIMove->SetIsWalk(true);

	// 方向設定
	m_pAI->SetRotDest(fAngle);
}






//==========================================================================
// 変数のリセット
//==========================================================================
void CPlayerAIControl::Reset()
{
	// 構造体情報リセット
	ZeroMemory(&m_sInfo, sizeof(m_sInfo));

	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 行動フラグリセット
	pControlAIMove->SetIsWalk(false);
	pControlAIMove->SetIsDash(false);
	pControlAIMove->SetIsBlink(false);
	pControlAIAction->SetIsJump(false);
}
