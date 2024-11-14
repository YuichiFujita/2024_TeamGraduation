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
#include "gymWallManager.h"
#include "timerUI.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	//ドッジボールコート情報
	namespace Court
	{
		const MyLib::Vector3 SIZE = MyLib::Vector3(950.0f, 100.0f, 560.0f);	// サイズ
	}
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CGameManager::SCENE_FUNC CGameManager::m_SceneFunc[] =	// シーン関数
{
	&CGameManager::SceneMain,			// メイン
	&CGameManager::SceneStart,			// 開始演出
	&CGameManager::SceneSpecial_Stag,	// スペシャル演出
	&CGameManager::SceneEnd,			// 終了演出
	&CGameManager::SceneDebug,			// デバッグ
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CGameManager* CGameManager::m_pThisPtr = nullptr;	// 自身のポインタ

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
	m_courtSize = MyLib::Vector3();

	m_pGymWallManager = nullptr;	// ジム壁マネジャー
	m_pCharmManager = nullptr;		// モテマネージャ
	m_pTimerUI = nullptr;			// タイマーUI

	memset(&m_pTeamStatus[0], 0, sizeof(m_pTeamStatus));	// チームステータス

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
	if (m_pThisPtr != nullptr)
	{// 既にある場合
		return m_pThisPtr;
	}

	// インスタンス生成
	switch (mode)
	{
	case CScene::MODE_GAME:
		m_pThisPtr = DEBUG_NEW CGameManager;
		break;
	}

	if (m_pThisPtr != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		HRESULT hr = m_pThisPtr->Init();

		if (FAILED(hr))
		{// 失敗していたら
			return nullptr;
		}
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CGameManager::Init()
{
	m_bControll = true;	// 操作できるか

	// コートサイズ
	m_courtSize = Court::SIZE;

	// ジム壁マネージャ生成
	m_pGymWallManager = CGymWallManager::Create();

#if _DEBUG

	// コートサイズのボックス
	if (m_pCourtSizeBox == nullptr)
	{
		m_pCourtSizeBox = CCollisionLine_Box::Create(MyLib::AABB(-m_courtSize, m_courtSize), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
#endif

	SetSceneType(ESceneType::SCENE_START);	// シーンの種類

	//チームステータス
	CreateTeamStatus();

	// モテマネージャ生成
	m_pCharmManager = CCharmManager::Create();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CGameManager::Uninit()
{
	// チームステータス
	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
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

	// モテマネージャ
	if (m_pCharmManager != nullptr)
	{
		m_pCharmManager->Uninit();
		m_pCharmManager = nullptr;
	}

	// 自身の開放
	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CGameManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// TODO : UpdateScene的なのでまとめたい
	m_fSceneTimer += fDeltaTime * fSlowRate;	// シーンタイマー
	
	// シーン別更新
	(this->*(m_SceneFunc[m_SceneType]))();

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
// メイン
//==========================================================================
void CGameManager::SceneMain()
{
	// 操作出来る
	m_bControll = true;

	// 観客更新
	UpdateAudience();

	// 制限時間更新
	UpdateLimitTimer();
}

//==========================================================================
// 制限時間更新
//==========================================================================
void CGameManager::UpdateLimitTimer()
{
	// タイマー無いと抜ける
	if (m_pTimerUI == nullptr) return;

	// 終了したら
	if (m_pTimerUI->IsEnd())
	{
		SetSceneType(ESceneType::SCENE_END);
	}
}

//==========================================================================
// 開始演出
//==========================================================================
void CGameManager::SceneStart()
{
	// 操作出来ない
	m_bControll = false;

#if _DEBUG
	// コートサイズのボックス
	if (m_pCourtSizeBox == nullptr)
	{
		m_pCourtSizeBox = CCollisionLine_Box::Create(MyLib::AABB(-m_courtSize, m_courtSize), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
#endif

	//チームステータス
	CreateTeamStatus();

	// タイマーUI
	if (m_pTimerUI == nullptr)
	{
		m_pTimerUI = CTimerUI::Create(
			90.0f,
			90.0f,
			MyLib::Vector3(640.0f, 70.0f, 0.0f),
			D3DXVECTOR2(40.0f, 40.0f),
			D3DXVECTOR2(30.0f, 40.0f),
			D3DXVECTOR2(40.0f, 0.0f),
			D3DXVECTOR2(60.0f, 0.0f),
			CTimerUI::EAlignX::XALIGN_CENTER,
			CTimerUI::EAlignY::YALIGN_CENTER,
			MyLib::Vector3(0.0f)
		);

		// 開始
		m_pTimerUI->Start();
	}

	// メインへ遷移
	SetSceneType(ESceneType::SCENE_MAIN);
}

//==========================================================================
// スペシャル演出
//==========================================================================
void CGameManager::SceneSpecial_Stag()
{
	// 操作出来ない
	m_bControll = false;

	// スペシャル演出更新
	UpdateSpecialStag();
}

//==========================================================================
// 終了演出
//==========================================================================
void CGameManager::SceneEnd()
{
	// 操作出来ない
	m_bControll = false;

	// 遷移
	GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_RESULT);
}

//==========================================================================
// デバッグ
//==========================================================================
void CGameManager::SceneDebug()
{
	// 操作出来る
	m_bControll = true;
}

//==========================================================================
// 観客更新
//==========================================================================
void CGameManager::UpdateAudience()
{
	GET_MANAGER->GetDebugProc()->Print("\n----------------- 観客情報 -----------------\n");
	for (int i = 0; i < CGameManager::ETeamSide::SIDE_MAX; i++)
	{
		CTeamStatus::SCharmInfo info = m_pTeamStatus[i]->GetCharmInfo();	// モテ情報
		float fMoteRate = info.fValue / info.fValueMax;				// モテ割合
		int nNumAudience = (int)(CAudience::MAX_WATCH * fMoteRate);	// 現在の観客数

		// 観客数を設定
		CAudience::SetNumWatch(nNumAudience, (CGameManager::ETeamSide)(i));
		GET_MANAGER->GetDebugProc()->Print("【チーム0%d観客】[%d]\n", i, nNumAudience);
	}
}

//==========================================================================
// スペシャル演出更新
//==========================================================================
void CGameManager::UpdateSpecialStag()
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
// シーンの種類設定
//==========================================================================
void CGameManager::SetSceneType(ESceneType type)
{
	m_OldSceneType = m_SceneType;
	m_SceneType = type;
	m_fSceneTimer = 0.0f;
}

/**
@brief	コートサイズ取得(チーム)
@param	team[in]	取得したいチーム
@param	pos[out]	取得したコートの中心点
@return	コートサイズ
*/
MyLib::Vector3 CGameManager::GetCourtSize(const ETeamSide team, MyLib::Vector3& pos)
{
	// 片側コートサイズ
	MyLib::Vector3 size = m_courtSize;
	size.x *= 0.5f;

	// チームに応じた位置
	pos = MyLib::Vector3();
	switch (team)
	{
	case SIDE_LEFT:
		pos.x = -size.x;
		break;

	case SIDE_RIGHT:
		pos.x = size.x;
		break;

	default:
		break;
	}

	return size;
}

//==========================================================================
// コート移動制限
//==========================================================================
bool CGameManager::SetPosLimit(MyLib::Vector3& pos)
{
	bool bHit = false;

	if (pos.x > m_courtSize.x)
	{
		pos.x = m_courtSize.x;
		bHit = true;
	}
	else if (pos.x < -m_courtSize.x)
	{
		pos.x = -m_courtSize.x;
		bHit = true;
	}

	if (pos.z > m_courtSize.z)
	{
		pos.z = m_courtSize.z;
		bHit = true;
	}
	else if (pos.z < -m_courtSize.z)
	{
		pos.z = -m_courtSize.z;
		bHit = true;
	}

	return bHit;
}

//==========================================================================
// モテ加算
//==========================================================================
void CGameManager::AddCharmValue(ETeamSide side, CCharmManager::ETypeAdd charmType)
{
	// チームステータス
	float value = CCharmManager::GetInstance()->GetAddValue(charmType);
	m_pTeamStatus[side]->AddCharmValue(value);
}

//==========================================================================
// モテ減算
//==========================================================================
void CGameManager::SubCharmValue(ETeamSide side, CCharmManager::ETypeSub charmType)
{
	// チームステータス
	float value = CCharmManager::GetInstance()->GetSubValue(charmType);
	m_pTeamStatus[side]->SubCharmValue(value);
}

//==========================================================================
// チームステータス生成
//==========================================================================
void CGameManager::CreateTeamStatus()
{
	ETeamSide side = CGameManager::SIDE_NONE;

	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
	{
		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->Uninit();
			m_pTeamStatus[i] = nullptr;
		}

		m_pTeamStatus[i] = CTeamStatus::Create();

		// チーム設定
		side = static_cast<ETeamSide>(i);
		m_pTeamStatus[i]->TeamSetting(side);

	}
}

//==========================================================================
// デバッグ
//==========================================================================
void CGameManager::Debug()
{
#if _DEBUG

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
	for (int i = 0; i < ETeamSide::SIDE_MAX; i++)
	{
		//-----------------------------
		// コート
		//-----------------------------
		std::string treename = "Special" + std::to_string(i);	// ツリー名
		if (ImGui::TreeNode(treename.c_str()))
		{
			float fValue = m_pTeamStatus[i]->GetSpecialValue();

			ImGui::DragFloat("GaugeValue", (float*)&fValue, 1.0f, 0.0f, m_pTeamStatus[i]->GetSpecialInfo().fValueMax, "%.2f");

			// 位置設定
			ImGui::TreePop();

			m_pTeamStatus[i]->SetSpecialValue(fValue);
		}

		if (m_pTeamStatus[i] != nullptr)
		{
			m_pTeamStatus[i]->Debug();
		}
	}

#endif
}
