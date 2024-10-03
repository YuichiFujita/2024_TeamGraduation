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
#include "calculation.h"
#include "map_ui.h"
#include "lostrssmanager.h"

namespace
{
	const D3DXCOLOR ALPHACOLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
	const D3DXCOLOR NONE_ALPHACOLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	const D3DXVECTOR2 NORMALSIZE = D3DXVECTOR2(640.0f, 360.0f);
	const D3DXVECTOR2 MINISIZE = D3DXVECTOR2(640.0f, 360.0f) * 1.0f;
	const float FULLSCREEN_MINISIZE_RATIO = 1.0f;
}

//==========================================================================
// コンストラクタ
//==========================================================================
CRenderer::CRenderer()
{
	m_pD3D = nullptr;			// Direct3Dオブジェクトへのポインタ
	m_pD3DDevice = nullptr;		// Direct3Dデバイスへのポインタ
	m_bResetWait = false;		// リセット待ち状態解除
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

	// デバイスのプレゼンテーションパラメータの設定（ウィンドウ）
	ZeroMemory(&m_d3dppWindow, sizeof(m_d3dppWindow));							// パラメータのゼロクリア
	m_d3dppWindow.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	m_d3dppWindow.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	m_d3dppWindow.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	m_d3dppWindow.BackBufferCount = 1;									// バックバッファの数
	m_d3dppWindow.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え(映像信号に同期)
	m_d3dppWindow.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	m_d3dppWindow.AutoDepthStencilFormat = D3DFMT_D24S8;					// デバイスバッファとして16bitを使う
	m_d3dppWindow.Windowed = bWindow;									// ウィンドウモード
	m_d3dppWindow.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	m_d3dppWindow.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル
	m_d3dppWindow.hDeviceWindow = hWnd;

	// デバイスのプレゼンテーションパラメータの設定（フルスク）
	ZeroMemory(&m_d3dppFull, sizeof(m_d3dppFull));
	//<画面サイズは後で設定>
	m_d3dppFull.BackBufferFormat = d3ddm.Format;		//バックバッファの形式
	m_d3dppFull.BackBufferCount = 1;					//バックバッファの数
	m_d3dppFull.SwapEffect = D3DSWAPEFFECT_DISCARD;		//ダブルバッファの切り替え（同期）
	m_d3dppFull.EnableAutoDepthStencil = TRUE;			//デプスバッファとステンシルバッファを作成
	m_d3dppFull.AutoDepthStencilFormat = D3DFMT_D24S8;	//デプスバッファとして24ビット、ステンシルバッファとして8ビット使用
	m_d3dppFull.Windowed = TRUE;						//ウィンドウモード
	m_d3dppFull.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	m_d3dppFull.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//インターバル
	m_d3dppFull.hDeviceWindow = hWnd;

	// ディスプレイサイズ取得
	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	int displayWidth = desktop.right;
	int displayHeight = desktop.bottom;

	// ディスプレイとゲームのアスペクト比計算
	float displayAspect = (float)displayWidth / (float)displayHeight;
	float gameAspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	if (displayAspect > gameAspect)
	{//横長
		m_d3dppFull.BackBufferWidth = displayWidth * ((float)SCREEN_HEIGHT / (float)displayHeight);			//画面サイズ（幅）
		m_d3dppFull.BackBufferHeight = SCREEN_HEIGHT;		//画面サイズ（高さ）
	}
	else
	{//縦長・変わらない
		m_d3dppFull.BackBufferWidth = SCREEN_WIDTH;			//画面サイズ（幅）
		m_d3dppFull.BackBufferHeight = displayHeight * ((float)SCREEN_WIDTH / (float)displayWidth);		//画面サイズ（高さ）
	}

	//ディスプレイモード設定
	m_dispMode = (bWindow == TRUE) ? CRenderer::DISPLAYMODE::MODE_WINDOW : CRenderer::DISPLAYMODE::MODE_FULLSCREEN;

	// ディスプレイモード設定
	SetDisplayMode(m_dispMode);
	m_bResetWait = false;

	// Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&m_d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダラーステート設定
	ResetRendererState();

	// マルチターゲットレンダラーの初期化
	InitMTRender();

	CLostResourceManager::GetInstance()->Regist(this);

	return S_OK;
}

