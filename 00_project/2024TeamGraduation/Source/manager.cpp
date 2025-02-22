//=============================================================================
// 
//  マネージャ処理 [manager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "object2D.h"
#include "object3D.h"
#include "objectBillboard.h"
#include "objectX.h"
#include "texture.h"
#include "calculation.h"

#include "pause.h"
#include "fade.h"
#include "blackframe.h"
#include "lightManager.h"
#include "camera.h"
#include "edit.h"
#include "loadmanager.h"
#include "Imguimanager.h"
#include "fog.h"
#include "font.h"
#include "characterAnim.h"
#include "shader.h"
#include "renderTextureManager.h"
#include "motionManager.h"

#include "entry_setupTeam.h"
#include "entry_dressup.h"
#include "entryRuleManager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
#if _DEBUG
	const int MIN_LOOP_LOAD = 1;	// 必須ロードループ数
#else
	const int MIN_LOOP_LOAD = 1;	// 必須ロードループ数
#endif

#if _DEBUG
#ifdef ENTRYSTART
	const CScene::MODE STARTMODE = CScene::MODE::MODE_TITLE;
#else
	const CScene::MODE STARTMODE = CScene::MODE::MODE_GAME;
#endif
#else	// TODO: ENTRY画面完成したらTITLEにする
	const CScene::MODE STARTMODE = CScene::MODE::MODE_TITLE;
#endif
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CManager *CManager::m_pManager = nullptr;	// マネージャのオブジェクト
bool CManager::m_bDisp_ImGui = false;		// ImGuiの描画判定
bool CManager::m_bDisp_BoxColliders = true;	// 当たり判定ボックスの描画判定
bool CManager::m_bDisp_CheckPoint = true;	// チェックポイントの描画判定

//==========================================================================
// コンストラクタ
//==========================================================================
CManager::CManager()
{
	m_pRenderer = nullptr;			// レンダラー
	m_pInput = nullptr;				// 入力
	m_pSound = nullptr;				// サウンド
	m_pLight = nullptr;				// ライト
	m_pCamera = nullptr;			// カメラ
	m_pTexture = nullptr;			// テクスチャ
	m_pXLoad = nullptr;				// Xファイル
	m_pFont = nullptr;				// フォント
	m_pCharacterAnim = nullptr;		// キャラクターアニメーション
	m_pEdit = nullptr;				// エディット
	m_pScene = nullptr;				// シーン
	m_pFade = nullptr;				// フェード
	m_pInstantFade = nullptr;		// 遷移なしフェード
	m_pPause = nullptr;				// ポーズ
	m_pRankingManager = nullptr;	// ランキングマネージャ
	m_pMyEffekseer = nullptr;		// エフェクシア
	m_bWireframe = false;			// ワイヤーフレーム
	m_OldMode = CScene::MODE_NONE;	// 前回のモード
	m_nNumPlayer = 0;				// プレイヤーの数
	m_fLoadTimer = 0.0f;			// ロードのタイマー
	m_bLoadComplete = false;		// ロード完了のフラグ
	m_bFirstLoad = false;			// 初回ロード
	m_bDisp_2D = false;				// 2Dの表示
	m_bDisp_UI = true;				// UIの表示
	m_bWindowed = true;				// ウィンドウモードか
	m_bWorldPaused = false;			// 世界のポーズ判定
	m_dwOldTime = timeGetTime();	// 前回の処理開始時刻
	m_dwCurTime = m_dwOldTime;		// 今回の処理開始時刻
	m_fDeltaTime = 0.0f;			// 経過時間
	m_fDeltaRate = 0.0f;			// 経過時間の割合
	m_fSlowRate = 1.0f;				// 速度低下の割合

	// ロードフラグリセット
	m_bLoadComplete = false;

	// ロードのフェード設定フラグ
	m_bLoadFadeSet = false;
	m_bNowLoading = false;				// ロード完了のフラグ

	// デバッグ情報
	m_debugEasing = SDebugEasing();			// デバッグのイージング情報

}

//==========================================================================
// デストラクタ
//==========================================================================
CManager::~CManager()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CManager *CManager::Create()
{
	if (m_pManager == nullptr)
	{// まだ生成していなかったら

		// マネージャのインスタンス生成
		m_pManager = DEBUG_NEW CManager;
	}
	else
	{
		// インスタンス取得
		m_pManager->GetInstance();
	}

	return m_pManager;
}

