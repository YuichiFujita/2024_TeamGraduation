//=============================================================================
// 
//  マネージャ処理 [manager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "object2D.h"
#include "object3D.h"
#include "objectBillboard.h"
#include "objectX.h"
#include "texture.h"
#include "calculation.h"
#include "lostrssmanager.h"

#include "pause.h"
#include "fade.h"
#include "instantfade.h"
#include "blackframe.h"
#include "light.h"
#include "camera.h"
#include "edit.h"
#include "resultmanager.h"
#include "rankingmanager.h"
#include "loadmanager.h"
#include "Imguimanager.h"
#include "fog.h"


//#include <vlc/vlc.h>

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float TIME_LOAD = 2.0f;	// 必須ロード時間

#if _DEBUG
	const CScene::MODE STARTMODE = CScene::MODE::MODE_GAME;
#else
	const CScene::MODE STARTMODE = CScene::MODE::MODE_TITLE;
#endif
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CManager *CManager::m_pManager = nullptr;		// マネージャのオブジェクト
bool CManager::m_bDisp_ImGui = false;			// ImGuiの描画判定
bool CManager::m_bDisp_BoxColliders = false;	// 当たり判定ボックスの描画判定
bool CManager::m_bDisp_CheckPoint = true;		// チェックポイントの描画判定

