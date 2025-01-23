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
	// 行動タイプ
	const int MOVETYPE_WAIT_MODE_MAX = 10;		// 待機に入る確率幅 (最低は0固定)
	const int MOVETYPE_WAIT_MODE_IN = 6;		// 待機に入る確率(この数値以下の場合、待機状態)
	const int MOVETYPE_WAIT_TIME_MAX = 20;		// 最大：待機時間
	const int MOVETYPE_WAIT_TIME_MIN = 10;		// 最低：
	const int MOVETYPE_ATYAKOTYA_TIME_MAX = 10;		// 最大：あっちゃこっちゃ時間
	const int MOVETYPE_ATYAKOTYA_TIME_MIN = 5;		// 最低：
	const int MOVETYPE_LEFTRIGHT_TIME_MAX = 10;		// 最大：左右移動時間
	const int MOVETYPE_LEFTRIGHT_TIME_MIN = 5;		// 最低：
	const int MOVETYPE_UPDOWN_TIME_MAX = 10;		// 最大：上下移動時間
	const int MOVETYPE_UPDOWN_TIME_MIN = 5;		// 最低：

	// パス
	const float STEAL_CANCEL_LENGTH = 100.0f;	// あきらめる距離

	const float MOTIVATION_MAX = 100.0f;		// モチベーション(MAX)

	// 距離間(デフォルト)
	const float LENGTH_TARGET = 400.0f;		// ターゲットとの距離(デフォルト)
	const float LENGTH_VALUE = 100.0f;

	const float BALL_DISTANCE = 300.0f;			// 

	const float CATCH_JUMP_LENGTH = 100.0f;
	const float CATCH_JUMP_HEIGHT = 140.0f;

	const float JUMP_RATE = 1.0f;				// ジャンプの割合(高さ)
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerAIControlDefense::ACTION_FUNC CPlayerAIControlDefense::m_ActionFunc[] =	// キャッチ関数
{
	&CPlayerAIControlDefense::MoveIdle,				// なし
	&CPlayerAIControlDefense::MoveDodge,				// 回避
	&CPlayerAIControlDefense::MoveSupport,				// サポート
	&CPlayerAIControlDefense::MoveChaseBall,			// ボールを追いかける
	&CPlayerAIControlDefense::MoveRetreat,				// 後退
	&CPlayerAIControlDefense::MoveRandom,				// ランダム
	&CPlayerAIControlDefense::MoveLeave,				// 離れる
};