//==========================================================================
// レンダラーステート設定
//==========================================================================
void CRenderer::ResetRendererState()
{
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
}


//==========================================================================
// マルチターゲットレンダラーの初期化
//==========================================================================
void CRenderer::InitMTRender()
{
	// 保存用バッファ
	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;

	// 現在の画面幅取得
	float screen_width = m_d3dpp.BackBufferWidth;
	float screen_height = m_d3dpp.BackBufferHeight;
	
	// レンダリングターゲット用テクスチャの生成
	for (int i = 0; i < 2; i++)
	{
		m_pD3DDevice->CreateTexture(screen_width, screen_height,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_X8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_Multitarget.pTextureMT[i],
			NULL);


		// テクスチャレンダリング用インターフェースの生成
		m_Multitarget.pTextureMT[i]->GetSurfaceLevel(0, &m_Multitarget.pRenderMT[i]);
	}

	// テクスチャレンダリング用Zバッファの生成
	m_pD3DDevice->CreateDepthStencilSurface(
		screen_width, screen_height,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_Multitarget.pZBuffMT,
		NULL);

	// 現在のレンダリングターゲットを取得(保存)
	m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

	// 現在のZバッファを取得(保存)
	m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);

	// レンダリングターゲットを生成したテクスチャに設定
	m_pD3DDevice->SetRenderTarget(0, m_Multitarget.pRenderMT[0]);

	// Zバッファを生成したZバッファに設定
	m_pD3DDevice->SetDepthStencilSurface(m_Multitarget.pZBuffMT);

	// レンダリングターゲット用のテクスチャのクリア
	m_pD3DDevice->Clear(
		0, nullptr,
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0);


	// レンダリングターゲットを元に戻す
	m_pD3DDevice->SetRenderTarget(0, pRenderDef);

	// Zバッファを元に戻す
	m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);

	// 仮置きレンダリングターゲット・Zバッファの開放（これをしないとメモリリークが起きる）
	if (pRenderDef != nullptr)
	{
		pRenderDef->Release();
		pRenderDef = nullptr;
	}
	if (pZBuffDef != nullptr)
	{
		pZBuffDef->Release();
		pZBuffDef = nullptr;
	}

	// テクスチャ用ビューポートの設定
	m_Multitarget.viewportMT.X = 0;						// 描画する画面の左上X座標
	m_Multitarget.viewportMT.Y = 0;						// 描画する画面の左上Y座標
	m_Multitarget.viewportMT.Width = screen_width;		// 描画する画面の幅
	m_Multitarget.viewportMT.Height = screen_height;	// 描画する画面の高さ
	m_Multitarget.viewportMT.MinZ = 0.0f;
	m_Multitarget.viewportMT.MaxZ = 1.0f;


	m_pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_Multitarget.pVtxBuff,
		nullptr);



	// 頂点情報へのポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_Multitarget.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// スクリーン表示状態に応じて通常のサイズ変更
	D3DXVECTOR2 size;
	if (m_dispMode == CRenderer::DISPLAYMODE::MODE_WINDOW)
	{
		size = NORMALSIZE;
	}
	else if (m_dispMode == CRenderer::DISPLAYMODE::MODE_FULLSCREEN)
	{
		// 現在の画面幅取得
		float screen_width = m_d3dpp.BackBufferWidth;
		float screen_height = m_d3dpp.BackBufferHeight;

		size = D3DXVECTOR2(screen_width * 0.5f, screen_height * 0.5f);
	}
	

	// 頂点座標の設定
	pVtx[0].pos.x = (screen_width * 0.5f) - size.x;
	pVtx[0].pos.y = (screen_height * 0.5f) - size.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (screen_width * 0.5f) + size.x;
	pVtx[1].pos.y = (screen_height * 0.5f) - size.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (screen_width * 0.5f) - size.x;
	pVtx[2].pos.y = (screen_height * 0.5f) + size.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (screen_width * 0.5f) + size.x;
	pVtx[3].pos.y = (screen_height * 0.5f) + size.y;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = ALPHACOLOR;
	pVtx[1].col = ALPHACOLOR;
	pVtx[2].col = ALPHACOLOR;
	pVtx[3].col = ALPHACOLOR;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックロック
	m_Multitarget.pVtxBuff->Unlock();

}