//==========================================================================
// インスタンス取得
//==========================================================================
CManager *CManager::GetInstance()
{
	return m_pManager;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{

	HRESULT hr;
	m_nNumPlayer = 0;		// プレイヤーの数

	//**********************************
	// 入力
	//**********************************
	// 生成
	m_pInput = CInput::Create(hInstance, hWnd);


	//**********************************
	// レンダラー
	//**********************************
	if (m_pRenderer != nullptr)
	{// 確保されていたら

		return E_FAIL;
	}

	// メモリ確保
	m_pRenderer = DEBUG_NEW CRenderer;

	if (m_pRenderer != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		hr = m_pRenderer->Init(hWnd, bWindow);
		if (FAILED(hr))
		{// 初期化処理が失敗した場合
			return E_FAIL;
		}
	}


	// Imguiの初期化
	ImguiMgr::Init(hWnd, m_pRenderer->GetDevice());

	//**********************************
	// サウンド
	//**********************************
	m_pSound = CSound::Create(hWnd);
	if (m_pSound == nullptr)
	{
		return E_FAIL;
	}

	//**********************************
	// ライト
	//**********************************
	if (m_pLight != nullptr)
	{// 確保されていたら
		return E_FAIL;
	}

	// ライトの生成
	m_pLight = CLightManager::Create();
	if (m_pLight == nullptr)
	{// 生成に失敗した場合

		return E_FAIL;
	}


	//**********************************
	// カメラ
	//**********************************
	if (m_pCamera != nullptr)
	{// 確保されていたら
		return E_FAIL;
	}

	// メモリ確保
	m_pCamera = DEBUG_NEW CCamera;

	if (m_pCamera != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		hr = m_pCamera->Init();
		if (FAILED(hr))
		{// 初期化処理が失敗した場合
			return E_FAIL;
		}
		m_pCamera->Reset();

		// カメラのマトリックス設定
		CInputMouse::GetInstance()->SetCameraMtx(m_pCamera->GetMtxViewPtr(), m_pCamera->GetMtxProjectionPtr());
	}

	//**********************************
	// エフェクシア
	//**********************************
	m_pMyEffekseer = CMyEffekseer::Create();
	if (m_pMyEffekseer == nullptr)
	{
		return E_FAIL;
	}

	//**********************************
	// テクスチャ
	//**********************************
	m_pTexture = CTexture::Create();
	if (m_pTexture == nullptr)
	{
		return E_FAIL;
	}

	//**********************************
	// Xファイル
	//**********************************
	m_pXLoad = CXLoad::Create();
	if (m_pXLoad == nullptr)
	{
		return E_FAIL;
	}

	//**********************************
	// フォント
	//**********************************
	m_pFont = CFont::Create();
	if (m_pFont == nullptr)
	{
		return E_FAIL;
	}

	//**********************************
	// キャラクターアニメーション
	//**********************************
	m_pCharacterAnim = CCharacterAnim::Create();
	if (m_pCharacterAnim == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	//**********************************
	// モーション
	//**********************************
	CMotionManager::Create();

	// レンダーテクスチャーの生成
	if (FAILED(m_pRenderer->CreateRenderTexture()))
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// シェーダーの生成
	if (FAILED(CShader::Create()))
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	//**********************************
	// フェード
	//**********************************
	m_pFade = CFade::Create();
	if (m_pFade == nullptr)
	{// 失敗していたら
		return E_FAIL;
	}

	// ロードフラグリセット
	m_bLoadComplete = false;
	m_bLoadFadeSet = false;	// ロードのフェード設定フラグ
	m_bNowLoading = true;

	// シーンのロードを開始
	GetLoadManager()->LoadScene(CScene::MODE_NONE);

#if 0
	/* VLCエンジンを読み込む */
	libvlc_instance_t* instance = libvlc_new(0, NULL);
	if (instance == NULL) {
		std::cerr << "libvlc_new failed: " << libvlc_errmsg() << std::endl;
		return -1;
	}

	/* 新しい項目を作成する */
	libvlc_media_t* media = libvlc_media_new_path(instance, "data\\MOVIE\\ohayo01mayu.mp3"); // ファイルパス (相対パス)
	// media = libvlc_media_new_path(instance, "C:\\path/sample.mp3");          // ファイルパス (絶対パス)
	// media = libvlc_media_new_location(instance, "http://example.com/sample.mp3"); // URL

	/* メディアプレーヤーの再生環境を作成する */
	libvlc_media_player_t* media_player = libvlc_media_player_new_from_media(media);

	/* メディアを保持する必要がなくなったため、メディアの参照カウントを減少させる */
	libvlc_media_release(media);

	/* メディアプレーヤーを再生する */
	libvlc_media_player_play(media_player);

	/* 待機する */
	while (libvlc_state_t::libvlc_Ended != libvlc_media_player_get_state(media_player))
	{
		Sleep(100);
	}

	/* 再生を止める */
	libvlc_media_player_stop(media_player);

	/* メディアプレーヤーを解放する */
	libvlc_media_player_release(media_player);

	libvlc_release(instance);
#endif

	return S_OK;
}

//==========================================================================
// 読み込み
//==========================================================================
void CManager::Load()
{
	// 全てのテクスチャ読み込み
	m_pTexture->LoadAll();

	// 全てのモデル読み込み
	m_pXLoad->LoadAll();

	// 全てのフォント読み込み
	m_pFont->LoadAll();

	// 全てのキャラクターアニメーションの読み込み
	m_pCharacterAnim->LoadAll();

	//**********************************
	// 遷移なしフェード
	//**********************************
	m_pInstantFade = CInstantFade::Create();
	if (m_pInstantFade == nullptr)
	{
		return;
	}

	//**********************************
	// ポーズ
	//**********************************
	m_pPause = CPause::Create(STARTMODE);
	if (m_pPause == nullptr)
	{
		return;
	}

	// モード設定
	NoLoadSetMode(STARTMODE);
}

//==========================================================================
// 次のモード設定
//==========================================================================
void CManager::SetMode(CScene::MODE mode)
{
	// ロード解放
	GetLoadManager()->UnLoad();

	// 次のモード設定
	NoLoadSetMode(mode);

	// ロードのタイマーリセット
	m_fLoadTimer = 0.0f;

	// ロードフラグリセット
	m_bLoadComplete = false;
	m_bLoadFadeSet = false;				// ロードのフェード設定フラグ
	m_bNowLoading = true;

	// シーンのロードを開始
	GetLoadManager()->LoadScene(mode);
}

//==========================================================================
// 次のモード設定
//==========================================================================
void CManager::NoLoadSetMode(CScene::MODE mode)
{
	// リセット
	Reset(mode);

	// 生成処理
	m_pScene = CScene::Create(mode);

	// ロードのタイマーリセット
	m_fLoadTimer = 0.0f;
}

//==========================================================================
// リセット
//==========================================================================
void CManager::Reset(CScene::MODE mode)
{
	// 前回のモード設定
	m_OldMode = GetMode();

	// BGMストップ
	if (m_pSound != nullptr)
	{
		m_pSound->StopSound();
	}

	// メモリの確保がされていたら
	if (m_pScene != nullptr)
	{
		// 終了処理
		m_pScene->Uninit();
		m_pScene->Kill();
		delete m_pScene;
		m_pScene = nullptr;
	}

	// 全てのオブジェクト破棄
	CObject::ReleaseAll();

	// ポーズ状況入れ替え
	if (m_pPause != nullptr)
	{
		// 種類切り替え
		ChangePauseMode(mode);
	}

	// カメラの情報リセット
	if (m_pCamera != nullptr)
	{
		m_pCamera->Reset();
		m_pCamera->ResetByMode(mode);
	}

	// モーションマネージャーリセット
	CMotionManager* pMotionMgr = CMotionManager::GetInstance();
	if (pMotionMgr != nullptr)
	{
		pMotionMgr->Reset();
	}
}

//==========================================================================
// 現在のモード取得
//==========================================================================
CScene::MODE CManager::GetMode()
{
	// モード取得
	if (m_pScene != nullptr)
	{
		return m_pScene->GetMode();
	}
	else
	{
		return CScene::MODE_NONE;
	}
}

//==========================================================================
// 終了処理
//==========================================================================
void CManager::Uninit()
{
	// エントリー情報の初期化
	CEntry_SetUpTeam::SaveInit();
#ifdef NDEBUG
	CEntry_Dressup::SaveInit();
	CEntryRuleManager::SaveInit();
#endif

	if (m_pScene != nullptr)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pScene->Kill();
	}

	// ライトの破棄
	SAFE_REF_RELEASE(m_pLight);

	// カメラの破棄
	if (m_pCamera != nullptr)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pCamera->Uninit();

		// メモリの開放
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	// 入力機器の破棄
	if (m_pInput != nullptr)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pInput->Release();
	}

	// レンダラーの破棄
	if (m_pRenderer != nullptr)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pRenderer->Uninit();

		// メモリの開放
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	// シーンの破棄
	if (m_pScene != nullptr)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pScene->Uninit();

		// メモリの開放
		delete m_pScene;
		m_pScene = nullptr;
	}

	// ロード画面の破棄
	CLoadManager* pLoadManager = GetLoadManager();
	if (pLoadManager != nullptr)
	{
		// 終了処理
		pLoadManager->Uninit();
	}

	// シェーダーの破棄
	CShader::Release();

	// 全てのオブジェクト破棄
	CObject::ReleaseAll();

	CRenderTextureManager* pRenderTextureManager = CRenderTextureManager::GetInstance();	// レンダーテクスチャマネージャー
	if (pRenderTextureManager != nullptr)
	{ // メモリが確保済みの場合

		// レンダーテクスチャマネージャーの終了
		pRenderTextureManager->Uninit();
	}

	// BGMストップ
	m_pSound->StopSound();

	// Imguiの終了
	ImguiMgr::Uninit();

	if (m_pMyEffekseer != nullptr)
	{
		// 終了処理
		m_pMyEffekseer->Uninit();
		m_pMyEffekseer = nullptr;
	}

	// サウンドの破棄
	if (m_pSound != nullptr)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pSound->Uninit();

		// メモリの開放
		delete m_pSound;
		m_pSound = nullptr;
	}

	// エディットの破棄
	if (m_pEdit != nullptr)
	{// メモリの確保が出来ていたら

		m_pEdit = nullptr;
	}

	//**********************************
	// 全てのテクスチャ破棄
	//**********************************
	if (m_pTexture != nullptr)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pTexture->Unload();

		// メモリの開放
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pXLoad != nullptr)
	{
		// 終了処理
		m_pXLoad->Uninit();

		// メモリの開放
		m_pXLoad = nullptr;
	}

	// フォントの破棄
	SAFE_REF_RELEASE(m_pFont);

	// キャラクターアニメーションの破棄
	SAFE_REF_RELEASE(m_pCharacterAnim);

	// フェードの破棄
	if (m_pFade != nullptr)
	{// メモリの確保がされていたら

		// 終了処理
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

	// 遷移なしフェードの破棄
	if (m_pInstantFade != nullptr)
	{// メモリの確保がされていたら

		// 終了処理
		m_pInstantFade->Uninit();
		delete m_pInstantFade;
		m_pInstantFade = nullptr;
	}

	if (m_pPause != nullptr)
	{// メモリの確保がされていたら

		// 終了処理
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = nullptr;
	}


	// モーションマネージャーリセット
	CMotionManager* pMotionMgr = CMotionManager::GetInstance();
	if (pMotionMgr != nullptr)
	{
		pMotionMgr->Uninit();
	}

	// ロストするリソース管理マネージャー破棄
	//CLostResourceManager::Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CManager::Update()
{
	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	// 経過時間の更新
	UpdateDeltaTime();

	// 経過時間の割合の更新
	UpdateDeltaRate();

	// Imguiの更新
	ImguiMgr::Update();

	// 初回ロード判定
	if (!m_bFirstLoad)
	{// 初回ロードが完了していないとき

		// 初回ロード処理
		FirstLoad();
		return;
	}

	// 通常ロード
	NormalLoad();

	if (!m_bLoadComplete)
	{// ロード完了するまで抜ける
		return;
	}

	// 入力機器の更新処理
	m_pInput->Update(m_fDeltaTime, m_fDeltaRate, m_fSlowRate);

	if (m_pPause != nullptr)
	{
		// ポーズの更新
		m_pPause->Update(m_fDeltaTime, m_fDeltaRate, m_fSlowRate);

		// ポーズ状況取得
		bool bPause = m_pPause->IsPause();
		CCameraMotion* pCamMotion = m_pCamera->GetCameraMotion();
		if (pCamMotion != nullptr)
		{
			pCamMotion->SetEnableSystemPause(bPause);
		}

		// ポーズの更新処理
		if (bPause)
		{// ポーズ中だったら

			if (!GetLoadManager()->IsLoadComplete())
			{
				return;
			}

			// カメラの更新処理
			m_pCamera->Update(m_fDeltaTime, m_fDeltaRate, m_fSlowRate);
			return;
		}
	}

#if _DEBUG
	// デバッグ処理
	Debug();
#endif

	// シーンの更新処理
	if (m_pScene != nullptr)
	{
		m_pScene->Update(m_fDeltaTime, m_fDeltaRate, m_fSlowRate);
	}

	// レンダラーの更新処理
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Update(m_fDeltaTime, m_fDeltaRate, m_fSlowRate);
	}

	// ライトの更新処理
	m_pLight->Update(m_fDeltaTime, m_fDeltaRate, m_fSlowRate);

	// カメラの更新処理
	m_pCamera->Update(m_fDeltaTime, m_fDeltaRate, m_fSlowRate);
}

