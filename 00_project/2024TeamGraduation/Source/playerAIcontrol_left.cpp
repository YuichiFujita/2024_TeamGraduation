//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerAIcontrol_left.h"
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
#include "playerManager.h"

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
	const float JUMP_END_POS = 130.0f;
	const float JUMP_RATE = 1.0f;

	// 距離間(デフォルト)
	const float LENGTH_IN = 500.0f;
	const float LENGTH_SPACE = 10.0f;
	const float LENGTH_OUT = 200.0f;
	const float LENGTH_LINE = 100.0f;

	// 線越え判定(中心(x)からの距離)
	const float LINE_DISTANCE_OVER = 10.0f;	// 線超え判定の距離(中心 x:0.0f)
	const float RETURN_POS = 500.0f;			// 戻る位置(中心 x:0.0f)

	const float OK_LENGTH = 10.0f;				// 判定の範囲(目的との距離)

	// 距離
	const float CHATCH_LENGTH_IN = 500.0f;
	const float CHATCH_LENGTH_OUT = 700.0f;
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerAIControlLeft::AREA_FUNC CPlayerAIControlLeft::m_AreaFunc[] =	// モード関数
{
	&CPlayerAIControlLeft::AreaNone,				// なし
	&CPlayerAIControlLeft::AreaLeft,				// 左
	&CPlayerAIControlLeft::AreaRight,				// 右
};

CPlayerAIControlLeft::MODE_FUNC CPlayerAIControlLeft::m_ModeFunc[] =	// モード関数
{
	&CPlayerAIControlLeft::ModeNone,				// なし
	&CPlayerAIControlLeft::ModeThrowManager,		// 投げ
	&CPlayerAIControlLeft::ModeCatchManager,		// キャッチ
};

CPlayerAIControlLeft::THROWTYPE_FUNC CPlayerAIControlLeft::m_ThrowTypeFunc[] =	// 投げタイプ関数
{
	&CPlayerAIControlLeft::ThrowTypeNone,			// なし
	&CPlayerAIControlLeft::ThrowTypeNormal,			// 通常
	&CPlayerAIControlLeft::ThrowTypeJump,			// ジャンプ
	&CPlayerAIControlLeft::ThrowTypeSpecial,		// スペシャル
};

CPlayerAIControlLeft::THROWMOVE_FUNC CPlayerAIControlLeft::m_ThrowMoveFunc[] =	// 投げタイプ関数
{
	&CPlayerAIControlLeft::ThrowMoveNone,			// その場
	&CPlayerAIControlLeft::ThrowMoveWalk,			// 歩き
	&CPlayerAIControlLeft::ThrowMoveDash,			// 走り
};

CPlayerAIControlLeft::THROWTIMING_FUNC CPlayerAIControlLeft::m_ThrowTimingFunc[] =	// 投げタイミング関数
{
	&CPlayerAIControlLeft::ThrowTimingNone,			// なし
	&CPlayerAIControlLeft::ThrowTimingNormal,		// 通常
	&CPlayerAIControlLeft::ThrowTimingQuick,		// 速
	&CPlayerAIControlLeft::ThrowTimingDelay,		// 遅
	&CPlayerAIControlLeft::ThrowTimingJumpNormal,	// ジャンプ通常
	&CPlayerAIControlLeft::ThrowTimingJumpQuick,	// ジャンプ速
	&CPlayerAIControlLeft::ThrowTimingJumpDelay,	// ジャンプ遅
};

CPlayerAIControlLeft::THROW_FUNC CPlayerAIControlLeft::m_ThrowFunc[] =	// 投げタイミング関数
{
	&CPlayerAIControlLeft::ThrowNone,				// なし
	&CPlayerAIControlLeft::Throw,					// 投げ
	&CPlayerAIControlLeft::ThrowPass,				// パス
	&CPlayerAIControlLeft::ThrowSpecial,			// スペシャル
};

CPlayerAIControlLeft::CATCH_FUNC CPlayerAIControlLeft::m_CatchFunc[] =	// キャッチ関数
{
	&CPlayerAIControlLeft::CatchNone,			// なし
	&CPlayerAIControlLeft::CatchNormal,			// 通常
	&CPlayerAIControlLeft::CatchJust,			// ジャスト
	&CPlayerAIControlLeft::CatchDash,			// ダッシュ
	&CPlayerAIControlLeft::CatchPass,			// パス
	&CPlayerAIControlLeft::CatchFindBall,		// 見つける
};

