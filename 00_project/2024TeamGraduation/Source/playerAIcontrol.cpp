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

#include "playerAIcontrol_left.h"
#include "playerAIcontrol_right.h"

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
	// キャッチ関連
	const float CHATCH_LENGTH_IN_PAIR = 700.0f;		// 内野：相手との距離
	const float CHATCH_LENGTH_IN_ALLY = 100.0f;		// 内野：味方との距離

	const float CHATCH_LENGTH_OUT = 400.0f;		// 外野との距離
	const float CHATCH_LENGTH_TARGET = 400.0f;	// ターゲットとの距離

	const float CATCH_JUMP_LENGTH = 100.0f;		// ジャンプキャッチの距離
	const float CATCH_JUMP_HEIGHT = 300.0f;		// ジャンプキャッチする高さ

	// 距離間(デフォルト)
	const float LENGTH = 500.0f;				// 内野
	const float LENGTH_SPACE = 10.0f;			// 
	const float LENGTH_OUT = 200.0f;			// 外野
	const float LENGTH_LINE = 100.0f;			// 線

	// パス
	const float STEAL_CANCEL_LENGTH = 100.0f;	// あきらめる距離

	// ステータス関連
	const float JUMP_RATE = 1.0f;				// ジャンプの割合(高さ)

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

CPlayerAIControl::THROW_FUNC CPlayerAIControl::m_ThrowFunc[] =	// 投げ関数
{
	&CPlayerAIControl::ThrowNone,				// なし
	&CPlayerAIControl::Throw,					// 投げ
	&CPlayerAIControl::ThrowPass,				// パス
	&CPlayerAIControl::ThrowSpecial,			// スペシャル
};

CPlayerAIControl::THROWMOVE_FUNC CPlayerAIControl::m_ThrowMoveFunc[] =	// 投げ行動関数
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

CPlayerAIControl::MOVEFORCIBLY_FUNC CPlayerAIControl::m_MoveForciblyFunc[] =	// 行動関数
{
	&CPlayerAIControl::ForciblyNone,			// なし
	&CPlayerAIControl::ForciblyStop,			// 止まる
	&CPlayerAIControl::ForciblyReturn,			// 戻る
	&CPlayerAIControl::ForciblyStart,			// 初め
};

CPlayerAIControl::MOVE_FUNC CPlayerAIControl::m_MoveFunc[] =	// 行動関数
{
	&CPlayerAIControl::MoveStop,				// 止まる
	&CPlayerAIControl::MoveWalk,				// 歩く
	&CPlayerAIControl::MoveDash,				// 走る
};

CPlayerAIControl::ACTION_FUNC CPlayerAIControl::m_ActionFunc[] =	// 行動関数
{
	&CPlayerAIControl::ActionNone,				// なし
	&CPlayerAIControl::ActionJump,				// ジャンプ
};

CPlayerAIControl::CATCH_FUNC CPlayerAIControl::m_CatchFunc[] =	// キャッチ関数
{
	&CPlayerAIControl::CatchNone,				// なし
	&CPlayerAIControl::CatchNormal,				// 通常
	&CPlayerAIControl::CatchJust,				// ジャスト
	&CPlayerAIControl::CatchDash,				// ダッシュ
	&CPlayerAIControl::CatchPassGive,			// パス貰う
	&CPlayerAIControl::CatchPassSteal,			// パス奪う
	&CPlayerAIControl::CatchFindBall,			// 見つける
};


//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControl::CPlayerAIControl()
{
	// 列挙の初期化
	m_eMode = EMode::MODE_NONE;
	m_eForcibly = EMoveForcibly::FORCIBLY_START;
	m_eMove = EMoveType::MOVETYPE_STOP;
	m_eHeart = EHeart::HEART_NONE;
	m_eAction = EAction::ACTION_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
	m_eThrow = EThrow::THROW_NONE;
	m_eThrowTiming = EThrowTiming::TIMING_NONE;
	m_eCatchType = ECatchType::CATCH_TYPE_NONE;

	// 構造体の初期化
	ZeroMemory(&m_sThrow, sizeof(m_sThrow));
	ZeroMemory(&m_sDistance, sizeof(m_sDistance));
}

