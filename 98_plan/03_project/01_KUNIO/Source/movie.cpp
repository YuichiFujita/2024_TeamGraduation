////=============================================================================
//// 
////  サンプル_オブジェクト2D処理 [movie.cpp]
////  Author : 相馬靜雅
//// 
////=============================================================================
//#include "movie.h"
//#include "manager.h"
//#include "calculation.h"
//#include "input.h"
//
//
//#include <dshow.h>
//#include <atlbase.h>
//
//// その他変数
//IGraphBuilder* pGraph = nullptr;
//IMediaControl* pControl = nullptr;
//IMediaSeeking* pSeeking = nullptr;
//IBaseFilter* pSourceFilter = nullptr;
//IBasicVideo* pBasicVideo = nullptr;
//CComPtr<IVideoWindow> pVideoWindow;
//
//BYTE* pFrameBuffer = nullptr;
//
//LPDIRECT3DTEXTURE9 m_ptexture = nullptr;
//long lWidth = 0, lHeight = 0;
//
////==========================================================================
//// 定数定義
////==========================================================================
//namespace
//{
//	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\key\\A.png";	// テクスチャのファイル
//}
//
//HRESULT CMovie::RemoveVideoRenderer()
//{
//	CComPtr<IEnumFilters> pEnum;
//	HRESULT hr = pGraph->EnumFilters(&pEnum);
//	if (FAILED(hr)) return hr;
//
//	IBaseFilter* pFilter = nullptr;
//	while (pEnum->Next(1, &pFilter, NULL) == S_OK)
//	{
//		CComPtr<IBaseFilter> pBaseFilter = pFilter;
//		CComPtr<IFileSourceFilter> pFileSource;
//
//		// フィルタの種類をチェック
//		CLSID clsid;
//		hr = pBaseFilter->GetClassID(&clsid);
//		if (FAILED(hr)) continue;
//
//		// Video Renderer の CLSID は CLSID_VideoRenderer です
//		if (clsid == CLSID_VideoRenderer)
//		{
//			// Video Renderer フィルタを削除
//			hr = pGraph->RemoveFilter(pBaseFilter);
//			if (FAILED(hr)) return hr;
//			break;
//		}
//	}
//
//	return S_OK;
//}
//
//
////==========================================================================
//// コンストラクタ
////==========================================================================
//CMovie::CMovie(int nPriority) : CObject2D(nPriority)
//{
//	HRESULT hr;
//
//	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);
//	if (FAILED(hr)) return;
//
//	hr = pGraph->RenderFile(L"data\\MOVIE\\TKD.mp4", NULL);
//	if (FAILED(hr)) return;
//
//	// Video Renderer フィルタの削除
//	hr = RemoveVideoRenderer();
//	if (FAILED(hr)) return;
//
//	hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
//	if (FAILED(hr)) return;
//
//	hr = pGraph->QueryInterface(IID_IMediaSeeking, (void**)&pSeeking);
//	if (FAILED(hr)) return;
//
//	hr = pGraph->QueryInterface(IID_IBasicVideo, (void**)&pBasicVideo);
//	if (FAILED(hr)) return;
//
//	hr = pGraph->QueryInterface(IID_IVideoWindow, (void**)&pVideoWindow);
//	if (SUCCEEDED(hr))
//	{
//		pVideoWindow->put_Visible(OAFALSE); // ウィンドウを非表示に設定
//		pVideoWindow->put_AutoShow(OAFALSE); // 自動的に表示しない設定
//	}
//
//	// ビデオの幅と高さを取得
//	hr = pBasicVideo->GetVideoSize(&lWidth, &lHeight);
//	if (FAILED(hr)) return;
//
//	// フレームバッファの確保
//	pFrameBuffer = new BYTE[lWidth * lHeight * 4]; // RGB32 フォーマット
//
//}
//
//// ビデオフレームをテクスチャに転送する関数
//void CMovie::UpdateTextureFromVideoFrame(IBasicVideo* pBasicVideo)
//{
//	// ビデオフレームの取得
//	long bufferSize = lWidth * lHeight * 4; // RGB32 フォーマット
//	HRESULT hr = pBasicVideo->GetCurrentImage(&bufferSize, reinterpret_cast<long*>(pFrameBuffer));
//	if (SUCCEEDED(hr))
//	{
//		if (m_ptexture != nullptr)
//		{
//			m_ptexture->Release();
//			m_ptexture = nullptr;
//		}
//
//		if (m_ptexture == nullptr)
//		{
//			// デバイスの取得
//			LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
//
//			// テクスチャの生成
//			hr = pDevice->CreateTexture(
//				lWidth,          // テクスチャの幅
//				lHeight,         // テクスチャの高さ
//				0,               // ミップマップレベル（1 でミップマップなし）
//				D3DUSAGE_DYNAMIC, // 使用方法（動的テクスチャ）
//				D3DFMT_A8R8G8B8, // ピクセルフォーマット（例：A8R8G8B8）
//				D3DPOOL_DEFAULT, // メモリプール
//				&m_ptexture,     // テクスチャのポインタ
//				NULL             // ハンドル（使用しない）
//			);
//		}
//
//		// テクスチャのロックとデータのコピー
//		D3DLOCKED_RECT lockedRect;
//		hr = m_ptexture->LockRect(0, &lockedRect, NULL, D3DLOCK_DISCARD);
//		if (SUCCEEDED(hr))
//		{
//			memcpy(lockedRect.pBits, pFrameBuffer, bufferSize);
//			m_ptexture->UnlockRect(0);
//		}
//	}
//
//
//
//
//
//
//
//	//hr = pGraph->QueryInterface(IID_IBasicVideo, (void**)&pBasicVideo);
//	//if (SUCCEEDED(hr))
//	//{
//	//	hr = pBasicVideo->GetCurrentImage(&bufferSize, reinterpret_cast<long*>(pFrameBuffer));
//	//	pBasicVideo->Release();
//
//	//	if (SUCCEEDED(hr))
//	//	{
//	//		if (m_ptexture == nullptr)
//	//		{
//	//			// デバイスの取得
//	//			LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
//
//	//			// テクスチャの生成
//	//			HRESULT hr = pDevice->CreateTexture(
//	//				lWidth,          // テクスチャの幅
//	//				lHeight,         // テクスチャの高さ
//	//				1,               // ミップマップレベル（1 でミップマップなし）
//	//				D3DUSAGE_DYNAMIC, // 使用方法（ここでは動的テクスチャ）
//	//				D3DFMT_A8R8G8B8, // ピクセルフォーマット（例：A8R8G8B8）
//	//				D3DPOOL_DEFAULT, // メモリプール
//	//				&m_ptexture,       // テクスチャのポインタ
//	//				NULL             // ハンドル（使用しない）
//	//			);
//	//		}
//
//	//		// テクスチャのロックとデータのコピー
//	//		D3DLOCKED_RECT lockedRect;
//	//		hr = m_ptexture->LockRect(0, &lockedRect, NULL, D3DLOCK_DISCARD);
//	//		if (SUCCEEDED(hr))
//	//		{
//	//			memcpy(lockedRect.pBits, pFrameBuffer, bufferSize);
//	//			m_ptexture->UnlockRect(0);
//	//		}
//	//	}
//	//}
//
//	//pControl->Run(); // 再生を再開
//}
//
//
////==========================================================================
//// デストラクタ
////==========================================================================
//CMovie::~CMovie()
//{
//	// リソースの解放
//	pBasicVideo->Release();
//	pControl->Release();
//	pSeeking->Release();
//	pGraph->Release();
//	CoUninitialize();
//}
//
////==========================================================================
//// 生成処理
////==========================================================================
//CMovie* CMovie::Create()
//{
//	// メモリの確保
//	CMovie* pObj = DEBUG_NEW CMovie;
//
//	if (pObj != nullptr)
//	{
//		// 初期化処理
//		pObj->Init();
//	}
//
//	return pObj;
//}
//
////==========================================================================
//// 初期化処理
////==========================================================================
//HRESULT CMovie::Init()
//{
//
//	// オブジェクト2Dの初期化
//	CObject2D::Init();
//
//	// テクスチャ設定
//	int texID = CTexture::GetInstance()->Regist(TEXTURE_SAMPLE);
//	BindTexture(texID);
//
//	// サイズ設定
//	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
//
//#if 0	// 横幅を元にサイズ設定
//	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
//
//#else	// 縦幅を元にサイズ設定
//	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
//#endif
//	SetSize(D3DXVECTOR2(854 / 2, 480 / 2));
//	SetSizeOrigin(D3DXVECTOR2(854 / 2, 480 / 2));
//
//
//	// 位置、向き設定は必要があれば追加
//
//	SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));
//
//	// 種類の設定
//	SetType(CObject::TYPE::TYPE_OBJECT2D);
//
//	return S_OK;
//}
//
//
//
////==========================================================================
//// 終了処理
////==========================================================================
//void CMovie::Uninit()
//{
//	// 終了処理
//	CObject2D::Uninit();
//}
//
////==========================================================================
//// 更新処理
////==========================================================================
//void CMovie::Update()
//{
//	if (CInputKeyboard::GetInstance()->GetTrigger(DIK_1))
//	{
//		UpdateTextureFromVideoFrame(pBasicVideo);
//	}
//
//	// 更新処理
//	CObject2D::Update();
//}
//
////==========================================================================
//// 描画処理
////==========================================================================
//void CMovie::Draw()
//{
//	// デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
//
//	// 頂点バッファをデータストリームに設定
//	pDevice->SetStreamSource(0, GetVtxBuff(), 0, sizeof(VERTEX_2D));
//
//	// 頂点フォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	// テクスチャの設定
//	pDevice->SetTexture(0, m_ptexture);
//
//	// ポリゴンの描画
//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//}