CPlayerAIControlLeft::MOVEFORCIBLY_FUNC CPlayerAIControlLeft::m_MoveForciblyFunc[] =	// 行動関数
{
	&CPlayerAIControlLeft::ForciblyNone,		// なし
	&CPlayerAIControlLeft::ForciblyStop,		// 止まる
	&CPlayerAIControlLeft::ForciblyReturn,		// 戻る
	&CPlayerAIControlLeft::ForciblyStart,		// 初め
};

CPlayerAIControlLeft::MOVE_FUNC CPlayerAIControlLeft::m_MoveFunc[] =	// 行動関数
{
	&CPlayerAIControlLeft::MoveStop,			// 止まる
	&CPlayerAIControlLeft::MoveWalk,			// 歩く
	&CPlayerAIControlLeft::MoveDash,			// 走る
};

CPlayerAIControlLeft::ACTION_FUNC CPlayerAIControlLeft::m_ActionFunc[] =	// 行動関数
{
	&CPlayerAIControlLeft::ActionNone,
	&CPlayerAIControlLeft::ActionJump,
};


//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControlLeft::CPlayerAIControlLeft()
{
	// メンバ変数の初期化
	ZeroMemory(&m_sInfo, sizeof(m_sInfo));
	ZeroMemory(&m_sTarget, sizeof(m_sTarget));

	m_pAI = nullptr;
	m_pTarget = nullptr;
	m_bStart = false;
	m_bEnd = false;
	m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
	m_eMove = EMoveType::MOVETYPE_STOP;
	m_eHeart = EHeart::HEART_NONE;
	m_eAction = EAction::ACTION_NONE;
	m_eThrow = EThrow::THROW_NONE;
}

