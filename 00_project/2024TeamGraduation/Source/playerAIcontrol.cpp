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
	const float CATCH_JUMP_LENGTH = 100.0f;		// ジャンプキャッチの距離
	const float CATCH_JUMP_HEIGHT = 300.0f;		// ジャンプキャッチする高さ

	// 行動タイプ
	const int MOVETYPE_WAIT_MODE_MAX		= 10;		// 待機に入る確率幅 (最低は0固定)
	const int MOVETYPE_WAIT_MODE_IN			= 6;		// 待機に入る確率(この数値以下の場合、待機状態)
	const int MOVETYPE_WAIT_TIME_MAX		= 20;		// 最大：待機時間
	const int MOVETYPE_WAIT_TIME_MIN		= 10;		// 最低：
	const int MOVETYPE_ATYAKOTYA_TIME_MAX	= 10;		// 最大：あっちゃこっちゃ時間
	const int MOVETYPE_ATYAKOTYA_TIME_MIN	= 5;		// 最低：
	const int MOVETYPE_LEFTRIGHT_TIME_MAX	= 10;		// 最大：左右移動時間
	const int MOVETYPE_LEFTRIGHT_TIME_MIN	= 5;		// 最低：
	const int MOVETYPE_UPDOWN_TIME_MAX		= 10;		// 最大：上下移動時間
	const int MOVETYPE_UPDOWN_TIME_MIN		= 5;			// 最低：

	// パス
	const float STEAL_CANCEL_LENGTH = 100.0f;	// あきらめる距離

	// ステータス関連
	const float JUMP_RATE = 1.0f;				// ジャンプの割合(高さ)
	const float MOTIVATION_MAX = 100;			// モチベーション(MAX)
		
	// 距離間(デフォルト)
	const float LENGTH_TARGET	= 400.0f;			// ターゲットとの距離(デフォルト)
	const float LENGTH_FRIEND	= 100.0f;		// 内野：味方との距離(デフォルト)
	const float LENGTH_OUT		= 100.0f;		// 外野との距離(デフォルト)

	const float BALL_DISTANCE = 300.0f;				// 
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerAIControl::MODE_FUNC CPlayerAIControl::m_ModeFunc[] =	// モード関数
{
	&CPlayerAIControl::ModeIdle,				// 待機
	&CPlayerAIControl::ModeAttack,				// 攻め
	&CPlayerAIControl::ModeDefense,				// 守り
};

CPlayerAIControl::MOVEFORCIBLY_FUNC CPlayerAIControl::m_MoveForciblyFunc[] =	// 強制行動関数
{
	&CPlayerAIControl::ForciblyNone,			// なし
	&CPlayerAIControl::ForciblyStop,			// 止まる
	&CPlayerAIControl::ForciblyReturn,			// 戻る
	&CPlayerAIControl::ForciblyStart,			// 初め
};

CPlayerAIControl::THROWTYPE_FUNC CPlayerAIControl::m_ThrowTypeFunc[] =	// 投げタイプ関数
{
	&CPlayerAIControl::ThrowTypeNone,			// なし
	&CPlayerAIControl::ThrowTypeNormal,			// 通常
	&CPlayerAIControl::ThrowTypeJump,			// ジャンプ
	&CPlayerAIControl::ThrowTypeSpecial,		// スペシャル
};

CPlayerAIControl::ACTION_FUNC CPlayerAIControl::m_ActionFunc[] =	// キャッチ関数
{
	&CPlayerAIControl::MoveIdle,				// なし
	&CPlayerAIControl::MoveDodge,				// 回避
	&CPlayerAIControl::MoveSupport,				// サポート
	&CPlayerAIControl::MoveChaseBall,			// ボールを追いかける
	&CPlayerAIControl::MoveRetreat,				// 後退
	&CPlayerAIControl::MoveRandom,				// ランダム
};

// フラグ
CPlayerAIControl::MOVEFLAG_FUNC CPlayerAIControl::m_MoveFlagFunc[] =	// 行動フラグ関数
{
	&CPlayerAIControl::MoveFlagStop,			// 止まる
	&CPlayerAIControl::MoveFlagWalk,			// 歩く
	&CPlayerAIControl::MoveFlagBlink,			// 歩く
	&CPlayerAIControl::MoveFlagDash,			// 走る
};