//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControlDefense::CPlayerAIControlDefense()
{

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

	// 変数の初期化
	m_fDistanse = 200.0f;

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAIControlDefense::Uninit()
{
	delete this;

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

	if (pBall->GetTarget() == GetPlayer())
	{// ボールのターゲットが自分の場合
		return;
	}

	if (pPlayer)
	{// 味方がボールを持っている場合
		TeammateBall(fDeltaTime, fDeltaRate, fSlowRate);
	}
	else
	{// 敵がボールを持っている場合
		TeamEnemyBall(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// 親クラスの更新（最後尾に設置）
	CPlayerAIControlMode::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// チームメイトボール
//================================================================================
void CPlayerAIControlDefense::TeammateBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;
	CPlayer* pPlayer = pBall->GetPlayer();

	// チーム
	CGameManager::ETeamSide side = pPlayer->GetTeam();
	CGameManager::ETeamSide side1 = GetPlayer()->GetTeam();

	switch (pPlayer->GetAreaType())
	{// エリア別

	case CPlayer::EFieldArea::FIELD_IN:	// 内野

		if (side == side1)
		{// 同じチーム
			MoveRandom();
		}
		else
		{
			// 守りの更新
			UpdateDefense(fDeltaTime, fDeltaRate, fSlowRate);
		}

		break;

	case CPlayer::EFieldArea::FIELD_OUT:	// 外野

		MoveRandom();

		break;

	default:
		break;
	}
}

//================================================================================
// 相手チームボール
//================================================================================
void CPlayerAIControlDefense::TeamEnemyBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!IsLineOverBall())
	{
		// ボールを追う
		m_eAction = EAction::CHASE_BALL;
	}

	UpdateDefense(fDeltaTime, fDeltaRate, fSlowRate);
}

//================================================================================
// 守りの更新処理
//================================================================================
void CPlayerAIControlDefense::UpdateDefense(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// ボールねぇぞ
	CBall::EState stateBall = pBall->GetState();	// ボール状態取得

	// 持ち主情報取得
	CPlayer* pPlayer = GetBallOwner();

	switch (m_eActionStatus)
	{
	case EActionStatus::ACTIONSTATUS_IDLE:		// 待機

		if (pPlayer)
		{// ボール所持者がいる
			Action0();
		}
		else
		{// いない
			Action1();
		}

		if (m_eAction != EAction::IDLE)
		{
			// アクション状態：アクション
			m_eActionStatus = EActionStatus::ACTIONSTATUS_ACTION;
		}

		break;

	case EActionStatus::ACTIONSTATUS_ACTION:		// 行動

		if (pBall->GetTarget() == GetPlayer() &&	// ターゲットが自分
			m_eAction == EAction::RNDOM)			// 行動がランダム
		{
			m_eAction = EAction::LEAVE;

			// 行動の構造体初期化
			ZeroMemory(&m_sAction, sizeof(m_sAction));

			break;
		}

		// アクション種類更新
		(this->*(m_ActionFunc[m_eAction]))();

		break;

	case EActionStatus::ACTIONSTATUS_COOLDOWN:	// クールダウン

		if (IsCancel())
		{
			// アクション状態：アクション
			m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;

			// 行動の構造体初期化
			ZeroMemory(&m_sAction, sizeof(m_sAction));

			return;
		}

		// アクションタイマーの設定
		SetActionTimer(1, 5);

		// 更新：アクションタイマー
		UpdateActionTimer(fDeltaTime, fDeltaRate, fSlowRate);

		break;

	default:
		assert(false);
		break;
	}
}

//================================================================================
// アクション決め
//================================================================================
void CPlayerAIControlDefense::Action0()
{
	// 持ち主情報取得
	CPlayer* pPlayer = GetBallOwner();

	CPlayer::EFieldArea area = pPlayer->GetAreaType();

	if (area == CPlayer::EFieldArea::FIELD_IN)
	{// ボール持ち主が内野の場合
		m_eAction = EAction::RNDOM;
	}
	else
	{
		m_eAction = EAction::LEAVE;
	}
}

//================================================================================
// アクション決め
//================================================================================
void CPlayerAIControlDefense::Action1()
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;	// ボールねぇぞ
	CBall::EState stateBall = pBall->GetState();	// ボール状態取得

	if (IsPassTarget() && stateBall == CBall::EState::STATE_PASS)
	{// パスが自分に来る&&ボールがパス状態の合
		// 何もしない
		m_eAction = EAction::IDLE;
	}

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	if (pBall->GetTarget() == pAI)
	{// ターゲットが自分
		float distanceBall = GetDistanceBall();		// ボールとの距離

		if (distanceBall > 300.0f) {
			// 後退(安全地帯へ)
			m_eAction = EAction::RNDOM;
		}
		else {

			m_eAction = EAction::IDLE;
		}
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

	if (stateBall == CBall::EState::STATE_PASS ||
		stateBall == CBall::EState::STATE_HOM_PASS)
	{// パス||ホーミングパスの場合

		// ボールを奪う
		BallSteal();

		return;
	}
	else if (!IsLineOverBall())
	{// 線を越えていない場合

		if (!IsPicksUpBall())
		{// 自分がボールに近い場合

			// キャッチ：取りに行く
			BallChase();

			return;
		}
	}

	// アクション状態：なし
	m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;
}

//==========================================================================
// ボールを奪う
//==========================================================================
void CPlayerAIControlDefense::BallSteal()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return;

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return;

	// 同じチームの場合
	//if (pBall->GetTypeTeam() == pAI->GetTeam()) return;

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

		if (distanth0 < STEAL_CANCEL_LENGTH)
		{// ボールとパス先の距離が範囲内ならあきらめる
			SetMoveFlag(EMoveFlag::MOVEFLAG_IDLE);
			m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;
			return;
		}
	}
	
	// ボールとの距離
	float distance = posMy.DistanceXZ(posBall);

	// 行動状態：走る
	SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);

	// ボールの方へ行く
	if (Approatch(pos, CATCH_JUMP_LENGTH) || distance < CATCH_JUMP_LENGTH)
	{// 終了位置に近づけた||ボールとの距離が範囲内の場合

		if (posBall.y < CATCH_JUMP_HEIGHT)
		{// 取れそうな高さに来た！
			SetActionFlag(EActionFlag::ACTION_JUMP);
		}

		m_eActionStatus = EActionStatus::ACTIONSTATUS_IDLE;
	}
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

	if (distance > BALL_DISTANCE) {
		// 行動：走る
		SetMoveFlag(EMoveFlag::MOVEFLAG_DASH);
	}
	else {
		// 行動：歩く
		SetMoveFlag(EMoveFlag::MOVEFLAG_WALK);
	}

	// 近づく
	if (Approatch(pBall->GetPosition(), 30.0f))
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
// 回避
//--------------------------------------------------------------------------
void CPlayerAIControlDefense::MoveDodge()
{
	// ボールとの距離
	float distanceBall = GetDistanceBall();

	// 距離が離れている場合
	if (distanceBall > 5.0f) return;

	// 行動：ブリンク
	//m_eMoveFlag = EMoveFlag::MOVEFLAG_BLINK;
}