#if 0
//==========================================================================
// 生成
//==========================================================================
CPlayerAIControlLeft* CPlayerAIControlLeft::Create(CPlayer* player)
{
	// メモリの確保
	CPlayerAIControlLeft* pObj = DEBUG_NEW CPlayerAIControlLeft;
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
#endif

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerAIControlLeft::Init()
{
	// 値の初期化
	m_pAI = CPlayerAIControl::GetPlayer();

	m_sTarget.fDistanceIN = LENGTH_IN;
	m_sTarget.fDistanceOUT = LENGTH_OUT;
	m_eLine = ELine::LINE_IN;
	m_eHeart = EHeart::HEART_NORMAL;
	m_eThrow = EThrow::THROW_NONE;
	m_eForcibly = EMoveForcibly::FORCIBLY_START;

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAIControlLeft::Uninit()
{
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIControlLeft::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モード管理
	ModeManager(fDeltaTime, fDeltaRate, fSlowRate);

	IsJumpCatch();

	// 行動管理
	MoveManager(fDeltaTime, fDeltaRate, fSlowRate);

	// アクション管理
	ActionManager();

	// 投げ管理
	ThrowManager();

	// 心
	//PlanHeart();
}

//==========================================================================
// エリア：なし
//==========================================================================
void CPlayerAIControlLeft::AreaNone()
{
	assert(false);
}

//==========================================================================
// エリア：左
//==========================================================================
void CPlayerAIControlLeft::AreaLeft()
{

}

//==========================================================================
// エリア：右
//==========================================================================
void CPlayerAIControlLeft::AreaRight()
{

}


//==========================================================================
// モード管理
//==========================================================================
void CPlayerAIControlLeft::ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsLineOverPlayer() && m_eForcibly != EMoveForcibly::FORCIBLY_START)
	{// 線を超えていた&&強制行動：初め 以外の場合
		m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
		return;
	}

	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() == nullptr ||
		pBall->GetTypeTeam() != m_pAI->GetTeam())
	{// ボールが取得されていない場合||自分とボールを持っているチームが違う場合
		m_sInfo.eMode = EMode::MODE_CATCH;
	}
	else if (pBall->GetPlayer() == m_pAI)
	{// ボールを持っているのが自分だった場合
		m_sInfo.eMode = EMode::MODE_THROW;
	}

	// モード更新
	(this->*(m_ModeFunc[m_sInfo.eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 投げの管理
//==========================================================================
void CPlayerAIControlLeft::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 1:ターゲットを決める
	m_pTarget = nullptr;
	m_pTarget = GetThrowTarget();

	// 2:何を投げるか考える関数
	PlanThrow(m_pTarget, fDeltaTime, fDeltaRate, fSlowRate);


#ifdef _DEBUG
	if (!m_pTarget)
	{
		return;
	}

	CEffect3D::Create
	(// デバッグ用エフェクト(ターゲット)
		m_pTarget->GetPosition(),
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
void CPlayerAIControlLeft::PlanHeart()
{
	// 体力の取得
	int nLifeMax = m_pAI->GetLifeOrigin();
	int nLife = m_pAI->GetLife();

	float n = (float)nLifeMax / (float)nLife;

	// 残り体力で心持を決める
	if ((nLifeMax / nLife) > 8)
	{
		m_eHeart = EHeart::HEART_NORMAL;
	}
}

//==========================================================================
// 何を投げるか
//==========================================================================
void CPlayerAIControlLeft::PlanThrow(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
			int n = 1;
			// 今はランダムで決定
			//int n = rand() % 2;

			/*float distance = m_pAI->GetPosition().DistanceXZ(m_pTarget->GetPosition());
			if (distance > 500.0f)
			{
				m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_JUMP;
			}
			else
			{
				m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_NORMAL;
			}*/

			switch (n)
			{
			case 0:	// 通常
				m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_NORMAL;
				break;

			case 1:	// ジャンプ
				m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_JUMP;
				break;

			default:
				assert(false);
				break;
			}
		}
	}

	// 投げるタイプの更新
	(this->*(m_ThrowTypeFunc[m_sInfo.sThrowInfo.eType]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 通常投げ
//==========================================================================
void CPlayerAIControlLeft::ThrowTypeNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//// その場なのか歩くのか走るのか
	//m_eMove = EMoveType::MOVETYPE_WALK;

	//// 投げるまでの行動の更新
	//(this->*(m_ThrowMoveFunc[m_eMove]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);

	m_eThrow = EThrow::THROW_NORMAL;
}

//==========================================================================
// ジャンプ投げ
//==========================================================================
void CPlayerAIControlLeft::ThrowTypeJump(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// その場なのか歩くのか走るのか
	m_eMove = EMoveType::MOVETYPE_WALK;

	// 走り投げ
	AttackDash();
}

//==========================================================================
// スペシャル投げ
//==========================================================================
void CPlayerAIControlLeft::ThrowTypeSpecial(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// その場なのか歩くのか走るのか

	m_eThrow = EThrow::THROW_SPECIAL;

	// 投げるまでの行動の更新
	//(this->*(m_ThrowMoveFunc[m_eMove]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// その場投げ
//==========================================================================
void CPlayerAIControlLeft::ThrowMoveNone(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControlLeft::ThrowMoveWalk(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (IsLineOverPlayer() && m_eForcibly != EMoveForcibly::FORCIBLY_START)
	{// 線を超えていた場合(数値は固定変数)
		m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
		return;
	}

	// タイミングはどうする？
	m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_NORMAL;

	//Timing(pTarget);

	// 投げるまでのタイミングの更新
	(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 走り投げ
//==========================================================================
void CPlayerAIControlLeft::ThrowMoveDash(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsLineOverPlayer())
	{// 線を超えていた場合(数値は固定変数)
		m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
		return;
	}

	// タイミングはどうする？
	m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_NORMAL;

	// 投げるまでのタイミングの更新
	(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 跳ぶかどうか まだ何を参考にするかは未定
//==========================================================================
void CPlayerAIControlLeft::PlanIsJump(CPlayer* pTarget)
{
	if (pTarget == nullptr) return;

	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// 距離を見る
	MyLib::Vector3 posTarget = pTarget->GetPosition();
	MyLib::Vector3 MyPos = m_pAI->GetPosition();

	// 自分から相手の距離
	float fDistance = MyPos.DistanceXZ(posTarget);
	
	if (fDistance > m_sTarget.fDistanceIN + 50.0f && fDistance < m_sTarget.fDistanceIN - 50.0f)
	{// 離れていたら
		//pControlAIAction->SetIsJump(true);

		/*m_sMoveInfo.bJump = false;*/
	}
}

//==========================================================================
// ジャンプ投げタイミング
//==========================================================================
void CPlayerAIControlLeft::JumpThrowTiming(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// タイミング思考
	Timing(pTarget);

	// 状態更新
	(this->*(m_ThrowTimingFunc[m_sInfo.sThrowInfo.eTiming]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// タイミングの思考 今はRAND
//==========================================================================
void CPlayerAIControlLeft::Timing(CPlayer* pTarget)
{
	if (m_sInfo.sThrowInfo.bTiming) { return; }

	if (m_sInfo.sThrowInfo.eTiming == EThrowType::THROWTYPE_NORMAL)
	{// 通常投げ
		// ランダムでタイミングを決める
		m_sInfo.sThrowInfo.fTiming = timing::TIMING_NORMAL + UtilFunc::Transformation::Random(timing::TIMING_RAND_MIN, timing::TIMING_RAND_MAX) * 0.01f;

		m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_NORMAL;	// 通常投げ
		m_sInfo.sThrowInfo.bTiming = true;	// タイミングフラグオン


		//// 中心との距離を見る
		//MyLib::Vector3 myPos = m_pAI->GetPosition();
		//MyLib::Vector3 pos = { 0.0f, myPos.y, myPos.z };
		//float fRito = myPos.DistanceXZ(pos);

		//if (fRito > 0)
		//{
		//	return;
		//}

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
			else if (n == 1 && fDistance < m_sTarget.fDistanceIN - 100.0f)
			{// 番号2&&ターゲットとの距離が以内の場合
				m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_QUICK;
				m_sInfo.sThrowInfo.bTiming = true;
				break;
			}
			else if (n == 2 && fDistance < m_sTarget.fDistanceIN - 100.0f)
			{// 番号1&&ターゲットとの距離が以内の場合

				m_sInfo.sThrowInfo.eTiming = ETiming::TIMING_DELAY;
				m_sInfo.sThrowInfo.bTiming = true;
				break;
			}
		}
	}
}

//--------------------------------------------------------------------------
// 投げタイミング(通常)
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::ThrowTimingNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 止まる
	m_eMove = EMoveType::MOVETYPE_STOP;

	// 投げる
	m_eThrow = EThrow::THROW_NORMAL;

	// 初期化
	ZeroMemory(&m_sInfo, sizeof(m_sInfo));
}

//--------------------------------------------------------------------------
// 投げタイミング(速い)
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::ThrowTimingQuick(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sInfo.sThrowInfo.fTiming > 0.0f)
	{
		m_sInfo.sThrowInfo.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// 止まる
	m_eMove = EMoveType::MOVETYPE_STOP;

	// 投げる
	m_eThrow = EThrow::THROW_NORMAL;

	// 初期化
	ZeroMemory(&m_sInfo, sizeof(m_sInfo));
}

//--------------------------------------------------------------------------
// 投げタイミング(遅い)
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::ThrowTimingDelay(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sInfo.sThrowInfo.fTiming > 0.0f)
	{
		m_sInfo.sThrowInfo.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// 止まる
	m_eMove = EMoveType::MOVETYPE_STOP;

	// 投げる
	m_eThrow = EThrow::THROW_NORMAL;

	// 初期化
	ZeroMemory(&m_sInfo, sizeof(m_sInfo));
}

//--------------------------------------------------------------------------
// 跳び投げタイミング(通常)
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::ThrowTimingJumpNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	m_eMove = EMoveType::MOVETYPE_STOP;
	m_eAction = EAction::ACTION_JUMP;

	if (m_pAI->GetPosition().y >= JUMP_END_POS)	// 高さによって変わる
	{
		m_eThrow = EThrow::THROW_NORMAL;	// 投げる
	}
}

//--------------------------------------------------------------------------
// 跳び投げタイミング(速い)
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::ThrowTimingJumpQuick(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
		//ResetFlag();
	}
}

//--------------------------------------------------------------------------
// 跳び投げタイミング(遅い)
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::ThrowTimingJumpDelay(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// どれくらいの距離移動するか処理を追加
	m_eMove = EMoveType::MOVETYPE_STOP;
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
		//ResetFlag();
	}
}

// キャッチ関連 =========================================================================================================================================================

//==========================================================================
// キャッチの管理
//==========================================================================
void CPlayerAIControlLeft::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;
	CPlayer* pPlayer = pBall->GetPlayer();

	if (pPlayer != nullptr)
	{// 誰かがボールを持っている&&自分と同じ場合

		if (pPlayer->GetTeam() == m_pAI->GetTeam() && pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT)
		{// 同じチーム&&外野の場合
			m_sInfo.eCatchType = ECatchType::CATCH_TYPE_PASS;
		}
		else
		{
			m_sInfo.eCatchType = ECatchType::CATCH_TYPE_NORMAL;
		}
	}
	else if (pBall->GetPlayer() == nullptr)
	{// 誰もボールを持っていない場合
		if (IsWhoPicksUpTheBall() && IsLineOverBall())
		{// 自分よりもボールに近い人がいる&&ボールが相手側にある
			m_sInfo.eCatchType = ECatchType::CATCH_TYPE_NORMAL;
			
		}
		else
		{// ボールを取りに行く
			m_sInfo.eCatchType = ECatchType::CATCH_TYPE_FIND;
		}
	}

	// キャッチ種類更新
	(this->*(m_CatchFunc[m_sInfo.eCatchType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//--------------------------------------------------------------------------
// 通常キャッチ
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// パスターゲットが自分の場合
	if (IsPassTarget()) return;

	// ボールを持つ相手を取得
	CPlayer* pTarget = GetCatchTarget();
	if (!pTarget) { return; }

	// 距離(ボールを持っている奴との)
	CatchDistance(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//--------------------------------------------------------------------------
// キャッチ：ジャスト
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//--------------------------------------------------------------------------
// キャッチ：ダッシュ
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//--------------------------------------------------------------------------
// キャッチ：パス
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::CatchPass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	if (m_pAI->GetBall()) return;

	// 自分からボールの位置を見る
	float rot = m_pAI->GetPosition().AngleXZ(pBall->GetPosition());

	// 設定
	m_pAI->SetRotDest(rot);

	// 行動種類：止まる
	m_eMove = EMoveType::MOVETYPE_STOP;
}

//--------------------------------------------------------------------------
// キャッチ：取りに行く
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::CatchFindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (pBall == nullptr || pBall->GetPlayer() != nullptr) 
	{// ボールがnullptr&&プレイヤーがボールを取っている場合
		m_eMove = EMoveType::MOVETYPE_STOP;
		return;
	}

	// 行動状態：歩く
	m_eMove = EMoveType::MOVETYPE_WALK;

	// 近づく
	if (Approatch(pBall->GetPosition(), 5.0f))
	{
		m_eMove = EMoveType::MOVETYPE_STOP;
	}
}

//==========================================================================
// キャッチ時の距離
//==========================================================================
void CPlayerAIControlLeft::CatchDistance(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ターゲットのエリアの取得
	CPlayer::EFieldArea area = pTarget->GetAreaType();

	MyLib::Vector3 myPos = m_pAI->GetPosition();

	// エリアごと
	if (area == CPlayer::EFieldArea::FIELD_IN)
	{// ターゲットが内野

		m_eMove = EMoveType::MOVETYPE_WALK;

		if (Leave(pTarget->GetPosition(), 500.0f))
		{
			m_eMove = EMoveType::MOVETYPE_STOP;
		}
	}
	else if (area = CPlayer::EFieldArea::FIELD_OUT)
	{// ターゲットが外野

		m_eMove = EMoveType::MOVETYPE_WALK;

		if (Leave(pTarget->GetPosition(), 500.0f))
		{
			m_eMove = EMoveType::MOVETYPE_STOP;
		}
	}
	else
	{// エリアの取得に失敗
		assert(false);
	}
}


// 行動関連 =========================================================================================================================================================

//==========================================================================
// 強制：止まる
//==========================================================================
void CPlayerAIControlLeft::ForciblyStop()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 歩く
	m_eMove = EMoveType::MOVETYPE_STOP;
}

//==========================================================================
// 強制：戻る
//==========================================================================
void CPlayerAIControlLeft::ForciblyReturn()
{
	// プレイヤー情報取得
	MyLib::Vector3 myPos = m_pAI->GetPosition();
	
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 歩く
	m_eMove = EMoveType::MOVETYPE_WALK;

	// 近づく
	if (Approatch({ -RETURN_POS, myPos.y, myPos.z }, OK_LENGTH))
	{
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eMove = EMoveType::MOVETYPE_STOP;
	}
}

//==========================================================================
// 強制：初めのダッシュ
//==========================================================================
void CPlayerAIControlLeft::ForciblyStart()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall)
	{// ボールがない場合
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		return;
	}

	CPlayer* pPlayer = pBall->GetPlayer();
	if (pPlayer)
	{// 誰かがボールを持った場合
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		return;
	}

	m_eAction = EAction::ACTION_JUMP;
	m_eMove = EMoveType::MOVETYPE_WALK;

}

//==========================================================================
// 行動管理
//==========================================================================
void CPlayerAIControlLeft::MoveManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 強制行動更新
	(this->*(m_MoveForciblyFunc[m_eForcibly]))();

	//if (m_eForcibly == EMoveForcibly::FORCIBLY_RETURN) return;

	// 行動更新
	(this->*(m_MoveFunc[m_eMove]))();
}

//--------------------------------------------------------------------------
// 行動：止まる
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::MoveStop()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 行動フラグリセット
	pControlAIMove->SetIsWalk(false);
	pControlAIMove->SetIsDash(false);
	pControlAIMove->SetIsBlink(false);
}

//--------------------------------------------------------------------------
// 行動：歩く
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::MoveWalk()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 歩く
	pControlAIMove->SetIsWalk(true);
}

//--------------------------------------------------------------------------
// 行動：走る
//--------------------------------------------------------------------------
void CPlayerAIControlLeft::MoveDash()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 歩く
	pControlAIMove->SetIsWalk(true);
	// ブリンク
	pControlAIMove->SetIsBlink(true);
	// 走る
	pControlAIMove->SetIsDash(true);
}

// アクション =========================================================================================================================================================

//==========================================================================
// アクション管理
//==========================================================================
void CPlayerAIControlLeft::ActionManager()
{
	// アクション更新
	(this->*(m_ActionFunc[m_eAction]))();
}

//==========================================================================
// アクション：無
//==========================================================================
void CPlayerAIControlLeft::ActionNone()
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// ジャンプ：true
	pControlAIAction->SetIsJump(false);
	pControlAIAction->SetIsJumpFloat(false);

	// アクション列挙：なし
	m_eAction = EAction::ACTION_NONE;
}

//==========================================================================
// アクション：ジャンプ
//==========================================================================
void CPlayerAIControlLeft::ActionJump()
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// ジャンプ：true
	pControlAIAction->SetIsJump(true);
	pControlAIAction->SetIsJumpFloat(true);

	pControlAIAction->SetJumpRate(JUMP_RATE);

	// アクション列挙：なし
	m_eAction = EAction::ACTION_NONE;
}