//==========================================================================
// 終了処理
//==========================================================================
void CRenderer::Uninit()
{
	for (int cnt = 0; cnt < 2; cnt++)
	{
		if (m_Multitarget.pTextureMT[cnt] != nullptr)
		{
			m_Multitarget.pTextureMT[cnt]->Release();
			m_Multitarget.pTextureMT[cnt] = nullptr;
		}
		if (m_Multitarget.pRenderMT[cnt] != nullptr)
		{
			m_Multitarget.pRenderMT[cnt]->Release();
			m_Multitarget.pRenderMT[cnt] = nullptr;
		}
	}

	if (m_Multitarget.pZBuffMT != nullptr)
	{
		m_Multitarget.pZBuffMT->Release();
		m_Multitarget.pZBuffMT = nullptr;
	}

	if (m_Multitarget.pVtxBuff != nullptr)
	{
		m_Multitarget.pVtxBuff->Release();
		m_Multitarget.pVtxBuff = nullptr;
	}

	// Direct3Dデバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	CLostResourceManager::GetInstance()->Remove(this);
}

//==========================================================================
// 更新処理
//==========================================================================
void CRenderer::Update()
{
	// 全ての更新
	CObject::UpdateAll();
}

//==========================================================================
// 描画処理
//==========================================================================
void CRenderer::Draw()
{
	static bool bDisp = true;

	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

//#if _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_F1))
	{// 描画切り替え
		bDisp = bDisp ? false : true;
	}
//#endif

	// 画面クリア(バックバッファとZバッファのクリア)
	m_pD3DDevice->Clear
	(	0, nullptr,
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0
	);

	// カメラの設定
	CCamera* pCamerea = CManager::GetInstance()->GetCamera();

	// 切替用
	LPDIRECT3DTEXTURE9 pTextureWk;	// 切替用用テクスチャ
	LPDIRECT3DSURFACE9 pRenderWk;	// テクスチャレンダリング用インターフェース

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// 描画が成功したとき

		// フォグを解除
		MyFog::DisableFog(m_pD3DDevice);

		// フォグ有効
		MyFog::SetFog();

		D3DVIEWPORT9 viewportDef;	//ビューポート保存

		// 現在のビューポートを取得
		m_pD3DDevice->GetViewport(&viewportDef);

		if (CManager::GetInstance()->IsLoadComplete())
		{

			// 保存用バッファ
			LPDIRECT3DSURFACE9 pRenderDef = nullptr, pZBuffDef = nullptr;
			D3DXMATRIX mtxView, mtxProjection;

			// デフォルトのレンダラーターゲットを取得
			GetDefaultRenderTarget(&pRenderDef, &pZBuffDef, &mtxView, &mtxProjection);


			// ターゲット切替
			CManager::GetInstance()->GetRenderer()->ChangeTarget(pCamerea->GetPositionV(), pCamerea->GetPositionR(), MyLib::Vector3(0.0f, 1.0f, 0.0f));

			// テクスチャ[0]のクリア
			m_pD3DDevice->Clear(
				0, nullptr,
				(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
				D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
				1.0f,
				0);

			// 全ての描画
			CObject::DrawAll();

			// フィードバックエフェクトにテクスチャ[1]を貼り付けて描画
			float multi = UtilFunc::Correction::EasingLinear(m_MultitargetInfo.fMulti, m_MultitargetInfo.fStartMulti, m_MultitargetInfo.fTimer);
			float alpha = UtilFunc::Correction::EasingLinear(m_MultitargetInfo.fStartColAlpha, m_MultitargetInfo.fColAlpha, m_MultitargetInfo.fTimer);
			// スクリーン表示状態に応じて通常のサイズ変更
			if (m_dispMode == CRenderer::DISPLAYMODE::MODE_WINDOW)
			{
				DrawMultiTargetScreen(1, D3DXCOLOR(ALPHACOLOR.r, ALPHACOLOR.g, ALPHACOLOR.b, alpha), MINISIZE * multi);
			}
			else if (m_dispMode == CRenderer::DISPLAYMODE::MODE_FULLSCREEN)
			{
				// 現在の画面幅取得
				float screen_width = m_d3dpp.BackBufferWidth;
				float screen_height = m_d3dpp.BackBufferHeight;
				D3DXVECTOR2 screen_size = D3DXVECTOR2(screen_width * 0.5f, screen_height * 0.5f) * FULLSCREEN_MINISIZE_RATIO;

				DrawMultiTargetScreen(1, D3DXCOLOR(ALPHACOLOR.r, ALPHACOLOR.g, ALPHACOLOR.b, alpha), screen_size * multi);
			}

			// カメラの設定
			CManager::GetInstance()->GetCamera()->SetCamera();
			// レンダーターゲットをもとに戻す
			CManager::GetInstance()->GetRenderer()->ChangeRendertarget(pRenderDef, pZBuffDef, mtxView, mtxProjection);

			// スクリーン表示状態に応じて通常のサイズ変更
			if (m_dispMode == CRenderer::DISPLAYMODE::MODE_WINDOW)
			{
				DrawMultiTargetScreen(0, NONE_ALPHACOLOR, NORMALSIZE);
			}
			else if (m_dispMode == CRenderer::DISPLAYMODE::MODE_FULLSCREEN)
			{
				// 現在の画面幅取得
				float screen_width = m_d3dpp.BackBufferWidth;
				float screen_height = m_d3dpp.BackBufferHeight;

				DrawMultiTargetScreen(0, NONE_ALPHACOLOR, D3DXVECTOR2(screen_width * 0.5f, screen_height * 0.5f));
			}

			// テクスチャ0と1の切替
			pTextureWk = m_Multitarget.pTextureMT[0];
			m_Multitarget.pTextureMT[0] = m_Multitarget.pTextureMT[1];
			m_Multitarget.pTextureMT[1] = pTextureWk;

			pRenderWk = m_Multitarget.pRenderMT[0];
			m_Multitarget.pRenderMT[0] = m_Multitarget.pRenderMT[1];
			m_Multitarget.pRenderMT[1] = pRenderWk;

			// 仮置きレンダリングターゲット・Zバッファの開放（これをしないとメモリリークが起きる）
			if (pRenderDef != nullptr)
			{
				pRenderDef->Release();
				pRenderDef = nullptr;
			}
			if (pZBuffDef != nullptr)
			{
				pZBuffDef->Release();
				pZBuffDef = nullptr;
			}

			// デバッグ表示の描画処理
			CManager::GetInstance()->GetDebugProc()->Draw();

			// テキストの設定
			CDebugProc::SetText();

			// ポーズ描画処理
			if (bDisp)
			{
				if (CManager::GetInstance()->GetPause() != nullptr)
				{
					CManager::GetInstance()->GetPause()->Draw();
				}
			}

		}
		else
			// ロードマネージャの更新
		{
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

	//バックバッファとフロントバッファの入れ替え（同時にデバイスロスト検知）
	HRESULT hr = m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
	if (hr == D3DERR_DEVICELOST || m_bResetWait)
	{
		ResetDevice();
	}

	// マルチターゲット調整
	if (m_MultitargetInfo.bActive) {
		SetMultiTarget();
	}
}

//==========================================================================
// マルチターゲット画面の描画
//==========================================================================
void CRenderer::DrawMultiTargetScreen(int texIdx, const D3DXCOLOR& col, const D3DXVECTOR2& size)
{
	// 現在の画面幅取得
	float screen_width = m_d3dpp.BackBufferWidth;
	float screen_height = m_d3dpp.BackBufferHeight;

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_Multitarget.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = (screen_width * 0.5f) - size.x;
	pVtx[0].pos.y = (screen_height * 0.5f) - size.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (screen_width * 0.5f) + size.x;
	pVtx[1].pos.y = (screen_height * 0.5f) - size.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (screen_width * 0.5f) - size.x;
	pVtx[2].pos.y = (screen_height * 0.5f) + size.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (screen_width * 0.5f) + size.x;
	pVtx[3].pos.y = (screen_height * 0.5f) + size.y;
	pVtx[3].pos.z = 0.0f;

	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロックロック
	m_Multitarget.pVtxBuff->Unlock();

	// 頂点バッファをデータストリームに設定
	m_pD3DDevice->SetStreamSource(0, m_Multitarget.pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	m_pD3DDevice->SetTexture(0, m_Multitarget.pTextureMT[texIdx]);

	// ポリゴンの描画
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================================
// デバイスの取得
//==========================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice() const
{
	return m_pD3DDevice;
}

HRESULT CRenderer::SetFullScreen()
{
	m_pD3DDevice->Reset(&m_d3dpp);

	D3DDISPLAYMODE d3ddm;			// ディスプレイモード

	// Direct3Dデバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}


	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{// 失敗したとき
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));							// パラメータのゼロクリア

	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	m_d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	m_d3dpp.BackBufferCount = 1;									// バックバッファの数
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え(映像信号に同期)
	m_d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デバイスバッファとして16bitを使う
	m_d3dpp.Windowed = TRUE;										// ウィンドウモード
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル


	ResetWnd();

	// Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		GetWnd(),
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&m_d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			GetWnd(),
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				GetWnd(),
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// ウインドウの表示
	ShowWindow(GetWnd(), GetCmbShow());		// ウインドウの表示状態を設定
	UpdateWindow(GetWnd());				// クライアント領域を更新

	return S_OK;
}

//==========================================================================
// デフォルトのレンダーターゲット取得
//==========================================================================
void CRenderer::GetDefaultRenderTarget(LPDIRECT3DSURFACE9* pRender, LPDIRECT3DSURFACE9* pZBuff, D3DXMATRIX* viewport, D3DXMATRIX* projection)
{
	// 現在のレンダリングターゲットを取得(保存)
	m_pD3DDevice->GetRenderTarget(0, &(*pRender));

	// 現在のZバッファを取得(保存)
	m_pD3DDevice->GetDepthStencilSurface(&(*pZBuff));

	m_pD3DDevice->GetTransform(D3DTS_VIEW, &(*viewport));
	m_pD3DDevice->GetTransform(D3DTS_PROJECTION, &(*projection));
}

//==========================================================================
// レンダリングターゲットの切替
//==========================================================================
void CRenderer::ChangeRendertarget(LPDIRECT3DSURFACE9 pRender, LPDIRECT3DSURFACE9 pZBuff, D3DXMATRIX viewport, D3DXMATRIX projection)
{
	// レンダリングターゲットを生成したテクスチャに設定
	m_pD3DDevice->SetRenderTarget(0, pRender);

	// Zバッファを生成したZバッファに設定
	m_pD3DDevice->SetDepthStencilSurface(pZBuff);

	// テクスチャレンダリング用のビューポートを設定
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &viewport);

	// テクスチャレンダリング用のプロジェクションマトリックスを設定
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &projection);
}

//==========================================================================
// レンダリングターゲットの切替
//==========================================================================
void CRenderer::ChangeTarget(MyLib::Vector3 posV, MyLib::Vector3 posR, MyLib::Vector3 vecU)
{
	D3DXMATRIX mtxview, mtxProjection;

	// 現在の画面幅取得
	float screen_width = m_d3dpp.BackBufferWidth;
	float screen_height = m_d3dpp.BackBufferHeight;

	// レンダリングターゲットを生成したテクスチャに設定
	m_pD3DDevice->SetRenderTarget(0, m_Multitarget.pRenderMT[0]);

	// Zバッファを生成したZバッファに設定
	m_pD3DDevice->SetDepthStencilSurface(m_Multitarget.pZBuffMT);

	// テクスチャレンダリング用のビューポートを設定
	m_pD3DDevice->SetViewport(&m_Multitarget.viewportMT);


	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(
		&mtxProjection,
		D3DXToRadian(45.0f),	// 視野角
		(float)screen_width / (float)screen_height,	// アスペクト比
		10.0f,		// 手前の制限
		100000.0f);	// 奥行きの制限

	// プロジェクションマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&mtxview);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(
		&mtxview,
		&posV,	// 視点
		&posR,	// 注視点
		&vecU);	// 上方向ベクトル

	// ビューマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxview);
}

