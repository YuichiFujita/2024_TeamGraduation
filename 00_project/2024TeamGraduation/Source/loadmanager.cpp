//=============================================================================
// 
//  ロードマネージャ [loadmanager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "loadmanager.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "loadscreen.h"
#include "fade.h"
#include "instantfade.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CLoadManager::CLoadManager()
{
	m_bLoadComplete = false;
	m_ModeNext = CScene::MODE_NONE;
	m_pLoadScreen = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CLoadManager::~CLoadManager()
{
	// ロードスレッドがまだ生きているなら待機
	if (m_LoadingThread.joinable()) 
	{
		m_LoadingThread.join();
	}
}

//==========================================================================
// 生成処理
//==========================================================================
CLoadManager* CLoadManager::Create()
{

	// メモリの確保
	CLoadManager* pLoadManager = DEBUG_NEW CLoadManager;

	if (pLoadManager != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		pLoadManager->Init();
	}

	return pLoadManager;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CLoadManager::Init()
{
	// ロードスレッドがまだ生きているなら待機
	if (m_LoadingThread.joinable()) 
	{
		m_LoadingThread.join();
	}
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CLoadManager::Uninit()
{
	if (m_pLoadScreen != nullptr)
	{
		m_pLoadScreen->Kill();
		delete m_pLoadScreen;
		m_pLoadScreen = nullptr;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CLoadManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ロード画面の更新
	if (m_pLoadScreen != nullptr)
	{
		m_pLoadScreen->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// ロード解放
//==========================================================================
void CLoadManager::UnLoad()
{
	if (m_LoadingThread.joinable())
	{
		m_LoadingThread.join();
	}
}

//==========================================================================
// ロードリセット
//==========================================================================
void CLoadManager::ResetLoad()
{
	// ロードリセット
	ResetInternalLoad();
}

//==========================================================================
// 新しいシーンをセットする前のリセット
//==========================================================================
void CLoadManager::ResetInternalLoad()
{
	// 終了を待つ
	if (m_LoadingThread.joinable())
	{
		m_LoadingThread.join();
	}

	// ロードが完了フラグをリセット
	m_bLoadComplete = false;
}

//==========================================================================
// シーンのロードを開始
//==========================================================================
void CLoadManager::LoadScene(CScene::MODE mode)
{
	// 次のモード
	m_ModeNext = mode;

	// ロード画面生成
	if (m_pLoadScreen == nullptr)
	{
		m_pLoadScreen = CLoadScreen::Create();
	}

	if (m_LoadingThread.joinable())
	{
		// スレッドが完了するまで待機
		m_LoadingThread.join();
	}

	// ロードを準備
	ResetLoad();

	// ロードが再び始まるのでフラグをリセット
	m_bLoadComplete = false;

    // ロード処理の開始
	m_LoadingThread = std::thread(&CLoadManager::LoadInBackground, this);

	// ロード画面のアニメーション再生
	SetEnableAutoPlay(true);

	// サブスレッドをデタッチ
	m_LoadingThread.detach();
}

//==========================================================================
// シーンの初期化を非同期で行う処理
//==========================================================================
void CLoadManager::LoadInBackground()
{
	// ロードが再び始まるのでフラグをリセット
	m_bLoadComplete = false;

	// 読み込み処理
	Load();

	if (m_LoadingThread.joinable())
	{
		m_LoadingThread.join();
	}

	// ロード完了フラグをセット
	m_bLoadComplete = true;
}

//==========================================================================
// ロード処理
//==========================================================================
void CLoadManager::Load()
{
	// シーンの初期化処理
	if (m_ModeNext != CScene::MODE_NONE)
	{
		CManager::GetInstance()->GetScene()->Init();
	}
	else
	{
		CManager::GetInstance()->Load();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CLoadManager::Draw()
{
	if (m_pLoadScreen != nullptr)
	{
		m_pLoadScreen->Draw();
	}
}

//==========================================================================
// ロードが完了したかどうかを返す
//==========================================================================
bool CLoadManager::IsLoadComplete()
{
	return m_bLoadComplete;
}

//==========================================================================
// ロード画面繰り返し瞬間取得
//==========================================================================
bool CLoadManager::IsLoopMoment() const
{
	return m_pLoadScreen->IsLoopMoment();
}

//==========================================================================
// ロード画面ループ数取得
//==========================================================================
DWORD CLoadManager::GetNumLoop() const
{
	return m_pLoadScreen->GetNumLoop();
}

//==========================================================================
// ロード画面ループ数リセット
//==========================================================================
void CLoadManager::ResetNumLoop()
{
	m_pLoadScreen->ResetNumLoop();
}

//==========================================================================
// ロード画面自動再生設定
//==========================================================================
void CLoadManager::SetEnableAutoPlay(const bool bPlay)
{
	m_pLoadScreen->SetEnableAutoPlay(bPlay);
}
