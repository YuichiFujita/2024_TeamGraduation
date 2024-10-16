//=============================================================================
// 
//  AIプレイヤー処理 [playerAI.cpp]
//  Author : 藤田勇一
// 
//=============================================================================
#include "playerAI.h"
#include "manager.h"
#include "calculation.h"

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
// コンストラクタ
//==========================================================================
CPlayerAI::CPlayerAI()
{
	// パターン用インスタンス
	m_pControlMove = nullptr;	// 移動操作
	m_pControlAction = nullptr;	// アクション操作
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
CPlayerAI *CPlayerAI::Create(const int nIdx, const CGameManager::TeamSide team)
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

		// プレイヤーインデックスを設定
		pPlayer->SetMyPlayerIdx(nIdx);

		// チームサイドを設定
		pPlayer->GetStatus()->SetTeam(team);
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
	CPlayer::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerAI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親クラスの更新
	CPlayer::Update(fDeltaTime, fDeltaRate, fSlowRate);
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
// 移動処理
//==========================================================================
void CPlayerAI::Move(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 移動操作
	//m_pControlMove->Move(this, fDeltaTime, fDeltaRate, fSlowRate);
	//m_pControlAction->Action(this, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayerAI::ChangeMoveControl(CPlayerAIControlMove* control)
{
	delete m_pControlMove;
	m_pControlMove = control;
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayerAI::ChangeActionControl(CPlayerAIControlAction* control)
{
	delete m_pControlAction;
	m_pControlAction = control;
}

//==========================================================================
// 操作関連削除
//==========================================================================
void CPlayerAI::DeleteControl()
{
	if (m_pControlMove != nullptr)
	{// 移動操作
		delete m_pControlMove;
		m_pControlMove = nullptr;
	}

	if (m_pControlAction != nullptr)
	{// アクション操作
		delete m_pControlAction;
		m_pControlAction = nullptr;
	}

	// 操作関連削除
	CPlayer::DeleteControl();
}
