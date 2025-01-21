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

#include "playerAIcontrol_idle.h"
#include "playerAIcontrol_attack.h"
#include "playerAIcontrol_defense.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControl::CPlayerAIControl()
{
	
}

//==========================================================================
// 生成
//==========================================================================
CPlayerAIControl* CPlayerAIControl::Create(CPlayer* player)
{
	if (!player) assert(("プレイヤー情報が無いよ", false));

	// メモリの確保
	CPlayerAIControl* pObj = nullptr;

	if (pObj == nullptr)
	{
		pObj = DEBUG_NEW CPlayerAIControl;

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
	// モード生成
	m_pAIControlMode = CPlayerAIControlMode::Create(m_pAI, CPlayerAIControlMode::EMode::MODE_IDLE);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAIControl::Uninit()
{
	if (!m_pAIControlMode) return;
	m_pAIControlMode->Uninit();
	m_pAIControlMode = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIControl::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// モード管理
	ModeManager();

	if (m_pAIControlMode)
	{
		m_pAIControlMode->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// モード管理
//==========================================================================
void CPlayerAIControl::ModeManager()
{
	if (!m_pAIControlMode) return;

	CPlayerAIControlMode::EMode modeCurrent, mode;
	modeCurrent = m_pAIControlMode->GetMode();
	mode = modeCurrent;

	// ボールの取得
	CBall* pBall = CGameManager::GetInstance()->GetBall();
	if (pBall == nullptr) {// ボールが世界に存在しない場合
		mode = CPlayerAIControlMode::EMode::MODE_IDLE;
		return;
	}

	if (pBall->GetPlayer() == m_pAI)
	{// ボールを持っているのが自分だった場合
		mode = CPlayerAIControlMode::EMode::MODE_ATTACK;	// 投げ
	}
	else if (pBall->GetPlayer() == nullptr || pBall->GetPlayer() != nullptr && pBall->GetTypeTeam() != m_pAI->GetTeam())
	{// ボールが取得されていない場合||自分とボールを持っているチームが違う場合
		mode = CPlayerAIControlMode::EMode::MODE_DEFENCE;	// キャッチ
	}
	else if (pBall->GetPlayer() != nullptr && pBall->GetTypeTeam() == m_pAI->GetTeam())
	{// ボールが取得されていない場合||自分とボールを持っているチームが同じ場合
		mode = CPlayerAIControlMode::EMode::MODE_IDLE;	// 待機
	}

	if (mode != modeCurrent)
	{// 現在のモードと違う場合
		// モード変更
		ChangeMode(m_pAI, mode);
	}
}

//==========================================================================
// モード切り替え
//==========================================================================
void CPlayerAIControl::ChangeMode(CPlayer* pPlayer, CPlayerAIControlMode::EMode mode)
{
	if (!m_pAIControlMode) return;

	m_pAIControlMode->Uninit();
	m_pAIControlMode = nullptr;

	if (!m_pAIControlMode)
	{
		m_pAIControlMode = CPlayerAIControlMode::Create(pPlayer, mode);
	}
}
