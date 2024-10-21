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

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int POINT_WAVECLEAR = 5;		// ウェーブクリアのポイント
	const float POSR_Y_APPROACH_COEF = 0.3f;	//カメラが近づく際の慣性
	const float POSR_Y_PULL_SCREEN_POS = 210.0f;	// カメラが引き始めるスクリーン座標
	const float POSR_Y_APPROACH_SCREEN_POS = SCREEN_HEIGHT * 0.5f;	// カメラが近づき始めるスクリーン座標
	const float POSR_YDEST_BAGGTOPLAYER_RATIO = 0.4f;	// 荷物とプレイヤー距離の割合（posRYDest）
	const int GUIDE_NUM = 100;

	const int CHANGE_BASEPOINT[] =	// ポイント変更する基準
	{
		40,	// AAA
		20,	// BBB
		10,	// CCC
		1,	// DDD
	};
	const float DEFAULT_INTERVAL_AIRSPAWN = 2.3f;	// デフォの空気生成間隔
	const float DEFAULT_INTERVAL_FLOWLEAFSPAWN = 0.8f;	// デフォの流れる葉生成間隔
	const float DEFAULT_INTERVAL_LEAFSPAWN = 2.0f;		// デフォの降る葉生成間隔
}

namespace SceneTime
{
	const float RequestStart = 3.5f;	// 依頼開始
}

namespace Court
{//ドッジボールコート情報

	const D3DXVECTOR3 SIZE = D3DXVECTOR3(600.0f, 0.0f, 800.0f);		// サイズ
}

//==========================================================================
// コンストラクタ
//==========================================================================
CGameManager::CGameManager()
{
	// 値のクリア
	m_SceneType = SCENE_MAIN;	// シーンの種類
	m_OldSceneType = SCENE_MAIN;	// シーンの種類
	m_bControll = false;		// 操作できるか
	m_fSceneTimer = 0.0f;		// シーンタイマー

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
	m_bControll = true;			// 操作できるか

#if _DEBUG
	m_SceneType = SceneType::SCENE_MAIN;	// シーンの種類 
#else
	m_SceneType = SceneType::SCENE_START;	// シーンの種類 
#endif

	m_OldSceneType = m_SceneType;

	m_courtSize = Court::SIZE;

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CGameManager::Uninit()
{
	
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