// 投げ関連 =========================================================================================================================================================

//==========================================================================
// 投げ管理
//==========================================================================
void CPlayerAIControlLeft::ThrowManager()
{
	// 投げ更新
	(this->*(m_ThrowFunc[m_eThrow]))();
}

//==========================================================================
// 投げる
//==========================================================================
void CPlayerAIControlLeft::Throw()
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// 投げる
	pControlAIAction->SetIsThrow(true);

	// 投げなし
	m_eThrow = EThrow::THROW_NONE;
	m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_NONE;
}

//==========================================================================
// パス
//==========================================================================
void CPlayerAIControlLeft::ThrowPass()
{

}

//==========================================================================
// スペシャル
//==========================================================================
void CPlayerAIControlLeft::ThrowSpecial()
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// スペシャル投げ
	pControlAIAction->SetIsSpecial(true);

	// 投げなし
	m_eThrow = EThrow::THROW_NONE;
	m_sInfo.sThrowInfo.eType = EThrowType::THROWTYPE_NONE;
}


// 近づく・離れる =========================================================================================================================================================
//==========================================================================
// 離れる : Leave(離れる相手、離れる距離)
//==========================================================================
bool CPlayerAIControlLeft::Leave(MyLib::Vector3 targetPos, float distance)
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 自分の位置情報
	MyLib::Vector3 myPos = m_pAI->GetPosition();

	// 相手と自分の距離
	float length = myPos.DistanceXZ(targetPos);	// 距離：相手と自分

	if (distance < length) {// 移動タイプ：無
		return true;
	}

	// 自分からターゲットの角度
	//float direction = myPos.AngleXZ(targetPos);

	float direction = targetPos.AngleXZ(myPos);

	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall && pBall->GetPlayer())
	{
		if (pBall->GetPlayer()->GetTeam() != m_pAI->GetTeam() ||
			pBall->GetPlayer() == m_pAI)
		{// 違うチームが持っている場合
			// カニ進行方向の設定
			pControlAIMove->SetClabDirection(direction);
			//m_pAI->SetRotDest(direction);
		}
	}
	else
	{
		//pControlAIMove->SetClabDirection(direction);
		m_pAI->SetRotDest(direction);
	}

	return false;
}

