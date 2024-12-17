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
	&CPlayerAIControl::ModeNone,				// なし
	&CPlayerAIControl::ModeIdle,				// 待機
	&CPlayerAIControl::ModeThrow,				// 投げ
	&CPlayerAIControl::ModeCatch,				// キャッチ
};

CPlayerAIControl::CATCH_FUNC CPlayerAIControl::m_CatchFunc[] =	// キャッチ関数
{
	&CPlayerAIControl::CatchNone,				// なし
	&CPlayerAIControl::CatchNormal,				// 通常
	&CPlayerAIControl::CatchPassGive,			// パス貰う
	&CPlayerAIControl::CatchPassSteal,			// パス奪う
	&CPlayerAIControl::CatchFindBall,			// 見つける
};

CPlayerAIControl::MOVETYPE_FUNC CPlayerAIControl::m_MoveTypeFunc[] =	// 行動タイプ関数
{
	&CPlayerAIControl::MoveTypeNone,			// なし
	&CPlayerAIControl::MoveTypeDistance,		// 距離
	& CPlayerAIControl::MoveTypeLeftRight,		// 距離
	&CPlayerAIControl::MoveTypeAtyakotya,		// あちゃこっちゃ
	&CPlayerAIControl::MoveTypeAvoid,			// 回避
	&CPlayerAIControl::MoveTypeCrazy,			// 狂う
};


// フラグ関連

CPlayerAIControl::MOVEFLAG_FUNC CPlayerAIControl::m_MoveFlagFunc[] =	// 行動フラグ関数
{
	&CPlayerAIControl::MoveFlagStop,			// 止まる
	&CPlayerAIControl::MoveFlagWalk,			// 歩く
	&CPlayerAIControl::MoveFlagBlink,			// 歩く
	&CPlayerAIControl::MoveFlagDash,			// 走る
};

CPlayerAIControl::THROW_FUNC CPlayerAIControl::m_ThrowFlagFunc[] =	// 投げフラグ関数
{
	&CPlayerAIControl::ThrowFlagNone,			// なし
	&CPlayerAIControl::ThrowFlag,				// 投げ
	&CPlayerAIControl::ThrowFlagPass,			// パス
	&CPlayerAIControl::ThrowFlagSpecial,		// スペシャル
};

