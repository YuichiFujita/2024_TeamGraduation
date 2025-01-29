//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerAIcontrol_defense.h"
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

#include "playerAIcontrol_leftAttack.h"
#include "playerAIcontrol_leftDefense.h"
#include "playerAIcontrol_rightAttack.h"
#include "playerAIcontrol_rightDefense.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	// 距離
	const float TARGET_DISTANCE = 400.0f;		// ターゲット
	const float STEAL_HEIGHT_JUMP = 140.0f;		// 奪う高さ

	// クールダウン
	const int COOLDOWN_MIN_RANDOM = 1;			// ランダム行動
	const int COOLDOWN_MAX_RANDOM = 5;			// 

	// キャッチ精度
	const float CATCH_RADIUS_JUST = 200.0f;		// ジャストキャッチ(120～250 確定！！)
	const float CATCH_RADIUS_FAST = 300.0f;		// 速い(ジャストする時もある)
	const float CATCH_RADIUS_LATE = 100.0f;		// 遅い(ミス)

	// 終了半径
	const float END_RADIUS = 30.0f;
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerAIControlDefense::ACTION_FUNC CPlayerAIControlDefense::m_ActionFunc[] =	// キャッチ関数
{
	&CPlayerAIControlDefense::MoveIdle,					// なし
	&CPlayerAIControlDefense::MoveChaseBall,			// ボールを追いかける
	&CPlayerAIControlDefense::MoveRetreat,				// 後退
	&CPlayerAIControlDefense::MoveRandom,				// ランダム
	&CPlayerAIControlDefense::MoveLeave,				// 離れる
};

CPlayerAIControlDefense::ACTIONSTATUS_FUNC CPlayerAIControlDefense::m_ActionStatusFunc[] =	// キャッチ関数
{
	&CPlayerAIControlDefense::StatusIdle,
	&CPlayerAIControlDefense::StatusAction,
	&CPlayerAIControlDefense::StatusCooldown,
};

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControlDefense::CPlayerAIControlDefense()
{
	m_eAction = EAction::IDLE;
	m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;
	m_eCatch = ECatch::CATCH_NORMAL;

	// 構造体の初期化
	ZeroMemory(&m_sAction, sizeof(m_sAction));
}