//==========================================================================
// 初回ロード
//==========================================================================
void CManager::FirstLoad()
{
	// ロード完了フラグ取得
	bool bComplete = GetLoadManager()->IsLoadComplete();

	if (bComplete &&
		!m_bLoadFadeSet)
	{// ロードが完了している && フェードセット済み
		CManager::GetInstance()->GetInstantFade()->SetFade();
		m_bLoadFadeSet = true;	// フェードのセットフラグ
	}

	// ロードマネージャの更新
	GetLoadManager()->Update(m_fDeltaTime, m_fDeltaRate, m_fSlowRate);

	if (m_bLoadFadeSet)
	{// フェードが設定されてる状態

		// 遷移なしフェードの更新処理
		m_pInstantFade->Update(m_fDeltaTime, m_fDeltaRate, m_fSlowRate);

		if (m_pInstantFade->GetState() == CInstantFade::STATE_FADECOMPLETION)
		{
			m_bLoadComplete = true;	// ロード完了
			m_bNowLoading = false;	// ロード中フラグオフ
			m_bFirstLoad = true;	// 初回ロード終了

			// モード設定
			SetMode(STARTMODE);
		}
	}
}

//==========================================================================
// 通常ロード
//==========================================================================
void CManager::NormalLoad()
{
	if (m_bNowLoading)
	{// ロード中

		CLoadManager* pLoadManager = GetLoadManager();	// ロードマネージャー

		if (!m_bLoadComplete)
		{// ロードが完了していない

			// ロード時間加算
			m_fLoadTimer += m_fDeltaTime;
		}

		// ロード完了フラグ取得
		bool bComplete = pLoadManager->IsLoadComplete();

		if (bComplete
#ifdef NDEBUG
		&&  pLoadManager->IsLoopMoment()
#endif
		&&  pLoadManager->GetNumLoop() >= MIN_LOOP_LOAD
		&&  !m_bLoadFadeSet)
		{// ロード完了の条件完了

			CManager::GetInstance()->GetInstantFade()->SetFade();
			m_bLoadFadeSet = true;	// フェードのセットフラグ
			pLoadManager->SetEnableAutoPlay(false);
			pLoadManager->ResetNumLoop();
		}

		if (m_bLoadFadeSet)
		{// フェードが設定されてる状態

			if (m_pInstantFade->GetState() == CInstantFade::STATE_FADECOMPLETION)
			{// フェードが終了している場合

				m_bLoadComplete = true;	// ロード完了
				m_bNowLoading = false;	// ロード中フラグオフ
			}
		}

		// ロードマネージャの更新
		if (!m_bLoadComplete)
		{
			pLoadManager->Update(m_fDeltaTime, m_fDeltaRate, m_fSlowRate);
		}
	}

	// フェードの更新処理
	m_pFade->Update(m_fDeltaTime, m_fDeltaRate, m_fSlowRate);

	// 遷移なしフェードの更新処理
	m_pInstantFade->Update(m_fDeltaTime, m_fDeltaRate, m_fSlowRate);
}