//==========================================================================
// コンストラクタ
//==========================================================================
CManager::CManager()
{
	m_pRenderer = nullptr;			// レンダラー
	m_pInput = nullptr;				// 入力
	m_pSound = nullptr;				// サウンド
	m_pDebugProc = nullptr;			// デバッグ表示
	m_pLight = nullptr;				// ライト
	m_pCamera = nullptr;			// カメラ
	m_pTexture = nullptr;			// テクスチャ
	m_pXLoad = nullptr;				// Xファイル
	m_pEdit = nullptr;				// エディット
	m_pScene = nullptr;				// シーン
	m_pFade = nullptr;				// フェード
	m_pInstantFade = nullptr;		// 遷移なしフェード
	m_pPause = nullptr;				// ポーズ
	m_pResultManager = nullptr;		// リザルトマネージャ
	m_pRankingManager = nullptr;	// ランキングマネージャ
	m_pMyEffekseer = nullptr;		// エフェクシア
	m_bWireframe = false;			// ワイヤーフレーム
	m_bHitStop = false;				// ヒットストップの判定
	m_nCntHitStop = 0;				// ヒットストップのカウンター
	m_OldMode = CScene::MODE_NONE;	// 前回のモード
	m_CurrentTime = 0;				// 現在時間
	m_OldTime = 0;					// 過去の時間
	m_fDeltaTime = 0.0f;			// 経過時間
	m_nNumPlayer = 0;				// プレイヤーの数
	m_fLoadTimer = 0.0f;			// ロードのタイマー
	m_bLoadComplete = false;		// ロード完了のフラグ
	m_bFirstLoad = false;			// 初回ロード
	m_bDisp_2D = false;				// 2Dの表示
	m_bDisp_UI = true;				// UIの表示
	m_bWindowed = true;

	// ロードフラグリセット
	m_bLoadComplete = false;

	// ロードのフェード設定フラグ
	m_bLoadFadeSet = false;
	m_bNowLoading = false;				// ロード完了のフラグ
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
	m_bHitStop = false;		// ヒットストップの判定
	m_nCntHitStop = 0;		// ヒットストップのカウンター
	m_nNumPlayer = 0;		// プレイヤーの数

	//**********************************
	// ロストするリソースのマネージャー
	//**********************************
	CLostResourceManager::GetInstance();	//取得はしないけど生成はする

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
	// デバッグ表示
	//**********************************
	if (m_pDebugProc != nullptr)
	{// 確保されていたら
		return E_FAIL;
	}

	// メモリ確保
	m_pDebugProc = DEBUG_NEW CDebugProc;

	if (m_pDebugProc != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		hr = m_pDebugProc->Init(hInstance, hWnd);
		if (FAILED(hr))
		{// 初期化処理が失敗した場合
			return E_FAIL;
		}
	}


	//**********************************
	// ライト
	//**********************************
	if (m_pLight != nullptr)
	{// 確保されていたら
		return E_FAIL;
	}

	// メモリ確保
	m_pLight = DEBUG_NEW CLight;

	if (m_pLight != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		hr = m_pLight->Init();
		if (FAILED(hr))
		{// 初期化処理が失敗した場合
			return E_FAIL;
		}
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
		m_pCamera->Reset(STARTMODE);

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

	//**********************************
	// リザルトマネージャ
	//**********************************
	m_pResultManager = CResultManager::Create();
	if (m_pResultManager == nullptr)
	{
		return;
	}

	//**********************************
	// ランキングマネージャ
	//**********************************
	m_pRankingManager = CRankingManager::Create();
	if (m_pRankingManager == nullptr)
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
	m_bHitStop = false;		// ヒットストップの判定
	m_nCntHitStop = 0;		// ヒットストップのカウンター

	// 前回のモード設定
	m_OldMode = GetMode();

	if (mode == CScene::MODE_GAME && m_pResultManager != nullptr && m_pRankingManager != nullptr)
	{// 次のモードがゲームだったら

		// スコア情報リセット
		m_pResultManager->Reset();
		m_pRankingManager->Reset();
	}

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

	// マップ
	//MyMap::Release();

	// ポーズ状況入れ替え
	if (m_pPause != nullptr)
	{
		ChangePauseMode(mode);

		if (m_pPause->IsPause())
		{// ポーズ中だったら
			m_pPause->SetPause();
		}
	}

	// カメラの情報リセット
	if (m_pCamera != nullptr)
	{
		m_pCamera->Reset(mode);
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
	if (m_pScene != nullptr)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pScene->Kill();
	}

	// 全てのオブジェクト破棄
	CObject::ReleaseAll();

	// BGMストップ
	m_pSound->StopSound();

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

	// Imguiの終了
	ImguiMgr::Uninit();

	// ライトの破棄
	if (m_pLight != nullptr)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pLight->Uninit();

		// メモリの開放
		delete m_pLight;
		m_pLight = nullptr;
	}

	// カメラの破棄
	if (m_pCamera != nullptr)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pCamera->Uninit();

		// メモリの開放
		delete m_pCamera;
		m_pCamera = nullptr;
	}

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

	// デバッグ表示の破棄
	if (m_pDebugProc != nullptr)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pDebugProc->Uninit();

		// メモリの開放
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
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

	if (m_pScene != nullptr)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

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

	if (m_pResultManager != nullptr)
	{// メモリの確保がされていたら

		// 終了処理
		m_pResultManager->Uninit();
		m_pResultManager = nullptr;
	}

	if (m_pRankingManager != nullptr)
	{// メモリの確保がされていたら

		// 終了処理
		m_pRankingManager->Uninit();
		delete m_pRankingManager;
		m_pRankingManager = nullptr;
	}

	// ロストするリソース管理マネージャー破棄
	CLostResourceManager::Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CManager::Update()
{
	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad *pInputGamepad = CInputGamepad::GetInstance();

	// 過去の時間保存
	m_OldTime = m_CurrentTime;

	// 経過時間
	m_CurrentTime = timeGetTime();
	m_fDeltaTime = (float)(m_CurrentTime - m_OldTime) / 1000;

	// Imguiの更新
	ImguiMgr::Update();

	// 初回ロード判定
	if (!m_bFirstLoad)
	{
		bool bComplete = GetLoadManager()->IsLoadComplete();

		if (bComplete &&
			!m_bLoadFadeSet)
		{
			CManager::GetInstance()->GetInstantFade()->SetFade();
			m_bLoadFadeSet = true;	// フェードのセットフラグ
		}

		// ロードマネージャの更新
		GetLoadManager()->Update();

		if (m_bLoadFadeSet)
		{// フェードが設定されてる状態

			// 遷移なしフェードの更新処理
			m_pInstantFade->Update();

			if (m_pInstantFade->GetState() == CInstantFade::STATE_FADECOMPLETION)
			{
				m_bLoadComplete = true;	// ロード完了
				m_bNowLoading = false;	// ロード中フラグオフ
				m_bFirstLoad = true;	// 初回ロード終了
				SetMode(STARTMODE);
			}
		}
		return;
	}

	if (m_bNowLoading)
	{
		// ロード中
		if (!m_bLoadComplete)
		{
			// ロード時間加算
			m_fLoadTimer += m_fDeltaTime;
		}

		bool bComplete = GetLoadManager()->IsLoadComplete();

		if (bComplete &&
			m_fLoadTimer >= TIME_LOAD &&
			!m_bLoadFadeSet)
		{// ロード完了の条件完了

			CManager::GetInstance()->GetInstantFade()->SetFade();
			m_bLoadFadeSet = true;	// フェードのセットフラグ
		}

		if (m_bLoadFadeSet)
		{// フェードが設定されてる状態
			if (m_pInstantFade->GetState() == CInstantFade::STATE_FADECOMPLETION)
			{
				m_bLoadComplete = true;	// ロード完了
				m_bNowLoading = false;	// ロード中フラグオフ
			}
		}

		// ロードマネージャの更新
		if (!m_bLoadComplete)
		{
			GetLoadManager()->Update();
		}
	}

	// フェードの更新処理
	m_pFade->Update();

	// 遷移なしフェードの更新処理
	m_pInstantFade->Update();

	if (!m_bLoadComplete)
	{
		return;
	}

	if (m_bLoadComplete)
	{

		// 入力機器の更新処理
		m_pInput->Update();

		if ((pInputKeyboard->GetTrigger(DIK_P) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_START, 0)) &&
			m_pFade->GetState() == CFade::STATE_NONE &&
			(GetMode() == CScene::MODE_GAME || GetMode() == CScene::MODE::MODE_GAMETUTORIAL))
		{// フェード中じゃないとき

			// サウンド再生
			m_pSound->PlaySound(CSound::LABEL::LABEL_SE_CURSOR_END);
			m_pPause->SetPause();
		}

		if (m_pPause != nullptr)
		{
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
				m_pPause->Update();

				if (!GetLoadManager()->IsLoadComplete())
				{
					return;
				}
//#if _DEBUG

				// カメラの更新処理
				m_pCamera->Update();
//#endif

				return;
			}
		}

		if (m_bHitStop)
		{// ヒットストップ中は更新停止

			// ヒットストップカウンター減算
			m_nCntHitStop--;

			if (m_nCntHitStop <= 0)
			{// カウンターがなくなったら
				m_bHitStop = false;
			}
		}
		else
		{
			m_nCntHitStop = 20;
		}