//--------------------------------------------------------------------------
// サポート
//--------------------------------------------------------------------------
void CPlayerAIControlDefense::MoveSupport()
{
	//// 体力の少ない味方をカバー
	//int nLife = pAI->GetLife();

	//if (!m_sMove.pDefenseTarget)
	//{
	//	int nMinLife = 100000000;

	//	// 自分情報
	//	MyLib::Vector3 posMy = pAI->GetPosition();		// 位置情報の取得
	//	CGameManager::ETeamSide TeamMy = pAI->GetTeam();	// 所属チーム

	//	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	//	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	//	while (list.ListLoop(itr))
	//	{ // リスト内の要素数分繰り返す

	//		CPlayer* pPlayer = (*itr);	// プレイヤー情報

	//		// 内野以外&&同じチームじゃない
	//		if (pPlayer->GetAreaType() != CPlayer::EFieldArea::FIELD_IN &&
	//			pPlayer->GetTeam() != pAI->GetTeam()) continue;

	//		// 自分
	//		if (pPlayer == pAI) continue;

	//		// 他人の体力
	//		int nLifeOther = pPlayer->GetLife();

	//		if (nLifeOther < nLife && nLifeOther < nMinLife)
	//		{// 自分の体力より少ない&&チームの中で一番体力が少ない場合

	//			// 最小体力の更新
	//			nMinLife = nLifeOther;

	//			// 守備対象の設定
	//			m_sMove.pDefenseTarget = pPlayer;
	//		}
	//	}

	//	return;
	//}


	// 味方との距離を取る(標的を分散)

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
	if (Leave(pPlayer->GetPosition(), 200.0f))
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
// 行動キャンセル
//==========================================================================
bool CPlayerAIControlDefense::IsCancel()
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return false;	// ボールねぇぞ
	CBall::EState stateBall = pBall->GetState();	// ボール状態取得

	if (!IsLineOverBall() &&							// 線を超えていない
		!IsPicksUpBall() &&								// 自分より近いプレイヤーがいない
		stateBall == CBall::EState::STATE_LAND)			// 床に事がっている
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

	return false;
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
	//CBall* pBall = CGameManager::GetInstance()->GetBall();
	//if (!pBall) return;

	//CPlayer* pPlayer = pBall->GetPlayer();

	//if (pPlayer)
	//{
	//	// 
	//	SeeTarget(pPlayer->GetPosition());
	//}

	// ボールを見る
	//SeeBall();
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
// ボール持ち主との距離を計算
//==========================================================================
float CPlayerAIControlDefense::GetDistanceBallowner()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return 0.0f;

	// AIの取得
	CPlayer* pAI = GetPlayer();
	if (!pAI) return 0.0f;

	CPlayer* pPlayer = pBall->GetPlayer();
	if (!pPlayer) return 0.0f;
	if (pPlayer->GetTeam() == pAI->GetTeam()) return 0.0f;

	// 距離を計算
	float distance = pAI->GetPosition().DistanceXZ(pPlayer->GetPosition());

	// 計算結果を返す
	return distance;
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

	bool b = pBall->IsPass();

	if (b)
	{
		return false;
	}

	// ボールパス&&ターゲットが自分
	if (pBall->IsPass() && pBall->GetTarget() == pAI)
	{
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
