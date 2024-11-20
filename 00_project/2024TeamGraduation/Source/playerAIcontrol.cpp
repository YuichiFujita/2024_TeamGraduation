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

// デバッグ用
#include "3D_effect.h"

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
	// ジャンプ投げの最大位置(ジャンプ力MAX：150)
	const float JUMP_END_POS = 140.0f;

	// 距離間(デフォルト)
	const float LENGTH_IN = 500.0f;
	const float LENGTH_SPACE = 10.0f;
	const float LENGTH_OUT = 200.0f;
	const float LENGTH_LINE = 100.0f;
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

CPlayerAIControl::THROWTYPE_FUNC CPlayerAIControl::m_ThrowTypeFunc[] =	// 投げタイプ関数
{
	&CPlayerAIControl::ThrowTypeNone,			// なし
	&CPlayerAIControl::ThrowTypeNormal,			// 通常
	&CPlayerAIControl::ThrowTypeJump,			// ジャンプ
	&CPlayerAIControl::ThrowTypeSpecial,		// スペシャル
};

CPlayerAIControl::THROWMOVE_FUNC CPlayerAIControl::m_ThrowMoveFunc[] =	// 投げタイプ関数
{
	&CPlayerAIControl::ThrowMoveNone,			// その場
	&CPlayerAIControl::ThrowMoveWalk,			// 歩き
	&CPlayerAIControl::ThrowMoveDash,			// 走り
};

CPlayerAIControl::THROWTIMING_FUNC CPlayerAIControl::m_ThrowTimingFunc[] =	// 投げタイミング関数
{
	&CPlayerAIControl::ThrowJumpTimingNone,		// なし
	&CPlayerAIControl::ThrowJumpTimingNormal,	// 通常
	&CPlayerAIControl::ThrowJumpTimingQuick,	// 速
	&CPlayerAIControl::ThrowJumpTimingDelay,	// 遅
};