//==========================================================================
// 離れる外野 : Leave()
//==========================================================================
void CPlayerAIControlLeft::LeaveOut(float distance)
{
	MyLib::Vector3 Mypos = m_pAI->GetPosition();	// 位置情報の取得

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayerOut = (*itr);	// プレイヤー情報

		if ((m_pAI->GetTeam() == pPlayerOut->GetTeam()) ||
			(pPlayerOut->GetAreaType() == CPlayer::EFieldArea::FIELD_IN))
		{// 同じチーム||内野の場合
			continue;
		}
		// プレイヤー位置
		MyLib::Vector3 posPlayerOut = pPlayerOut->GetCenterPosition();

		// 敵との距離を求める
		float fLength = Mypos.DistanceXZ(posPlayerOut);

		if (fLength < distance)
		{ // 範囲より近い外野がいた場合
			Leave(posPlayerOut, m_sTarget.fDistanceOUT);	// 離れる：外野
		}
	}
}

//==========================================================================
// 近づく : Approatch(近づく相手、近づく距離)
//==========================================================================
bool CPlayerAIControlLeft::Approatch(MyLib::Vector3 targetPos, float distance)
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 自分の位置情報
	MyLib::Vector3 myPos = m_pAI->GetPosition();

	// 相手と自分の距離
	float length = myPos.DistanceXZ(targetPos);	// 距離：相手と自分

	if (distance > length) {// 移動タイプ：無
		return true;
	}

	// 自分からターゲットの角度
	float direction = myPos.AngleXZ(targetPos);

	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall && pBall->GetPlayer())
	{
		if (pBall->GetPlayer()->GetTeam() != m_pAI->GetTeam() ||
			pBall->GetPlayer() != m_pAI)
		{// 違うチームが持っている||持ち主が自分場合
			// カニ進行方向の設定
			pControlAIMove->SetClabDirection(direction);
			//m_pAI->SetRotDest(direction);
		}
	}
	else
	{
		//pControlAIMove->SetClabDirection(direction);
		m_pAI->SetRotDest(direction);
	}