//==========================================================================
// 生成
//==========================================================================
CPlayerAIControl* CPlayerAIControl::Create(CPlayer* player)
{
	if (!player) assert(("プレイヤー情報が無いよ", false));

	// メモリの確保
	CPlayerAIControl* pObj = nullptr;

	switch (player->GetTeam())
	{// チーム別
	case CGameManager::ETeamSide::SIDE_LEFT:
		pObj = DEBUG_NEW CPlayerAIControlLeft;
		break;

	case CGameManager::ETeamSide::SIDE_RIGHT:
		pObj = DEBUG_NEW CPlayerAIControlRight;
		break;

	default:
		assert(("チームが無いよ", false));
		break;
	}

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
	m_sDistance.fInPair = CHATCH_LENGTH_IN_PAIR;
	m_sDistance.fInAlly = CHATCH_LENGTH_IN_ALLY;
	m_sDistance.fOut = CHATCH_LENGTH_OUT;
	m_sDistance.fTarget = CHATCH_LENGTH_TARGET;


	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAIControl::Uninit()
{
	if (!m_pAI) return;
	m_pAI->Uninit();
	m_pAI = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIControl::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
 {
	// モードの更新
	UpdateMode();

	// 見る更新
	//UpdateSee();

	// モード管理
	ModeManager(fDeltaTime, fDeltaRate, fSlowRate);

	{
		// 更新：行動
		UpdateMove();
		// 更新：アクション
		UpdateAction();
		// 更新：投げ
		UpdateThrow();
	}

#ifdef _DEBUG

	if (IsWhoPicksUpTheBall())
	{// 自分より近いプレイヤーがいた場合
		return;
	}

	CEffect3D::Create
	(// デバッグ用エフェクト(ターゲット)
		m_pAI->GetPosition(),
		VEC3_ZERO,
		MyLib::color::Yellow(),
		20.0f,
		0.1f,
		1,
		CEffect3D::TYPE::TYPE_NORMAL
	);	//
#endif
}

//==========================================================================
// モード更新
//==========================================================================
void CPlayerAIControl::UpdateMode()
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) {// ボールがない場合
		m_eMode = EMode::MODE_NONE;
		return;
	}

	if (pBall->GetPlayer() == nullptr || pBall->GetTypeTeam() != m_pAI->GetTeam())
	{// ボールが取得されていない場合||自分とボールを持っているチームが違う場合
		m_eMode = EMode::MODE_CATCH;
	}
	else if (pBall->GetPlayer() == m_pAI)
	{// ボールを持っているのが自分だった場合
		m_eMode = EMode::MODE_THROW;
	}
}