CPlayerAIControl::CATCH_FUNC CPlayerAIControl::m_CatchFunc[] =	// キャッチ関数
{
	&CPlayerAIControl::CatchNone,			// なし
	&CPlayerAIControl::CatchNormal,			// 通常
	&CPlayerAIControl::CatchJust,			// ジャスト
	&CPlayerAIControl::CatchDash,			// ダッシュ
	&CPlayerAIControl::FindBall,			// 見つける
};

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControl::CPlayerAIControl()
{
	// メンバ変数の初期化
	ZeroMemory(&m_sInfo, sizeof(m_sInfo));
	ZeroMemory(&m_sLearn, sizeof(m_sLearn));

	m_pAI = nullptr;
	m_bStart = false;
	m_bEnd = false;
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
	// 値の初期化
	m_sLearn.fDistanceIN = LENGTH_IN;
	m_sLearn.fDistanceOUT = LENGTH_OUT;
	m_sLearn.fDistanceLine = LENGTH_LINE;
	m_eLine = ELine::LINE_IN;

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

#ifdef _DEBUG
	Debug();
#endif
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
		pBall->GetTypeTeam() != m_pAI->GetTeam())
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
	PlanThrowFlow(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 投げるまでの流れを考える
//==========================================================================
void CPlayerAIControl::PlanThrowFlow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 1:ターゲットを決める
	CPlayer* pTarget = nullptr;
	pTarget = GetThrowTarget();

	// 2:何を投げるか考える関数	// 常
	PlanThrow(pTarget, fDeltaTime, fDeltaRate, fSlowRate);

#ifdef _DEBUG
	CEffect3D::Create
	(// デバッグ用エフェクト(ターゲット)
		pTarget->GetPosition(),
		VEC3_ZERO,
		MyLib::color::Red(),
		20.0f,
		0.1f,
		1,
		CEffect3D::TYPE::TYPE_NORMAL
	);	//
#endif

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
	//if (!m_sInfo.sThrowInfo.bThrow && m_sInfo.sThrowInfo.bTiming) return;

	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	CGameManager* pGameManager = CGameManager::GetInstance();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(m_pAI->GetTeam());

	if (m_sInfo.sThrowInfo.eType == EThrowType::THROWTYPE_NONE)
	{
		if (pTeamStatus->IsMaxSpecial())
		{// ゲージが溜まっていたら
			// スペシャル投げ
			m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_SPECIAL;
		}
		else
		{// 投げろフラグがオンだったら
			int n = 0;
			// 今はランダムで決定
			//n = rand() % 2;
			n = 0;

			switch (n)
			{
			case 0:	// 通常
				m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_NORMAL;
				break;

			case 1:	// ジャンプ
				m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_JUMP;
				break;

			default:
				
				break;
			}

			// タイミングの決定完了
			m_sInfo.sThrowInfo.bTiming = true;
		}
	}

	// 投げるタイプの更新
	(this->*(m_ThrowTypeFunc[m_sInfo.sThrowInfo.eType]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 通常投げ
//==========================================================================
void CPlayerAIControl::ThrowTypeNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// その場なのか歩くのか走るのか

	m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_NONE;

	// 投げるまでの行動の更新
	(this->*(m_ThrowMoveFunc[m_sInfo.sMoveInfo.eType]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ジャンプ投げ
//==========================================================================
void CPlayerAIControl::ThrowTypeJump(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// その場なのか歩くのか走るのか
	m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_WALK;

	// 投げるまでの行動の更新
	(this->*(m_ThrowMoveFunc[m_sInfo.sMoveInfo.eType]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// スペシャル投げ
//==========================================================================
void CPlayerAIControl::ThrowTypeSpecial(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// その場なのか歩くのか走るのか

	// 投げるまでの行動の更新
	(this->*(m_ThrowMoveFunc[m_sInfo.sMoveInfo.eType]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// その場投げ
//==========================================================================
void CPlayerAIControl::ThrowMoveNone(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// タイミングはどうする？

	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	pControlAIMove->SetIsWalk(false);	// 歩きリセット

	// 投げる
	pControlAIAction->SetIsThrow(true);

	m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_NONE;

	
	// 投げるまでのタイミングの更新
	//(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 歩き投げ
//==========================================================================
void CPlayerAIControl::ThrowMoveWalk(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 歩け！
	pControlAIMove->SetIsWalk(false);

	// 走り投げ
	//RunStartPos(pTarget);

	// タイミングはどうする？
	m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_NORMAL;


	// 投げるまでのタイミングの更新
	(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 走り投げ
//==========================================================================
void CPlayerAIControl::ThrowMoveDash(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// タイミングはどうする？

	// 投げるまでのタイミングの更新
	(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
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
	MyLib::Vector3 MyPos = m_pAI->GetPosition();

	// 自分から相手の距離
	float fDistance = MyPos.DistanceXZ(posTarget);

	if (fDistance < m_sLearn.fDistanceIN - 50.0f)
	{// 離れろ！
		m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_LEAVE;
		
		// 相手から自分の方向
		m_pAI->SetRotDest(posTarget.AngleXZ(MyPos));

		// 歩け！
		pControlAIMove->SetIsWalk(true);

		return;
	}
	else if(fDistance > m_sLearn.fDistanceIN + 50.0f)
	{// 近づけ！
		m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_APPROATCH;

		// 相手から自分の方向
		m_pAI->SetRotDest(MyPos.AngleXZ(posTarget));

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
	MyLib::Vector3 MyPos = m_pAI->GetPosition();

	// 自分から相手の距離
	float fDistance = MyPos.DistanceXZ(posTarget);
	
	if (fDistance > m_sLearn.fDistanceIN + 50.0f && fDistance < m_sLearn.fDistanceIN - 50.0f)
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

	// 線に対する思考
	Line(pTarget);
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

	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();

	if (typeTeam == CGameManager::ETeamSide::SIDE_LEFT)
	{
		if (pBall->GetPosition().x > 0.0f)
		{
			//Reset();
			bOver = true;
		}
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{
		if (pBall->GetPosition().x < 0.0f)
		{
			//Reset();
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
	Line(pTarget);

	// タイミング思考
	Timing(pTarget);

	// 状態更新
	(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// タイミングの思考 今はRAND
//==========================================================================
void CPlayerAIControl::Timing(CPlayer* pTarget)
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
			else if (n == 1 && fDistance < m_sLearn.fDistanceIN - 100.0f)
			{// 番号2&&ターゲットとの距離が以内の場合
				m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_QUICK;
				m_sInfo.sThrowInfo.bTiming = true;
				break;
			}
			else if (n == 2 && fDistance < m_sLearn.fDistanceIN - 100.0f)
			{// 番号1&&ターゲットとの距離が以内の場合

				m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_DELAY;
				m_sInfo.sThrowInfo.bTiming = true;
				break;
			}
		}
	}
}

//--------------------------------------------------------------------------
// 跳び投げタイミング(通常)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowJumpTimingNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

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
// 跳び投げタイミング(速い)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowJumpTimingQuick(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// どれくらいの距離移動するか処理を追加


	pControlAIMove->SetIsWalk(false);	// 歩きリセット
	pControlAIAction->SetIsJump(true);	// ジャンプオン

	if (m_pAI->GetPosition().y >= JUMP_END_POS * 0.5f)
	{
		// 投げる
		pControlAIAction->SetIsThrow(true);

		// 変数リセット
		//Reset();
	}
}

//--------------------------------------------------------------------------
// 跳び投げタイミング(遅い)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowJumpTimingDelay(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// どれくらいの距離移動するか処理を追加


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
		//Reset();
	}
}

//==========================================================================
// 線に対しての思考
//==========================================================================
void CPlayerAIControl::Line(CPlayer* pTarget)
{
	// チームタイプの取得
	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();

	if (typeTeam == CGameManager::ETeamSide::SIDE_LEFT)
	{// 左側チーム
		LineLeftTeam(pTarget);
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{// 右側チーム
		LineRightTeam(pTarget);
	}
}

//--------------------------------------------------------------------------
// 線に対して左側チーム
//--------------------------------------------------------------------------
void CPlayerAIControl::LineLeftTeam(CPlayer* pTarget)
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ターゲット距離(中央)
	MyLib::Vector3 posTarget = {};					// ターゲット位置
	MyLib::Vector3 MyPos = m_pAI->GetPosition();	// 自身位置

	if (MyPos.x < 0.0f)
	{// 距離が指定値以内の場合

		// 移動状態を離れろ！
		m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_LEAVE;

		// 相手から自分の方向
		m_pAI->SetRotDest(posTarget.AngleXZ(MyPos));

		// 歩け！
		pControlAIMove->SetIsWalk(true);
	}
}

//--------------------------------------------------------------------------
// 線に対して右側チーム
//--------------------------------------------------------------------------
void CPlayerAIControl::LineRightTeam(CPlayer* pTarget)
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ターゲット距離(中央)
	MyLib::Vector3 posTarget = {};	// ターゲット位置
	MyLib::Vector3 MyPos = m_pAI->GetPosition();	// 自身位置

	if (MyPos.x < 0.0f)
	{// 距離が指定値以内の場合

		// 移動状態を離れろ！
		m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_LEAVE;

		// 相手から自分の方向
		m_pAI->SetRotDest(MyPos.AngleXZ(posTarget));

		// 歩け！
		pControlAIMove->SetIsWalk(true);
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

	if (pBall->GetPlayer() != nullptr)
	{// 誰かがボールを持っている場合
		m_sInfo.sCatchInfo.eCatchType = ECatchType::CATCH_TYPE_NORMAL;
	}
	else if (pBall->GetPlayer() == nullptr)
	{// 誰もボールを持っていない場合

		// 自分よりボールの近くに味方がいた場合
		if (!IsWhoPicksUpTheBall()) return;

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
	// ボールを持つ相手を取得
	CPlayer* pTarget = GetCatchTarget();

	// 線との距離
	CatchLineLeftDistance();

	// 距離(ボールを持っている奴との)
	CatchDistance(pTarget);

	// 外野との距離
	CatchOutDistance();


	CatchMoveFlag();
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
// ボールを取りに行く
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
	{// ボールがnullptr&&プレイヤーがボールを取っている場合
		Reset();	// 変数リセット
		return;
	}

	// 角度を求める(playerからみたボール)
	float fAngle = m_pAI->GetPosition().AngleXZ(pBall->GetPosition());

	// 歩きオン!
	pControlAIMove->SetIsWalk(true);

	// 方向設定
	m_pAI->SetRotDest(fAngle);


#ifdef _DEBUG

	CEffect3D::Create
	(// デバッグ用エフェクト(ターゲット)
		m_pAI->GetPosition(),
		VEC3_ZERO,
		MyLib::color::Purple(),
		20.0f,
		0.1f,
		1,
		CEffect3D::TYPE::TYPE_NORMAL
	);	// 
#endif

}

//==========================================================================
// 誰がボールを取りにいくか判定
//==========================================================================
bool CPlayerAIControl::IsWhoPicksUpTheBall()
{
	float fMyDis = 1000000.0f;	// 自分のボールとの距離
	float fTeamMemberDis = 1000000.0f;	// チームメンバーのボールとの距離

	// 自分の情報取得
	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();	// チームタイプ
	MyLib::Vector3 Mypos = m_pAI->GetPosition();	// 位置情報

	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return false;

	// ボール位置取得
	MyLib::Vector3 posBall = pBall->GetPosition();
	fMyDis = Mypos.DistanceXZ(posBall);	// 自分からボールとの距離

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報

		// 違うチーム||外野の場合
		if ((typeTeam != pPlayer->GetTeam()) ||
			(pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT))
		{
			continue;
		}

		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// プレイヤー位置

		// チームからボールとの距離を求める
		float fLength = posPlayer.DistanceXZ(posBall);

		if (fLength < fMyDis)
		{ // より近い味方プレイヤーがいた場合

			return false;
		}
	}

	return true;
}

//==========================================================================
// キャッチ時の距離(内野)
//==========================================================================
void CPlayerAIControl::CatchDistance(CPlayer* pTarget)
{
	if (pTarget == nullptr) return;

	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 距離を見る
	MyLib::Vector3 TargetPos = pTarget->GetPosition();
	MyLib::Vector3 MyPos = m_pAI->GetPosition();

	// 自分から相手の距離
	float fDistance = MyPos.DistanceXZ(TargetPos);

	if (fDistance < m_sLearn.fDistanceIN - LENGTH_SPACE)
	{// 相手との距離が近かった場合
		// 行動状態：離れる
		m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_LEAVE;

		// カニ進行方向の設定
		float direction = TargetPos.AngleXZ(MyPos);
		pControlAIMove->SetClabDirection(direction);

		return;
	}
	else if (fDistance > m_sLearn.fDistanceIN + LENGTH_SPACE)
	{// 相手との距離が遠かった場合

		if (m_eLine == ELine::LINE_OVER) return;

		// 行動状態：近づく
		m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_APPROATCH;

		// カニ進行方向の設定
		float direction = MyPos.AngleXZ(TargetPos.AngleXZ(MyPos));
		pControlAIMove->SetClabDirection(direction);

		return;
	}

	// 動くんじゃない！
	m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_NONE;
}

//==========================================================================
// キャッチ時の距離(外野)
//==========================================================================
void CPlayerAIControl::CatchOutDistance()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	CPlayer* pTarget = nullptr;	// 目標ターゲット

	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) { return; }
	CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// チームタイプの取得

	// 自分情報の取得
	MyLib::Vector3 myPos = m_pAI->GetPosition();		// 位置情報の取得
	CGameManager::ETeamSide myTeam = m_pAI->GetTeam();	// 自分チームの取得

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター

	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// プレイヤー位置		

		// 味方||外野以外の場合
		if ((pPlayer->GetTeam() == myTeam) ||
			(pPlayer->GetAreaType() != CPlayer::EFieldArea::FIELD_OUT))
		{
			continue;
		}

		// 相手との距離を求める
		float fLength = posPlayer.DistanceXZ(myPos);

		if (fLength < m_sLearn.fDistanceOUT)
		{ // 数値より近い相手プレイヤーがいた場合

			// 離れる行動状態へ
			m_sInfo.sMoveInfo.eType = EMoveType::MOVETYPE_LEAVE;

			// ターゲット設定
			pTarget = pPlayer;

			// カニ進行方向の設定
			float direction = pTarget->GetPosition().AngleXZ(myPos);
			pControlAIMove->SetClabDirection(direction);
		}
	}
}

//==========================================================================
// 線に対しての距離(キャッチ)
//==========================================================================
void CPlayerAIControl::CatchLineLeftDistance()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ターゲット距離(中央)
	MyLib::Vector3 myPos = m_pAI->GetPosition();	// 自身位置
	MyLib::Vector3 targetPos = { 0.0f, 0.0f, myPos.x };	// ターゲット位置

	// 自分からターゲットとの距離
	float fDistance = myPos.DistanceXZ(targetPos);

	if (fDistance < m_sLearn.fDistanceLine)
	{// 距離が指定値以内の場合

		// 移動状態を離れろ！
		m_eLine = ELine::LINE_OVER;
	}
	else if (fDistance > m_sLearn.fDistanceLine + 20.0f)
	{
		m_eLine = ELine::LINE_IN;
	}
}

//==========================================================================
// ターゲット設定(キャッチ時)
//==========================================================================
void CPlayerAIControl::CatchMoveFlag()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_sInfo.sMoveInfo.eType == EMoveType::MOVETYPE_NONE)
	{
		// 歩きオフ
		pControlAIMove->SetIsWalk(false);
	}
	else
	{
		// 歩きオン
		pControlAIMove->SetIsWalk(true);
	}
}




//==========================================================================
// 走り投げ(スタート位置へ)
//==========================================================================
void CPlayerAIControl::RunStartPos(CPlayer* pTarget)
{
	//if (!pTarget) return;
	if (m_bStart) return;

	// 自分の位置
	MyLib::Vector3 MyPos = m_pAI->GetPosition();
	// 走り投げスタート位置
	MyLib::Vector3 StartPos = { 700.0f, 0.0f, 0.0f };

	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 自分からスタート位置の角度
	float fAngle = MyPos.AngleXZ(StartPos);

	// 向き設定
	m_pAI->SetRotation(MyLib::Vector3(0.0f, fAngle, 0.0f));

	// 自分とスタート位置の距離
	float fDistance = MyPos.DistanceXZ(StartPos);

	if (fDistance < 10.0f && !m_bStart)
	{
		m_bStart = true;	// スタート位置へ着いた
		pControlAIMove->SetIsWalk(false);	// 止まる
		return;
	}

	// 歩く
	pControlAIMove->SetIsWalk(true);
	
}

//==========================================================================
// 走り投げ(終了位置へ)
//==========================================================================
void CPlayerAIControl::RunEndPos(CPlayer* pTarget)
{
	//if (!pTarget) return;
	if (m_bEnd) return;

	// 自分の位置
	MyLib::Vector3 MyPos = m_pAI->GetPosition();
	// 走り投げスタート位置
	MyLib::Vector3 StartPos = { 200.0f, 0.0f, 0.0f };

	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 自分からスタート位置の角度
	float fAngle = MyPos.AngleXZ(StartPos);

	// 向き設定
	m_pAI->SetRotation(MyLib::Vector3(0.0f, fAngle, 0.0f));

	// 自分とスタート位置の距離
	float fDistance = MyPos.DistanceXZ(StartPos);

	if (fDistance < 10.0f && !m_bEnd)
	{
		m_bEnd = true;	// 終了位置へ着いた
		pControlAIMove->SetIsWalk(false);	// 止まる
		return;
	}

	// 歩く
	pControlAIMove->SetIsWalk(true);
}



// その他 ===================================================================================================================
//==========================================================================
// ターゲット設定(投げる時)
//==========================================================================
CPlayer* CPlayerAIControl::GetThrowTarget()
{
	CPlayer* pTarget = nullptr;	// 目標ターゲット
	float fMinDis = 1000000.0f;	// 近いプレイヤー

	MyLib::Vector3 Mypos = m_pAI->GetPosition();	// 位置情報の取得

	// ボールの取得
	CBall* pBall = m_pAI->GetBall();
	if (pBall == nullptr) { return pTarget; }
	CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// チームタイプの取得

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// プレイヤー位置

		// 同じチーム||外野の場合
		if ((typeTeam == pPlayer->GetTeam()) || 
			(pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT))
		{ continue; }

		// 敵との距離を求める
		float fLength = Mypos.DistanceXZ(posPlayer);

		if (fLength < fMinDis)
		{ // より近い相手プレイヤーがいた場合

			// 最小距離の更新
			fMinDis = fLength;

			// ターゲットを更新
			pTarget = pPlayer;

			// 方向設定(そっちを向く)
			m_pAI->SetRotDest(Mypos.AngleXZ(pTarget->GetPosition()));
		}
	}

	return pTarget;
}

//==========================================================================
// ターゲット設定(キャッチ時)
//==========================================================================
CPlayer* CPlayerAIControl::GetCatchTarget()
{
	CPlayer* pTarget = nullptr;	// 目標ターゲット

	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) { return pTarget; }
	CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// チームタイプの取得

	// ボールを持っている奴と自分が同じチームの場合
	if (typeTeam == m_pAI->GetTeam()) return pTarget;

	// ターゲットの設定
	pTarget = pBall->GetPlayer();

	return pTarget;
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

// デバッグ
void CPlayerAIControl::Debug()
{
	//if (ImGui::TreeNode("AI Info"))
	//{
	//	if (ImGui::Button("Reset"))
	//	{// リセット
	//		ZeroMemory(&m_sInfo, sizeof(m_sInfo));
	//	}

	//	//ImGui::DragFloat("Distance", &m_fDistanceIN, 1.0f, 0.0f, 1000.0f, "%.2f");

	//	ImGui::Text("mode : %d", m_sInfo.sMode.eMode);
	//	ImGui::Text("Distance : %0.2f", m_fDistanceIN);
	//	ImGui::DragFloat("Distance", &m_fDistanceIN, 1.0f, 0.0f, 1000.0f, "%.2f");

	//	ImGui::TreePop();
	//}
}