CPlayerAIControl::THROWFLAG_FUNC CPlayerAIControl::m_ThrowFlagFunc[] =	// 投げフラグ関数
{
	&CPlayerAIControl::ThrowFlagNone,			// なし
	&CPlayerAIControl::ThrowFlag,				// 投げ
	&CPlayerAIControl::ThrowFlagPass,			// パス
	&CPlayerAIControl::ThrowFlagSpecial,		// スペシャル
};

CPlayerAIControl::ACTIONFLAG_FUNC CPlayerAIControl::m_ActionFlagFunc[] =	// アクションフラグ関数
{
	&CPlayerAIControl::ActionFlagNone,			// なし
	&CPlayerAIControl::ActionFlagJump,			// ジャンプ
};

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControl::CPlayerAIControl()
{
	// 列挙の初期化
	m_eMode = EMode::MODE_IDLE;
	m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
	m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
	m_eActionFlag = EActionFlag::ACTION_NONE;
	m_eThrowFlag = EThrowFlag::THROW_NONE;
	m_eAction = EAction::IDLE;

	// 構造体の初期化
	ZeroMemory(&m_sThrow, sizeof(m_sThrow));
	ZeroMemory(&m_sMove, sizeof(m_sMove));
	ZeroMemory(&m_sParameter, sizeof(m_sParameter));

	// 変数の初期化
	m_fDistance = 0.0f;
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
	// 値の初期化
	//m_eForcibly = EMoveForcibly::FORCIBLY_START;
	m_sParameter.nMotivation = MOTIVATION_MAX;

	// 心の初期化
	InitHeart();

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
	// パラメータ更新
	UpdateParameter();

	// 管理：モード
	ModeManager(fDeltaTime, fDeltaRate, fSlowRate);

	// 更新：強制行動
	UpdateForcibly();
	
	{// フラグの更新

		// 更新：行動
		UpdateMoveFlag();
		// 更新：アクション
		UpdateActionFlag();
		// 更新：投げ
		UpdateThrowFlag();
	}

	// 更新：行動時間
	UpdateMoveTimer(fDeltaTime, fDeltaRate, fSlowRate);

#ifdef _DEBUG

	MyLib::Vector3 pos = m_pAI->GetPosition();

	// 心ごとにパラメータの割り当て
	switch (m_sParameter.eHeartMain)
	{
	case EHeartMain::HEART_MAIN_NORMAL:	// 通常
		CEffect3D::Create
		(// デバッグ用エフェクト(ターゲット)
			{ pos.x, pos.y + 90.0f, pos.z },
			VEC3_ZERO,
			MyLib::color::White(),
			10.0f,
			0.1f,
			1,
			CEffect3D::TYPE::TYPE_NORMAL
		);	//
		break;

	case EHeartMain::HEART_MAIN_STRONG:	// 強気
		CEffect3D::Create
		(// デバッグ用エフェクト(ターゲット)
			{ pos.x, pos.y + 90.0f, pos.z },
			VEC3_ZERO,
			MyLib::color::Red(),
			10.0f,
			0.1f,
			1,
			CEffect3D::TYPE::TYPE_NORMAL
		);	//
		break;

	case EHeartMain::HEART_MAIN_TIMID:	// 弱気
		CEffect3D::Create
		(// デバッグ用エフェクト(ターゲット)
			{ pos.x, pos.y + 90.0f, pos.z },
			VEC3_ZERO,
			MyLib::color::Blue(),
			10.0f,
			0.1f,
			1,
			CEffect3D::TYPE::TYPE_NORMAL
		);	//
		break;

	default:
		assert(false);
		break;
	}
#endif
}