#ifdef _DEBUG
	CEffect3D::Create
	(// デバッグ用エフェクト(ターゲット)
		targetPos,
		VEC3_ZERO,
		MyLib::color::Red(),
		20.0f,
		0.1f,
		1,
		CEffect3D::TYPE::TYPE_NORMAL
	);
#endif

	return false;

}


//==========================================================================
// 走り投げ
//==========================================================================
void CPlayerAIControlLeft::AttackDash()
{
	// ターゲットの取得
	CPlayer* pTarget = GetThrowTarget();
	if (!pTarget) return;
	MyLib::Vector3 posTarget = pTarget->GetPosition();

	// 自分の位置取得
	MyLib::Vector3 posMy = m_pAI->GetPosition();

	// ラインの位置
	MyLib::Vector3 linePos = { 0.0f, posMy.y, posMy.z };

	// ラインとの距離
	float distanceLine = 0.0f;
	float distanceTarget = 0.0f;
	float JUMP_LENGTH_TARGET = 500.0f;
	float JUMP_LENGTH_LINE = 300.0f;

	if (m_pTarget)
	{// ターゲットがいた場合
		distanceTarget = posMy.DistanceXZ(m_pTarget->GetPosition());	// 自分と相手の距離
		distanceLine = posMy.DistanceXZ(linePos);	// 自分と中心線との距離
	}
	else
	{
		return;
	}

	if (distanceTarget > JUMP_LENGTH_TARGET && distanceLine > JUMP_LENGTH_TARGET)
	{// 自分とターゲットの距離が700.0f以上&&
		// 走る
		m_eMove = EMoveType::MOVETYPE_DASH;

		// 相手の位置に近づく
		if (Approatch(posTarget, JUMP_LENGTH_LINE))
		{// 範囲内の場合
			m_eForcibly = EMoveForcibly::FORCIBLY_NONE;	// 強制行動：なし
			m_eMove = EMoveType::MOVETYPE_STOP;			// 行動：止まる
		}

		return;
	}
		
	if (distanceTarget > JUMP_LENGTH_TARGET && distanceLine > JUMP_LENGTH_LINE)
	{// ターゲットとの距離が範囲以上&&中央線との距離が範囲内の場合
		// アクション：跳ぶ
		m_eAction = EAction::ACTION_JUMP;
	}
	else
	{
		m_eThrow = EThrow::THROW_NORMAL;		// 投げる
		m_eMove = EMoveType::MOVETYPE_STOP;
	}

	if (m_pAI->GetPosition().y >= JUMP_END_POS)	// 高さによって変わる
	{
		m_eMove = EMoveType::MOVETYPE_STOP;
		m_eThrow = EThrow::THROW_NORMAL;		// 投げる
	}
}