//==========================================================================
// モード管理
//==========================================================================
void CPlayerAIControl::ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_eForcibly == EMoveForcibly::FORCIBLY_RETURN) return;

	if (IsLineOverPlayer() && GetForcibly() != EMoveForcibly::FORCIBLY_START)
	{// 線を超えていた&&強制行動：初め以外の場合
		m_eMove = EMoveType::MOVETYPE_STOP;
		m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
		return;
	}

	// モード更新
	(this->*(m_ModeFunc[m_eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 投げの管理
//==========================================================================
void CPlayerAIControl::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 投げの流れ
	PlanThrow();

	// 投げの種類
	UpdateThrowType();

#ifdef _DEBUG

	// 1:ターゲットを決める
	CPlayer* pTarget = nullptr;
	pTarget = GetThrowTarget();

	if (!pTarget)
	{
		return;
	}

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
// キャッチの管理
//==========================================================================
void CPlayerAIControl::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// キャッチの更新
	UpdateCatch();

	//CatchState();
}

//==========================================================================
// 投げ流れ
//==========================================================================
void CPlayerAIControl::PlanThrow()
{
	// 取得
	CGameManager* pGameManager = CGameManager::GetInstance();	// ゲームマネージャ
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(m_pAI->GetTeam());	// 自分のチーム情報

	if (m_eThrowType == EThrowType::THROWTYPE_NONE)
	{
		if (pTeamStatus->IsMaxSpecial())
		{// ゲージが溜まっていたら
			// スペシャル投げ
			m_eThrowType = EThrowType::THROWTYPE_SPECIAL;
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
}

//================================================================================
// 強制行動の更新処理
//================================================================================
void CPlayerAIControl::UpdateForcibly()
{
	// 強制行動更新
	(this->*(m_MoveForciblyFunc[m_eForcibly]))();
}

//--------------------------------------------------------------------------
// 強制：止まる
//--------------------------------------------------------------------------
void CPlayerAIControl::ForciblyStop()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 歩く
	m_eMove = EMoveType::MOVETYPE_STOP;
}

#if 0
//--------------------------------------------------------------------------
// 強制：戻る
//--------------------------------------------------------------------------
void CPlayerAIControl::ForciblyReturn()
{
	// プレイヤー情報取得
	MyLib::Vector3 myPos = m_pAI->GetPosition();
	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();

	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 歩く
	m_eMove = EMoveType::MOVETYPE_WALK;

	// 近づく
	if (Approatch({ RETURN_POS, myPos.y, myPos.z }, OK_LENGTH))
	{
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eMove = EMoveType::MOVETYPE_STOP;
	}
}
#endif

//--------------------------------------------------------------------------
// 強制：初めのダッシュ
//--------------------------------------------------------------------------
void CPlayerAIControl::ForciblyStart()
{
	if (IsWhoPicksUpTheBall())
	{// 自分より近いプレイヤーがいた場合
		m_eMove = EMoveType::MOVETYPE_STOP;
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eAction = EAction::ACTION_NONE;
		return;
	}

	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall)
	{// ボールがない場合
		m_eMove = EMoveType::MOVETYPE_STOP;
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eAction = EAction::ACTION_NONE;
		return;
	}

	CPlayer* pPlayer = pBall->GetPlayer();
	if (pPlayer)
	{// 誰かがボールを持った場合
		m_eMove = EMoveType::MOVETYPE_STOP;
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eAction = EAction::ACTION_NONE;
		return;
	}

	m_eAction = EAction::ACTION_JUMP;
	m_eMove = EMoveType::MOVETYPE_WALK;
}

//================================================================================
// 行動の更新処理
//================================================================================
void CPlayerAIControl::UpdateMove()
{
	// 強制行動更新
	(this->*(m_MoveForciblyFunc[m_eForcibly]))();

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

//================================================================================
// アクションの更新処理
//================================================================================
void CPlayerAIControl::UpdateAction()
{
	// アクション更新
	(this->*(m_ActionFunc[m_eAction]))();
}

//--------------------------------------------------------------------------
// アクション：無
//--------------------------------------------------------------------------
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

//--------------------------------------------------------------------------
// アクション：ジャンプ
//--------------------------------------------------------------------------
void CPlayerAIControl::ActionJump()
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// ジャンプ：true
	pControlAIAction->SetIsJump(true);
	pControlAIAction->SetIsJumpFloat(true);

	// 割合
	pControlAIAction->SetJumpRate(JUMP_RATE);

	// アクション列挙：なし
	m_eAction = EAction::ACTION_NONE;
}

//================================================================================
// 投げタイプ
//================================================================================
void CPlayerAIControl::UpdateThrowType()
{
	// 投げ種類の更新
	(this->*(m_ThrowTypeFunc[m_eThrowType]))();
}

//--------------------------------------------------------------------------
// 通常投げ
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTypeNormal()
{
	//// その場なのか歩くのか走るのか
	//m_eMove = EMoveType::MOVETYPE_WALK;

	//// 投げるまでの行動の更新
	//(this->*(m_ThrowMoveFunc[m_eMove]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);

	m_eThrow = EThrow::THROW_NORMAL;
}

//--------------------------------------------------------------------------
// ジャンプ投げ
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTypeJump()
{
	// その場なのか歩くのか走るのか
	//m_eMove = EMoveType::MOVETYPE_WALK;

	CPlayer* pTartget = GetThrowTarget();

	// 走り投げ
	AttackDash(pTartget);
}

//--------------------------------------------------------------------------
// スペシャル投げ
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTypeSpecial()
{
	m_eThrow = EThrow::THROW_SPECIAL;
}

//================================================================================
// 投げ行動の更新処理
//================================================================================
void CPlayerAIControl::UpdateThrowMove()
{

}

//--------------------------------------------------------------------------
// その場投げ
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowMoveNone()
{
	// タイミングはどうする？

	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 歩きリセット
	pControlAIMove->SetIsWalk(false);

	// 投げる
	pControlAIAction->SetIsThrow(true);

	m_eThrowType = EThrowType::THROWTYPE_NONE;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;
}

//--------------------------------------------------------------------------
// 歩き投げ
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowMoveWalk()
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
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;
}

//--------------------------------------------------------------------------
// 走り投げ
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowMoveDash()
{
	if (IsLineOverPlayer())
	{// 線を超えていた場合(数値は固定変数)
		m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
		return;
	}

	// タイミングはどうする？
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;
}

//================================================================================
// 投げの更新処理
//================================================================================
void CPlayerAIControl::UpdateThrow()
{
	// 投げ更新
	(this->*(m_ThrowFunc[m_eThrow]))();
}

//--------------------------------------------------------------------------
// 投げる
//--------------------------------------------------------------------------
void CPlayerAIControl::Throw()
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// 投げる
	pControlAIAction->SetIsThrow(true);

	// 投げ状態：無
	m_eThrow = EThrow::THROW_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
}