//==========================================================================
// 生成
//==========================================================================
CPlayerAIControlDefense* CPlayerAIControlDefense::Create(CPlayer* player)
{
	// メモリの確保
	CPlayerAIControlDefense* pObj = nullptr;

	if (!player) {
		assert(("プレイヤー情報が無いよ", false));
	}
	switch (player->GetTeam())
	{// チーム別
	case CGameManager::ETeamSide::SIDE_LEFT:
		pObj = DEBUG_NEW CPlayerAIControlLeftDefense;
		break;

	case CGameManager::ETeamSide::SIDE_RIGHT:
		pObj = DEBUG_NEW CPlayerAIControlRightDefense;
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
HRESULT CPlayerAIControlDefense::Init()
{
	// 親クラスの初期化
	CPlayerAIControlMode::Init();

	// 列挙の初期化
	m_eAction = EAction::IDLE;
	m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;

	// 構造体の初期化
	ZeroMemory(&m_sAction, sizeof(m_sAction));

	 //変数の初期化
	m_fDistanse = 200.0f;

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAIControlDefense::Uninit()
{
	// 親クラスの終了処理
CPlayerAIControlMode::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIControlDefense::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;
	CPlayer* pPlayer = pBall->GetPlayer();

	if (pPlayer)
	{// 誰かがボールを持っている場合
		PlayerBall(fDeltaTime, fDeltaRate, fSlowRate);
	}
	else
	{// 誰もボールを持っていない
		NotPlayerBall(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// キャッチ精度の更新
	UpdateCatchAccuracy();

	// 守りの更新
	UpdateDefense(fDeltaTime, fDeltaRate, fSlowRate);

	// 親クラスの更新（最後尾に設置）
	CPlayerAIControlMode::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// プレイヤーがボールを持っている
//================================================================================
void CPlayerAIControlDefense::PlayerBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ボール情報の取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;
	CPlayer* pPlayer = pBall->GetPlayer();	// ボールからプレイヤー情報を取得

	// チーム
	CGameManager::ETeamSide sideBall = pPlayer->GetTeam();			// ボール持ち主のチーム取得
	CGameManager::ETeamSide sideMy = GetPlayer()->GetTeam();		// 自分のチーム取得

	switch (pPlayer->GetAreaType())
	{// エリア別

	case CPlayer::EFieldArea::FIELD_IN:	// 内野

		if (sideBall == sideMy)
		{// 同じチーム

			{// ランダム行動
				if (m_eActionStatus == EActionStatus::ACTIONSTATUS_COOLDOWN) return;	// クールダウン中は通らない
				m_eAction = EAction::RNDOM;												// ランダム状態
				m_eActionStatus = EActionStatus::ACTIONSTATUS_ACTION;					// アクション状態：アクション
			}
		}
		else
		{// 違うチーム

			if (IsTargetDistanse())
			{
				m_eAction = EAction::LEAVE;					// 離れる状態
				ZeroMemory(&m_sAction, sizeof(m_sAction));	// 行動の構造体初期化
				return;
			}
			// アクション選択
			SelectAction();
		}

		break;

	case CPlayer::EFieldArea::FIELD_OUT:	// 外野

		if (IsTargetDistanse())
		{
			m_eAction = EAction::LEAVE;						// 離れる状態
			ZeroMemory(&m_sAction, sizeof(m_sAction));		// 行動の構造体初期化
			return;
		}

		{// ランダム行動
			if (m_eActionStatus == EActionStatus::ACTIONSTATUS_COOLDOWN) return;	// クールダウン中は通らない
			m_eAction = EAction::RNDOM;												// ランダム状態
			m_eActionStatus = EActionStatus::ACTIONSTATUS_ACTION;					// アクション状態：アクション
		}

		break;

	default:		// エラー
		assert(false);
		break;
	}
}

//================================================================================
// 誰もボールを持っていない
//================================================================================
void CPlayerAIControlDefense::NotPlayerBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;
	CBall::EState stateBall = pBall->GetState();

	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	if (IsPassTarget() &&							// パスが自分に来る
		stateBall == CBall::EState::STATE_PASS)		// ボールがパス状態
	{
		// 何もしない
		m_eAction = EAction::IDLE;

		// アクション状態：待機
		m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;

		return;
	}

	if (IsLineOverBall())
	{// ボールが相手側にある

		if (m_eActionStatus != EActionStatus::ACTIONSTATUS_COOLDOWN ||
			m_eAction != EAction::RNDOM)
		{
			// ランダム
			m_eAction = EAction::RNDOM;

			// アクション状態：アクション
			m_eActionStatus = EActionStatus::ACTIONSTATUS_ACTION;

			return;
		}
	}

	if (!IsLineOverBall() &&							// 自陣にある
		stateBall == CBall::EState::STATE_LAND ||		// 転がっている
		stateBall == CBall::EState::STATE_FREE ||		// 触れて取れる状態
		stateBall == CBall::EState::STATE_REBOUND ||	// リバウンド
		stateBall == CBall::EState::STATE_PASS ||		// パス
		stateBall == CBall::EState::STATE_HOM_PASS)		// ホーミングパス
	{
		// ボールを追う
		m_eAction = EAction::CHASE_BALL;

		// アクション状態：アクション
		m_eActionStatus = EActionStatus::ACTIONSTATUS_ACTION;

		return;
	}
}

//================================================================================
// 守りの更新処理
//================================================================================
void CPlayerAIControlDefense::UpdateDefense(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// アクション状態更新
	(this->*(m_ActionStatusFunc[m_eActionStatus]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 状態：待機
//==========================================================================
void CPlayerAIControlDefense::StatusIdle(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 行動フラグ：待機
	SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);

	// アクションフラグ：なし
	SetActionFlag(EActionFlag::ACTION_NONE);
}

//==========================================================================
// 状態：アクション
//==========================================================================
void CPlayerAIControlDefense::StatusAction(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsCancel() &&
		m_eAction == EAction::RNDOM)
	{
		m_eAction = EAction::LEAVE;

		// 行動の構造体初期化
		ZeroMemory(&m_sAction, sizeof(m_sAction));
	}

	// アクション種類更新
	(this->*(m_ActionFunc[m_eAction]))();
}

//==========================================================================
// 状態：クールダウン
//==========================================================================
void CPlayerAIControlDefense::StatusCooldown(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsCancel())
	{
		// アクション状態：アクション
		m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;

		// 行動の構造体初期化
		ZeroMemory(&m_sAction, sizeof(m_sAction));

		return;
	}

	// アクションタイマーの設定
	SetActionTimer(COOLDOWN_MIN_RANDOM, COOLDOWN_MAX_RANDOM);

	// 更新：アクションタイマー
	UpdateActionTimer(fDeltaTime, fDeltaRate, fSlowRate);

	SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);

	{// ターゲットを見る
		CPlayer* pTarget = GetTarget();
		if (!pTarget) return;
		SeeTarget(pTarget->GetPosition());
	}
}

//================================================================================
// キャッチ精度の更新
//================================================================================
void CPlayerAIControlDefense::UpdateCatchAccuracy()
{
	int n = UtilFunc::Transformation::Random(0, 100);

	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	if (pBall->GetTarget() == GetPlayer())
	{
		// 投げる
		pControlAIAction->SetIsCatch(true);
	}
	else{
		// 投げる
		pControlAIAction->SetIsCatch(false);
	}

	if (n > 90)
	{
		SetCatchRadius(CATCH_RADIUS_JUST);
		return;
	}
	if (n > 20)
	{
		SetCatchRadius(CATCH_RADIUS_FAST);
		return;
	}

	SetCatchRadius(CATCH_RADIUS_LATE);
}

//================================================================================
// アクション決め
//================================================================================
void CPlayerAIControlDefense::SelectAction()
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// ボールねぇぞ

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	if (pBall->GetTarget() == pAI)
	{// ターゲットが自分

		m_eAction = EAction::LEAVE;
	}
	else
	{
		// クールダウン中の場合
		if (m_eActionStatus == EActionStatus::ACTIONSTATUS_COOLDOWN) return;

		// 行動：ランダム
		m_eAction = EAction::RNDOM;
	}

	if (m_eAction != EAction::IDLE)
	{
		// アクション状態：アクション
		m_eActionStatus = EActionStatus::ACTIONSTATUS_ACTION;
	}
}

//--------------------------------------------------------------------------
// ボールを追いかける
//--------------------------------------------------------------------------
void CPlayerAIControlDefense::MoveChaseBall()
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// ボールねぇぞ

	// ボール状態取得
	CBall::EState stateBall = pBall->GetState();
	
	if (!IsLineOverBall())
	{// 線を越えていない場合

		if (!IsPicksUpBall())
		{// 自分がボールに一番近い

			if (stateBall == CBall::EState::STATE_PASS ||		// パス
				stateBall == CBall::EState::STATE_HOM_PASS)		// ホーミングパス
			{// ボールを奪う
				BallSteal();
				return;
			}

			if (stateBall == CBall::EState::STATE_REBOUND)		// リバウンド
			{// リバウンドを取る
				BallChaseRebound();
				return;
			}
			
			// キャッチ：取りに行く
			BallChase();
			return;
		}
	}

	// アクション状態：なし
	m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;
}

//==========================================================================
// ボールを追う
//==========================================================================
void CPlayerAIControlDefense::BallChase()
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (pBall == nullptr || pBall->GetPlayer() != nullptr)
	{// ボールがnullptr&&プレイヤーがボールを取っている場合
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
		return;
	}

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// ボール位置
	MyLib::Vector3 posBall = pBall->GetPosition();

	// 自分からボールの距離
	float distance = pAI->GetPosition().DistanceXZ(posBall);

	// 行動：走る
	SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

	// 近づく
	if (Approatch(pBall->GetPosition(), END_RADIUS))
	{// 近づけた場合
		// 行動：止まる
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
	}
}