//==========================================================================
// デバッグ
//==========================================================================
void CManager::Debug()
{
	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	// 描画判定
	if (pInputKeyboard->GetTrigger(DIK_F1))
	{// F1でワイヤーフレーム切り替え
		m_bDisp_ImGui = m_bDisp_ImGui ? false : true;
	}
	ImGui::Checkbox("Disp BoxColliders", &m_bDisp_BoxColliders);
	ImGui::Checkbox("Disp CheckPoint", &m_bDisp_CheckPoint);
	ImGui::Checkbox("Disp 2D", &m_bDisp_2D);

	if (pInputKeyboard->GetTrigger(DIK_F2))
	{// F2でワイヤーフレーム切り替え
		m_bWireframe = m_bWireframe ? false : true;
	}

	if (pInputKeyboard->GetTrigger(DIK_F6))
	{
		MyFog::ToggleFogFrag();
	}


	// イージングのサンプル
	SampleEasing();

}

//==========================================================================
// イージングのサンプル / FUJITA
//==========================================================================
void CManager::SampleEasing()
{
	if (ImGui::TreeNode("Easing"))
	{
		if (ImGui::Button("Reset"))
		{
			m_debugEasing = SDebugEasing();
		}

		//--------------------------
		// 最大時間変更
		//--------------------------
		if (ImGui::DragFloat("playtime", &m_debugEasing.maxtime, 0.001f, 0.0f, 0.0f, "%.2f"))
		{
			m_debugEasing.maxtime = UtilFunc::Transformation::Clamp(m_debugEasing.maxtime, 0.001f, 100.0f);
		}

		ImGui::DragFloat("backPower", &m_debugEasing.backPower, 0.001f, 0.0f, 0.0f, "%.2f");

		//--------------------------
		// タイマー加算
		//--------------------------
		m_debugEasing.time += m_fDeltaTime * m_fDeltaRate * m_fSlowRate;

		if (m_debugEasing.maxtime + m_debugEasing.aftertime <= m_debugEasing.time)
		{// 終わった後の時間も経過
			m_debugEasing.time = 0.0f;
		}


		// プログレスバーの背景色とバーの色を一時的に変更
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(20, 140, 20, 255));    // プログレスバーの色

		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		//--------------------------
		// イージング
		//--------------------------
		// Linear
		ImGui::ProgressBar(UtilFunc::Correction::EasingLinear(0.0f, 1.0f, m_debugEasing.time / m_debugEasing.maxtime) * 0.25f + 0.5f, ImVec2(0.0f, 0.0f), "Linear");

		// InOut
		ImGui::ProgressBar(UtilFunc::Correction::EasingEaseIn(0.0f, 1.0f, m_debugEasing.time / m_debugEasing.maxtime) * 0.25f + 0.5f, ImVec2(0.0f, 0.0f), "EaseIn");
		ImGui::ProgressBar(UtilFunc::Correction::EasingEaseOut(0.0f, 1.0f, m_debugEasing.time / m_debugEasing.maxtime) * 0.25f + 0.5f, ImVec2(0.0f, 0.0f), "EaseOut");
		ImGui::ProgressBar(UtilFunc::Correction::EasingEaseInOut(0.0f, 1.0f, m_debugEasing.time / m_debugEasing.maxtime) * 0.25f + 0.5f, ImVec2(0.0f, 0.0f), "EaseInOut");

		// Back
		ImGui::ProgressBar(UtilFunc::Correction::EaseInBack(0.0f, 1.0f, 0.0f, m_debugEasing.maxtime, m_debugEasing.time, m_debugEasing.backPower) * 0.25f + 0.5f, ImVec2(0.0f, 0.0f), "EaseInBack");
		ImGui::ProgressBar(UtilFunc::Correction::EaseOutBack(0.0f, 1.0f, 0.0f, m_debugEasing.maxtime, m_debugEasing.time, m_debugEasing.backPower) * 0.25f + 0.5f, ImVec2(0.0f, 0.0f), "EaseOutBack");
		ImGui::ProgressBar(UtilFunc::Correction::EaseInOutBack(0.0f, 1.0f, 0.0f, m_debugEasing.maxtime, m_debugEasing.time, m_debugEasing.backPower) * 0.25f + 0.5f, ImVec2(0.0f, 0.0f), "EaseInOutBack");

		// Expo
		ImGui::ProgressBar(UtilFunc::Correction::EaseInExpo(0.0f, 1.0f, 0.0f, m_debugEasing.maxtime, m_debugEasing.time) * 0.25f + 0.5f, ImVec2(0.0f, 0.0f), "EaseInExpo");
		ImGui::ProgressBar(UtilFunc::Correction::EaseOutExpo(0.0f, 1.0f, 0.0f, m_debugEasing.maxtime, m_debugEasing.time) * 0.25f + 0.5f, ImVec2(0.0f, 0.0f), "EaseOutExpo");
		ImGui::ProgressBar(UtilFunc::Correction::EaseInOutExpo(0.0f, 1.0f, 0.0f, m_debugEasing.maxtime, m_debugEasing.time) * 0.25f + 0.5f, ImVec2(0.0f, 0.0f), "EaseInOutExpo");


		// カラーの設定を元に戻す
		ImGui::PopStyleColor(1);

		ImGui::TreePop();
	}
}