//==========================================================================
// モード管理
//==========================================================================
void CPlayerAIControl::ModeManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) {// ボールが世界に存在しない場合
		m_eMode = EMode::MODE_IDLE;
		return;
	}

	if (pBall->GetPlayer() == m_pAI)
	{// ボールを持っているのが自分だった場合
		m_eMode = EMode::MODE_ATTACK;	// 投げ
	}
	else if (pBall->GetPlayer() == nullptr || pBall->GetPlayer() != nullptr && pBall->GetTypeTeam() != m_pAI->GetTeam())
	{// ボールが取得されていない場合||自分とボールを持っているチームが違う場合
		m_eMode = EMode::MODE_DEFENSE;	// キャッチ
	}
	else if (pBall->GetPlayer() != nullptr && pBall->GetTypeTeam() == m_pAI->GetTeam())
	{// ボールが取得されていない場合||自分とボールを持っているチームが同じ場合
		m_eMode = EMode::MODE_IDLE;	// 待機
	}

	// モードの更新
	UpdateMode(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// モード更新
//==========================================================================
void CPlayerAIControl::UpdateMode(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsLineOverPlayer() && m_eForcibly != EMoveForcibly::FORCIBLY_START)
	{// 線を超えていた&&強制行動：初め以外の場合
		m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
		return;
	}

	// モード更新
	(this->*(m_ModeFunc[m_eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
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
	m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
}

#if 0
//--------------------------------------------------------------------------
// 強制：戻る
//--------------------------------------------------------------------------
void CPlayerAIControl::ForciblyReturn()
{
	// プレイヤー情報取得
	MyLib::Vector3 posMy = m_pAI->GetPosition();
	CGameManager::ETeamSide typeTeam = m_pAI->GetTeam();

	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 歩く
	m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

	// 近づく
	if (Approatch({ RETURN_POS, posMy.y, posMy.z }, OK_LENGTH))
	{
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
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
		m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eActionFlag = EActionFlag::ACTION_NONE;
		return;
	}

	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall)
	{// ボールがない場合
		m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eActionFlag = EActionFlag::ACTION_NONE;
		return;
	}

	CPlayer* pPlayer = pBall->GetPlayer();
	if (pPlayer)
	{// 誰かがボールを持った場合
		m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
		m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
		m_eActionFlag = EActionFlag::ACTION_NONE;
		return;
	}

	m_eActionFlag = EActionFlag::ACTION_JUMP;
	m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;
}

//==========================================================================
// モード：待機
//==========================================================================
void CPlayerAIControl::ModeIdle(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 行動：止まる
	m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
}

//==========================================================================
// モード：投げ
//==========================================================================
void CPlayerAIControl::ModeAttack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
	UpdateThrowType();
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
	//m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

	//// 投げるまでの行動の更新
	//(this->*(m_ThrowMoveFunc[m_eMoveFlag]))(pTarget, fDeltaTime, fDeltaRate, fSlowRate);

	m_eThrowFlag = EThrowFlag::THROW_NORMAL;
}

//--------------------------------------------------------------------------
// ジャンプ投げ
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTypeJump()
{
	// その場なのか歩くのか走るのか
	//m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

	CPlayer* pTartget = GetThrowTarget();

	// 走り投げ
	AttackDash(pTartget);
}

//--------------------------------------------------------------------------
// スペシャル投げ
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowTypeSpecial()
{
	m_eThrowFlag = EThrowFlag::THROW_SPECIAL;
}


//==========================================================================
// 守りの管理
//==========================================================================
void CPlayerAIControl::ModeDefense(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// キャッチの更新
	UpdateDefense(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// 守りの更新処理
//================================================================================
void CPlayerAIControl::UpdateDefense(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// ボールねぇぞ
	CBall::EState stateBall = pBall->GetState();	// ボール状態取得
	
	// 持ち主情報取得
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{// ボール所持者がいる

		// 後退(安全地帯へ)
		m_eAction = EAction::RNDOM;
	}
	else
	{
		if (IsPassTarget() && stateBall == CBall::EState::STATE_PASS)
		{// パスが自分に来る&&ボールがパス状態の合

			// 何もしない
			m_eAction = EAction::IDLE;
		}

		if (pBall->GetTarget() == m_pAI)
		{// ターゲットが自分

			float distanceBall = GetDistanceBall();		// ボールとの距離

			//if (distanceBall < 50.0f) {
			//	// 回避
			//	m_eAction = EAction::IDLE;
			//}
			//else if (distanceBall < 300.0f) {
			//	// 後退(安全地帯へ)
			//	m_eAction = EAction::RETREAT;
			//}
		}
		else {

			m_eAction = EAction::SUPPORT;	// サポート状態
		}

		if (IsLineOverBall())
		{// ボールが線を超えている場合

			// 何もしない
			//m_eAction = EAction::IDLE;

			// 後退(安全地帯へ)
			m_eAction = EAction::RETREAT;
		}
		else if (!IsLineOverBall() &&						// 線を超えていない
			stateBall == CBall::EState::STATE_FREE ||		// フリー
			stateBall == CBall::EState::STATE_PASS ||		// パス
			stateBall == CBall::EState::STATE_HOM_PASS)		// ホーミングパス
		{
			// ボールを追う
			m_eAction = EAction::CHASE_BALL;
		}
	}

	// アクション種類更新
	(this->*(m_ActionFunc[m_eAction]))();
}

//================================================================================
// カバー
//================================================================================
void CPlayerAIControl::MoveCover(CPlayer* pPlayer)
{
	if (!pPlayer) return;

	CPlayer* pPlayerBall = GetBallOwner();
	if (!pPlayerBall) return;

	MyLib::Vector3 pos = pPlayer->GetPosition();

}

//==========================================================================
// 行動関連の処理
//==========================================================================

//--------------------------------------------------------------------------
// 何もしない
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveIdle()
{
	// 行動フラグ：なし
	m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
}

//--------------------------------------------------------------------------
// 回避
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveDodge()
{
	// ボールとの距離
	float distanceBall = GetDistanceBall();

	// 距離が離れている場合
	if (distanceBall > 5.0f) return;

	// 行動：ブリンク
	m_eMoveFlag = EMoveFlag::MOVEFLAG_BLINK;
}

//--------------------------------------------------------------------------
// サポート
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveSupport()
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// ボールねぇぞ
	CBall::EState stateBall = pBall->GetState();	// ボール状態取得

	// リバウンドを取る
	if (stateBall == CBall::EState::STATE_REBOUND)
	{// ボール状態がリバウンドの場合

		if (IsLineOverBall()) return;	// ボールが線を超えている

		// 位置の取得
		MyLib::Vector3 posBall = pBall->GetPosition();		// ボール
		MyLib::Vector3 posEnd = pBall->GetPosPassEnd();		// ボールのパス終了位置
		MyLib::Vector3 posMy = m_pAI->GetPosition();		// 自分の位置

		// 終了位置のx,zを参照した位置の設定
		MyLib::Vector3 pos = { posEnd.x, posMy.y, posEnd.z };

		// ボールとの距離
		float distance = posMy.DistanceXZ(posBall);

		// 行動状態：走る
		m_eMoveFlag = EMoveFlag::MOVEFLAG_DASH;

		// ボールの方へ行く
		if (Approatch(pos, CATCH_JUMP_LENGTH) || distance < CATCH_JUMP_LENGTH)
		{// 終了位置に近づけた||ボールとの距離が範囲内の場合

			// 行動：何もしない
			m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
		}

		return;
	}

	// 体力の少ない味方をカバー
	int nLife = m_pAI->GetLife();

	if (!m_sMove.pDefenseTarget)
	{
		int nMinLife = 100000000;

		// 自分情報
		MyLib::Vector3 posMy = m_pAI->GetPosition();		// 位置情報の取得
		CGameManager::ETeamSide TeamMy = m_pAI->GetTeam();	// 所属チーム

		CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
		std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
		while (list.ListLoop(itr))
		{ // リスト内の要素数分繰り返す

			CPlayer* pPlayer = (*itr);	// プレイヤー情報

			// 内野以外&&同じチームじゃない
			if (pPlayer->GetAreaType() != CPlayer::EFieldArea::FIELD_IN &&
				pPlayer->GetTeam() != m_pAI->GetTeam()) continue;

			// 自分
			if (pPlayer == m_pAI) continue;

			// 他人の体力
			int nLifeOther = pPlayer->GetLife();

			if (nLifeOther < nLife && nLifeOther < nMinLife)
			{// 自分の体力より少ない&&チームの中で一番体力が少ない場合

				// 最小体力の更新
				nMinLife = nLifeOther;

				// 守備対象の設定
				m_sMove.pDefenseTarget = pPlayer;
			}
		}

		return;
	}


	// 味方との距離を取る(標的を分散)

}

//--------------------------------------------------------------------------
// ボールを追いかける
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveChaseBall()
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// ボールねぇぞ

	// ボール状態取得
	CBall::EState stateBall = pBall->GetState();

	if (stateBall == CBall::EState::STATE_PASS ||
		stateBall == CBall::EState::STATE_HOM_PASS ||
		stateBall == CBall::EState::STATE_MOVE)
	{// パス||ホーミングパス||移動状態の場合

		// ボールを奪う
		BallSteal();
	}
	else if (!IsLineOverBall() && !IsWhoPicksUpTheBall())
	{
		// キャッチ：取りに行く
		BallChase();
	}
}

#if 0	// オーバーライドする可能性あり
//--------------------------------------------------------------------------
// 後退（安全地帯へ）
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveRetreat()
{
	// ボール持ち主の取得
	CPlayer* pPlayer = GetBallOwner();
	if (!pPlayer) return;

	// 自分の位置
	MyLib::Vector3 posMy = m_pAI->GetPosition();

	// 安全地帯
	float posSafeX = m_fDistance + 300.0f;

	if (posMy.x > posSafeX) {// 移動タイプ：無
		m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
		m_eAction = EAction::IDLE;
		return;
	}

	// 行動フラグ：歩く
	m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

	// 右移動
	MoveRight();
}
#endif

//--------------------------------------------------------------------------
// ランダム移動
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveRandom()
{
	// x950 z560
	MyLib::Vector3 posSafeMax = { 950.0f, 0.0f, 560.0f };
	MyLib::Vector3 posSafeMin = { m_fDistance, 0.0f, -560.0f };

	if (!m_sMove.bSet) {
		// 位置の設定
		// x座標
		float fRand = (float)UtilFunc::Transformation::Random(posSafeMin.x, posSafeMax.x);
		m_sMove.pos.x = fRand;
		// z座標
		fRand = (float)UtilFunc::Transformation::Random(posSafeMin.z, posSafeMax.z);
		m_sMove.pos.z = fRand;

		m_sMove.bSet = true;
	}

	// 行動：歩き
	m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

	// 近づく
	if (Approatch(m_sMove.pos, 10.0f)) {
		m_sMove.bSet = false;
	}
}


//--------------------------------------------------------------------------
// 上移動
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveUp()
{
	// ボール持ち主の取得
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{// 持ち主がいた

		// AIコントロール情報の取得
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		float rotDest = D3DX_PI;

		// カニ進行方向の設定
		pControlAIMove->SetClabDirection(rotDest);
	}
	else
	{
		// カニ進行方向の設定
		float rotDest = D3DX_PI;
		m_pAI->SetRotDest(rotDest);
	}
}

//--------------------------------------------------------------------------
// 下移動
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveDown()
{
	// ボール持ち主の取得
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{
		// AIコントロール情報の取得
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// 進行方向を決める
		float rotDest = 0.0f;

		// カニ進行方向の設定
		pControlAIMove->SetClabDirection(rotDest);
	}
	else
	{
		// カニ進行方向の設定
		float rotDest = 0.0f;
		m_pAI->SetRotDest(rotDest);
	}
}

//--------------------------------------------------------------------------
// 左移動
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveLeft()
{
	// ボール持ち主の取得
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{
		// AIコントロール情報の取得
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// 進行方向を決める
		float rotDest = D3DX_PI * 0.5f;

		// カニ進行方向の設定
		pControlAIMove->SetClabDirection(rotDest);
	}
	else
	{
		// カニ進行方向の設定
		float rotDest = D3DX_PI * 0.5f;
		m_pAI->SetRotDest(rotDest);
	}
}

//--------------------------------------------------------------------------
// 右移動
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveRight()
{
	// ボール持ち主の取得
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{
		// AIコントロール情報の取得
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// 進行方向を決める
		float rotDest = -D3DX_PI * 0.5f;

		// カニ進行方向の設定
		pControlAIMove->SetClabDirection(rotDest);
	}
	else
	{
		// カニ進行方向の設定
		float rotDest = -D3DX_PI * 0.5f;
		m_pAI->SetRotDest(rotDest);
	}
}




//==========================================================================
// ボール持ち主との距離を計算
//==========================================================================
float CPlayerAIControl::GetDistanceBallowner()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return 0.0f;

	CPlayer* pPlayer = pBall->GetPlayer();
	if (!pPlayer) return 0.0f;
	if (pPlayer->GetTeam() == m_pAI->GetTeam()) return 0.0f;

	// 距離を計算
	float distance = m_pAI->GetPosition().DistanceXZ(pPlayer->GetPosition());

	// 計算結果を返す
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
// ボール持ち主の取得（敵の場合）
//==========================================================================
CPlayer* CPlayerAIControl::GetBallOwner()
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

//==========================================================================
// ボールとの距離
//==========================================================================
float CPlayerAIControl::GetDistanceBall()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) { return 0.0f; }

	float distance = m_pAI->GetPosition().DistanceXZ(pBall->GetPosition());

	return distance;
}

