//=============================================================================
// 
//  タイトル処理 [title.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "sound.h"
#include "particle.h"
#include "MyEffekseer.h"
#include "titlelogo.h"
#include "title_pressenter.h"
#include "camera.h"
#include "waterfield.h"
#include "course.h"
#include "mapmesh.h"
#include "stonewall.h"
#include "peoplemanager.h"
#include "environment.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float TIME_FADELOGO = 0.6f;	// ロゴのフェードアウト時間
	const char* TEXTURE = "data\\TEXTURE\\title\\title.png";
}

namespace STARTCAMERA
{
	const MyLib::Vector3 POSV = (321.91f, 233.25f, -654.0f);
	const MyLib::Vector3 POSR = (321.91f, 160.74f, 296.27f);
	const MyLib::Vector3 ROT = (0.0f, 0.0f, -0.2f);
	const float LENGTH = 365.0f;
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CTitle* CTitle::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// 関数ポインタ
//==========================================================================
CTitle::SCENE_FUNC CTitle::m_SceneFunc[] =
{
	&CTitle::SceneNone,			// なにもなし
	&CTitle::SceneFadeInLogo,	// ロゴフェードイン
	&CTitle::SceneFadeOutLoGo,	// ロゴフェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CTitle::CTitle()
{
	// 値のクリア
	m_SceneType = SCENETYPE::SCENETYPE_NONE;	// シーンの種類
	m_fSceneTime = 0.0f;						// シーンカウンター
	m_pLogo = nullptr;		// ロゴのポインタ
	m_pPressEnter = nullptr;	// プレスエンター
	m_pConfigSetting = nullptr;
	m_pSpawn_Leaf = nullptr;		// 降る葉生成
	m_pPeopleManager = nullptr;	// 人マネージャ
}

//==========================================================================
// デストラクタ
//==========================================================================
CTitle::~CTitle()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTitle* CTitle::Create()
{
	if (m_pThisPtr == nullptr)
	{// まだ生成していなかったら

		// インスタンス生成
		m_pThisPtr = DEBUG_NEW CTitle;
	}

	return m_pThisPtr;
}

//==========================================================================
// インスタンス取得
//==========================================================================
CTitle* CTitle::GetInstance()
{
	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTitle::Init()
{

	// BGM再生
	CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_TITLE);

	// 初期化処理
	if (FAILED(CScene::Init()))
	{// 失敗した場合
		return E_FAIL;
	}

	// タイトルロゴ生成
	m_pLogo = CTitleLogo::Create(1.0f);

	// プレスエンター
	m_pPressEnter = CTitle_PressEnter::Create(1.0f);

	// シーンの種類
	m_SceneType = SCENETYPE::SCENETYPE_NONE;

	// カメラの初期値設定
	{
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		pCamera->SetStateCameraV(DEBUG_NEW CStateCameraV);
	}


	//=============================
	// コース
	//=============================
	CCourse::Create("", CScene::MODE::MODE_TITLE);

	//=============================
	// 固定平面街フィールド
	//=============================
	CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_FIXEDPLANE);

	//=============================
	// 石垣(奥)
	//=============================
	{
		CStoneWall* pStoneWall = CStoneWall::Create();

		// 基点地点設定
		std::vector<MyLib::Vector3> vecpos =
		{
			MyLib::Vector3(-1500.0f, 0.0f, 3000.0f),
			MyLib::Vector3(0.0f, 0.0f, 3000.0f),
			MyLib::Vector3(500.0f, 0.0f, 3000.0f),
			MyLib::Vector3(2500.0f, 0.0f, 3000.0f),
		};
		pStoneWall->SetVecPosition(vecpos);
		pStoneWall->Reset();

		// 各頂点座標
		std::vector<MyLib::Vector3> vecVtxpos =
		{
			MyLib::Vector3(20000.0f, 0.0f, 2000.0f),
			MyLib::Vector3(20010.0f, 0.0f, 2000.0f),
			MyLib::Vector3(90000.0f, 0.0f, 2000.0f),
			MyLib::Vector3(90010.0f, 0.0f, 2000.0f),
		};
		pStoneWall->SetVecVtxPosition(vecVtxpos);
		pStoneWall->BindVtxPosition();
	}

	//=============================
	// 水フィールド
	//=============================
	CWaterField::Create(CWaterField::TYPE::TYPE_RIGHT);
	CWaterField::Create(CWaterField::TYPE::TYPE_LEFT);

	//=============================
	// 人マネージャ
	//=============================
	m_pPeopleManager = CPeopleManager::Create(CPeopleManager::Type::TYPE_TITLE);


	// 降る葉生成クラス生成
	m_pSpawn_Leaf = DEBUG_NEW CSpawn_Leaf_Title(0.0f, 0.6f);

	// 成功
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTitle::Uninit()
{
	m_pThisPtr = nullptr;

	// 降る葉生成クラス
	if (m_pSpawn_Leaf != nullptr)
	{
		delete m_pSpawn_Leaf;
		m_pSpawn_Leaf = nullptr;
	}

	// 人マネージャ
	if (m_pPeopleManager != nullptr)
	{
		m_pPeopleManager->Uninit();
		m_pPeopleManager = nullptr;
	}

	// 終了処理
	CScene::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTitle::Update()
{
	// デルタタイム取得
	float deltaTime = CManager::GetInstance()->GetDeltaTime();

	// 降る葉生成の更新
	if (m_pSpawn_Leaf != nullptr)
	{
		m_pSpawn_Leaf->Update(deltaTime);
	}

	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// フェード中は抜ける
		return;
	}

	// 状態別更新処理
	(this->*(m_SceneFunc[m_SceneType]))();
}

//==========================================================================
// なにもなし
//==========================================================================
void CTitle::SceneNone()
{
	
}

//==========================================================================
// ロゴフェードイン
//==========================================================================
void CTitle::SceneFadeInLogo()
{

}

//==========================================================================
// ロゴフェードアウト
//==========================================================================
void CTitle::SceneFadeOutLoGo()
{
	// シーンカウンター減算
	m_fSceneTime -= CManager::GetInstance()->GetDeltaTime();

	// 不透明度更新
	float alpha = m_fSceneTime / TIME_FADELOGO;

	// エンターの色
	m_pPressEnter->SetAlpha(alpha);

	if (m_fSceneTime <= 0.0f)
	{
		m_fSceneTime = 0.0f;
		m_SceneType = SCENETYPE_NONE;

		// 不透明度更新
		m_pLogo->Uninit();
		m_pLogo = nullptr;

		// エンターの色
		m_pPressEnter->SetAlpha(1.0f);
		m_pPressEnter->Uninit();
		m_pPressEnter = nullptr;
		return;
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CTitle::Draw()
{

}
