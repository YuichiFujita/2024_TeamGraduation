//=============================================================================
// 
//  ユーザープレイヤー処理 [playerUser.cpp]
//  Author : 藤田勇一
// 
//=============================================================================
#include "playerUser.h"
#include "manager.h"
#include "calculation.h"

// 使用クラス
#include "playerStatus.h"
#include "playercontrol_move.h"
#include "playercontrol_action.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayerUser::CPlayerUser()
{
	// パターン用インスタンス
	m_pControlMove = nullptr;	// 移動操作
	m_pControlAction = nullptr;	// アクション操作
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayerUser::~CPlayerUser()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CPlayerUser *CPlayerUser::Create(const CGameManager::TeamSide team, const MyLib::Vector3& rPos)
{
	// メモリの確保
	CPlayerUser* pPlayer = DEBUG_NEW CPlayerUser;
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
HRESULT CPlayerUser::Init()
{
	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 親クラスの初期化
	HRESULT hr = CPlayer::Init();
	if (FAILED(hr)) { return E_FAIL; }

	// 操作関連
	ChangeMoveControl(DEBUG_NEW CPlayerControlMove());
	ChangeActionControl(DEBUG_NEW CPlayerControlAction());

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayerUser::Uninit()
{
	// 親クラスの終了
	CPlayer::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CPlayerUser::Kill()
{
	// 親クラスの終了
	CPlayer::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayerUser::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 親クラスの更新
	CPlayer::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CPlayerUser::Draw()
{
	// 親クラスの描画
	CPlayer::Draw();
}

//==========================================================================
// 操作処理
//==========================================================================
void CPlayerUser::Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 移動操作
	m_pControlMove->Move(this, fDeltaTime, fDeltaRate, fSlowRate);
	m_pControlAction->Action(this, fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayerUser::ChangeMoveControl(CPlayerControlMove* control)
{
	delete m_pControlMove;
	m_pControlMove = control;
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayerUser::ChangeActionControl(CPlayerControlAction* control)
{
	delete m_pControlAction;
	m_pControlAction = control;
}

//==========================================================================
// 操作関連削除
//==========================================================================
void CPlayerUser::DeleteControl()
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