//--------------------------------------------------------------------------
// パス
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowPass()
{

}

//--------------------------------------------------------------------------
// スペシャル
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowSpecial()
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// スペシャル投げ
	pControlAIAction->SetIsSpecial(true);

	// 投げなし
	m_eThrow = EThrow::THROW_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
}

//================================================================================
// タイミングの更新処理
//================================================================================
void CPlayerAIControl::UpdateThrowTiming(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態更新
	(this->*(m_ThrowTimingFunc[m_eThrowTiming]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//--------------------------------------------------------------------------
// 投げタイミング：(通常)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 止まる
	m_eMove = EMoveType::MOVETYPE_STOP;

	// 投げる
	m_eThrow = EThrow::THROW_NORMAL;
}

//--------------------------------------------------------------------------
// 投げタイミング：(速い)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sThrow.fTiming > 0.0f)
	{
		m_sThrow.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// 止まる
	m_eMove = EMoveType::MOVETYPE_STOP;

	// 投げる
	m_eThrow = EThrow::THROW_NORMAL;
}

//--------------------------------------------------------------------------
// 投げタイミング：(遅い)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sThrow.fTiming > 0.0f)
	{
		m_sThrow.fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// 止まる
	m_eMove = EMoveType::MOVETYPE_STOP;

	// 投げる
	m_eThrow = EThrow::THROW_NORMAL;
}

//--------------------------------------------------------------------------
// 跳び投げタイミング：(通常)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingJumpNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	m_eMove = EMoveType::MOVETYPE_STOP;
	m_eAction = EAction::ACTION_JUMP;

	if (m_pAI->GetPosition().y >= playerAIcontrol::THROW_JUMP_END)	// 高さによって変わる
	{
		m_eThrow = EThrow::THROW_NORMAL;	// 投げる
	}
}

//--------------------------------------------------------------------------
// 跳び投げタイミング：(速い)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingJumpQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// どれくらいの距離移動するか処理を追加


	pControlAIMove->SetIsWalk(false);	// 歩きリセット
	pControlAIAction->SetIsJump(true);	// ジャンプオン

	if (m_pAI->GetPosition().y >= playerAIcontrol::THROW_JUMP_END * 0.5f)
	{
		// 投げる
		pControlAIAction->SetIsThrow(true);

		// 変数リセット
		//ResetFlag();
	}
}

//--------------------------------------------------------------------------
// 跳び投げタイミング：(遅い)
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTimingJumpDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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

	if (pos.y >= playerAIcontrol::THROW_JUMP_END)	// 高さによって変わる
	{
		m_sThrow.bFoldJump = true;	// 折り返しオン
	}

	if (!m_sThrow.bFoldJump) { return; }

	if (pos.y <= playerAIcontrol::THROW_JUMP_END * 0.5f)	// 高さによって変わる
	{
		// 投げる
		pControlAIAction->SetIsThrow(true);

		// それぞれの状態のリセット
		//ResetFlag();
	}
}