//==========================================================================
// キャッチ時の距離(外野)
//==========================================================================
void CPlayerAIControlLeft::CatchOutDistance()
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

		if (fLength < m_sTarget.fDistanceIN)
		{ // 数値より近い相手プレイヤーがいた場合

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
void CPlayerAIControlLeft::CatchLineLeftDistance()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 自身の位置
	MyLib::Vector3 myPos = m_pAI->GetPosition();

	// 目標位置
	MyLib::Vector3 Destpos = { 500.0f, myPos.y, myPos.z };

	// 自分から見た位置
	myPos.AngleXZ(Destpos);

	//if (myPos.x < LINE_DISTANCE_OVER)
	//{// 距離が指定値以内の場合
	//	// 移動状態を離れろ！
	//	m_eLine = ELine::LINE_OVER;
	//	// 動くんじゃない！
	//	m_eMove = EMoveType::MOVETYPE_STOP;
	//}
	//else if (myPos.x > LINE_DISTANCE_OVER)
	//{// 距離が指定値以外の場合
	//	// 移動状態を近づけ！
	//	m_eLine = ELine::LINE_IN;
	//}
}


// 判定関連 ===================================================================================================================

//==========================================================================
// パスの相手は自分ですか？
//==========================================================================
bool CPlayerAIControlLeft::IsPassTarget()
{
	// ボール情報の取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return false;

	bool b = pBall->IsPass();

	if (b)
	{
		return false;
	}

	if (pBall->GetTarget() == m_pAI)
	{
		return false;
	}

	// ボールパス&&ターゲットが自分
	if (pBall->IsPass() && pBall->GetTarget() == m_pAI)
	{
		return true;
	}

	return false;
}

