//=============================================================================
// 
//  AIプレイヤー処理 [playerAI.cpp]
//  Author : 藤田勇一
// 
//=============================================================================
#include "playerAI.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "teamStatus.h"

#include "ball.h"

// 使用クラス
#include "playerStatus.h"
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace timing
{
	// タイミング
	const float TIMING_NORMAL = 1.0f;	// 通常
	const int TIMING_RAND_MAX = 100;
	const int TIMING_RAND_MIN = -80;

	// ジャンプ投げの最大位置
	const float JUMP_END_POS = 140.0f;
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerAI::MODE_FUNC CPlayerAI::m_ModeFunc[] =	// モード関数
{
	&CPlayerAI::ModeNone,				// なし
	&CPlayerAI::ModeThrowManager,		// 投げマネージャー
	&CPlayerAI::ModeCatchManager,		// キャッチマネージャー
};

CPlayerAI::TYPE_FUNC CPlayerAI::m_ThrowTypeFunc[] =	// 投げ種類関数
{
	&CPlayerAI::TypeNone,				// なし
	&CPlayerAI::TypeThrow,				// 通常投げ
	&CPlayerAI::TypeJumpThrow,			// ジャンプ投げ
	&CPlayerAI::TypeSpecialThrow,		// スペシャル投げ
};

CPlayerAI::MOVE_FUNC CPlayerAI::m_ThrowMoveFunc[] =	// 投げ状態関数
{
	&CPlayerAI::MoveNormal,			// 通常(その場)
	&CPlayerAI::MoveWalk,				// 歩き
	&CPlayerAI::MoveDash,				// 走り
};

CPlayerAI::TIMING_FUNC CPlayerAI::m_ThrowTimingFunc[] =	// タイミング関数
{
	&CPlayerAI::TimingNormal,			// 通常
	&CPlayerAI::TimingFeint,			// フェイント

	&CPlayerAI::TimingJumpNormal,			// 通常
	&CPlayerAI::TimingJumpQuick,			// 速く
	&CPlayerAI::TimingJumpDelay,			// 遅く
	&CPlayerAI::TimingJumpFeint,			// フェイント
};

CPlayerAI::CATCH_FUNC CPlayerAI::m_CatchFunc[] =	// キャッチ関数
{
	&CPlayerAI::CatchNone,
	&CPlayerAI::CatchNormal,
	&CPlayerAI::CatchJust,
	&CPlayerAI::CatchDash,
	&CPlayerAI::FindBall,
};

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAI::CPlayerAI()
{
	m_eMode = EMode::MODE_NONE;
	m_eThrowType = EThrowType::TYPE_NONE;
	m_eThrowMove = EThrowMove::MOVE_NORMAL;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;

	m_eCatchType = ECatchType::CATCH_TYPE_NONE;

	m_fTiming = 0.0f;	// タイミングカウント
	m_fTimingRate = 0.0f;	// タイミングの割合
	m_bTiming = false;
	m_bFoldJump = false;

	m_fJumpEnd = 0.0f;
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAI::~CPlayerAI()
{
	
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerAI::Init()
{
	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 親クラスの初期化
	HRESULT hr = CPlayer::Init();
	if (FAILED(hr)) { return E_FAIL; }

	// 操作関連
	ChangeMoveControl(DEBUG_NEW CPlayerAIControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());

	// 投げの最大位置設定
	m_fJumpEnd = timing::JUMP_END_POS;

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAI::Uninit()
{
	// 親クラスの終了
	CPlayer::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CPlayerAI::Kill()
{
	// 親クラスの終了
	CPlayer::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親クラスの更新
	CPlayer::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// 状態更新
	UpdateMode(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CPlayerAI::Draw()
{
	// 親クラスの描画
	CPlayer::Draw();
}

//==========================================================================
// 状態更新
//==========================================================================
void CPlayerAI::UpdateMode(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (pBall != nullptr && pBall->GetPlayer() == nullptr) {
		
		m_eMode = EMode::MODE_CATCH;
	}

	// 状態更新
	(this->*(m_ModeFunc[m_eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ヒット処理
//==========================================================================
CPlayerAI::SHitInfo CPlayerAI::Hit(CBall* pBall)
{
	CPlayer::SHitInfo hitInfo = CPlayer::Hit(pBall);	// ヒット情報

	if (hitInfo.eHit == EHit::HIT_NONE){
		return hitInfo;
	}

	if (hitInfo.eHit == EHit::HIT_CATCH){// キャッチ状態
		// 投げモード
		m_eMode = EMode::MODE_THROW;
	}

	return hitInfo;
}

//==========================================================================
// 投げの管理
//==========================================================================
void CPlayerAI::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CGameManager* pGameManager = CGameManager::GetInstance();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(this->GetCharStatus()->GetTeam());

	if (m_eThrowType == EThrowType::TYPE_NONE)
	{
		if (pTeamStatus->IsMaxSpecial())
		{// スペシャル
			m_eThrowType = EThrowType::TYPE_SPECIAL;
		}
		else
		{
			int n = 0;
			// 今はランダムで決定
			n = rand() % 2;
			//n = 1;

			switch (n)
			{
			case 0:
				m_eThrowType = EThrowType::TYPE_NORMAL;
				break;

			case 1:
				m_eThrowType = EThrowType::TYPE_JUMP;
				break;

			default:
				break;
			}
		}
	}

	Target();

	// 投げる種類更新
	(this->*(m_ThrowTypeFunc[m_eThrowType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// キャッチの管理
//==========================================================================
void CPlayerAI::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_eCatchType = ECatchType::CATCH_TYPE_FIND;

	// 投げる種類更新
	(this->*(m_CatchFunc[m_eCatchType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 通常投げ処理
//==========================================================================
void CPlayerAI::TypeThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 動きの設定 : 現在(その場)
	m_eThrowMove = EThrowMove::MOVE_WALK;

	// 投げる動き更新
	(this->*(m_ThrowMoveFunc[m_eThrowMove]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ジャンプ投げ処理
//==========================================================================
void CPlayerAI::TypeJumpThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 動きの設定 : 現在(その場)
	m_eThrowMove = EThrowMove::MOVE_WALK;

	// 投げる動き更新
	(this->*(m_ThrowMoveFunc[m_eThrowMove]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// スペシャル投げ処理
//==========================================================================
void CPlayerAI::TypeSpecialThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// スペシャル投げ
	pControlAIAction->SetIsSpecial(true);

	// それぞれの状態を戻す
	Reset();
}

//==========================================================================
// 通常投げ(その場)
//==========================================================================
void CPlayerAI::MoveNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// タイミング管理
	TimingManager(fDeltaTime, fDeltaRate, fSlowRate);

	// 投げる動き更新
	(this->*(m_ThrowTimingFunc[m_eThrowTiming]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 歩き投げ
//==========================================================================
void CPlayerAI::MoveWalk(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// 歩きフラグ設定
	pControlAIMove->SetIsWalk(true);

	// タイミング管理
	TimingManager(fDeltaTime, fDeltaRate, fSlowRate);

	// 投げる動き更新
	(this->*(m_ThrowTimingFunc[m_eThrowTiming]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 走り投げ
//==========================================================================
void CPlayerAI::MoveDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// タイミング管理
//==========================================================================
void CPlayerAI::TimingManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_bTiming) { return; }

	if (m_eThrowType == EThrowType::TYPE_NORMAL)
	{// 通常投げ
		// ランダムでタイミングを決める
		m_fTiming = timing::TIMING_NORMAL + UtilFunc::Transformation::Random(timing::TIMING_RAND_MIN, timing::TIMING_RAND_MAX) * 0.01f;
		
		m_eThrowTiming = EThrowTiming::TIMING_NORMAL;	// 通常投げ
		m_bTiming = true;	// タイミング設定完了フラグオン
	}
	else if (m_eThrowType == EThrowType::TYPE_JUMP)
	{// ジャンプ投げ

		// タイミングの設定
		int n = rand() % 3;
		//int n = 3;

		switch (n)
		{
		case 0:
			m_eThrowTiming = EThrowTiming::TIMING_JUMP_NORMAL;
			m_bTiming = true;
			break;

		case 1:
			m_eThrowTiming = EThrowTiming::TIMING_JUMP_QUICK;
			m_bTiming = true;
			break;

		case 2:
			m_eThrowTiming = EThrowTiming::TIMING_JUMP_DELAY;
			m_bTiming = true;
			break;

		case 3:
			m_eThrowTiming = EThrowTiming::TIMING_JUMP_FEINT;
			m_bTiming = true;
			break;
		default:
			break;
		}
	}
}

//==========================================================================
// 投げタイミング(その場)
//==========================================================================
void CPlayerAI::TimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	if (m_fTiming > 0.0f)
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	// 投げる
	pControlAIAction->SetIsThrow(true);

	// 変数リセット
	Reset();
}

//==========================================================================
// フェイント(普通の投げ)
//==========================================================================
void CPlayerAI::TimingFeint(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 通常(ジャンプの投げ)
//==========================================================================
void CPlayerAI::TimingJumpNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_fTiming > 0.0f && (m_eThrowMove == EThrowMove::MOVE_WALK || m_eThrowMove == EThrowMove::MOVE_DASH))
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// 歩きリセット
	pControlAIAction->SetIsJump(true);	// ジャンプオン

	if (GetPosition().y >= timing::JUMP_END_POS)	// 高さによって変わる
	{
		// 投げる
		pControlAIAction->SetIsThrow(true);

		// 変数リセット
		Reset();
	}
	
}

//==========================================================================
// 速い(ジャンプの投げ)
//==========================================================================
void CPlayerAI::TimingJumpQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_fTiming > 0.0f && m_eThrowMove == EThrowMove::MOVE_WALK || m_eThrowMove == EThrowMove::MOVE_DASH)
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// 歩きオフ
	pControlAIAction->SetIsJump(true);	// ジャンプオン

	if (GetPosition().y >= timing::JUMP_END_POS * 0.5f)	// 高さによって変わる
	{
		// 投げる
		pControlAIAction->SetIsThrow(true);

		// それぞれの状態のリセット
		Reset();
	}
}

//==========================================================================
// 遅い(ジャンプの投げ)
//==========================================================================
void CPlayerAI::TimingJumpDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_fTiming > 0.0f && m_eThrowMove == EThrowMove::MOVE_WALK || m_eThrowMove == EThrowMove::MOVE_DASH)
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// 歩きオフ
	pControlAIAction->SetIsJump(true);	// ジャンプオン

	if (GetPosition().y >= timing::JUMP_END_POS)	// 高さによって変わる
	{
		m_bFoldJump = true;	// 折り返しオン
	}
		
	if (!m_bFoldJump) { return; }

	if (GetPosition().y <= timing::JUMP_END_POS * 0.5f)	// 高さによって変わる
	{
		// 投げる
		pControlAIAction->SetIsThrow(true);

		// それぞれの状態のリセット
		Reset();
	}
}

//==========================================================================
// フェイント(ジャンプの投げ)
//==========================================================================
void CPlayerAI::TimingJumpFeint(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_fTiming > 0.0f && m_eThrowMove == EThrowMove::MOVE_WALK || m_eThrowMove == EThrowMove::MOVE_DASH)
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// 歩きオフ
	pControlAIAction->SetIsJump(true);	// ジャンプオン

	if (GetPosition().y >= timing::JUMP_END_POS)	// 高さによって変わる
	{
		// それぞれの状態のリセット
		Reset();

		//m_eMode = EMode::MODE_THROW;
	}
}

//==========================================================================
// 通常キャッチ
//==========================================================================
void CPlayerAI::CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ジャストキャッチ
//==========================================================================
void CPlayerAI::CatchJust(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ダッシュキャッチ
//==========================================================================
void CPlayerAI::CatchDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// ターゲット
//==========================================================================
void CPlayerAI::Target()
{
	CPlayer* pTarget = nullptr;	// 目標ターゲット
	float fMinDis = 1000000.0f;	// ボールから近いプレイヤー

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// プレイヤー位置

		// ボールの取得
		CBall* pBall = GetBall();
		if (pBall == nullptr) { return; }
		CGameManager::TeamSide typeTeam = pBall->GetTypeTeam();	// チームタイプの取得

		// 同じチームの場合次へ
		if (typeTeam == pPlayer->GetStatus()->GetTeam()) { continue; }

		// 敵との距離を求める
		float fLength = sqrtf(
			(pPlayer->GetPosition().x - GetPosition().x) * (pPlayer->GetPosition().x - GetPosition().x) +
			(pPlayer->GetPosition().z - GetPosition().z) * (pPlayer->GetPosition().z - GetPosition().z));

		if (fLength < fMinDis)
		{ // より近い相手プレイヤーがいた場合

			// ターゲットを更新
			pTarget = pPlayer;

			// 方向設定
			SetRotDest(GetPosition().AngleXZ(pTarget->GetPosition()));
		}
	}
}

//==========================================================================
// 待て
//==========================================================================
bool CPlayerAI::IsWait()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	bool b = false;

	if (!pBall) { return b; }

	CGameManager::TeamSide typeTeam = GetStatus()->GetTeam();

	if (typeTeam == CGameManager::TeamSide::SIDE_LEFT)
	{
		if (pBall->GetPosition().x > 0.0f)
		{
			//m_eCatchType = ECatchType::CATCH_TYPE_NONE;
			Reset();
			b = true;
		}
	}
	else if (typeTeam == CGameManager::TeamSide::SIDE_RIGHT)
	{
		if (pBall->GetPosition().x < 0.0f)
		{
			//m_eCatchType = ECatchType::CATCH_TYPE_NONE;
			Reset();
			b = true;
		}
	}

	return b;
}

//==========================================================================
// 取りに行く
//==========================================================================
void CPlayerAI::FindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsWait()) { return; }

	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (pBall == nullptr || pBall->GetPlayer() != nullptr) {
		Reset();
		return;
	}

	// 角度を求める(playerからみたボール)
	float fAngle = GetPosition().AngleXZ(pBall->GetPosition());

	// 長さを求める
	//float fLength = sqrtf((pBall->GetPosition().x - player->GetPosition().x) * (pBall->GetPosition().x - player->GetPosition().x) +
	//	(pBall->GetPosition().z - player->GetPosition().z) * (pBall->GetPosition().z - player->GetPosition().z));

	// 歩きオン
	pControlAIMove->SetIsWalk(true);

	// 方向設定
	SetRotDest(fAngle);
}

//==========================================================================
// 変数のリセット
//==========================================================================
void CPlayerAI::Reset()
{
	// 列挙リセット
	m_eMode = EMode::MODE_NONE;
	m_eThrowType = EThrowType::TYPE_NONE;
	m_eThrowMove = EThrowMove::MOVE_NORMAL;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;

	// タイミングリセット
	m_fTiming = timing::TIMING_NORMAL;
	m_bTiming = false;

	// 折り返しリセット
	m_bFoldJump = false;

	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// 行動フラグリセット
	pControlAIMove->SetIsWalk(false);
	pControlAIMove->SetIsDash(false);
	pControlAIMove->SetIsBlink(false);

	pControlAIAction->SetIsJump(false);
}

//==========================================================================
// 操作処理
//==========================================================================
void CPlayerAI::Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// 移動操作	// TODO：AIむじ～
	pControlMove->Move(this, fDeltaTime, fDeltaRate, fSlowRate);
	pControlAction->Action(this, fDeltaTime, fDeltaRate, fSlowRate);

	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayerAI::ChangeMoveControl(CPlayerAIControlMove* control)
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();

	delete pControlMove;
	pControlMove = control;

	SetPlayerControlMove(pControlMove);
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayerAI::ChangeActionControl(CPlayerAIControlAction* control)
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	delete pControlAction;
	pControlAction = control;

	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// 操作関連削除
//==========================================================================
void CPlayerAI::DeleteControl()
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	if (pControlMove != nullptr)
	{// 移動操作
		delete pControlMove;
		pControlMove = nullptr;
	}

	if (pControlAction != nullptr)
	{// アクション操作
		delete pControlAction;
		pControlAction = nullptr;
	}

	// 操作関連削除
	CPlayer::DeleteControl();

	SetPlayerControlMove(pControlMove);
	SetPlayerControlAction(pControlAction);
}

//==========================================================================
// デバッグ処理
//==========================================================================
void CPlayerAI::Debug()
{
#if _DEBUG
	// デバッグ処理
	CPlayer::Debug();

	CPlayerAIControlAction* pControlAction = dynamic_cast<CPlayerAIControlAction*> (GetPlayerControlAction());
	if (pControlAction == nullptr) return;

	// 自動投げフラグ設定
	bool autoThrow = pControlAction->IsAutoThrow();
	if (ImGui::Checkbox("Change Auto Throw", &autoThrow))
	{ 
		pControlAction->SetEnableAutoThrow(autoThrow);
	}
#endif
}