//==========================================================================
// マルチターゲット画面の描画判定
//==========================================================================
void CRenderer::SetEnableDrawMultiScreen(float fGoalAlpha, float fGoalMulti, float fTimer)
{ 
	// パラメーターの設定
	m_MultitargetInfo.fTimer = 0.0f;
	if (fTimer != 0.0f){ m_MultitargetInfo.fAddTimer = 1.0f / fTimer; }	// 1フレームごとの加算量設定
	else { m_MultitargetInfo.fAddTimer = 1.0f; }	// 0なら即
	m_MultitargetInfo.fStartColAlpha = m_MultitargetInfo.fColAlpha;
	m_MultitargetInfo.fColAlpha = fGoalAlpha;
	m_MultitargetInfo.fStartMulti = m_MultitargetInfo.fMulti;
	m_MultitargetInfo.fMulti = fGoalMulti;
	m_MultitargetInfo.bActive = true;

	return;
}

//==========================================================================
// マルチターゲット画面の描画調整
//==========================================================================
void CRenderer::SetMultiTarget()
{
	m_MultitargetInfo.fTimer += m_MultitargetInfo.fAddTimer;

	if (m_MultitargetInfo.fTimer >= 1.0f) { 
		m_MultitargetInfo.bActive = false; 
	}
}

