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
	const int TIMING_RAND_MAX = 100;
	const int TIMING_RAND_MIN = -80;

	// ジャンプ投げの最大位置
	const float JUMP_END_POS = 140.0f;
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerAIControl::MODE_FUNC CPlayerAIControl::m_ModeFunc[] =	// モード関数
{
	&CPlayerAIControl::ModeNone,				// なし
	&CPlayerAIControl::ModeThrowManager,		// 投げマネージャー
	&CPlayerAIControl::ModeCatchManager,		// キャッチマネージャー
};

CPlayerAIControl::TYPE_FUNC CPlayerAIControl::m_ThrowTypeFunc[] =	// 投げ種類関数
{
	&CPlayerAIControl::TypeNone,				// なし
	&CPlayerAIControl::TypeThrow,				// 通常投げ
	&CPlayerAIControl::TypeJumpThrow,			// ジャンプ投げ
	&CPlayerAIControl::TypeSpecialThrow,		// スペシャル投げ
};

CPlayerAIControl::MOVE_FUNC CPlayerAIControl::m_ThrowMoveFunc[] =	// 投げ状態関数
{
	&CPlayerAIControl::MoveNormal,			// 通常(その場)
	&CPlayerAIControl::MoveWalk,				// 歩き
	&CPlayerAIControl::MoveDash,				// 走り
};

CPlayerAIControl::TIMING_FUNC CPlayerAIControl::m_ThrowTimingFunc[] =	// タイミング関数
{
	&CPlayerAIControl::TimingNormal,			// 通常
	&CPlayerAIControl::TimingFeint,			// フェイント

	&CPlayerAIControl::TimingJumpNormal,			// 通常
	&CPlayerAIControl::TimingJumpQuick,			// 速く
	&CPlayerAIControl::TimingJumpDelay,			// 遅く
	&CPlayerAIControl::TimingJumpFeint,			// フェイント
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
	m_eMode = EMode::MODE_NONE;
	m_eThrowType = EThrowType::TYPE_NONE;
	m_eThrowMove = EThrowMove::MOVE_NORMAL;
	m_eThrowTiming = EThrowTiming::TIMING_NORMAL;
	m_eCatchType = ECatchType::CATCH_TYPE_NONE;
	m_eMoveType = EMoveType::MOVETYPE_NONE;
	m_fTiming = 0.0f;		// タイミングカウント
	m_fTimingRate = 0.0f;	// タイミングの割合
	m_bTiming = false;
	m_bFoldJump = false;
	m_fJumpEnd = timing::JUMP_END_POS;	// 投げの最大位置

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

}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIControl::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall != nullptr && pBall->GetPlayer() == nullptr) {

		m_eMode = EMode::MODE_CATCH;
	}

	// 状態更新
	(this->*(m_ModeFunc[m_eMode]))(fDeltaTime, fDeltaRate, fSlowRate);

	//// AIコントロール情報の取得
	//CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	//CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	//// 歩きフラグ設定
	//if (m_eMoveType == EMoveType::MOVETYPE_DISTANCE)
	//{
	//	pControlAIMove->SetIsWalk(true);
	//}
	//else
	//{
	//	pControlAIMove->SetIsWalk(false);
	//}

	//Distance();
}