//================================================================================
// キャッチの更新処理
//================================================================================
void CPlayerAIControl::UpdateCatch()
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// ボールねぇぞ
	CPlayer* pPlayer = pBall->GetPlayer();

	if (pPlayer != nullptr)
	{// 誰かがボールを持っている

		if (pPlayer->GetTeam() == m_pAI->GetTeam() && pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT)
		{// 同じチーム&&外野の場合
			m_eCatchType = ECatchType::CATCH_TYPE_PASS_GIVE;	// パス待ち
		}
		else
		{
			m_eCatchType = ECatchType::CATCH_TYPE_NORMAL;	// 通常
		}
	}
	else
	{// 誰もボールを持っていない場合

		// ボールステート取得
		CBall::EState stateBall = pBall->GetState();

		if (stateBall == CBall::EState::STATE_PASS ||
			stateBall == CBall::EState::STATE_HOM_PASS ||
			stateBall == CBall::EState::STATE_MOVE)
		{// パス||ホーミングパス||移動状態の場合
			m_eCatchType = ECatchType::CATCH_TYPE_PASS_STEAL;
		}
		else if (IsWhoPicksUpTheBall() && IsLineOverBall())
		{// 自分よりもボールに近い人がいる&&ボールが相手側にある
			m_eCatchType = ECatchType::CATCH_TYPE_NORMAL;
		}
		else
		{// ボールを取りに行く
			m_eCatchType = ECatchType::CATCH_TYPE_FIND;
		}
	}

	// キャッチ種類更新
	(this->*(m_CatchFunc[m_eCatchType]))();
}

//--------------------------------------------------------------------------
// 通常キャッチ
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchNormal()
{
	// パスターゲットが自分の場合
	//if (IsPassTarget()) return;

	// ボールを持つ相手を取得
	CPlayer* pTarget = GetCatchTarget();
	if (!pTarget) { return; }

	// 距離(ボールを持っている奴との)
	DistanceCatch(pTarget);
}

//--------------------------------------------------------------------------
// キャッチ：ジャスト
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchJust()
{

}

//--------------------------------------------------------------------------
// キャッチ：ダッシュ
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchDash()
{
	m_eMove = EMoveType::MOVETYPE_DASH;
}

//--------------------------------------------------------------------------
// キャッチ：パスを貰う
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchPassGive()
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
// キャッチ：パスを奪う
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchPassSteal()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	// 同じチームの場合
	if (pBall->GetTypeTeam() == m_pAI->GetTeam()) return;

	// ボールステート取得
	CBall::EState stateBall = pBall->GetState();
	CPlayer* pTarget = pBall->GetTarget();

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

		if (pTarget)
		{
			// ターゲットとボールの位置
			float distanth0 = pTarget->GetPosition().DistanceXZ(posBall);

			if (distanth0 < STEAL_CANCEL_LENGTH)
			{// ボールとパス先の距離が範囲内ならあきらめる
				m_eMove = EMoveType::MOVETYPE_STOP;
				return;
			}
		}

		// ボールとの距離
		float distance = posMy.DistanceXZ(posBall);

		// 行動状態：走る
		m_eMove = EMoveType::MOVETYPE_DASH;

		// ボールの方へ行く
		if (Approatch(pos, CATCH_JUMP_LENGTH) || distance < CATCH_JUMP_LENGTH)
		{// 終了位置に近づけた||ボールとの距離が範囲内の場合
			if (posBall.y < CATCH_JUMP_HEIGHT)
			{// 取れそうな高さに来た！
				m_eAction = EAction::ACTION_JUMP;
			}
		}
	}
}

//--------------------------------------------------------------------------
// キャッチ：取りに行く
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchFindBall()
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
	if (Approatch(pBall->GetPosition(), 50.0f))
	{
		m_eMove = EMoveType::MOVETYPE_STOP;
	}
}



//==========================================================================
// 心
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
		//m_eHeart = EHeart::HEART_NORMAL;
	}
}



