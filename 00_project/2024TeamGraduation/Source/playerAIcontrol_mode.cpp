//=============================================================================
// 
//  プレイヤーAIコントロール_移動処理 [playerAIcontrol_move.cpp]
//  Author : Takeru Ogasawara
// 
//=============================================================================
#include "playerAIcontrol_mode.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "playerAction.h"

// player
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol_start.h"
#include "playerAIcontrol_idle.h"
#include "playerAIcontrol_attack.h"
#include "playerAIcontrol_defense.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float JUMP_RATE_MAX = 1.0f;				// ジャンプの最大
	const float JUMP_RATE = 1.0f;					// ジャンプ割合
}

// フラグ
CPlayerAIControlMode::MOVEFLAG_FUNC CPlayerAIControlMode::m_MoveFlagFunc[] =	// 行動フラグ関数
{
	&CPlayerAIControlMode::MoveFlagStop,			// 止まる
	&CPlayerAIControlMode::MoveFlagWalk,			// 歩く
	&CPlayerAIControlMode::MoveFlagBlink,			// 歩く
	&CPlayerAIControlMode::MoveFlagDash,			// 走る
};

CPlayerAIControlMode::ACTIONFLAG_FUNC CPlayerAIControlMode::m_ActionFlagFunc[] =	// アクションフラグ関数
{
	&CPlayerAIControlMode::ActionFlagNone,			// なし
	&CPlayerAIControlMode::ActionFlagJump,			// ジャンプ
};


//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControlMode::CPlayerAIControlMode()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAIControlMode::~CPlayerAIControlMode()
{

}

//==========================================================================
// 生成
//==========================================================================
CPlayerAIControlMode* CPlayerAIControlMode::Create(CPlayer* player, EMode mode)
{
	if (!player) assert(("プレイヤー情報が無いよ", false));

	// メモリの確保
	CPlayerAIControlMode* pObj = nullptr;

	if (pObj == nullptr)
	{
		switch (mode)
		{
		case EMode::MODE_START:
			pObj = CPlayerAIControlStart::Create(player);
			break;

		case EMode::MODE_IDLE:
			pObj = CPlayerAIControlIdle::Create(player);
			break;

		case EMode::MODE_ATTACK:
			pObj = CPlayerAIControlAttack::Create(player);
			break;

		case EMode::MODE_DEFENCE:
			pObj = CPlayerAIControlDefense::Create(player);
			break;

		default:
			break;
		}

		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pObj);
			return nullptr;
		}

		// 情報の設定(初期化の後に置く)
		pObj->m_pAI = player;
		pObj->m_eMode = mode;
	}

	return pObj;
}

//================================================================================
// 初期化処理
//================================================================================
HRESULT CPlayerAIControlMode::Init()
{
	m_eMode = EMode::MODE_IDLE;
	m_eMoveFlag = EMoveFlag::MOVEFLAG_IDLE;
	m_eActionFlag = EActionFlag::ACTION_NONE;

	m_fMaxJumpRate = JUMP_RATE;

	m_sParameter.fDistanse = 300.0f;
	m_sParameter.fRadius = 30.0f;
	m_sParameter.fJumpEnd = 130.0f;

	return S_OK;
}

//================================================================================
// 終了処理
//================================================================================
void CPlayerAIControlMode::Uninit()
{
	if (this)
	{
		delete this;
	}
}

//================================================================================
// 更新処理
//================================================================================
void CPlayerAIControlMode::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// アクションフラグ更新
	UpdateActionFlag();

	// 行動フラグ更新
	UpdateMoveFlag();
}

//================================================================================
// 行動フラグの更新処理
//================================================================================
void CPlayerAIControlMode::UpdateMoveFlag()
{
	// 行動フラグ更新
	(this->*(m_MoveFlagFunc[m_eMoveFlag]))();
}

//--------------------------------------------------------------------------
// 止まる
//--------------------------------------------------------------------------
void CPlayerAIControlMode::MoveFlagStop()
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
void CPlayerAIControlMode::MoveFlagWalk()
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
void CPlayerAIControlMode::MoveFlagBlink()
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
void CPlayerAIControlMode::MoveFlagDash()
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
void CPlayerAIControlMode::UpdateActionFlag()
{
	// アクション更新
	(this->*(m_ActionFlagFunc[m_eActionFlag]))();
}

//--------------------------------------------------------------------------
// なにもしない
//--------------------------------------------------------------------------
void CPlayerAIControlMode::ActionFlagNone()
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
void CPlayerAIControlMode::ActionFlagJump()
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// ジャンプ：true
	pControlAIAction->SetIsJump(true);

	// 割合
	pControlAIAction->SetMaxJumpRate(m_fMaxJumpRate);

	// アクション列挙：なし
	m_eActionFlag = EActionFlag::ACTION_NONE;
}

//================================================================================
// ジャンプ割合設定
//================================================================================
void CPlayerAIControlMode::SetMaxjumpRate(float rate)
{
	if (rate < 0.0f)
	{
		rate = 0.0f;
	}
	if (rate > JUMP_RATE_MAX)
	{
		rate = JUMP_RATE_MAX;
	}

	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	m_fMaxJumpRate = rate;

	// ジャンプ：true
	pControlAIAction->SetMaxJumpRate(m_fMaxJumpRate);
}

//================================================================================
// ジャンプ割合の取得
//================================================================================
float CPlayerAIControlMode::GetJumpRate()
{
	// AIコントロール情報の取得
	CPlayerControlAction* pControlAction = m_pAI->GetBase()->GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction = pControlAction->GetAI();

	// 割合取得
	float rate = pControlAIAction->GetJumpRate();

	return rate;
}
