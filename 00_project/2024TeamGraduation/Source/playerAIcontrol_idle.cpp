//=============================================================================
// 
//  プレイヤーAIコントロール_移動処理 [playerAIcontrol_move.cpp]
//  Author : 藤田勇一
// 
//=============================================================================
#include "playerAIcontrol_idle.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "playerAction.h"

// player
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"
#include "playerAIcontrol_idle.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float JUMP_RATE = 1.0f;				// ジャンプの割合(高さ)
}



//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAIControlIdle::CPlayerAIControlIdle()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAIControlIdle::~CPlayerAIControlIdle()
{

}

//==========================================================================
// 生成
//==========================================================================
CPlayerAIControlIdle* CPlayerAIControlIdle::Create(CPlayer* player)
{
	if (!player) assert(("プレイヤー情報が無いよ", false));

	// メモリの確保
	CPlayerAIControlIdle* pObj = nullptr;

	if (pObj == nullptr)
	{
		pObj = DEBUG_NEW CPlayerAIControlIdle;

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

//================================================================================
// 初期化処理
//================================================================================
HRESULT CPlayerAIControlIdle::Init()
{
	// 親クラスの初期化
	CPlayerAIControlMode::Init();

	return S_OK;
}

//================================================================================
// 終了処理
//================================================================================
void CPlayerAIControlIdle::Uninit()
{
	CPlayerAIControlMode::Uninit();
}

//================================================================================
// 更新処理
//================================================================================
void CPlayerAIControlIdle::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親クラスの更新（最後尾に設置）
	CPlayerAIControlMode::Update(fDeltaTime, fDeltaRate, fSlowRate);
}