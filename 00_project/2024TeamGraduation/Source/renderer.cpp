//=============================================================================
// 
//  レンダラー処理 [renderer.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "debugproc.h"
#include "camera.h"
#include "fade.h"
#include "instantfade.h"
#include "blackframe.h"
#include "pause.h"
#include "input.h"
#include "loadmanager.h"
#include "Imguimanager.h"
#include "fog.h"
#include "renderTexture.h"
#include "renderTextureManager.h"
#include "screen.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CRenderer::CRenderer() :
	m_pRenderTextureManager	(nullptr),	// レンダーテクスチャマネージャー
	m_pRenderScene	(nullptr),	// シーンレンダーテクスチャ
	m_pDrawScreen	(nullptr),	// スクリーン描画ポリゴン
	m_pDefSurScreen	(nullptr),	// 元の描画サーフェイス保存用
	m_pD3D			(nullptr),	// Direct3Dオブジェクト
	m_pD3DDevice	(nullptr)	// Direct3Dデバイス
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CRenderer::~CRenderer()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションモード

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{// 失敗したとき
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{// 失敗したとき
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				// 深度バッファ：24bit, ステンシルバッファ：8bitを使用
	d3dpp.Windowed = bWindow;									// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// 乱数の種を設定
	srand((unsigned int)time(0));

	// レンダーテクスチャマネージャーの生成
	m_pRenderTextureManager = CRenderTextureManager::Create();
	if (m_pRenderTextureManager == nullptr)
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CRenderer::Uninit()
{
	// レンダーテクスチャの破棄
	SAFE_REF_RELEASE(m_pRenderScene);

	// スクリーン描画ポリゴンの終了
	SAFE_UNINIT(m_pDrawScreen);

	// 元のスクリーン描画サーフェイスの破棄
	SAFE_RELEASE(m_pDefSurScreen);

	// Direct3Dデバイスの破棄
	SAFE_RELEASE(m_pD3DDevice);

	// Direct3Dオブジェクトの破棄
	SAFE_RELEASE(m_pD3D);
}

//==========================================================================
// 更新処理
//==========================================================================
void CRenderer::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 全ての更新
	CObject::UpdateAll(fDeltaTime, fDeltaRate, fSlowRate);

	// スクリーン描画ポリゴンの更新
	assert(m_pDrawScreen != nullptr);
	m_pDrawScreen->Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void CRenderer::Draw()
{
	CManager* pManager = GET_MANAGER;			// マネージャー
	CCamera*  pCamera = pManager->GetCamera();	// カメラ
	HRESULT   hr;	// 異常終了の確認用

	if (CManager::GetInstance()->IsLoadComplete())
	{ // ロード済みの場合

		// 全レンダーテクスチャの作成
		m_pRenderTextureManager->Draw();
	}

	// 塗りつぶしモードを設定
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// ポリゴンを塗りつぶす

	// スクリーン描画サーフェイスを元に戻す
	hr = m_pD3DDevice->SetRenderTarget(0, m_pDefSurScreen);
	assert(SUCCEEDED(hr));

	// バックバッファとZバッファのクリア
	m_pD3DDevice->Clear
	(
		0,
		nullptr,
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0
	);
	assert(SUCCEEDED(hr));

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// 描画が成功したとき

		// フォグを解除
		MyFog::DisableFog(m_pD3DDevice);

		// フォグ有効
		MyFog::SetFog();

		// 現在のビューポートを取得
		D3DVIEWPORT9 viewportDef;	//ビューポート保存
		m_pD3DDevice->GetViewport(&viewportDef);

		if (CManager::GetInstance()->IsLoadComplete())
		{ // ロード済みの場合

			// カメラの設定
			assert(pCamera != nullptr);
			pCamera->SetCamera();

			// スクリーン描画ポリゴンの描画
			assert(m_pDrawScreen != nullptr);
			m_pDrawScreen->Draw();

			// デバッグ表示の描画処理
			CManager::GetInstance()->GetDebugProc()->Draw();

			// テキストの設定
			CDebugProc::SetText();
		}
		else
		{ // ロード中の場合

			// ロードマネージャの描画
			GetLoadManager()->Draw();
		}

		// 遷移なしフェード描画処理
		CInstantFade* pfalsefade = CManager::GetInstance()->GetInstantFade();
		if (pfalsefade != nullptr)
		{
			pfalsefade->Draw();
		}

		// フェード描画処理
		CFade* pfade = CManager::GetInstance()->GetFade();
		if (pfade != nullptr)
		{
			pfade->Draw();
		}

		// ビューポートを元に戻す
		m_pD3DDevice->SetViewport(&viewportDef);

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// Imguiの描画
	ImguiMgr::Draw();

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//============================================================
//	レンダーテクスチャー生成処理
//============================================================
HRESULT CRenderer::CreateRenderTexture()
{
	// 元のスクリーン描画サーフェイスを保存
	HRESULT hr = m_pD3DDevice->GetRenderTarget(0, &m_pDefSurScreen);
	if (FAILED(hr))
	{ // サーフェイス取得に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// シーンレンダーテクスチャの生成
	m_pRenderScene = CRenderTexture::Create(CRenderTextureManager::LAYER_MAIN, &CObject::DrawAll);
	if (m_pRenderScene == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// スクリーン描画ポリゴンの生成
	m_pDrawScreen = CScreen::Create(m_pRenderScene->GetTextureIndex());
	if (m_pDrawScreen == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	レンダーテクスチャ描画処理
//============================================================
void CRenderer::DrawRenderTexture(LPDIRECT3DSURFACE9* pSurface, ADrawFunc pDrawFunc)
{
	CManager *pManager = GET_MANAGER;			// マネージャー
	CCamera  *pCamera  = pManager->GetCamera();	// カメラ

	D3DVIEWPORT9 viewportDef;	// カメラのビューポート保存用
	HRESULT hr;	// 異常終了の確認用

	if (GET_MANAGER->IsWireframe())
	{ // ワイヤーフレームがONの場合

		// 塗りつぶしモードを設定
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	// ワイヤーフレーム
	}

	// 描画サーフェイスをスクリーン描画サーフェイスに変更
	hr = m_pD3DDevice->SetRenderTarget(0, *pSurface);
	assert(SUCCEEDED(hr));

	// バックバッファとZバッファのクリア
	hr = m_pD3DDevice->Clear
	(
		0,
		nullptr,
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0
	);
	assert(SUCCEEDED(hr));

	// テクスチャ作成用の描画
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{ // 描画開始が成功した場合

		// 現在のビューポートを取得
		m_pD3DDevice->GetViewport(&viewportDef);

		// カメラの設定
		assert(pCamera != nullptr);
		pCamera->SetCamera();

		// オブジェクトの全描画
		assert(pDrawFunc != nullptr);
		pDrawFunc();

		// ビューポートを元に戻す
		m_pD3DDevice->SetViewport(&viewportDef);

		// 描画終了
		hr = m_pD3DDevice->EndScene();
		assert(SUCCEEDED(hr));
	}

	// 塗りつぶしモードを元に戻す
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// ポリゴンを塗りつぶす
}

//============================================================
//	スクリーンシェーダーフラグの設定処理
//============================================================
void CRenderer::SetEnableShader(const bool bShader)
{
	// シェーダーフラグの設定
	m_pDrawScreen->SetEnableShader(bShader);
}

//============================================================
//	スクリーンシェーダーフラグの取得処理
//============================================================
bool CRenderer::IsShader() const
{
	// シェーダーフラグの取得
	return m_pDrawScreen->IsShader();
}

//============================================================
//	レンダーテクスチャインデックスの取得処理
//============================================================
int CRenderer::GetRenderTextureIndex() const
{
	// レンダーテクスチャインデックスの取得
	return m_pRenderScene->GetTextureIndex();
}