//==========================================================================
// 行動関連の処理
//==========================================================================

//--------------------------------------------------------------------------
// 何もしない
//--------------------------------------------------------------------------
void CPlayerAIControlDefense::MoveIdle()
{
	// 行動フラグ：なし
	SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);

	// アクション状態：なし
	m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;
}

//--------------------------------------------------------------------------
// 離れる
//--------------------------------------------------------------------------
void CPlayerAIControlDefense::MoveLeave()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	CPlayer* pPlayer = pBall->GetPlayer();
	if (!pPlayer) return;

	// 止まる
	SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);

	// 離れる
	if (Leave(pPlayer->GetPosition(), TARGET_DISTANCE))
	{
		// 止まる
		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);

		m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;
	}
}

//==========================================================================
// 上移動
//==========================================================================
void CPlayerAIControlDefense::MoveUp()
{
	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// ボール持ち主の取得
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{// 持ち主がいた

		// AIコントロール情報の取得
		CPlayerControlMove* pControlMove = pAI->GetBase()->GetPlayerControlMove();
		CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

		float rotDest = D3DX_PI;

		// カニ進行方向の設定
		pControlAIMove->SetClabDirection(rotDest);
	}
	else
	{
		// カニ進行方向の設定
		float rotDest = D3DX_PI;
		pAI->SetRotDest(rotDest);
	}
}

