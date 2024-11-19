//==========================================================================
// 
//  AIプレイヤー処理 [playerAIOut.cpp]
//  Author : 藤田勇一
//  Adder  : 小笠原彪
// 
//==========================================================================
#include "playerAIOut.h"


// 使用クラス
#include "playerAIOut_controlMove.h"
#include "playerAIOut_controlAction.h"
#include "playerAIOut_control.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIOut::CPlayerAIOut(CPlayer* pPlayer, const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea) : CPlayerOut(pPlayer, typeTeam, typeArea)
{
	// 外野操作の割当
	ChangeMoveControl(DEBUG_NEW CPlayerAIOutControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerAIOutControlAction());

	// AIコントロール(外野)の生成
	m_pAIOutControl = CPlayerAIOutControl::Create(pPlayer);
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAIOut::~CPlayerAIOut()
{

}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIOut::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 基底クラスの更新
	CPlayerOut::Update(fDeltaTime, fDeltaRate, fSlowRate);

	if (m_pAIOutControl)
	{// AIコントロール(外野)の更新
		m_pAIOutControl->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// ヒット
//==========================================================================
CPlayer::SHitInfo CPlayerAIOut::Hit(CBall* pBall)
{
	// 基底クラスのヒット
	CPlayer::SHitInfo hitInfo = CPlayerOut::Hit(pBall);	// ヒット情報

#if 1
	if (hitInfo.eHit == CPlayer::EHit::HIT_NONE) { // 通常状態
		return hitInfo;
	}

	if (hitInfo.eHit == CPlayer::EHit::HIT_CATCH) { // キャッチ状態
		// 投げモード
		m_pAIOutControl->SetMode(CPlayerAIOutControl::EMode::MODE_THROW);
	}
#endif

	return hitInfo;
}

//==========================================================================
// デバッグ
//==========================================================================
void CPlayerAIOut::Debug()
{
#if _DEBUG
	// 基底クラスのデバッグ表示
	CPlayerOut::Debug();

	CPlayerAIOutControlAction* pControlAction = dynamic_cast<CPlayerAIOutControlAction*>(GetPlayerControlAction());
	if (pControlAction == nullptr) return;

	// 自動投げフラグ設定
	bool autoThrow = pControlAction->IsAutoThrow();
	if (ImGui::Checkbox("Change Auto Throw", &autoThrow))
	{
		pControlAction->SetEnableAutoThrow(autoThrow);
	}
#endif
}