//==========================================================================
// 距離：キャッチ
//==========================================================================
void CPlayerAIControl::DistanceCatch(CPlayer* pTarget)
{
	DistanceTeam();
}

//==========================================================================
// 距離：チーム
//==========================================================================
void CPlayerAIControl::DistanceTeam()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	float fMin = 10000000.0f;

	// 自分情報
	MyLib::Vector3 posMy = m_pAI->GetPosition();			// 位置情報の取得
	CGameManager::ETeamSide TeamMy = m_pAI->GetTeam();	// 所属チーム

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// プレイヤー位置
		CPlayer::EFieldArea areaPlayer = pPlayer->GetAreaType();	// エリアタイプ

		// 自分の場合
		if (pPlayer == m_pAI) continue;

		// 同じチームの外野
		if (TeamMy == pPlayer->GetTeam() && areaPlayer == CPlayer::EFieldArea::FIELD_OUT) continue;

		// 敵との距離を求める
		float fLength = posMy.DistanceXZ(posPlayer);

		// 距離：取得
		float length = GetDistance(areaPlayer, TeamMy, pPlayer->GetTeam());

		if (fLength < length) { // より近い相手プレイヤーがいた場合
		
			// 一番近い奴が優先
			fMin = length;

			// 近いプレイヤーとの角度
			float angle = posPlayer.AngleXZ(posMy);

			// カニ進行方向の設定
			pControlAIMove->SetClabDirection(angle);

			// 行動：歩く
			m_eMove = EMoveType::MOVETYPE_WALK;

			return;

		} else{
			// 行動：止まる
			m_eMove = EMoveType::MOVETYPE_STOP;
		}

	}
}

//==========================================================================
// 距離：取得
//==========================================================================
float CPlayerAIControl::GetDistance(CPlayer::EFieldArea area, CGameManager::ETeamSide teamMy, CGameManager::ETeamSide teamPair)
{
	float distance = 0.0f;

	// 体力によって変えたり
	// 心によって変えたり

	if (area == CPlayer::EFieldArea::FIELD_IN && teamMy == teamPair)
	{// 内野で同じチーム
		distance = m_sDistance.fInAlly;
	}
	else if(area == CPlayer::EFieldArea::FIELD_IN && teamMy != teamPair)
	{// 内野で違うチーム
		distance = m_sDistance.fInPair;
	}
	else if (area == CPlayer::EFieldArea::FIELD_OUT)
	{// 外野
		distance = m_sDistance.fOut;
	}

	return distance;
}

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
	if (!pTarget) return nullptr;

	// 見る
	SeeTarget(pTarget->GetPosition());

	return pTarget;
}


//================================================================================
// 見るの更新処理
//================================================================================
void CPlayerAIControl::UpdateSee()
{
	if (m_eMode == EMode::MODE_CATCH)
	{// ボールを見る
		SeeBall();
	}
	else if (m_eMode == EMode::MODE_THROW)
	{// ターゲットを見る		
		CPlayer* pTarget = GetThrowTarget();
		if (!pTarget) return;

		SeeTarget(pTarget->GetPosition());
	}
}

//================================================================================
// キャッチ時の状態
//================================================================================
void CPlayerAIControl::CatchState()
{
	// 位置
	MyLib::Vector3 posMy = m_pAI->GetPosition();

	MyLib::Vector3 posLine = { 0.0f, posMy.y, posMy.z };

	// 線と自分の距離
	float distance = posMy.DistanceXZ(posLine);

	if (m_eAction == EAction::ACTION_JUMP)
	{
		if (0.0f < posMy.x)
		{
			m_eAction = EAction::ACTION_NONE;
			m_eMove = EMoveType::MOVETYPE_STOP;
		}
	}
}

//==========================================================================
// ターゲットを見る
//==========================================================================
void CPlayerAIControl::SeeTarget(MyLib::Vector3 pos)
{
	if (!m_pAI) return;

	// 向き設定
	float angle = m_pAI->GetPosition().AngleXZ(pos);
	m_pAI->SetRotDest(angle);
}