//==========================================================================
// 下移動
//==========================================================================
void CPlayerAIControlDefense::MoveDown()
{
	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// ボール持ち主の取得
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{
		// AIコントロール情報の取得
		CPlayerControlMove* pControlMove = pAI->GetBase()->GetPlayerControlMove();
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
		pAI->SetRotDest(rotDest);
	}
}

//==========================================================================
// 左移動
//==========================================================================
void CPlayerAIControlDefense::MoveLeft()
{
	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// ボール持ち主の取得
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{
		// AIコントロール情報の取得
		CPlayerControlMove* pControlMove = pAI->GetBase()->GetPlayerControlMove();
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
		pAI->SetRotDest(rotDest);
	}
}

//==========================================================================
// 右移動
//==========================================================================
void CPlayerAIControlDefense::MoveRight()
{
	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// ボール持ち主の取得
	CPlayer* pPlayer = GetBallOwner();

	if (pPlayer)
	{
		// AIコントロール情報の取得
		CPlayerControlMove* pControlMove = pAI->GetBase()->GetPlayerControlMove();
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
		pAI->SetRotDest(rotDest);
	}
}

//==========================================================================
// 更新：アクションタイマー
//==========================================================================
void CPlayerAIControlDefense::UpdateActionTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_sAction.bSet)
	{
		// タイマーのカウントダウン
		m_sAction.fTimer -= fDeltaTime * fDeltaRate * fSlowRate;

		if (m_sAction.fTimer <= 0.0f)
		{
			// 行動の構造体初期化
			ZeroMemory(&m_sAction, sizeof(m_sAction));

			// アクション：なし
			m_eAction = EAction::IDLE;

			// アクション状態：なし
			m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;

			// 行動：止まる
			SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
		}
	}
}

//==========================================================================
// アクション時間の設定
//==========================================================================
void CPlayerAIControlDefense::SetActionTimer(int nMin, int nMax)
{
	// 角度の設定
	if (!m_sAction.bSet)
	{// 設定されていない場合

		// 行動時間の設定
		float fRand = (float)UtilFunc::Transformation::Random(nMin, nMax);
		m_sAction.fTimer = fRand;

		// 時間設定ON
		m_sAction.bSet = true;
	}
}

//==========================================================================
// キャッチ精度
//==========================================================================
void CPlayerAIControlDefense::SetCatchRadius(float fRadius)
{
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// 半径の設定
	pControlAIAction->SetCatchRadius(fRadius);
}

//==========================================================================
// 行動キャンセル
//==========================================================================
bool CPlayerAIControlDefense::IsCancel()
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return false;	// ボールねぇぞ
	CBall::EState stateBall = pBall->GetState();	// ボール状態取得

	if (IsLineOverBall() &&							// 線を超えている
		!IsPicksUpBall() &&							// 自分より近いプレイヤーがいない
		stateBall == CBall::EState::STATE_LAND)		// 床にころがっている
	{
		return true;
	}

	// 自分の情報取得
	CPlayer* pPlayer = GetPlayer();
	if (!pPlayer) return false;

	if (pBall->GetTarget() == pPlayer)
	{// ターゲットが自分の場合
		return true;
	}

	// ボール情報の取得
	pPlayer = nullptr;
	pPlayer = pBall->GetPlayer();	// ボールからプレイヤー情報を取得
	if (pPlayer)
	{
		if (IsTargetDistanse())
		{
			return true;
		}
	}

	return false;
}