//==========================================================================
// 敵との距離
//==========================================================================
float CPlayerAIControl::GetDistanceEnemy()
{
	float fMin = 10000000.0f;	// 最小値の保存用

	// 自分情報
	MyLib::Vector3 posMy = m_pAI->GetPosition();		// 位置情報の取得
	CGameManager::ETeamSide TeamMy = m_pAI->GetTeam();	// 所属チーム

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報

		// 自分の&&同じチーム場合
		if (pPlayer == m_pAI && TeamMy == pPlayer->GetTeam()) continue;

		// エリアタイプ
		CPlayer::EFieldArea areaPlayer = pPlayer->GetAreaType();

		// 外野の場合
		if (areaPlayer == CPlayer::EFieldArea::FIELD_OUT) continue;

		// プレイヤー位置
		MyLib::Vector3 pos = pPlayer->GetPosition();

		// 敵との距離を求める
		float fLength = posMy.DistanceXZ(pos);

		if (fLength < fMin)
		{// より近い場合

			fMin = fLength;
		}
	}

	return fMin;
}

//==========================================================================
// ボールを奪う
//==========================================================================
void CPlayerAIControl::BallSteal()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	// 同じチームの場合
	if (pBall->GetTypeTeam() == m_pAI->GetTeam()) return;

	// 位置の取得
	MyLib::Vector3 posBall = pBall->GetPosition();		// ボール
	MyLib::Vector3 posEnd = pBall->GetPosPassEnd();		// ボールのパス終了位置
	MyLib::Vector3 posMy = m_pAI->GetPosition();		// 自分の位置

	// 終了位置のx,zを参照した位置の設定
	MyLib::Vector3 pos = { posEnd.x, posMy.y, posEnd.z };

	// パス相手の取得
	CPlayer* pTarget = pBall->GetTarget();
	if (pTarget)
	{
		// ターゲットとボールの位置
		float distanth0 = pTarget->GetPosition().DistanceXZ(posBall);

		if (distanth0 < STEAL_CANCEL_LENGTH)
		{// ボールとパス先の距離が範囲内ならあきらめる
			m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
			return;
		}
	}

	// ボールとの距離
	float distance = posMy.DistanceXZ(posBall);

	// 行動状態：走る
	m_eMoveFlag = EMoveFlag::MOVEFLAG_DASH;

	// ボールの方へ行く
	if (Approatch(pos, CATCH_JUMP_LENGTH) || distance < CATCH_JUMP_LENGTH)
	{// 終了位置に近づけた||ボールとの距離が範囲内の場合

		if (posBall.y < CATCH_JUMP_HEIGHT)
		{// 取れそうな高さに来た！
			m_eActionFlag = EActionFlag::ACTION_JUMP;
		}
	}
}