//=================================
// ディスプレイモード設定
//=================================
void CRenderer::SetDisplayMode(DISPLAYMODE mode)
{
	// モード設定
	D3DDISPLAYMODE d3ddm;
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));

	// 現在のスクリーンモードを取得
	m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	//画面サイズ取得
	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);

	// モードに応じてプレゼンテーションパラメータの設定
	if (mode == CRenderer::DISPLAYMODE::MODE_WINDOW)
	{// ウィンドウモード
		m_d3dpp = m_d3dppWindow;

		// ウィンドウスタイル・位置サイズ変更
		SetWindowLong(GetWnd(), GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(GetWnd(), HWND_TOP, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOZORDER | SWP_FRAMECHANGED);

		// 再描画の強制
		ShowWindow(GetWnd(), SW_SHOWNORMAL);
		UpdateWindow(GetWnd());
	}
	else if (mode == CRenderer::DISPLAYMODE::MODE_FULLSCREEN)
	{// フルスクリーンモード
		m_d3dpp = m_d3dppFull;

		//画面サイズ取得
		RECT desktop;
		GetWindowRect(GetDesktopWindow(), &desktop);

		// ウィンドウスタイル・位置サイズ変更
		SetWindowLong(GetWnd(), GWL_STYLE, WS_POPUP | WS_VISIBLE);
		SetWindowPos(GetWnd(), HWND_TOP, 0, 0, desktop.right, desktop.bottom, SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	m_bResetWait = true;
	m_dispMode = mode;
}

//=================================
// デバイスリセット
//=================================
void CRenderer::ResetDevice()
{
	// リソースいったん解放
	CLostResourceManager::GetInstance()->Backup();

	// デバイスリセット
	HRESULT hr = m_pD3DDevice->TestCooperativeLevel();
	if (hr == S_OK || hr == D3DERR_DEVICENOTRESET)
	{
		hr = m_pD3DDevice->Reset(&m_d3dpp);
		if (FAILED(hr))
		{
			if (hr == D3DERR_INVALIDCALL)
			{
				assert(false);
			}

			return;
		}

		// レンダラーステート設定
		ResetRendererState();

		// リソース再生成
		CLostResourceManager::GetInstance()->Restore();
		m_bResetWait = false;
	}
}

//========================
// バックアップ
//========================
void CRenderer::Backup()
{
	for (int cnt = 0; cnt < 2; cnt++)
	{
		if (m_Multitarget.pTextureMT[cnt] != nullptr)
		{
			m_Multitarget.pTextureMT[cnt]->Release();
			m_Multitarget.pTextureMT[cnt] = nullptr;
		}
		if (m_Multitarget.pRenderMT[cnt] != nullptr)
		{
			m_Multitarget.pRenderMT[cnt]->Release();
			m_Multitarget.pRenderMT[cnt] = nullptr;
		}
	}

	if (m_Multitarget.pZBuffMT != nullptr)
	{
		m_Multitarget.pZBuffMT->Release();
		m_Multitarget.pZBuffMT = nullptr;
	}

	if (m_Multitarget.pVtxBuff != nullptr)
	{
		m_Multitarget.pVtxBuff->Release();
		m_Multitarget.pVtxBuff = nullptr;
	}
}

//========================
// 復元
//========================
void CRenderer::Restore()
{
	InitMTRender();
}