//==========================================================================
// ボール奪う
//==========================================================================
void CPlayerAIControlDefense::BallSteal()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// 位置の取得
	MyLib::Vector3 posBall = pBall->GetPosition();		// ボール
	MyLib::Vector3 posEnd = pBall->GetPosPassEnd();		// ボールのパス終了位置
	MyLib::Vector3 posMy = pAI->GetPosition();		// 自分の位置

	// 終了位置のx,zを参照した位置の設定
	MyLib::Vector3 pos = { posEnd.x, posMy.y, posEnd.z };

	// パス相手の取得
	CPlayer* pTarget = pBall->GetTarget();

	if (pTarget)
	{
		// ターゲットとボールの位置
		float distanth0 = pTarget->GetPosition().DistanceXZ(posBall);

		if (distanth0 < 100.0f)
		{// ボールとパス先の距離が範囲内ならあきらめる
			SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
			SetActionStatus(EActionStatus::ACTIONSTATUS_IDLE);
			return;
		}
	}
	else
	{// ターゲットが取得出来ない場合
		return;
	}

	// ボールとの距離
	float distance = posMy.DistanceXZ(posBall);

	// 行動状態：走る
	SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

	// ボールの方へ行く
	if (Approatch(pos, END_RADIUS) || distance < 100.0f)
	{// 終了位置に近づけた||ボールとの距離が範囲内の場合

		if (posBall.y < STEAL_HEIGHT_JUMP)
		{// 取れそうな高さに来た！
			SetActionFlag(EActionFlag::ACTION_JUMP);
		}

		SetActionStatus(EActionStatus::ACTIONSTATUS_IDLE);
	}
}

//==========================================================================
// リバウンド
//==========================================================================
void CPlayerAIControlDefense::BallChaseRebound()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// 位置の取得
	MyLib::Vector3 posBall = pBall->GetPosition();		// ボール
	MyLib::Vector3 posMy = pAI->GetPosition();		// 自分の位置

	// ボールとの距離
	float distance = posMy.DistanceXZ(posBall);

	// ボールの方へ行く
	if (Approatch(posBall, END_RADIUS))
	{// 終了位置に近づけた||ボールとの距離が範囲内の場合

		if (posBall.y < STEAL_HEIGHT_JUMP)
		{// 取れそうな高さに来た！
			SetActionFlag(EActionFlag::ACTION_JUMP);
		}

		SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);

		return;
	}

	// 行動状態：走る
	SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);
}

//==========================================================================
// 離れる : Leave(離れる相手、離れる距離)
//==========================================================================
bool CPlayerAIControlDefense::Leave(MyLib::Vector3 targetPos, float distance)
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
bool CPlayerAIControlDefense::Approatch(MyLib::Vector3 targetPos, float distance)
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
// 更新：見る
//==========================================================================
void CPlayerAIControlDefense::UpdateSee()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	CPlayer* pPlayer = pBall->GetPlayer();

	if (pPlayer)
	{
		// ターゲットを見る
		SeeTarget(pPlayer->GetPosition());
	}

	// ボールを見る
	SeeBall();
}

//==========================================================================
// ターゲットを見る
//==========================================================================
void CPlayerAIControlDefense::SeeTarget(MyLib::Vector3 pos)
{
	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// 向き設定
	float angle = pAI->GetPosition().AngleXZ(pos);
	pAI->SetRotDest(angle);
}

//==========================================================================
// ボールを見る
//==========================================================================
void CPlayerAIControlDefense::SeeBall()
{
	// ボール情報の取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;
	
	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// ボールとの角度
	float angle = pAI->GetPosition().AngleXZ(pBall->GetPosition());

	// 角度の設定
	pAI->SetRotDest(angle);
}

//==========================================================================
// ボール持ち主との距離
//==========================================================================
bool CPlayerAIControlDefense::IsTargetDistanse()
{
	float dis = 0.0f;
	if (!IsGetDistanceBallowner(&dis)) { return false; }

	if (dis < TARGET_DISTANCE) { return true; }

	return false;
}