//==========================================================================
// ボールを追う
//==========================================================================
void CPlayerAIControl::BallChase()
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
		m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
		return;
	}

	// ボール位置
	MyLib::Vector3 posBall = pBall->GetPosition();

	// 自分からボールの距離
	float distance = m_pAI->GetPosition().DistanceXZ(posBall);

	if (distance > BALL_DISTANCE) {
		// 行動：走る
		m_eMoveFlag = EMoveFlag::MOVEFLAG_DASH;
	}
	else {
		// 行動：歩く
		m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;
	}

	// 近づく
	if (Approatch(pBall->GetPosition(), 50.0f))
	{// 近づけた場合
		// 行動：止まる
		m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
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

	// 自分の位置情報
	MyLib::Vector3 posMy = m_pAI->GetPosition();

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
		if (pPlayerBall->GetTeam() != m_pAI->GetTeam() &&
			pPlayerBall != m_pAI) {// 違うチームが持っている && 持ち主が自分以外場合

			if (m_eMoveFlag == EMoveFlag::MOVEFLAG_DASH)
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
	MyLib::Vector3 posMy = m_pAI->GetPosition();

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
		if (pPlayerBall->GetTeam() != m_pAI->GetTeam() &&
			pPlayerBall != m_pAI) {// 違うチームが持っている && 持ち主が自分以外場合

			if (m_eMoveFlag == EMoveFlag::MOVEFLAG_DASH)
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
// 心の初期化
//==========================================================================
void CPlayerAIControl::InitHeart()
{
	// ランダム
	int fRand = UtilFunc::Transformation::Random(EHeartMain::HEART_MAIN_NORMAL, EHeartMain::HEART_MAIN_MAX - 1);

	// 心の割り当て
	m_sParameter.eHeartMain = (EHeartMain)fRand;
	m_sParameter.eHeartMain = EHeartMain::HEART_MAIN_STRONG;

	// 心ごとにパラメータの割り当て
	switch (m_sParameter.eHeartMain)
	{
	case EHeartMain::HEART_MAIN_NORMAL:	// 通常
		m_fDistance = LENGTH_TARGET;
		break;

	case EHeartMain::HEART_MAIN_STRONG:	// 強気
		m_fDistance = LENGTH_TARGET - 100.0f;
		break;

	case EHeartMain::HEART_MAIN_TIMID:	// 弱気
		m_fDistance = LENGTH_TARGET + 200.0f;
		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// パラメーター更新
//==========================================================================
void CPlayerAIControl::UpdateParameter()
{
	// 行動

	// 投げ

	// スペシャル


	//// 体力の取得
	int nLifeMax = m_pAI->GetLifeOrigin();
	int nLife = m_pAI->GetLife();

	//// 割合を求める
	//float rate = 0.0f;
	//rate = (float)nLife / (float)nLifeMax;


	// 値の設定
	float value = 0.0f;

	// ターゲットとの距離
	switch (m_sParameter.eHeartMain)
	{
	case EHeartMain::HEART_MAIN_NORMAL:
		m_fDistance = m_fDistance + (value);
		break;

	case EHeartMain::HEART_MAIN_STRONG:
		m_fDistance = m_fDistance + (value );
		break;

	case EHeartMain::HEART_MAIN_TIMID:
		m_fDistance = m_fDistance + (value );
		break;

	default:
		assert(false);
		break;
	}
}

//==========================================================================
// 行動時間の設定
//==========================================================================
void CPlayerAIControl::SetMoveTimer(int nMin, int nMax)
{
	// 角度の設定
	if (!m_sMove.bSet)
	{
		// 行動時間の設定
		float fRand = (float)UtilFunc::Transformation::Random(nMin, nMax);
		m_sMove.fTimer = fRand * 0.1f;

		// 時間設定ON
		m_sMove.bSet = true;
	}
}

//==========================================================================
// 行動時間の更新
//==========================================================================
void CPlayerAIControl::UpdateMoveTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sMove.bCancel)
	{


		// 行動の構造体初期化
		ZeroMemory(&m_sMove, sizeof(m_sMove));
	}

	if (m_sMove.bSet)
	{
		// タイマーのカウントダウン
		m_sMove.fTimer -= fDeltaTime * fDeltaRate * fSlowRate;

		if (m_sMove.fTimer <= 0.0f)
		{
			// 行動の構造体初期化
			ZeroMemory(&m_sMove, sizeof(m_sMove));

			// アクション：なし
			m_eAction = EAction::IDLE;

			// 行動：止まる
			m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
		}
	}
}


// フラグで管理 ===================================================================================================

//================================================================================
// 行動フラグの更新処理
//================================================================================
void CPlayerAIControl::UpdateMoveFlag()
{
	// 行動フラグ更新
	(this->*(m_MoveFlagFunc[m_eMoveFlag]))();
}

//--------------------------------------------------------------------------
// 止まる
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveFlagStop()
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
// 歩く
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveFlagWalk()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 歩きON
	pControlAIMove->SetIsWalk(true);

	// フラグOFF
	pControlAIMove->SetIsDash(false);
	pControlAIMove->SetIsBlink(false);
}

//--------------------------------------------------------------------------
// ブリンク
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveFlagBlink()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ブリンクON
	pControlAIMove->SetIsBlink(true);

	// フラグOFF
	pControlAIMove->SetIsWalk(false);
	pControlAIMove->SetIsDash(false);
}

//--------------------------------------------------------------------------
// 走る
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveFlagDash()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 歩きON
	pControlAIMove->SetIsWalk(true);
	// ブリンクON
	pControlAIMove->SetIsBlink(true);
	// 走るON
	pControlAIMove->SetIsDash(true);
}

