//=============================================================================
// 
//  マネージャ処理 [manager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "object2D.h"
#include "object3D.h"
#include "objectBillboard.h"
#include "objectX.h"
#include "texture.h"
#include "Xload.h"
#include "calculation.h"

#include "light.h"
#include "camera.h"
#include "player.h"
#include "3D_effect.h"
#include "particle.h"
#include "map.h"
#include "edit.h"
#include "meshfield.h"

//==========================================================================
// マクロ定義
//==========================================================================

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CRenderer *CManager::m_pRenderer = NULL;			// レンダラーのオブジェクト
CInputKeyboard *CManager::m_pInputKeyboard = NULL;	// キーボードのオブジェクト
CInputGamepad *CManager::m_pInputGamepad = NULL;	// ゲームパッドのオブジェクト
CSound *CManager::m_pSound = NULL;					// サウンドのオブジェクト
CInputMouse *CManager::m_pInputMouse = NULL;		// マウスのオブジェクト
CDebugProc *CManager::m_pDebugProc = NULL;			// デバッグ表示のオブジェクト
CPlayer *CManager::m_pPlayer = NULL;				// プレイヤーのオブジェクト
CParticle *CManager::m_pParticle = NULL;			// パーティクルのオブジェクト
CScore *CManager::m_pScore = NULL;					// スコアのオブジェクト
CTimer *CManager::m_pTimer = NULL;					// タイマーのオブジェクト
CBG *CManager::m_pBg = NULL;						// 背景のオブジェクト
CLight *CManager::m_pLight = NULL;					// ライトのオブジェクト
CCamera *CManager::m_pCamera = NULL;				// カメラのオブジェクト
CMap *CManager::m_pMap = NULL;						// マップのオブジェクト
CTexture *CManager::m_pTexture = NULL;				// テクスチャのオブジェクト
CXLoad *CManager::m_pXLoad = NULL;					// Xファイルのオブジェクト
CEdit *CManager::m_pEdit = NULL;					// エディットのオブジェクト
CElevation *CManager::m_pObject3DMesh = NULL;	// オブジェクト3Dメッシュのオブジェクト
CEnemyManager *CManager::m_pEnemyManager = NULL;	// 敵マネージャのオブジェクト
bool CManager::m_bWireframe = false;				// ワイヤーフレーム
CManager::EDITTYPE CManager::m_editType = EDITTYPE_EDIT;		// エディットの種類

// プロトタイプ宣言
std::string LoadMotionFileName(void);