//==========================================================================
// ボール持ち主との距離を計算
//==========================================================================
bool CPlayerAIControlDefense::IsGetDistanceBallowner(float* dis)
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return false;

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	CPlayer* pPlayer = pBall->GetPlayer();
	if (!pPlayer) return false;
	if (pPlayer->GetTeam() == pAI->GetTeam()) return false;

	// 距離を計算
	*dis = pAI->GetPosition().DistanceXZ(pPlayer->GetPosition());

	// 計算結果を返す

	return true;
}

//==========================================================================
// ボール持ち主の取得（敵の場合）
//==========================================================================
CPlayer* CPlayerAIControlDefense::GetBallOwner()
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
// ボールとの距離
//==========================================================================
float CPlayerAIControlDefense::GetDistanceBall()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return 0.0f;

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return 0.0f;

	float distance = pAI->GetPosition().DistanceXZ(pBall->GetPosition());

	return distance;
}

//==========================================================================
// 敵との距離
//==========================================================================
float CPlayerAIControlDefense::GetDistanceEnemy()
{
	float fMin = 10000000.0f;	// 最小値の保存用

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	// 自分情報
	MyLib::Vector3 posMy = pAI->GetPosition();		// 位置情報の取得
	CGameManager::ETeamSide TeamMy = pAI->GetTeam();	// 所属チーム

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報

		// 自分の&&同じチーム場合
		if (pPlayer == pAI && TeamMy == pPlayer->GetTeam()) continue;

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
// ターゲット設定
//==========================================================================
CPlayer* CPlayerAIControlDefense::GetTarget()
{
	CPlayer* pTarget = nullptr;			// 最終的なターゲット
	CPlayer* pTargetLength = nullptr;	// 距離ターゲットの保存用
	float fMinDis = 1000000.0f;	// 近いプレイヤー

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return pTarget;
	MyLib::Vector3 Mypos = pAI->GetPosition();			// 位置情報の取得
	CGameManager::ETeamSide teamSide = pAI->GetTeam();	// チームサイドの取得

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す
		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		MyLib::Vector3 posPlayer = pPlayer->GetPosition();	// プレイヤー位置

		if (teamSide == pPlayer->GetTeam() ||								// 同じチーム
			pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT ||		// 外野
			pPlayer->GetMotionFrag().bDead)									// 死亡している
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
		}
	}

	return pTarget;
}


// 判定 ==========================================================================

//==========================================================================
// パスの相手は自分ですか？
//==========================================================================
bool CPlayerAIControlDefense::IsPassTarget()
{
	// ボール情報の取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return false;

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	// ボールがパスしているかどうか
	bool bPass = pBall->IsPass();

	if (!bPass)
	{// パスしてない場合
		return false;
	}

	if (pBall->GetTarget() == pAI)
	{// ターゲットが自分
		return true;
	}

	return false;
}

//==========================================================================
// 誰がボールを取りに行きますか？
//==========================================================================
bool CPlayerAIControlDefense::IsPicksUpBall()
{
	float fMyDis = 1000000.0f;	// 自分のボールとの距離

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	// 自分の情報取得
	CGameManager::ETeamSide typeTeam = pAI->GetTeam();	// チームタイプ
	MyLib::Vector3 Mypos = pAI->GetPosition();	// 位置情報

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

		if (pPlayer == pAI) continue;		// 自分の場合
		if (pPlayer->GetTeam() != typeTeam) continue;		// チームが違う場合
		if ((pPlayer->GetBaseType() == CPlayer::EBaseType::TYPE_USER) ||
			(pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT))
		{// 外野&&ユーザー&&自分の場合
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
bool CPlayerAIControlDefense::IsDistanceBall()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return false;

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return false;

	// 位置の取得
	MyLib::Vector3 posBall = pBall->GetPosition();		// ボール
	MyLib::Vector3 posMy = pAI->GetPosition();		// 自分の位置

	// ボールとの距離
	float distance = posMy.DistanceXZ(posBall);

	if (distance < 100.0f)
	{// ボールとパス先の距離が範囲内ならあきらめる
		return true;
	}

	return false;
}
