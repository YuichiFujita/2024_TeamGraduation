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
	const float JUMP_END_POS = 140.0f;

	// 距離間(デフォルト)
	const float LENGTH_IN = 500.0f;
	const float LENGTH_SPACE = 10.0f;
	const float LENGTH_OUT = 200.0f;
	const float LENGTH_LINE = 100.0f;

	// 線越え判定(中心(x)からの距離)
	const float LINE_DISTANCE_OVER = 100.0f;	// 線超え判定の距離(中心 x:0.0f)
	const float RETURN_POS = 700.0f;			// 戻る位置(中心 x:0.0f)

	const float OK_LENGTH = 10.0f;				// 判定の範囲(目的との距離)

	// 距離
	const float CHATCH_LENGTH_IN = 500.0f;
	const float CHATCH_LENGTH_OUT = 700.0f;
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerAIControl::AREA_FUNC CPlayerAIControl::m_AreaFunc[] =	// モード関数
{
	&CPlayerAIControl::AreaNone,				// なし
	&CPlayerAIControl::AreaLeft,				// 左
	&CPlayerAIControl::AreaRight,				// 右
};

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
	&CPlayerAIControl::ThrowTimingNone,			// なし
	&CPlayerAIControl::ThrowTimingNormal,		// 通常
	&CPlayerAIControl::ThrowTimingQuick,		// 速
	&CPlayerAIControl::ThrowTimingDelay,		// 遅
	&CPlayerAIControl::ThrowTimingJumpNormal,	// ジャンプ通常
	&CPlayerAIControl::ThrowTimingJumpQuick,	// ジャンプ速
	&CPlayerAIControl::ThrowTimingJumpDelay,	// ジャンプ遅
};

CPlayerAIControl::THROW_FUNC CPlayerAIControl::m_ThrowFunc[] =	// 投げタイミング関数
{
	&CPlayerAIControl::ThrowNone,				// なし
	&CPlayerAIControl::Throw,					// 投げ
	&CPlayerAIControl::ThrowPass,				// パス
	&CPlayerAIControl::ThrowSpecial,			// スペシャル
};

CPlayerAIControl::CATCH_FUNC CPlayerAIControl::m_CatchFunc[] =	// キャッチ関数
{
	&CPlayerAIControl::CatchNone,			// なし
	&CPlayerAIControl::CatchNormal,			// 通常
	&CPlayerAIControl::CatchJust,			// ジャスト
	&CPlayerAIControl::CatchDash,			// ダッシュ
	&CPlayerAIControl::CatchPass,			// パス
	&CPlayerAIControl::CatchFindBall,		// 見つける
};

CPlayerAIControl::MOVEFORCIBLY_FUNC CPlayerAIControl::m_MoveForciblyFunc[] =	// 行動関数
{
	&CPlayerAIControl::ForciblyNone,		// なし
	&CPlayerAIControl::ForciblyReturn,		// 戻る
};

CPlayerAIControl::MOVE_FUNC CPlayerAIControl::m_MoveFunc[] =	// 行動関数
{
	&CPlayerAIControl::MoveStop,			// 止まる
	&CPlayerAIControl::MoveWalk,			// 歩く
	&CPlayerAIControl::MoveDash,			// 走る
};

CPlayerAIControl::ACTION_FUNC CPlayerAIControl::m_ActionFunc[] =	// 行動関数
{
	&CPlayerAIControl::ActionNone,
	&CPlayerAIControl::ActionJump,
};


//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControl::CPlayerAIControl()
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
	m_sTarget.fDistanceIN = LENGTH_IN;
	m_sTarget.fDistanceOUT = LENGTH_OUT;
	m_eLine = ELine::LINE_IN;
	m_eHeart = EHeart::HEART_NORMAL;
	m_eThrow = EThrow::THROW_NONE;

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
	ModeManager(fDeltaTime, fDeltaRate, fSlowRate);

	AttackDash();

	// 行動管理
	MoveManager(fDeltaTime, fDeltaRate, fSlowRate);

	// アクション更新
	(this->*(m_ActionFunc[m_eAction]))();

	// 投げ更新
	(this->*(m_ThrowFunc[m_eThrow]))();

	// 心
	//PlanHeart();
}

