//=============================================================================
// 
// ゲームマネージャ処理 [gamemanager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "gamemanager.h"
#include "game.h"
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "camera.h"
#include "sound.h"
#include "blackframe.h"
#include "map.h"
#include "calculation.h"
#include "fog.h"
#include "fade.h"
#include "input.h"
#include "ball.h"
#include "collisionLine_Box.h"
#include "teamStatus.h"
#include "audience.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

namespace Court
{//ドッジボールコート情報

	const D3DXVECTOR3 SIZE = D3DXVECTOR3(600.0f, 100.0f, 800.0f);		// サイズ
}

//==========================================================================
// コンストラクタ
//==========================================================================
CGameManager::CGameManager()
{
	// 値のクリア
	m_SceneType = SCENE_MAIN;	// シーンの種類
	m_OldSceneType = SCENE_MAIN;	// シーンの種類(過去)
	m_bControll = false;		// 操作できるか
	m_fSceneTimer = 0.0f;		// シーンタイマー

	memset(&m_pTeamStatus[0], 0, sizeof(m_pTeamStatus));	// チームステータス

	m_courtSize = MyLib::Vector3();
}

//==========================================================================
// デストラクタ
//==========================================================================
CGameManager::~CGameManager()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CGameManager* CGameManager::Create(CScene::MODE mode)
{
	// メモリ確保
	CGameManager* pManager = nullptr;

	// インスタンス生成
	switch (mode)
	{
	case CScene::MODE_GAME:
		pManager = DEBUG_NEW CGameManager;
		break;

	}

	if (pManager != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		HRESULT hr = pManager->Init();

		if (FAILED(hr))
		{// 失敗していたら
			return nullptr;
		}
	}

	return pManager;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CGameManager::Init()
{
	m_bControll = true;	// 操作できるか

	// コートサイズ
	m_courtSize = Court::SIZE;

#if _DEBUG
	m_SceneType = ESceneType::SCENE_MAIN;	// シーンの種類 

	// コートサイズのボックス
	if (m_pCourtSizeBox == nullptr)
	{
		m_pCourtSizeBox = CCollisionLine_Box::Create(MyLib::AABB(-m_courtSize, m_courtSize), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	//チームステータス
	CreateTeamStatus();

#else
	m_SceneType = ESceneType::SCENE_START;	// シーンの種類 
#endif

	m_OldSceneType = m_SceneType;

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CGameManager::Uninit()
{
	// チームステータス
	for (int i = 0; i < TeamType::TYPE_MAX; i++)
	{
		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->Uninit();
			m_pTeamStatus[i] = nullptr;
		}
	}

#if _DEBUG
	// 終了
	if (m_pCourtSizeBox != nullptr)
	{
		m_pCourtSizeBox = nullptr;
	}
#endif
}

//==========================================================================
// 更新処理
//==========================================================================
void CGameManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// TODO : UpdateScene的なのでまとめたい
	m_fSceneTimer += fDeltaTime * fSlowRate;	// シーンタイマー
	
	// TODO : 関数リストにしたい
	switch (m_SceneType)
	{
	case CGameManager::ESceneType::SCENE_MAIN:
		m_bControll = true;
		UpdateAudience();
		break;

	case CGameManager::ESceneType::SCENE_START:
		m_bControll = false;
		SceneStart();
		break;

	case ESceneType::SCENE_BEFOREBATTLE:
		m_bControll = false;
		break;

	case ESceneType::SCENE_BATTLESTART:
		m_bControll = false;
		break;

	case ESceneType::SCENE_DEBUG:
		m_bControll = true;
		break;

	default:
		break;
	}

#if _DEBUG	// デバッグ処理

	if (ImGui::TreeNode("GameManager"))
	{
		Debug();
		ImGui::TreePop();
	}
#endif
}

//==========================================================================
// スタート時の設定
//==========================================================================
void CGameManager::StartSetting()
{
	
}


//==========================================================================
// 開始演出
//==========================================================================
void CGameManager::SceneStart()
{
	
}

//==========================================================================
// 観客更新
//==========================================================================
void CGameManager::UpdateAudience()
{
	for (int i = 0; i < 2; i++)
	{
		CTeamStatus::SCharmInfo info = m_pTeamStatus[i]->GetCharmInfo();	// モテ情報
		float fMoteRate = info.fValue / info.fValueMax;				// モテ割合
		int nNumAudience = (int)(CAudience::MAX_WATCH * fMoteRate);	// 現在の観客数

		// 観客数を設定
		CAudience::SetNumWatch(nNumAudience, (CGameManager::TeamSide)(i + 1));

		GET_MANAGER->GetDebugProc()->Print("チーム%d：観客 %d\n", i, nNumAudience);
	}
}

//==========================================================================
// ボールの取得
//==========================================================================
CBall* CGameManager::GetBall()
{
	CListManager<CBall> list = CBall::GetListObj();	// ボールリスト
	assert(list.GetNumAll() == 1);	// 複数ボールがある場合エラー
	return *list.GetBegin();		// ボール情報を返す
}

//==========================================================================
// シーンの種類設定
//==========================================================================
void CGameManager::SetType(ESceneType type)
{
	m_OldSceneType = m_SceneType;
	m_SceneType = type;
}

//==========================================================================
// コート移動制限
//==========================================================================
void CGameManager::SetPosLimit(MyLib::Vector3& pos)
{
	if (pos.x > m_courtSize.x)
	{
		pos.x = m_courtSize.x;
	}
	else if (pos.x < -m_courtSize.x)
	{
		pos.x = -m_courtSize.x;
	}

	if (pos.z > m_courtSize.z)
	{
		pos.z = m_courtSize.z;
	}
	else if (pos.z < -m_courtSize.z)
	{
		pos.z = -m_courtSize.z;
	}
}

//==========================================================================
// チームステータス生成
//==========================================================================
void CGameManager::CreateTeamStatus()
{
	CGameManager::TeamSide side = CGameManager::SIDE_NONE;

	for (int i = 0; i < TYPE_MAX; i++)
	{
		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->Uninit();
			m_pTeamStatus[i] = nullptr;
		}

		m_pTeamStatus[i] = CTeamStatus::Create();

		switch (static_cast<CGameManager::TeamType>(i))
		{
		case TYPE_LEFT:
			side = CGameManager::SIDE_LEFT;
			break;

		case TYPE_RIGHT:
			side = CGameManager::SIDE_RIGHT;
			break;

		default:
			break;
		}
		
		m_pTeamStatus[i]->TeamSetting(side);

	}
}

//==========================================================================
// デバッグ
//==========================================================================
void CGameManager::Debug()
{

	//-----------------------------
	// コート
	//-----------------------------
	if (ImGui::TreeNode("Court"))
	{
		if (m_pCourtSizeBox == nullptr)
		{
			m_pCourtSizeBox = CCollisionLine_Box::Create(MyLib::AABB(-m_courtSize, m_courtSize), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}

		if (ImGui::Button("Reset"))
		{// リセット
			m_courtSize = Court::SIZE;
		}
		ImGui::SameLine();

		ImGui::DragFloat3("size", (float*)&m_courtSize, 10.0f, 0.0f, 10000.0f, "%.2f");

		// サイズ設定
		if (m_pCourtSizeBox != nullptr)
		{
			m_pCourtSizeBox->SetAABB(MyLib::AABB(-m_courtSize, m_courtSize));
		}

		// 位置設定
		ImGui::TreePop();
	}

	// チームステータス
	for (int i = 0; i < TeamType::TYPE_MAX; i++)
	{
		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->Debug();
		}
	}
}