//==========================================================================
// ディスプレイモード変更
//==========================================================================
void CManager::ChangeDisplayMode(bool bWindow)
{
	m_bWindowed = bWindow;
}

//==========================================================================
// 描画処理
//==========================================================================
void CManager::Draw()
{
	/*if (!m_bLoadComplete)
	{
		GetLoadManager()->Draw();
		return;
	}*/

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = m_pRenderer->GetDevice();

	if (m_bWireframe)
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	// 埋めるモード
	}

	// レンダラーの描画処理
	m_pRenderer->Draw();

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// 埋めるモード
}

//==========================================================================
// プレイヤーの数取得
//==========================================================================
int CManager::GetNumPlayer()
{
	return m_nNumPlayer;
}

//==========================================================================
// プレイヤーの数設定
//==========================================================================
void CManager::SetNumPlayer(int nNum)
{
	// 値の正規化処理
	UtilFunc::Transformation::ValueNormalize(nNum, mylib_const::MAX_PLAYER, 0);

	m_nNumPlayer = nNum;
}

//==========================================================================
// 経過時間更新
//==========================================================================
void CManager::UpdateDeltaTime()
{
	// 前フレームの開始時刻を保存
	m_dwOldTime = m_dwCurTime;

	// 現在時刻を開始時刻に保存
	m_dwCurTime = timeGetTime();

	// 処理開始時刻の差分を計算
	DWORD dwDiffDeltaTime = m_dwCurTime - m_dwOldTime;

	// 経過時間を計算
	m_fDeltaTime = dwDiffDeltaTime * 0.001f;
}