#if _DEBUG

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

		//if (pInputKeyboard->GetTrigger(DIK_F8))
		//{// F8でエディットモード切替え

		//	if (m_pEdit == nullptr)
		//	{// nullptrだったら

		//		// エディットの生成処理
		//		m_pEdit = CEdit::Create();
		//	}
		//	else
		//	{
		//		// 終了させる
		//		m_pEdit->Release();
		//		m_pEdit = nullptr;
		//	}
		//}
#endif


		if (m_pScene != nullptr)
		{
			m_pScene->Update();
		}

		// レンダラーの更新処理
		if (m_pRenderer != nullptr)
		{
			m_pRenderer->Update();
		}

		// ライトの更新処理
		m_pLight->Update();

		// カメラの更新処理
		m_pCamera->Update();

		// デバッグ表示の更新処理
		m_pDebugProc->Update();
	}
}

//==========================================================================
// ディスプレイモード変更
//==========================================================================
void CManager::ChangeDisplayMode(bool bWindow)
{
	m_bWindowed = bWindow;

	if (m_bWindowed)
	{// ウィンドウモード
		m_pRenderer->SetDisplayMode(CRenderer::DISPLAYMODE::MODE_WINDOW);
	}
	else
	{// ウィンドウレスモード
		m_pRenderer->SetDisplayMode(CRenderer::DISPLAYMODE::MODE_FULLSCREEN);
	}
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
// ポーズの種類切り替え
//==========================================================================
void CManager::ChangePauseMode(CScene::MODE mode)
{
	switch (mode)
	{
	case CScene::MODE::MODE_GAME:
	case CScene::MODE::MODE_GAMETUTORIAL:

		// 削除
		if (m_pPause!= nullptr)
		{
			m_pPause->Kill();
			delete m_pPause;
			m_pPause = nullptr;
		}

		// 再生成
		m_pPause = CPause::Create(mode);
		break;

	default:
		break;
	}
}

//==========================================================================
// 経過時間取得
//==========================================================================
float CManager::GetDeltaTime()
{
	return m_fDeltaTime;
}

//==========================================================================
// ヒットストップの設定
//==========================================================================
void CManager::SetEnableHitStop(int nCntHitStop)
{
	m_bHitStop = true;
	m_nCntHitStop = nCntHitStop;
}

//==========================================================================
// レンダラーの取得
//==========================================================================
CRenderer *CManager::GetRenderer()
{
	return m_pRenderer;
}

//==========================================================================
// デバッグ表示の取得
//==========================================================================
CDebugProc *CManager::GetDebugProc()
{
	return m_pDebugProc;
}

//==========================================================================
// ライトの取得
//==========================================================================
CLight *CManager::GetLight()
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
// リザルトマネージャ取得
//==========================================================================
CResultManager *CManager::GetResultManager()
{
	return m_pResultManager;
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