//==========================================================================
// コンストラクタ
//==========================================================================
CManager::CManager()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CManager::~CManager()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{

	HRESULT hr;
	m_editType = EDITTYPE_EDIT;

	//**********************************
	// キーボード
	//**********************************
	if (m_pInputKeyboard != NULL)
	{// 確保されていたら
		return E_FAIL;
	}

	// メモリ確保
	m_pInputKeyboard = DEBUG_NEW CInputKeyboard;

	if (m_pInputKeyboard != NULL)
	{// メモリの確保が出来ていたら

		// 初期化処理
		hr = m_pInputKeyboard->Init(hInstance, hWnd);
		if (FAILED(hr))
		{// 初期化処理が失敗した場合

			return E_FAIL;
		}
	}

	
	//**********************************
	// ゲームパッド
	//**********************************
	if (m_pInputGamepad != NULL)
	{// 確保されていたら
		return E_FAIL;
	}

	// メモリ確保
	m_pInputGamepad = DEBUG_NEW CInputGamepad;

	if (m_pInputGamepad != NULL)
	{// メモリの確保が出来ていたら

		// 初期化処理
		hr = m_pInputGamepad->Init(hInstance, hWnd);
		if (FAILED(hr))
		{// 初期化処理が失敗した場合

			return E_FAIL;
		}
	}


	//**********************************
	// マウス
	//**********************************
	if (m_pInputMouse != NULL)
	{// 確保されていたら
		return E_FAIL;
	}

	// メモリ確保
	m_pInputMouse = DEBUG_NEW CInputMouse;

	if (m_pInputMouse != NULL)
	{// メモリの確保が出来ていたら

		// 初期化処理
		hr = m_pInputMouse->Init(hInstance, hWnd);
		if (FAILED(hr))
		{// 初期化処理が失敗した場合

			return E_FAIL;
		}
	}


	//**********************************
	// レンダラー
	//**********************************
	if (m_pRenderer != NULL)
	{// 確保されていたら

		return E_FAIL;
	}

	// メモリ確保
	m_pRenderer = DEBUG_NEW CRenderer;

	if (m_pRenderer != NULL)
	{// メモリの確保が出来ていたら

		// 初期化処理
		hr = m_pRenderer->Init(hWnd, TRUE);
		if (FAILED(hr))
		{// 初期化処理が失敗した場合

			return E_FAIL;
		}
	}


	//**********************************
	// サウンド
	//**********************************
	if (m_pSound != NULL)
	{// 確保されていたら
		return E_FAIL;
	}

	// メモリ確保
	m_pSound = DEBUG_NEW CSound;

	if (m_pSound != NULL)
	{// メモリの確保が出来ていたら

		// 初期化処理
		hr = m_pSound->Init(hWnd);
		if (FAILED(hr))
		{// 初期化処理が失敗した場合

			return E_FAIL;
		}
	}


	//**********************************
	// デバッグ表示
	//**********************************
	if (m_pDebugProc != NULL)
	{// 確保されていたら
		return E_FAIL;
	}

	// メモリ確保
	m_pDebugProc = DEBUG_NEW CDebugProc;

	if (m_pDebugProc != NULL)
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
	if (m_pLight != NULL)
	{// 確保されていたら
		return E_FAIL;
	}

	// メモリ確保
	m_pLight = DEBUG_NEW CLight;

	if (m_pLight != NULL)
	{// メモリの確保が出来ていたら

		// 初期化処理
		hr = m_pLight->Init();
		if (FAILED(hr))
		{// 初期化処理が失敗した場合

			return E_FAIL;
		}
	}


	//**********************************
	// パーティクル
	//**********************************
	if (m_pParticle != NULL)
	{// 確保されていたら
		return E_FAIL;
	}

	// メモリ確保
	m_pParticle = DEBUG_NEW CParticle;

	if (m_pParticle != NULL)
	{// メモリの確保が出来ていたら

		// 初期化処理
		hr = m_pParticle->Init();
		if (FAILED(hr))
		{// 初期化処理が失敗した場合

			return E_FAIL;
		}
	}


	//**********************************
	// カメラ
	//**********************************
	if (m_pCamera != NULL)
	{// 確保されていたら
		return E_FAIL;
	}

	// メモリ確保
	m_pCamera = DEBUG_NEW CCamera;

	if (m_pCamera != NULL)
	{// メモリの確保が出来ていたら

		// 初期化処理
		hr = m_pCamera->Init();
		if (FAILED(hr))
		{// 初期化処理が失敗した場合

			return E_FAIL;
		}
	}

	//**********************************
	// 全てのテクスチャ読み込み
	//**********************************
	if (m_pTexture != NULL)
	{// 確保されていたら
		return E_FAIL;
	}

	// メモリ確保
	m_pTexture = DEBUG_NEW CTexture;

	if (m_pTexture != NULL)
	{// メモリの確保が出来ていたら

		// 初期化処理
		hr = m_pTexture->Load();
		if (FAILED(hr))
		{// 初期化処理が失敗した場合

			return E_FAIL;
		}

		// 初期化処理
		hr = m_pTexture->LoadAll();

		if (FAILED(hr))
		{// 初期化処理が失敗した場合

			return E_FAIL;
		}
	}


	//**********************************
	// Xファイル
	//**********************************
	if (m_pXLoad != NULL)
	{// 確保されていたら
		return E_FAIL;
	}

	// メモリ確保
	m_pXLoad = DEBUG_NEW CXLoad;

	if (m_pXLoad == NULL)
	{// メモリの確保が出来ていなかったら
		return E_FAIL;
	}

	//**********************************
	// マップ
	//**********************************
	MyMap::Create();

	//**********************************
	// オブジェクト生成
	//**********************************
	CObject2D *pObject2D = NULL;

	//**********************************
	// プレイヤー
	//**********************************
	// メモリ確保
	m_pPlayer = m_pPlayer->Create(LoadMotionFileName());
	m_pPlayer->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CManager::Uninit(void)
{
	// 全てのオブジェクト破棄
	CObject::ReleaseAll();

	// BGMストップ
	m_pSound->StopSound();

	// キーボードの破棄
	if (m_pInputKeyboard != NULL)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pInputKeyboard->Uninit();

		// メモリの開放
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// ゲームパッドの破棄
	if (m_pInputGamepad != NULL)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pInputGamepad->Uninit();

		// メモリの開放
		delete m_pInputGamepad;
		m_pInputGamepad = NULL;
	}

	// マウスの破棄
	if (m_pInputMouse != NULL)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pInputMouse->Uninit();

		// メモリの開放
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// レンダラーの破棄
	if (m_pRenderer != NULL)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pRenderer->Uninit();

		// メモリの開放
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// ライトの破棄
	if (m_pLight != NULL)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pLight->Uninit();

		// メモリの開放
		delete m_pLight;
		m_pLight = NULL;
	}

	// カメラの破棄
	if (m_pCamera != NULL)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pCamera->Uninit();

		// メモリの開放
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// サウンドの破棄
	if (m_pSound != NULL)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pSound->Uninit();

		// メモリの開放
		delete m_pSound;
		m_pSound = NULL;
	}

	// デバッグ表示の破棄
	if (m_pDebugProc != NULL)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pDebugProc->Uninit();

		// メモリの開放
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

	// パーティクルの破棄
	if (m_pParticle != NULL)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pParticle->Uninit();

		// メモリの開放
		delete m_pParticle;
		m_pParticle = NULL;
	}

	// プレイヤーの破棄
	if (m_pPlayer != NULL)
	{// メモリの確保が出来ていたら

		m_pPlayer = NULL;
	}

	// 背景の破棄
	if (m_pBg != NULL)
	{// メモリの確保が出来ていたら

		m_pBg = NULL;
	}

	// マップの破棄
	if (m_pMap != NULL)
	{// メモリの確保が出来ていたら

		m_pMap = NULL;
	}

	// Xファイルの破棄
	if (m_pXLoad != NULL)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pXLoad->Uninit();

		// メモリの開放
		delete m_pXLoad;
		m_pXLoad = NULL;
	}

	// エディットの破棄
	if (m_pEdit != NULL)
	{// メモリの確保が出来ていたら

		m_pEdit = NULL;
	}

	//**********************************
	// 全てのテクスチャ破棄
	//**********************************
	if (m_pTexture != NULL)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pTexture->Unload();

		// メモリの開放
		delete m_pTexture;
		m_pTexture = NULL;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CManager::Update(void)
{
	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	//if (pInputKeyboard->GetTrigger(DIK_F3) == true)
	//{// F3でワイヤーフレーム切り替え
	//	m_bWireframe = m_bWireframe ? false : true;
	//}

	if (pInputKeyboard->GetTrigger(DIK_F2) == true)
	{// F2でエディット切り替え

		m_editType = (EDITTYPE)((m_editType + 1) % EDITTYPE_SETUP);
	}

	if (pInputKeyboard->GetTrigger(DIK_F4) == true)
	{// F4でセットアップにすっる
		m_editType = EDITTYPE_SETUP;
	}

	// キーボードの更新処理
	m_pInputKeyboard->Update();

	// ゲームパッドの更新処理
	m_pInputGamepad->Update();

	// マウスの更新処理
	m_pInputMouse->Update();

	// レンダラーの更新処理
	m_pRenderer->Update();

	// ライトの更新処理
	m_pLight->Update();

	// カメラの更新処理
	m_pCamera->Update();

	// デバッグ表示の更新処理
	m_pDebugProc->Update();

	// パーティクルの更新処理
	m_pParticle->Update();

	if (CManager::GetEditType() != CManager::EDITTYPE_PLAY)
	{// エディット状態なら
		edit::Update();
	}
	else
	{
		if (pInputKeyboard->GetTrigger(DIK_F7) == true)
		{// F7が押された,追従切り替え

			m_pCamera->EnableChase();
		}
	}

	if (m_editType == EDITTYPE_SETUP)
	{// セットアップ状態
		m_pPlayer->SetSphereCollider();
	}

	//// ウィンドウ
	//ImGui::Begin("ChangeType");
	//{
	//	if (ImGui::BeginMenu("File"))
	//	{
	//		const char* names[] = { "Edit", "Play", "SetUp" };
	//		int selectedIndex = 0;

	//		for (int i = 0; i < IM_ARRAYSIZE(names); i++)
	//		{
	//			if (ImGui::MenuItem(names[i]))
	//			{
	//				selectedIndex = i;
	//				m_editType = static_cast<CManager::EDITTYPE>(selectedIndex);
	//			}
	//		}
	//		ImGui::EndMenu();
	//	}
	//}
	//ImGui::End();


}