//==========================================================================
// 投げの管理
//==========================================================================
void CPlayerAIControl::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CGameManager* pGameManager = CGameManager::GetInstance();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(m_pAI->GetStatus()->GetTeam());

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
void CPlayerAIControl::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_eCatchType = ECatchType::CATCH_TYPE_FIND;

	// 投げる種類更新
	(this->*(m_CatchFunc[m_eCatchType]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 通常投げ処理
//==========================================================================
void CPlayerAIControl::TypeThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 動きの設定 : 現在(その場)
	m_eThrowMove = EThrowMove::MOVE_WALK;

	// 投げる動き更新
	(this->*(m_ThrowMoveFunc[m_eThrowMove]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ジャンプ投げ処理
//==========================================================================
void CPlayerAIControl::TypeJumpThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 動きの設定 : 現在(その場)
	m_eThrowMove = EThrowMove::MOVE_WALK;

	// 投げる動き更新
	(this->*(m_ThrowMoveFunc[m_eThrowMove]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// スペシャル投げ処理
//==========================================================================
void CPlayerAIControl::TypeSpecialThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	
	// スペシャル投げ
	pControlAIAction->SetIsSpecial(true);

	// それぞれの状態を戻す
	Reset();
}

//==========================================================================
// 通常投げ(その場)
//==========================================================================
void CPlayerAIControl::MoveNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// タイミング管理
	TimingManager(fDeltaTime, fDeltaRate, fSlowRate);

	// 投げる動き更新
	(this->*(m_ThrowTimingFunc[m_eThrowTiming]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 歩き投げ
//==========================================================================
void CPlayerAIControl::MoveWalk(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
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
void CPlayerAIControl::MoveDash(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// タイミング管理
//==========================================================================
void CPlayerAIControl::TimingManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
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
void CPlayerAIControl::TimingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
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
void CPlayerAIControl::TimingFeint(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// 通常(ジャンプの投げ)
//==========================================================================
void CPlayerAIControl::TimingJumpNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_fTiming > 0.0f && (m_eThrowMove == EThrowMove::MOVE_WALK || m_eThrowMove == EThrowMove::MOVE_DASH))
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// 歩きリセット
	pControlAIAction->SetIsJump(true);	// ジャンプオン

	if (m_pAI->GetPosition().y >= timing::JUMP_END_POS)	// 高さによって変わる
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
void CPlayerAIControl::TimingJumpQuick(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_fTiming > 0.0f && m_eThrowMove == EThrowMove::MOVE_WALK || m_eThrowMove == EThrowMove::MOVE_DASH)
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// 歩きオフ
	pControlAIAction->SetIsJump(true);	// ジャンプオン

	if (m_pAI->GetPosition().y >= timing::JUMP_END_POS * 0.5f)	// 高さによって変わる
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
void CPlayerAIControl::TimingJumpDelay(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_fTiming > 0.0f && m_eThrowMove == EThrowMove::MOVE_WALK || m_eThrowMove == EThrowMove::MOVE_DASH)
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// 歩きオフ
	pControlAIAction->SetIsJump(true);	// ジャンプオン

	// 位置情報取得
	MyLib::Vector3 pos = m_pAI->GetPosition();

	if (pos.y >= timing::JUMP_END_POS)	// 高さによって変わる
	{
		m_bFoldJump = true;	// 折り返しオン
	}

	if (!m_bFoldJump) { return; }

	if (pos.y <= timing::JUMP_END_POS * 0.5f)	// 高さによって変わる
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
void CPlayerAIControl::TimingJumpFeint(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	if (m_fTiming > 0.0f && m_eThrowMove == EThrowMove::MOVE_WALK || m_eThrowMove == EThrowMove::MOVE_DASH)
	{
		m_fTiming -= fDeltaTime * fDeltaRate * fSlowRate;

		return;
	}

	pControlAIMove->SetIsWalk(false);	// 歩きオフ
	pControlAIAction->SetIsJump(true);	// ジャンプオン

	if (m_pAI->GetPosition().y >= timing::JUMP_END_POS)	// 高さによって変わる
	{
		// それぞれの状態のリセット
		Reset();

		//m_eMode = EMode::MODE_THROW;
	}
}

//==========================================================================
// 通常キャッチ
//==========================================================================
void CPlayerAIControl::CatchNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

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
// ターゲット
//==========================================================================
void CPlayerAIControl::Target()
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

			// 方向設定
			m_pAI->SetRotDest(pos.AngleXZ(pTarget->GetPosition()));
		}
	}
}

//==========================================================================
// 待て
//==========================================================================
bool CPlayerAIControl::IsWait()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	bool b = false;

	if (!pBall) { return b; }

	CGameManager::ETeamSide typeTeam = m_pAI->GetStatus()->GetTeam();

	if (typeTeam == CGameManager::ETeamSide::SIDE_LEFT)
	{
		if (pBall->GetPosition().x > 0.0f)
		{
			//m_eCatchType = ECatchType::CATCH_TYPE_NONE;
			Reset();
			b = true;
		}
	}
	else if (typeTeam == CGameManager::ETeamSide::SIDE_RIGHT)
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
// 距離を取る
//==========================================================================
void CPlayerAIControl::Distance()
{
	CPlayer* pTarget = nullptr;	// 目標ターゲット
	float fMinDis = 400.0f;	// 近いプレイヤー

	MyLib::Vector3 pos = m_pAI->GetPosition();	// 位置情報の取得

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// プレイヤー位置

		// ボールの取得
		CBall* pBall = CGameManager::GetInstance()->GetBall();
		if (pBall == nullptr) { return; }
		CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// チームタイプの取得

		// 同じチームまたはボールがどのサイドでも無い場合
		if ((typeTeam == m_pAI->GetStatus()->GetTeam()) || 
			(typeTeam == CGameManager::ETeamSide::SIDE_NONE))
		{ continue; }

		// 相手との距離を求める
		float fLength = sqrtf(
			(pPlayer->GetPosition().x - pos.x) * (pPlayer->GetPosition().x - pos.x) +
			(pPlayer->GetPosition().z - pos.z) * (pPlayer->GetPosition().z - pos.z));

		if (fLength < fMinDis)
		{ // より近い相手プレイヤーがいた場合

			m_eMoveType = EMoveType::MOVETYPE_DISTANCE;

			pTarget = pPlayer;

			float fRotDest = m_pAI->GetRotDest();

			float fSpeed = m_pAI->GetParameter().fVelocityNormal;

	

			/*m_pAI->SetMove({ 
				sinf(-fRotDest) * fSpeed,
				0.0f,
				cosf(-fRotDest) * fSpeed
				});*/
		}
	}
}

//==========================================================================
// 取りに行く
//==========================================================================
void CPlayerAIControl::FindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (IsWait()) { return; }

	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();
	CPlayerControlMove* pControlMove = m_pAI->GetBase()->GetPlayerControlMove();
	CPlayerAIControlMove* pControlAIMove = pControlMove->GetAI();

	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (pBall == nullptr || pBall->GetPlayer() != nullptr) {
		Reset();
		return;
	}

	// 角度を求める(playerからみたボール)
	float fAngle = m_pAI->GetPosition().AngleXZ(pBall->GetPosition());

	// 長さを求める
	//float fLength = sqrtf((pBall->GetPosition().x - player->GetPosition().x) * (pBall->GetPosition().x - player->GetPosition().x) +
	//	(pBall->GetPosition().z - player->GetPosition().z) * (pBall->GetPosition().z - player->GetPosition().z));

	// 歩きオン
	pControlAIMove->SetIsWalk(true);

	// 方向設定
	m_pAI->SetRotDest(fAngle);
}

//==========================================================================
// 変数のリセット
//==========================================================================
void CPlayerAIControl::Reset()
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
