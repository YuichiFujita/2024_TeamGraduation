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


// 使用クラス
#include "playerStatus.h"
#include "playerAIcontrol_move.h"
#include "playerAIcontrol_action.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayerAI::STATE_FUNC CPlayerAI::m_StateFunc[] =	// 状態関数
{
	&CPlayerAI::StateNone,				// なし
	//&CPlayerAI::StateThrowManager,		// 投げマネージャー
	//&CPlayerAI::StateCatchManager,		// キャッチマネージャー
};

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerAI::CPlayerAI()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerAI::~CPlayerAI()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CPlayerAI *CPlayerAI::Create(const CGameManager::TeamSide team, const MyLib::Vector3& rPos)
{
	// メモリの確保
	CPlayerAI* pPlayer = DEBUG_NEW CPlayerAI;
	if (pPlayer != nullptr)
	{
		// クラスの初期化
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pPlayer);
			return nullptr;
		}

		// チームサイドを設定
		pPlayer->GetStatus()->SetTeam(team);

		// 位置を設定
		pPlayer->SetPosition(rPos);
	}

	return pPlayer;
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
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);
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
// ヒット処理
//==========================================================================
CPlayerAI::SHitInfo CPlayerAI::Hit(CBall* pBall)
{
	CPlayer::SHitInfo hitInfo = CPlayer::Hit(pBall);	// ヒット情報

	if (hitInfo.eHit == EHit::HIT_NONE){
		return hitInfo;
	}

	if (hitInfo.eHit == EHit::HIT_CATCH)
	{// キャッチ状態
		StateThrowManager(pBall);
	}

	return hitInfo;
}

//==========================================================================
// 投げの管理
//==========================================================================
void CPlayerAI::StateThrowManager(CBall* pBall)
{
	CGameManager* pGameManager = CGame::GetInstance()->GetGameManager();
	CTeamStatus* pTeamStatus = pGameManager->GetTeamStatus(this->GetCharStatus()->GetTeam());

	// ボール所持か
	if (pBall == nullptr) return;

	// TODO: フラグか何かで発動操作
	if (false)
	{
		if (pTeamStatus->IsMaxSpecial())
		{
			SpecialThrow();
		}
		else
		{
			Throw();
		}
	}
}

//==========================================================================
// キャッチの管理
//==========================================================================
void CPlayerAI::StateCatchManager()
{

}

//==========================================================================
// 通常投げ処理
//==========================================================================
void CPlayerAI::Throw()
{
	CPlayerControlAction* pControlAction = GetPlayerControlAction();
	CPlayerAIControlAction* pControlAIAction =pControlAction->GetAI();

	pControlAIAction->SetIsThrow(true);
}

//==========================================================================
// ジャンプ投げ処理
//==========================================================================
void CPlayerAI::JumpThrow()
{

}

//==========================================================================
// スペシャル投げ処理
//==========================================================================
void CPlayerAI::SpecialThrow()
{
	
}

//==========================================================================
// 状態更新
//==========================================================================
void CPlayerAI::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 状態更新
	(this->*(m_StateFunc[m_throwMode]))();
}

//==========================================================================
// なし
//==========================================================================
void CPlayerAI::StateNone()
{

}

//==========================================================================
// 操作処理
//==========================================================================
void CPlayerAI::Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CPlayerControlMove* pControlMove = GetPlayerControlMove();
	CPlayerControlAction* pControlAction = GetPlayerControlAction();

	// 移動操作	// TODO：AIむじ～
	//m_pControlMove->Move(this, fDeltaTime, fDeltaRate, fSlowRate);
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