//==========================================================================
// ボールを見る
//==========================================================================
void CPlayerAIControl::SeeBall()
{
	// ボール情報の取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;
	if (!m_pAI) return;

	// ボールとの角度
	float angle = m_pAI->GetPosition().AngleXZ(pBall->GetPosition());

	// 角度の設定
	m_pAI->SetRotDest(angle);
}

//==========================================================================
// 離れる : Leave(離れる相手、離れる距離)
//==========================================================================
bool CPlayerAIControl::Leave(MyLib::Vector3 targetPos, float distance)
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ボール情報の取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	// 自分の位置情報
	MyLib::Vector3 myPos = m_pAI->GetPosition();

	// 相手と自分の距離
	float length = myPos.DistanceXZ(targetPos);	// 距離：相手と自分

	if (distance < length) {// 移動タイプ：無
		return true;
	}

	// ターゲットから自分の角度
	float direction = targetPos.AngleXZ(myPos);

	if (pBall)
	{// ボールが存在しない
		CPlayer* pPlayerBall = pBall->GetPlayer();

		if (!pPlayerBall) {// プレイヤーがいない
			// 角度設定
			m_pAI->SetRotDest(direction);
			return false;

		} if (pPlayerBall->GetTeam() != m_pAI->GetTeam() &&
			pPlayerBall != m_pAI) {// 違うチームが持っている && 持ち主が自分以外場合

			if (m_eMove == EMoveType::MOVETYPE_DASH)
			{// 行動：走る
				// 角度設定
				m_pAI->SetRotDest(direction);
				return false;
			}

			// カニ進行方向の設定
			pControlAIMove->SetClabDirection(direction);
			return false;
		}
	}

	// 角度設定
	m_pAI->SetRotDest(direction);


#if 1
	CEffect3D::Create
	(// デバッグ用エフェクト(ターゲット)
		targetPos,
		VEC3_ZERO,
		MyLib::color::Blue(),
		20.0f,
		0.1f,
		1,
		CEffect3D::TYPE::TYPE_NORMAL
	);
#endif

	return false;
}

//==========================================================================
// 近づく : Approatch(近づく相手、近づく距離)
//==========================================================================
bool CPlayerAIControl::Approatch(MyLib::Vector3 targetPos, float distance)
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ボール情報の取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	// 自分の位置情報
	MyLib::Vector3 myPos = m_pAI->GetPosition();

	// 相手と自分の距離
	float length = myPos.DistanceXZ(targetPos);	// 距離：相手と自分

	if (distance > length) {// 移動タイプ：無
		return true;
	}

	// 自分からターゲットの角度
	float direction = myPos.AngleXZ(targetPos);

	if (pBall)
	{// ボールが存在しない
		CPlayer* pPlayerBall = pBall->GetPlayer();

		if (!pPlayerBall) {// プレイヤーがいない
		
			// 角度設定
			m_pAI->SetRotDest(direction);

			return false;

		} if (pPlayerBall->GetTeam() != m_pAI->GetTeam() &&
			pPlayerBall != m_pAI) {// 違うチームが持っている && 持ち主が自分以外場合

			if (m_eMove == EMoveType::MOVETYPE_DASH)
			{// 行動：走る
				// 角度設定
				m_pAI->SetRotDest(direction);
				return false;
			}

			// カニ進行方向の設定
			pControlAIMove->SetClabDirection(direction);
			return false;
		}
	}

	// 角度設定
	m_pAI->SetRotDest(direction);
	

#if 1
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
// 誰がボールを取りに行きますか？
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
	if (!pBall) return true;
	if (pBall->GetPlayer()) return true;

	// ボール位置取得
	MyLib::Vector3 posBall = pBall->GetPosition();
	fMyDis = Mypos.DistanceXZ(posBall);	// 自分からボールとの距離

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報

		if (pPlayer == m_pAI) continue;

		if (
			(pPlayer->GetTeam() != typeTeam) ||
			(pPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER) &&
			(pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT))
		{// 違うチーム||外野&&ユーザー&&自分の場合
			continue;
		}

		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// プレイヤー位置

		// チームからボールとの距離を求める
		float fLength = posPlayer.DistanceXZ(posBall);

		if (fLength < fMyDis)
		{ // より近い味方プレイヤーがいた場合
			return true;
		}
	}

	return false;
}