//==========================================================================
// エリア：なし
//==========================================================================
void CPlayerAIControl::AreaNone()
{
	assert(false);
}

//==========================================================================
// エリア：左
//==========================================================================
void CPlayerAIControl::AreaLeft()
{

}

//==========================================================================
// エリア：右
//==========================================================================
void CPlayerAIControl::AreaRight()
{

}


//==========================================================================
// モード管理
//==========================================================================
void CPlayerAIControl::ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsLineOverPlayer())
	{// 線を超えていたら
		m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
		return;
	}

	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() == nullptr ||
		pBall->GetTypeTeam() != m_pAI->GetTeam())
	{// ボールが取得されていない場合||自分とボールを持っているチームが違う場合
		m_sInfo.sMode.eMode = EMode::MODE_CATCH;
	}
	//else if (pBall->GetPlayer() == m_pAI)
	//{// ボールを持っているのが自分だった場合
	//	m_sInfo.sMode.eMode = EMode::MODE_THROW;
	//}

	// モード更新
	(this->*(m_ModeFunc[m_sInfo.sMode.eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 投げの管理
//==========================================================================
void CPlayerAIControl::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	//// 1:ターゲットを決める
	//m_pTarget = nullptr;
	//m_pTarget = GetThrowTarget();

	//// 2:何を投げるか考える関数
	//PlanThrow(m_pTarget, fDeltaTime, fDeltaRate, fSlowRate);


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
void CPlayerAIControl::PlanHeart()
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
void CPlayerAIControl::ThrowTypeNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// その場なのか歩くのか走るのか
	m_eMove = EMoveType::MOVETYPE_WALK;

	// 投げるまでの行動の更新
	(this->*(m_ThrowMoveFunc[m_eMove]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ジャンプ投げ
//==========================================================================
void CPlayerAIControl::ThrowTypeJump(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// その場なのか歩くのか走るのか
	m_eMove = EMoveType::MOVETYPE_WALK;

	// 投げるまでの行動の更新
	(this->*(m_ThrowMoveFunc[m_eMove]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// スペシャル投げ
//==========================================================================
void CPlayerAIControl::ThrowTypeSpecial(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// その場なのか歩くのか走るのか

	// 投げるまでの行動の更新
	(this->*(m_ThrowMoveFunc[m_eMove]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);
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

	if (IsLineOverPlayer())
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
void CPlayerAIControl::ThrowMoveDash(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControl::PlanIsJump(CPlayer* pTarget)
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
void CPlayerAIControl::JumpThrowTiming(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
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
void CPlayerAIControl::ThrowTimingNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sInfo.sThrowInfo.fTiming > 0.0f)
	{
		m_sInfo.sThrowInfo.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// 止まる
	m_eMove = EMoveType::MOVETYPE_STOP;

	// 投げる
	m_eThrow = EThrow::THROW_THROW;

	// 初期化
	ZeroMemory(&m_sInfo, sizeof(m_sInfo));
}

//--------------------------------------------------------------------------
// 投げタイミング(速い)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingQuick(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//--------------------------------------------------------------------------
// 投げタイミング(遅い)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingDelay(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//--------------------------------------------------------------------------
// 跳び投げタイミング(通常)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingJumpNormal(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
		m_eThrow = EThrow::THROW_THROW;	// 投げる
	}
}

//--------------------------------------------------------------------------
// 跳び投げタイミング(速い)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingJumpQuick(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControl::ThrowTimingJumpDelay(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControl::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;
	CPlayer* pPlayer = pBall->GetPlayer();

	if (pPlayer != nullptr)
	{// 誰かがボールを持っている&&自分と同じ場合

		if (pPlayer->GetTeam() == m_pAI->GetTeam() && pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT)
		{// 同じチーム&&外野の場合
			m_sInfo.sCatchInfo.eCatchType = ECatchType::CATCH_TYPE_PASS;
		}
		else
		{
			m_sInfo.sCatchInfo.eCatchType = ECatchType::CATCH_TYPE_NORMAL;
		}
	}
	else if (pBall->GetPlayer() == nullptr)
	{// 誰もボールを持っていない場合
		if (IsWhoPicksUpTheBall() && IsLineOverBall())
		{// 自分よりもボールに近い人がいる&&ボールが相手側にある
			m_sInfo.sCatchInfo.eCatchType = ECatchType::CATCH_TYPE_NORMAL;
			
		}
		else
		{// ボールを取りに行く
			m_sInfo.sCatchInfo.eCatchType = ECatchType::CATCH_TYPE_FIND;
		}
	}

	// キャッチ種類更新
	(this->*(m_CatchFunc[m_sInfo.sCatchInfo.eCatchType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//--------------------------------------------------------------------------
// 通常キャッチ
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControl::CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//--------------------------------------------------------------------------
// キャッチ：ダッシュ
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//--------------------------------------------------------------------------
// キャッチ：パス
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchPass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControl::CatchFindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControl::CatchDistance(CPlayer* pTarget, const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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

		if (Leave(pTarget->GetPosition(), 300.0f))
		{
			m_eMove = EMoveType::MOVETYPE_STOP;
		}
	}
	else if (area = CPlayer::EFieldArea::FIELD_OUT)
	{// ターゲットが外野

		m_eMove = EMoveType::MOVETYPE_WALK;

		if (Leave(pTarget->GetPosition(), 300.0f))
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
// 強制：戻る
//==========================================================================
void CPlayerAIControl::ForciblyReturn()
{
	// プレイヤー情報取得
	MyLib::Vector3 myPos = m_pAI->GetPosition();
	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();

	if (typeTeam == CGameManager::ETeamSide::SIDE_LEFT)
	{// 左
		// AIコントロール情報の取得
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// 歩く
		m_eMove = EMoveType::MOVETYPE_WALK;

		// 近づく
		if (Approatch({ -RETURN_POS, myPos.y, myPos.z }, OK_LENGTH))
		{
			m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
			m_eMove = EMoveType::MOVETYPE_STOP;;
		}
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{// 右
		// AIコントロール情報の取得
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// 歩く
		m_eMove = EMoveType::MOVETYPE_WALK;

		// 近づく
		if (Approatch({ RETURN_POS, myPos.y, myPos.z }, OK_LENGTH))
		{
			m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
			m_eMove = EMoveType::MOVETYPE_STOP;;
		}
	}
	else
	{// エラー
		assert(false);
	}
}

//==========================================================================
// 行動管理
//==========================================================================
void CPlayerAIControl::MoveManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControl::MoveStop()
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
void CPlayerAIControl::MoveWalk()
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
void CPlayerAIControl::MoveDash()
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
// アクション：無
//==========================================================================
void CPlayerAIControl::ActionNone()
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
void CPlayerAIControl::ActionJump()
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// ジャンプ：true
	pControlAIAction->SetIsJump(true);
	pControlAIAction->SetIsJumpFloat(true);

	pControlAIAction->SetJumpRate(1.0f);

	// アクション列挙：なし
	m_eAction = EAction::ACTION_NONE;
}


// 投げ関連 =========================================================================================================================================================

//==========================================================================
// 投げる
//==========================================================================
void CPlayerAIControl::Throw()
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// 投げる
	pControlAIAction->SetIsThrow(true);

	// 投げなし
	m_eThrow = EThrow::THROW_NONE;
}

//==========================================================================
// パス
//==========================================================================
void CPlayerAIControl::ThrowPass()
{

}

//==========================================================================
// スペシャル
//==========================================================================
void CPlayerAIControl::ThrowSpecial()
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// スペシャル
	pControlAIAction->SetIsSpecial(true);

	// 投げなし
	m_eThrow = EThrow::THROW_SPECIAL;
}


// 近づく・離れる =========================================================================================================================================================
//==========================================================================
// 離れる : Leave(離れる相手、離れる距離)
//==========================================================================
bool CPlayerAIControl::Leave(MyLib::Vector3 targetPos, float distance)
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
	float direction = myPos.AngleXZ(targetPos);

	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall && pBall->GetPlayer())
	{
		if (pBall->GetPlayer()->GetTeam() != m_pAI->GetTeam() ||
			pBall->GetPlayer() == m_pAI)
		{// 違うチームが持っている場合
			// カニ進行方向の設定
			pControlAIMove->SetClabDirection(direction);
		}
	}
	else
	{
		m_pAI->SetRotDest(direction);
	}

	return false;
}

//==========================================================================
// 離れる外野 : Leave()
//==========================================================================
void CPlayerAIControl::LeaveOut(float distance)
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
bool CPlayerAIControl::Approatch(MyLib::Vector3 targetPos, float distance)
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
			pBall->GetPlayer() == m_pAI)
		{// 違うチームが持っている場合
			// カニ進行方向の設定
			pControlAIMove->SetClabDirection(direction);
		}
	}
	else
	{
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
void CPlayerAIControl::AttackDash()
{
	// ターゲットのしゅとく
	CPlayer* pTarget = GetThrowTarget();
	if (!pTarget) return;
	MyLib::Vector3 posTarget = pTarget->GetPosition();
	// 自分の位置取得
	MyLib::Vector3 posMy = m_pAI->GetPosition();

	// ラインの位置
	MyLib::Vector3 linePos = { 0.0f, posMy.y, posMy.z };

	// ラインとの距離
	float distance = posMy.DistanceXZ(linePos);

	const float JUMP_POS_X = 300.0f;	// ジャンプ開始位置

	if (distance > JUMP_POS_X)
	{
		// 走る
		m_eMove = EMoveType::MOVETYPE_DASH;

		// 相手の位置に近づく
		if (Approatch(posTarget, 0.0f))
		{
			m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
			m_eMove = EMoveType::MOVETYPE_STOP;
		}

		return;
	}

	m_eAction = EAction::ACTION_JUMP;

	if (m_pAI->GetPosition().y >= JUMP_END_POS)	// 高さによって変わる
	{
		m_eMove = EMoveType::MOVETYPE_STOP;
		m_eThrow = EThrow::THROW_THROW;	// 投げる
	}
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
void CPlayerAIControl::CatchLineLeftDistance()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 自身の位置
	MyLib::Vector3 myPos = m_pAI->GetPosition();

	// チームタイプの取得
	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();

	if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{// 右チームの場合

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
}


// 判定関連 ===================================================================================================================

//==========================================================================
// パスの相手は自分ですか？
//==========================================================================
bool CPlayerAIControl::IsPassTarget()
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
// 線超え判定(プレイヤー)
//==========================================================================
bool CPlayerAIControl::IsLineOverPlayer()
{
	bool bOver = false;	// 超えた判定用

	// プレイヤー情報取得
	MyLib::Vector3 myPos = m_pAI->GetPosition();
	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();

	if (typeTeam == CGameManager::ETeamSide::SIDE_LEFT)
	{// 左
		if (myPos.x > -LINE_DISTANCE_OVER)
		{// 位置が超えていた場合
			bOver = true;
		}
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{// 右
		if (myPos.x < LINE_DISTANCE_OVER)
		{// 位置が超えていた場合
			bOver = true;
		}
	}

	return bOver;
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
			bOver = true;
		}
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
	{
		if (pBall->GetPosition().x < 0.0f)
		{
			bOver = true;
		}
	}

	return bOver;
}


// ターゲット設定 ===================================================================================================================
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

// その他 ===================================================================================================================
//==========================================================================
// フラグのリセット
//==========================================================================
void CPlayerAIControl::ResetFlag()
{
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