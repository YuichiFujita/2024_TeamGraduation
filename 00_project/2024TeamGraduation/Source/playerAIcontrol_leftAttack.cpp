//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerAIcontrol_leftAttack.h"
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
#include "playerAIcontrol.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

};

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControlLeftAttack::CPlayerAIControlLeftAttack()
{

}

#if 1
//==========================================================================
// 生成
//==========================================================================
CPlayerAIControlLeftAttack* CPlayerAIControlLeftAttack::Create(CPlayer* player)
{
	// メモリの確保
	CPlayerAIControlLeftAttack* pObj = DEBUG_NEW CPlayerAIControlLeftAttack;
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
#endif

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayerAIControlLeftAttack::Init()
{
	CPlayerAIControlAttack::Init();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAIControlLeftAttack::Uninit()
{
	CPlayerAIControlAttack::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIControlLeftAttack::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerAIControlAttack::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 攻撃準備
//==========================================================================
void CPlayerAIControlLeftAttack::PreparationLeave()
{
	// 歩く
	SetMoveFlag(CPlayerAIControlMode::EMoveFlag::MOVEFLAG_WALK);

	// 近づく
	if (Approatch({ -500.0f, GetPlayer()->GetPosition().y, GetPlayer()->GetPosition().z }, 50.0f))
	{
		// 待機
		SetMoveFlag(CPlayerAIControlMode::EMoveFlag::MOVEFLAG_IDLE);

		// 攻撃モード：攻撃
		SetAttackMode(EATTACKMODE::ATTACKMODE_ATTACK);
	}
}