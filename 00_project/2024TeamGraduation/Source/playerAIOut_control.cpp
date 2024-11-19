//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerAIOut_control.h"
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

#include "playerAIOut_controlAction.h"
#include "playerAIOut_controlMove.h"

#include "playerAIOut.h"

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
CPlayerAIOutControl::MODE_FUNC CPlayerAIOutControl::m_ModeFunc[] =	// モード関数
{
	&CPlayerAIOutControl::ModeNone,				// なし
	& CPlayerAIOutControl::ModeThrowManager,		// 投げ
	& CPlayerAIOutControl::ModeCatchManager,		// キャッチ
};

CPlayerAIOutControl::CATCH_FUNC CPlayerAIOutControl::m_CatchFunc[] =	// キャッチ関数
{
	&CPlayerAIOutControl::CatchNone,		// なし
	&CPlayerAIOutControl::FindBall,			// 見つける
};


//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIOutControl::CPlayerAIOutControl()
{
	// 値の初期化
	m_eMode = EMode::MODE_NONE;
	m_eCatch = ECatch::CATCH_NONE;
	m_pAIOut = nullptr;
	m_bStart = false;
	m_bEnd = false;
}

//==========================================================================
// 生成
//==========================================================================
CPlayerAIOutControl* CPlayerAIOutControl::Create(CPlayer* player)
{
	// メモリの確保
	CPlayerAIOutControl* pObj = DEBUG_NEW CPlayerAIOutControl;
	if (pObj != nullptr)
	{
		pObj->m_pAIOut = player;

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
HRESULT CPlayerAIOutControl::Init()
{
	// 値の初期化


	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAIOutControl::Uninit()
{
	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIOutControl::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モード管理
	ModeManager();

	// 状態更新
	(this->*(m_ModeFunc[m_eMode]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// モード管理
//==========================================================================
void CPlayerAIOutControl::ModeManager()
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() == nullptr ||
		pBall->GetTypeTeam() != m_pAIOut->GetTeam())
	{// ボールが取得されていない場合||自分とボールを持っているチームが違う場合
		m_eMode = EMode::MODE_CATCH;
	}
	else if (pBall->GetPlayer() == m_pAIOut)
	{// ボールを持っているのが自分だった場合
		m_eMode = EMode::MODE_THROW;
	}
}

//==========================================================================
// 投げ管理
//==========================================================================
void CPlayerAIOutControl::ModeThrowManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_eMode == EMode::MODE_THROW)
	{// 投げ状態の場合(ボールを持っている)

		int n = rand() % 2;
		//int n = 0;

		if (n == 0)
		{
			// パスする
			Pass();
		}
		else
		{
			// 投げる
			Throw();
		}
	}
}

//==========================================================================
// キャッチ管理
//==========================================================================
void CPlayerAIOutControl::ModeCatchManager(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) return;

	if (pBall->GetPlayer() != nullptr)
	{// 誰かがボールを持っている場合
		
	}
	else if (pBall->GetPlayer() == nullptr)
	{// 誰もボールを持っていない場合

		// ボールを取りに行く
		m_eCatch = ECatch::CATCH_FIND;
	}

	// 投げる種類更新
	(this->*(m_CatchFunc[m_eCatch]))(fDeltaTime, fDeltaRate, fSlowRate);
}


//==========================================================================
// パス処理
//==========================================================================
void CPlayerAIOutControl::Pass()
{
	// AIコントロール情報(外野)の取得
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();
	CPlayerControlMove* pControlMove = m_pAIOut->GetBase()->GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// ボール情報の取得
	CBall* pBall = m_pAIOut->GetBall();
	if (!pBall) return;

	// ボールをパス
	pControlAIOutAction->SetPattern(m_pAIOut, CPlayer::EMotion::MOTION_THROW_PASS, CPlayer::EAction::ACTION_THROW);
}

//==========================================================================
// ボールを投げる
//==========================================================================
void CPlayerAIOutControl::Throw()
{
	// AIコントロール情報(外野)の取得
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();
	CPlayerControlMove* pControlMove = m_pAIOut->GetBase()->GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// ターゲット設定
	GetThrowTarget();

	// 投げる
	pControlAIOutAction->SetIsThrow(true);
}


//==========================================================================
// パス行動処理
//==========================================================================
bool CPlayerAIOutControl::IsPass()
{
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (!pBall) return false;

	// パス状態&&ターゲットが自分の場合
	if (pBall->IsPass() && pBall->GetTarget() == m_pAIOut) return false;

	return true;
}

//==========================================================================
// ボールを見つける
//==========================================================================
void CPlayerAIOutControl::FindBall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// AIコントロール情報(外野)の取得
	CPlayerControlAction* pControlAction = m_pAIOut->GetBase()->GetPlayerControlAction();
	CPlayerAIOutControlAction* pControlAIOutAction = pControlAction->GetAIOut();
	CPlayerControlMove* pControlMove = m_pAIOut->GetBase()->GetPlayerControlMove();
	CPlayerAIOutControlMove* pControlAIOutMove = pControlMove->GetAIOut();

	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();

	if (pBall == nullptr || pBall->GetPlayer() != nullptr)
	{// ボールがnullptr&&プレイヤーがボールを取っている場合
		
		return;
	}

	// 角度を求める(playerからみたボール)
	float fAngle = m_pAIOut->GetPosition().AngleXZ(pBall->GetPosition());

	// 歩きオン!
	pControlAIOutMove->SetIsWalk(true);

	// 方向設定
	m_pAIOut->SetRotDest(fAngle);
}

//==========================================================================
// ターゲット設定(投げる時)
//==========================================================================
CPlayer* CPlayerAIOutControl::GetThrowTarget()
{
	CPlayer* pTarget = nullptr;	// 目標ターゲット
	float fMinDis = 1000000.0f;	// 近いプレイヤー

	MyLib::Vector3 Mypos = m_pAIOut->GetPosition();	// 位置情報の取得

	// ボールの取得
	CBall* pBall = m_pAIOut->GetBall();
	if (pBall == nullptr) { return pTarget; }
	CGameManager::ETeamSide typeTeam = pBall->GetTypeTeam();	// チームタイプの取得

	CListManager<CPlayer> list = CPlayer::GetList();	// プレイヤーリスト
	std::list<CPlayer*>::iterator itr = list.GetEnd();	// 最後尾イテレーター
	while (list.ListLoop(itr))
	{ // リスト内の要素数分繰り返す

		CPlayer* pPlayer = (*itr);	// プレイヤー情報
		MyLib::Vector3 posPlayer = pPlayer->GetCenterPosition();	// プレイヤー位置

		// 味方チーム||外野の場合
		if ((typeTeam == pPlayer->GetTeam()) ||
			(pPlayer->GetAreaType() == CPlayer::EFieldArea::FIELD_OUT))
		{
			continue;
		}

		// 味方との距離を求める
		float fLength = Mypos.DistanceXZ(posPlayer);

		if (fLength < fMinDis)
		{ // より近い味方がいた場合

			// 最小距離の更新
			fMinDis = fLength;

			// ターゲットを更新
			pTarget = pPlayer;

			// 方向設定(そっちを向く)
			m_pAIOut->SetRotDest(Mypos.AngleXZ(pTarget->GetPosition()));
		}
	}

	return pTarget;
}