//==========================================================================
// 誰がボールを取りに行きますか？
//==========================================================================
bool CPlayerAIControlLeft::IsWhoPicksUpTheBall()
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
			(pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT) &&
			(m_pAI == pPlayer))
		{
			continue;
		}

		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// プレイヤー位置

		// チームからボールとの距離を求める
		float fLength = posPlayer.DistanceXZ(posBall);

		if (fLength < fMyDis)
		{ // より近い味方プレイヤーがいた場合

			return true;
		}
	}

	return false;
}

//==========================================================================
// プレイヤーは線を超えていますか？
//==========================================================================
bool CPlayerAIControlLeft::IsLineOverPlayer()
{
	bool bOver = false;	// 超えた判定用

	// プレイヤー情報取得
	MyLib::Vector3 myPos = m_pAI->GetPosition();
	
	if (myPos.x > -LINE_DISTANCE_OVER)
	{// 位置が超えていた場合
		bOver = true;
	}

	return bOver;
}

//==========================================================================
// ボールは線を超えていますか？
//==========================================================================
bool CPlayerAIControlLeft::IsLineOverBall()
{
	bool bOver = false;	// 超えた判定用

	// ボール情報取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) { return bOver; }

	if (pBall->GetPosition().x > 0.0f)
	{
		bOver = true;
	}

	return bOver;
}


//==========================================================================
// ジャンプでボールは取れますか？
//==========================================================================
void CPlayerAIControlLeft::IsJumpCatch()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	// ボールステート取得
	CBall::EState stateBall = pBall->GetState();

	if (stateBall == CBall::EState::STATE_PASS ||
		stateBall == CBall::EState::STATE_HOM_PASS ||
		stateBall == CBall::EState::STATE_MOVE)
	{// パス||ホーミングパス||移動状態の場合

		// 位置の取得
		MyLib::Vector3 posBall = pBall->GetPosition();		// ボール
		MyLib::Vector3 posEnd = pBall->GetPosPassEnd();		// ボールのパス終了位置
		MyLib::Vector3 posMy = m_pAI->GetPosition();		// 自分の位置

		// 終了位置のx,zを参照した位置の設定
		MyLib::Vector3 pos = { posEnd.x, posMy.y, posEnd.z };

		// ボールとの距離
		float distance = posMy.DistanceXZ(posBall);

		// 行動状態：歩く
		m_eMove = EMoveType::MOVETYPE_DASH;

		const float CATCH_JUMP_LENGTH = 100.0f;
		const float CATCH_JUMP_HEIGHT = 300.0f;

		// ボールの方へ行く
		if (Approatch(pos, CATCH_JUMP_LENGTH) || distance < CATCH_JUMP_LENGTH)
		{// 終了位置に近づけた||ボールとの距離が範囲内の場合
			if (posBall.y < CATCH_JUMP_HEIGHT)
			{
				m_eAction = EAction::ACTION_JUMP;
			}
		}

#ifdef _DEBUG
		CEffect3D::Create
		(// デバッグ用エフェクト(ターゲット)
			pos,
			VEC3_ZERO,
			MyLib::color::Red(),
			20.0f,
			0.1f,
			1,
			CEffect3D::TYPE::TYPE_NORMAL
		);
#endif

		return;
	}
}


// ターゲット設定 ===================================================================================================================

//==========================================================================
// ターゲット設定(投げる時)
//==========================================================================
CPlayer* CPlayerAIControlLeft::GetThrowTarget()
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
CPlayer* CPlayerAIControlLeft::GetCatchTarget()
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