//==========================================================================
// 描画処理
//==========================================================================
void CManager::Draw(void)
{
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
// レンダラーの取得
//==========================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//==========================================================================
// キーボードの取得
//==========================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//==========================================================================
// ゲームパッドの取得
//==========================================================================
CInputGamepad *CManager::GetInputGamepad(void)
{
	return m_pInputGamepad;
}

//==========================================================================
// マウスの取得
//==========================================================================
CInputMouse *CManager::GetInputMouse(void)
{
	return m_pInputMouse;
}

//==========================================================================
// サウンドの取得
//==========================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}

//==========================================================================
// デバッグ表示の取得
//==========================================================================
CDebugProc *CManager::GetDebugProc(void)
{
	return m_pDebugProc;
}

//==========================================================================
// パーティクルの取得
//==========================================================================
CParticle *CManager::GetParticle(void)
{
	return m_pParticle;
}

//==========================================================================
// プレイヤーの取得
//==========================================================================
CPlayer *CManager::GetPlayer(void)
{
	return m_pPlayer;
}

//==========================================================================
// プレイヤーの取得
//==========================================================================
CPlayer **CManager::GetPlayer(int n)
{
	return &m_pPlayer;
}

//==========================================================================
// スコアの取得
//==========================================================================
CScore *CManager::GetScore(void)
{
	return m_pScore;
}

//==========================================================================
// タイマーの取得
//==========================================================================
CTimer *CManager::GetTimer(void)
{
	return m_pTimer;
}

//==========================================================================
// 背景の取得
//==========================================================================
CBG *CManager::GetBg(void)
{
	return m_pBg;
}

//==========================================================================
// ライトの取得
//==========================================================================
CLight *CManager::GetLight(void)
{
	return m_pLight;
}

//==========================================================================
// カメラの取得
//==========================================================================
CCamera *CManager::GetCamera(void)
{
	return m_pCamera;
}

//==========================================================================
// テクスチャの取得
//==========================================================================
CTexture *CManager::GetTexture(void)
{
	return m_pTexture;
}

//==========================================================================
// Xファイルの取得
//==========================================================================
CXLoad *CManager::GetXLoad(void)
{
	return m_pXLoad;
}

//==========================================================================
// エディットの取得
//==========================================================================
CEdit *CManager::GetEdit(void)
{
	return m_pEdit;
}

//==========================================================================
// 3Dオブジェクトの取得
//==========================================================================
CElevation *CManager::GetObject3DMesh(void)
{
	return m_pObject3DMesh;
}

//==========================================================================
// 敵マネージャの取得
//==========================================================================
CEnemyManager *CManager::GetEnemyManager(void)
{
	return m_pEnemyManager;
}

//==========================================================================
// モーションファイル名読み込み
//==========================================================================
std::string LoadMotionFileName(void)
{
	FILE *pFile = NULL;	// ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen("data\\TEXT\\filename.txt", "r");

	if (pFile == NULL)
	{//ファイルが開けなかったら
		return NULL;
	}

	char aComment[MAX_COMMENT];	// 読み込み用
	std::string FileName;		// ファイル名

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		// モデル名の設定
		if (strcmp(aComment, "MOTION_FILENAME") == 0)
		{// MOTION_FILENAMEがきたら

			fscanf(pFile, "%s", &aComment[0]);	// =の分
			fscanf(pFile, "%s", &aComment[0]);	// ファイル名

			// ファイル名保存
			FileName = aComment;
		}

		if (strcmp(aComment, "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける
			break;
		}
	}

	return FileName;
}