CPlayerAIControl::ACTION_FUNC CPlayerAIControl::m_ActionFlagFunc[] =	// アクションフラグ関数
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
	m_eMode = EMode::MODE_NONE;
	//m_eForcibly = EMoveForcibly::FORCIBLY_NONE;
	m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
	m_eMoveType = EMoveTypeChatch::MOVETYPE_NONE;
	m_eActionFlag = EActionFlag::ACTION_NONE;
	m_eThrow = EThrowFlag::THROW_NONE;
	m_eCatchType = ECatchType::CATCH_TYPE_NONE;

	// 構造体の初期化
	ZeroMemory(&m_sThrow, sizeof(m_sThrow));
	ZeroMemory(&m_sMove, sizeof(m_sMove));
	ZeroMemory(&m_sMoveCount, sizeof(m_sMoveCount));
	ZeroMemory(&m_sDistance, sizeof(m_sDistance));
	ZeroMemory(&m_sParameter, sizeof(m_sParameter));
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
	m_eMoveType = EMoveTypeChatch::MOVETYPE_DISTANCE;
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
	//UpdateForcibly();
	
	{// フラグの更新

		// 更新：行動
		UpdateMoveFlag(fDeltaTime, fDeltaRate, fSlowRate);
		// 更新：アクション
		UpdateActionFlag();
		// 更新：投げ
		UpdateThrowFlag();
	}

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
		m_eMode = EMode::MODE_NONE;
		return;
	}

	if (pBall->GetPlayer() == m_pAI)
	{// ボールを持っているのが自分だった場合
		m_eMode = EMode::MODE_THROW;	// 投げ
	}
	else if (pBall->GetPlayer() == nullptr && pBall->GetTypeTeam() != m_pAI->GetTeam())
	{// ボールが取得されていない場合||自分とボールを持っているチームが違う場合
		m_eMode = EMode::MODE_CATCH;	// キャッチ
	}
	else if (pBall->GetPlayer() == nullptr && pBall->GetTypeTeam() == m_pAI->GetTeam())
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
	//if (IsLineOverPlayer() && m_eForcibly != EMoveForcibly::FORCIBLY_START)
	//{// 線を超えていた&&強制行動：初め以外の場合
	//	m_eForcibly = EMoveForcibly::FORCIBLY_RETURN;
	//	return;
	//}

	// モード更新
	(this->*(m_ModeFunc[m_eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// モード：投げ
//==========================================================================
void CPlayerAIControl::ModeThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}


//==========================================================================
// モード：待機
//==========================================================================
void CPlayerAIControl::ModeIdle(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 行動：止まる
	m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;

	// 行動タイプの更新
	UpdateMoveType(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// 行動タイプ
//================================================================================
void CPlayerAIControl::UpdateMoveType(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_eMoveType == EMoveTypeChatch::MOVETYPE_NONE) return;

	// 行動タイマーの設定
	//SetMoveTimer(fDeltaTime, fDeltaRate, fSlowRate);
	
	// 行動タイプ更新
	//(this->*(m_MoveTypeFunc[m_eMoveType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//--------------------------------------------------------------------------
// 行動：距離を取る
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveTypeDistance(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayer* pTarget = GetBallOwner();
	if (!pTarget) return;

	MyLib::Vector3 posMy = m_pAI->GetPosition();
	MyLib::Vector3 posTarget = pTarget->GetPosition();

	// 自分とボール持ち主(相手)の距離
	float distance = posMy.DistanceXZ(posTarget);

	if (distance < m_sDistance.fTarget)
	{// 離れる

		// 行動フラグ：歩き
		m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

		m_sMove.bReturn = false;

		if (Leave(posTarget, m_sDistance.fTarget))
		{
			m_sMove.bReturn = true;
			m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
		}
	}
}

//==========================================================================
// 行動：左右
//==========================================================================
void CPlayerAIControl::MoveTypeLeftRight(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	MyLib::Vector3 posMy = m_pAI->GetPosition();


	MyLib::Vector3 pos = { posMy.x, 0.0f, 560.0f };
	float distance = posMy.DistanceXZ(pos);

	if (distance < 100.0f)
	{
		m_sMove.bReturn = true;
	}

	MyLib::Vector3 pos1 = { posMy.x, 0.0f, -560.0f };
	float distance1 = posMy.DistanceXZ(pos1);

	if (distance1 < 100.0f)
	{
		m_sMove.bReturn = false;
	}

	if (m_sMove.bReturn)
	{
		MoveRight(GetBallOwner());
	}
	else
	{
		MoveLeft(GetBallOwner());
	}

	m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;
}

//==========================================================================
// 距離：取得
//==========================================================================
float CPlayerAIControl::GetDistance(CPlayer::EFieldArea area, CGameManager::ETeamSide teamMy, CPlayer* pPlayer)
{
	float distance = 0.0f;

	CGameManager::ETeamSide teamEnemy = pPlayer->GetTeam();


	if (area == CPlayer::EFieldArea::FIELD_IN && teamMy == teamEnemy)
	{// 味方
		distance = m_sDistance.fInFriend;
		return distance;
	}
	else if (area == CPlayer::EFieldArea::FIELD_IN && teamMy != teamEnemy)
	{// 敵
		distance = m_sDistance.fTarget;
		return distance;
	}
	else if (area == CPlayer::EFieldArea::FIELD_OUT)
	{// 外野
		if (pPlayer == GetBallOwner())
		{
			distance = m_sDistance.fTarget;
			return distance;
		}

		distance = m_sDistance.fOut;
	}

#if 0
	// テキスト表示
	ImGui::Text("distance:%f", distance);
	ImGui::DragFloat("rateFriend", &m_sDistance.fInFriend, 1.0f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("rateEnemy", &m_sDistance.fInEnemy, 1.0f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("rateOut", &m_sDistance.fOut, 1.0f, 0.0f, 0.0f, "%.2f");
#endif
	return distance;
}

//--------------------------------------------------------------------------
// 行動：あっちゃこっちゃ
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveTypeAtyakotya(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// タイマーセット
	SetMoveTimer(fDeltaTime, fDeltaRate, fSlowRate);

	if (m_sMove.bSetMove) return;

	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 行動時間の設定
	float fRand = (float)UtilFunc::Transformation::Random(MOVETYPE_ATYAKOTYA_TIME_MIN, MOVETYPE_ATYAKOTYA_TIME_MAX);
	m_sMove.fTimer = fRand * 0.1f;

	// 進行方向を決める
	int randRot = UtilFunc::Transformation::Random(-316, 316);
	float randRotDest = randRot * 0.01f;

	// カニ進行方向の設定
	pControlAIMove->SetClabDirection(randRotDest);

	// 行動：歩く
	m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

	// 設定しましたー！
	m_sMove.bSetMove = true;
}

//--------------------------------------------------------------------------
// 行動：回避
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveTypeAvoid(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 距離を取る
	//MoveDistance();
}

//==========================================================================
// キャッチの管理
//==========================================================================
void CPlayerAIControl::ModeCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// キャッチの更新
	UpdateCatch(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// キャッチの更新処理
//================================================================================
void CPlayerAIControl::UpdateCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// ボールねぇぞ

	// 持ち主情報取得
	CPlayer* pPlayer = pBall->GetPlayer();

	if (pPlayer != nullptr)
	{// 誰かがボールを持っている

		if (IsPassTarget())
		{// パスが自分に来る場合
			// キャッチ：パス待ち
			m_eCatchType = ECatchType::CATCH_TYPE_PASS_GIVE;
		}
		else
		{
			// キャッチ：通常
			m_eCatchType = ECatchType::CATCH_TYPE_NORMAL;
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

			// キャッチ：奪う
			m_eCatchType = ECatchType::CATCH_TYPE_PASS_STEAL;
		}
		else if (IsWhoPicksUpTheBall() && IsLineOverBall())
		{// 自分よりもボールに近い人がいる&&ボールが相手側にある	つまり相手側にボールが落ちている

			// キャッチ：通常
			m_eCatchType = ECatchType::CATCH_TYPE_NORMAL;
		}
		else if (pBall->GetTarget() == m_pAI &&
				stateBall == CBall::EState::STATE_HOM_NOR ||
				stateBall == CBall::EState::STATE_HOM_JUMP)
		{// ターゲットが自分&&ボールが投げ状態
			m_eCatchType = ECatchType::CATCH_TYPE_NORMAL;
		}
		else if (!IsLineOverBall() && !IsWhoPicksUpTheBall())
		{
			// キャッチ：取りに行く
			m_eCatchType = ECatchType::CATCH_TYPE_FIND;
		}
	}

	// キャッチ種類更新
	(this->*(m_CatchFunc[m_eCatchType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//--------------------------------------------------------------------------
// 通常キャッチ
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// パスターゲットが自分の場合
	//if (IsPassTarget()) return;

	// ボールを持つ相手を取得
	CPlayer* pTarget = GetBallOwner();
	if (!pTarget) { return; }

	// ボール持ち主との距離
	float distance = GetDistanceBallowner();

	if (distance < m_sDistance.fTarget)
	{// 持ち主と距離が近い場合
		m_eMoveType = EMoveTypeChatch::MOVETYPE_DISTANCE;
	}
	else
	{
		m_eMoveType = EMoveTypeChatch::MOVETYPE_RANDOM;
	}

	// 行動タイプの更新
	UpdateMoveType(fDeltaTime, fDeltaRate, fSlowRate);
}

//--------------------------------------------------------------------------
// キャッチ：パスを貰う
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchPassGive(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	if (m_pAI->GetBall()) return;

	// 自分からボールの位置を見る
	float rot = m_pAI->GetPosition().AngleXZ(pBall->GetPosition());

	// 設定
	m_pAI->SetRotDest(rot);

	// 行動種類：止まる
	m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
}

//--------------------------------------------------------------------------
// キャッチ：パスを奪う
//--------------------------------------------------------------------------
void CPlayerAIControl::CatchPassSteal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
			m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
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
		m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
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
	else{
		// 行動：歩く
		m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;
	}

	// 近づく
	if (Approatch(pBall->GetPosition(), 50.0f))
	{// 近づけた場合
		// 行動：止まる
		m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
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
// 距離を取る
//==========================================================================
void CPlayerAIControl::MoveDistance()
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	float fMin = 10000000.0f;

	// 自分情報
	MyLib::Vector3 posMy = m_pAI->GetPosition();		// 位置情報の取得
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
		float length = GetDistance(areaPlayer, TeamMy, pPlayer);

		if (fLength < length)
		{// 指定した距離より近い場合

			// 行動：歩く
			m_eMoveFlag = EMoveFlag::MOVEFLAG_WALK;

			// 離れる
			if (Leave(posPlayer, length))
			{
				m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
				m_eMoveType = EMoveTypeChatch::MOVETYPE_NONE;
			}

			return;
		}
		else {
			// 行動：止まる
			m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
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

		if (!pPlayerBall)
		{// プレイヤーがいない
			// 角度設定
			m_pAI->SetRotDest(direction);
			return false;

		}
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
		m_sDistance.fInFriend	= LENGTH_FRIEND;
		m_sDistance.fOut		= LENGTH_OUT;
		m_sDistance.fTarget		= LENGTH_TARGET;
		break;

	case EHeartMain::HEART_MAIN_STRONG:	// 強気
		m_sDistance.fInFriend	= LENGTH_FRIEND;
		m_sDistance.fOut		= LENGTH_OUT - 50.0f;
		m_sDistance.fTarget		= LENGTH_TARGET - 100.0f;
		break;

	case EHeartMain::HEART_MAIN_TIMID:	// 弱気
		m_sDistance.fInFriend	= LENGTH_FRIEND;
		m_sDistance.fOut		= LENGTH_OUT + 100.0f;
		m_sDistance.fTarget		= LENGTH_TARGET + 150.0f;
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
		m_sDistance.fTarget = m_sDistance.fTarget + (value);
		break;

	case EHeartMain::HEART_MAIN_STRONG:
		m_sDistance.fTarget = m_sDistance.fTarget + (value );
		break;

	case EHeartMain::HEART_MAIN_TIMID:
		m_sDistance.fTarget = m_sDistance.fTarget + (value );
		break;

	default:
		assert(false);
		break;

	}
}



//==========================================================================
// 行動時間の設定
//==========================================================================
void CPlayerAIControl::SetMoveTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 角度の設定
	if (m_sMoveCount.bSet)
	{
		// タイマーのカウントダウン
		m_sMoveCount.fTimer -= fDeltaTime * fDeltaRate * fSlowRate;

		if (m_sMoveCount.fTimer <= 0.0f)
		{
			// 行動の構造体初期化
			ZeroMemory(&m_sMoveCount, sizeof(m_sMoveCount));

			// 行動タイプ：なし
			m_eMoveType = EMoveTypeChatch::MOVETYPE_NONE;

			// 行動：止まる
			m_eMoveFlag = EMoveFlag::MOVEFLAG_STOP;
		}

		return;
	}
	else {// 設定が完了していない
		// 行動時間の設定
		float fRand = (float)UtilFunc::Transformation::Random(MOVETYPE_LEFTRIGHT_TIME_MIN, MOVETYPE_LEFTRIGHT_TIME_MAX);
		m_sMoveCount.fTimer = fRand * 0.1f;

		// 時間設定ON
		m_sMoveCount.bSet = true;
	}
}

//--------------------------------------------------------------------------
// 行動：左
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveLeft(CPlayer* pTarget)
{
	if (pTarget)
	{
		// AIコントロール情報の取得
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// 進行方向を決める
		float rotDest = m_pAI->GetPosition().AngleXZ(pTarget->GetPosition());
		// 角度の設定
		rotDest = rotDest + (-D3DX_PI * 0.5f);
		// 角度ノーマライズ
		UtilFunc::Transformation::RotNormalize(rotDest);

		// カニ進行方向の設定
		pControlAIMove->SetClabDirection(rotDest);

		// 行動設定ON
		m_sMove.bSetMove = true;
	}
	else
	{
		float rot = D3DX_PI * 0.5f;
		m_pAI->SetRotDest(rot);

		// 行動設定ON
		m_sMove.bSetMove = true;
	}
}

//--------------------------------------------------------------------------
// 行動：右
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveRight(CPlayer* pTarget)
{
	if (pTarget)
	{
		// AIコントロール情報の取得
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// 進行方向を決める
		float rotDest = m_pAI->GetPosition().AngleXZ(pTarget->GetPosition());

		// 角度の設定
		rotDest = rotDest + (D3DX_PI * 0.5f);
		// 角度ノーマライズ
		UtilFunc::Transformation::RotNormalize(rotDest);

		// カニ進行方向の設定
		pControlAIMove->SetClabDirection(rotDest);

		// 行動設定ON
		m_sMove.bSetMove = true;
	}
	else
	{
		float rot = -D3DX_PI * 0.5f;
		m_pAI->SetRotDest(rot);

		// 行動設定ON
		m_sMove.bSetMove = true;
	}
}

//--------------------------------------------------------------------------
// 行動：左上
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveFrontAndLeft(CPlayer* pPlayer)
{
	float rot = D3DX_PI * 0.75f;
	m_pAI->SetRotDest(rot);

	// 行動設定ON
	m_sMove.bSetMove = true;
}

//--------------------------------------------------------------------------
// 行動：右上
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveFrontAndRight(CPlayer* pPlayer)
{
	float rot = -D3DX_PI * 0.75f;
	m_pAI->SetRotDest(rot);

	// 行動設定ON
	m_sMove.bSetMove = true;
}

//--------------------------------------------------------------------------
// 行動：左下
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveDownAndLeft(CPlayer* pPlayer)
{
	float rot = D3DX_PI * 0.25f;
	m_pAI->SetRotDest(rot);

	// 行動設定ON
	m_sMove.bSetMove = true;
}

//--------------------------------------------------------------------------
// 行動：右下
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveDownAndRight(CPlayer* pPlayer)
{
	float rot = -D3DX_PI * 0.25f;
	m_pAI->SetRotDest(rot);

	// 行動設定ON
	m_sMove.bSetMove = true;
}

//--------------------------------------------------------------------------
// 行動：上
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveFront(CPlayer* pTarget)
{
	if (pTarget)
	{
		// AIコントロール情報の取得
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// 進行方向を決める
		float rotDest = m_pAI->GetPosition().AngleXZ(pTarget->GetPosition());

		// カニ進行方向の設定
		pControlAIMove->SetClabDirection(rotDest);

		// 行動設定ON
		m_sMove.bSetMove = true;
	}
	else
	{
		float rot = D3DX_PI;
		m_pAI->SetRotDest(rot);

		// 行動設定ON
		m_sMove.bSetMove = true;
	}
}

//--------------------------------------------------------------------------
// 行動：下
//--------------------------------------------------------------------------
void CPlayerAIControl::MoveDown(CPlayer* pTarget)
{
	if (pTarget)
	{
		// AIコントロール情報の取得
		CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		// 進行方向を決める
		float rotDest = m_pAI->GetPosition().AngleXZ(pTarget->GetPosition());
		UtilFunc::Transformation::RotNormalize(rotDest);

		// カニ進行方向の設定
		pControlAIMove->SetClabDirection(rotDest);

		// 行動設定ON
		m_sMove.bSetMove = true;
	}
	else
	{
		float rot = 0.0f;
		m_pAI->SetRotDest(rot);

		// 行動設定ON
		m_sMove.bSetMove = true;
	}
}




// フラグで管理 ===================================================================================================

//================================================================================
// 行動の更新処理
//================================================================================
void CPlayerAIControl::UpdateMoveFlag(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 行動フラグ更新
	(this->*(m_MoveFlagFunc[m_eMoveFlag]))();
}

//--------------------------------------------------------------------------
// 行動：止まる
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
// 行動：歩く
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
// 行動：ブリンク
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
// 行動：走る
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
// アクションの更新処理
//================================================================================
void CPlayerAIControl::UpdateActionFlag()
{
	// アクション更新
	(this->*(m_ActionFlagFunc[m_eActionFlag]))();
}

//--------------------------------------------------------------------------
// アクション：無
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
// アクション：ジャンプ
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
// 投げの更新処理
//================================================================================
void CPlayerAIControl::UpdateThrowFlag()
{
	// 投げ更新
	(this->*(m_ThrowFlagFunc[m_eThrow]))();
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
	m_eThrow = EThrowFlag::THROW_NONE;
	//m_eThrowType = EThrowType::THROWTYPE_NONE;
}

//--------------------------------------------------------------------------
// パス
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
	m_eThrow = EThrowFlag::THROW_NONE;
	//m_eThrowType = EThrowType::THROWTYPE_NONE;
}
