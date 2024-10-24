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
	m_SceneType = SceneType::SCENE_MAIN;	// シーンの種類 

	// コートサイズのボックス
	if (m_pCourtSizeBox == nullptr)
	{
		m_pCourtSizeBox = CCollisionLine_Box::Create(MyLib::AABB(-m_courtSize, m_courtSize), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	//チームステータス
	CreateTeamStatus();

#else
	m_SceneType = SceneType::SCENE_START;	// シーンの種類 
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
	// 操作状態
	m_fSceneTimer += fDeltaTime * fSlowRate;	// シーンタイマー
	switch (m_SceneType)
	{
	case CGameManager::SceneType::SCENE_MAIN:
		m_bControll = true;
		CheckJudgeZone();
		ContainPlayerBaggage();
		TurnAway();
		break;

	case CGameManager::SceneType::SCENE_START:
		m_bControll = false;
		SceneStart();
		break;

	case CGameManager::SceneType::SCENE_SKIP:
		m_bControll = false;
		SceneSkip();
		break;

	case CGameManager::SceneType::SCENE_COUNTDOWN:		// カウントダウン
		TurnAway();
		break;

	case CGameManager::SceneType::SCENE_MAINCLEAR:
		m_bControll = false;
		SceneGoal();
		break;

	case SceneType::SCENE_MAINRESULT:
		m_bControll = false;
		break;

	case SceneType::SCENE_DURING_MAINRESULT:
		m_bControll = false;
		break;

	case SceneType::SCENE_BEFOREBATTLE:
		m_bControll = false;
		break;

	case SceneType::SCENE_BATTLESTART:
		m_bControll = false;
		break;

	case SceneType::SCENE_SKILLTREE:	// スキルツリー
		m_bControll = false;
		break;

	case CGameManager::SceneType::SCENE_BOSS:
		m_bControll = true;
		break;

	case CGameManager::SceneType::SCENE_TRANSITIONWAIT:
		m_bControll = false;
		break;

	case CGameManager::SceneType::SCENE_TRANSITION:
		m_bControll = false;
		SceneTransition();
		break;

	case SceneType::SCENE_RESULT:
		m_bControll = false;
		break;

	case SceneType::SCENE_WAIT_AIRPUSH:
		m_bControll = false;
		SceneWaitAirPush();
		TurnAway();
		break;

	case SceneType::SCENE_GOAL:			// ゴール
		SceneGoal();
		break;

	case SceneType::SCENE_DEBUG:
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
// ゲームクリア時の設定
//==========================================================================
void CGameManager::GameClearSettings()
{

}

//==========================================================================
// スキップ
//==========================================================================
void CGameManager::SceneSkip()
{
	
}

//==========================================================================
// 開始演出
//==========================================================================
void CGameManager::SceneStart()
{
	
}

//==========================================================================
// ゲームリザルトの設定
//==========================================================================
void CGameManager::GameResultSettings()
{

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
// メイン遷移中
//==========================================================================
void CGameManager::SceneTransition()
{
	
}

//==========================================================================
// 空気送り待ち
//==========================================================================
void CGameManager::SceneWaitAirPush()
{
	
}

//==========================================================================
// ゴール状態
//==========================================================================
void CGameManager::SceneGoal()
{

}

//==========================================================================
// プレイヤーと荷物を画面内に収める
//==========================================================================
void CGameManager::ContainPlayerBaggage()
{
	
}

//==========================================================================
// カメラが常に横を向くようにする
//==========================================================================
void CGameManager::TurnAway()
{
	
}

//==========================================================================
// 判定ゾーン確認
//==========================================================================
void CGameManager::CheckJudgeZone()
{
	

}

//==========================================================================
// ボス設定
//==========================================================================
void CGameManager::SetBoss()
{

}

//==========================================================================
// 敵設定
//==========================================================================
void CGameManager::SetEnemy()
{

	

}

//==========================================================================
// シーンの種類設定
//==========================================================================
void CGameManager::SetType(SceneType type)
{
	m_OldSceneType = m_SceneType;
	m_SceneType = type;
}

//==========================================================================
// シーンの種類取得
//==========================================================================
CGameManager::SceneType CGameManager::GetType()
{
	return m_SceneType;
}

//==========================================================================
// コート移動制限
//==========================================================================
void CGameManager::PosLimit(MyLib::Vector3& pos)
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