//==========================================================================
// 経過時間の割合更新
//==========================================================================
void CManager::UpdateDeltaRate()
{
	// 経過時間の割合を計算
	m_fDeltaRate = m_fDeltaTime / (1.0f / 60.0f);
}

//==========================================================================
// ポーズの種類切り替え
//==========================================================================
void CManager::ChangePauseMode(CScene::MODE mode)
{
	// 削除
	if (m_pPause != nullptr)
	{
		m_pPause->Kill();
		delete m_pPause;
		m_pPause = nullptr;
	}

	// 再生成
	m_pPause = CPause::Create(mode);
}

//==========================================================================
// レンダラーの取得
//==========================================================================
CRenderer *CManager::GetRenderer()
{
	return m_pRenderer;
}

//==========================================================================
// ライトの取得
//==========================================================================
CLightManager *CManager::GetLight()
{
	return m_pLight;
}

//==========================================================================
// カメラの取得
//==========================================================================
CCamera *CManager::GetCamera()
{
	return m_pCamera;
}

//==========================================================================
// エディットの取得
//==========================================================================
CEdit *CManager::GetEdit()
{
	return m_pEdit;
}

//==========================================================================
// フォントの取得
//==========================================================================
CFont *CManager::GetFont()
{
	return m_pFont;
}

//==========================================================================
// キャラクターアニメーションの取得
//==========================================================================
CCharacterAnim *CManager::GetCharacterAnim()
{
	return m_pCharacterAnim;
}

//==========================================================================
// フェードの取得
//==========================================================================
CFade *CManager::GetFade()
{
	return m_pFade;
}

//==========================================================================
// 遷移なしフェードの取得
//==========================================================================
CInstantFade *CManager::GetInstantFade()
{
	return m_pInstantFade;
}

//==========================================================================
// ポーズの取得
//==========================================================================
CPause *CManager::GetPause()
{
	return m_pPause;
}

//==========================================================================
// ランキングマネージャのオブジェクト
//==========================================================================
CRankingManager *CManager::GetRankingManager()
{
	return m_pRankingManager;
}

//==========================================================================
// シーン取得
//==========================================================================
CScene *CManager::GetScene()
{
	return m_pScene;
}