//================================================================================
// アクションフラグの更新処理
//================================================================================
void CPlayerAIControl::UpdateActionFlag()
{
	// アクション更新
	(this->*(m_ActionFlagFunc[m_eActionFlag]))();
}

//--------------------------------------------------------------------------
// なにもしない
//--------------------------------------------------------------------------
void CPlayerAIControl::ActionFlagNone()
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// ジャンプ：true
	pControlAIAction->SetIsJump(false);
	pControlAIAction->SetIsJumpFloat(false);

	// アクション列挙：なし
	m_eActionFlag = EActionFlag::ACTION_NONE;
}

//--------------------------------------------------------------------------
// ジャンプ
//--------------------------------------------------------------------------
void CPlayerAIControl::ActionFlagJump()
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
	m_eActionFlag = EActionFlag::ACTION_NONE;
}

//================================================================================
// 投げフラグの更新処理
//================================================================================
void CPlayerAIControl::UpdateThrowFlag()
{
	// 投げ更新
	(this->*(m_ThrowFlagFunc[m_eThrowFlag]))();
}

//--------------------------------------------------------------------------
// 投げる
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowFlag()
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
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
void CPlayerAIControl::ThrowFlagPass()
{

}

//--------------------------------------------------------------------------
// スペシャル
//--------------------------------------------------------------------------
void CPlayerAIControl::ThrowFlagSpecial()
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// スペシャル投げ
	pControlAIAction->SetIsSpecial(true);

	// 投げなし
	m_eThrowFlag = EThrowFlag::THROW_NONE;
	m_eThrowType = EThrowType::THROWTYPE_NONE;
}



// 判定 ==========================================================================

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

//==========================================================================
// ボールが取れる範囲にいますか？
//==========================================================================
bool CPlayerAIControl::IsDistanceBall()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return false;

	// 位置の取得
	MyLib::Vector3 posBall = pBall->GetPosition();		// ボール
	MyLib::Vector3 posMy = m_pAI->GetPosition();		// 自分の位置

	// ボールとの距離
	float distance = posMy.DistanceXZ(posBall);

	if (distance < STEAL_CANCEL_LENGTH)
	{// ボールとパス先の距離が範囲内ならあきらめる
		return true;
	}

	return false;
}
