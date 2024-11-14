//==========================================================================
// 
//  AIプレイヤー処理 [playerAIIn.cpp]
//  Author : 藤田勇一
//  Adder  : 小笠原彪
// 
//==========================================================================
#include "playerAIIn.h"

// 使用クラス
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIIn::CPlayerAIIn(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerAI(pPlayer, typeTeam, typeArea)
{
	// 内野操作の割当
	ChangeMoveControl(DEBUG_NEW CPlayerAIControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIControlAction());

	// プレイヤーAIコントロールの生成
	m_pAIControl = CPlayerAIControl::Create(pPlayer);
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAIIn::~CPlayerAIIn()
{
	delete m_pAIControl;
	m_pAIControl = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIIn::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 基底クラスの更新
	CPlayerAI::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// コントロール更新
	if (m_pAIControl)
	{
		m_pAIControl->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// ヒット
//==========================================================================
CPlayer::SHitInfo CPlayerAIIn::Hit(CBall* pBall)
{
	// 基底クラスのヒット
	CPlayer::SHitInfo hitInfo = CPlayerAI::Hit(pBall);	// ヒット情報

	if (hitInfo.eHit == CPlayer::EHit::HIT_NONE) { // 通常状態
		return hitInfo;
	}

	if (hitInfo.eHit == CPlayer::EHit::HIT_CATCH) { // キャッチ状態
		// 投げモード
		m_pAIControl->SetMode(CPlayerAIControl::EMode::MODE_THROW);
	}

	return hitInfo;
}

//==========================================================================
// デバッグ
//==========================================================================
void CPlayerAIIn::Debug()
{
#if _DEBUG
	// 基底クラスのデバッグ表示
	CPlayerAI::Debug();

	CPlayerAIControlAction* pControlAction = dynamic_cast<CPlayerAIControlAction*>(GetPlayerControlAction());
	if (pControlAction == nullptr) return;

	// 自動投げフラグ設定
	bool autoThrow = pControlAction->IsAutoThrow();
	if (ImGui::Checkbox("Change Auto Throw", &autoThrow))
	{
		pControlAction->SetEnableAutoThrow(autoThrow);
	}
#endif
}
