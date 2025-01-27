//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playerAIcontrol_rightAttack.h"
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
CPlayerAIControlRightAttack::CPlayerAIControlRightAttack()
{
	
}

#if 1
//==========================================================================
// 生成
//==========================================================================
CPlayerAIControlRightAttack* CPlayerAIControlRightAttack::Create()
{
	// メモリの確保
	CPlayerAIControlRightAttack* pObj = DEBUG_NEW CPlayerAIControlRightAttack;
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
HRESULT CPlayerAIControlRightAttack::Init()
{
	CPlayerAIControlAttack::Init();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerAIControlRightAttack::Uninit()
{
	CPlayerAIControlAttack::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAIControlRightAttack::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerAIControlAttack::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 攻撃準備：離れる
//==========================================================================
void CPlayerAIControlRightAttack::PreparationLeave()
{
	// 歩く
	SetMoveFlag(CPlayerAIControlMode::EMoveFlag::MOVEFLAG_WALK);

	// 近づく
	if (Approatch({ 500.0f, GetPlayer()->GetPosition().y, GetPlayer()->GetPosition().z }, 50.0f))
	{
		// 待機
		SetMoveFlag(CPlayerAIControlMode::EMoveFlag::MOVEFLAG_IDLE);

		// 攻撃モード：攻撃
		SetAttackMode(EATTACKMODE::ATTACKMODE_ATTACK);
	}
}

void CPlayerAIControlRightAttack::Preparation()
{
}
