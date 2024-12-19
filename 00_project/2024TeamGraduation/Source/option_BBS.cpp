//=============================================================================
// 
//  オプションの掲示板処理 [option_BBS.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "option_BBS.h"
#include "renderTexture.h"
#include "balloonFrame.h"
#include "camera.h"
#include "objectX.h"
#include "optionMenu.h"

#if 0
#define EDIT()	// 2Dでエディット
#endif

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string MODEL = "data\\MODEL\\title\\BBS.x";			// 掲示板のモデル
	const MyLib::Vector3 DEFAULT_POSITION = MyLib::Vector3(0.0f, 80.0f, 0.0f);	// 初期位置
}

//==========================================================================
// コンストラクタ
//==========================================================================
COption_BBS::COption_BBS(int nPriority) : CObjectX(nPriority),
	m_pRenderScene	(nullptr),	// シーンレンダーテクスチャ
	m_pOptionMenu	(nullptr)	// オプションメニュー
{

}

//==========================================================================
// デストラクタ
//==========================================================================
COption_BBS::~COption_BBS()
{

}

//==========================================================================
// 生成処理
//==========================================================================
COption_BBS* COption_BBS::Create()
{
	// メモリの確保
	COption_BBS* pObj = DEBUG_NEW COption_BBS;
	if (pObj != nullptr)
	{
		// クラスの初期化
		if (FAILED(pObj->Init()))
		{ // 初期化に失敗した場合

			// クラスの終了
			SAFE_UNINIT(pObj);
			return nullptr;
		}
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT COption_BBS::Init()
{
	// オブジェクトXの初期化
	if (FAILED(CObjectX::Init(MODEL)))
	{ // 初期化に失敗した場合
		return E_FAIL;
	}

	// 位置と向き設定
	SetPosition(DEFAULT_POSITION);
	SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));

	// レンダーテクスチャの生成
	if (FAILED(CreateRenderTexture()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// テクスチャ用オブジェクトの生成
	if (FAILED(CreateTextureObject()))
	{ // 生成に失敗した場合

		return E_FAIL;
	}

	// テクスチャインデックスの設定
	int* pIdxTexture = GetIdxTexture();
	pIdxTexture[0] = m_pRenderScene->GetTextureIndex();
	BindTexture(pIdxTexture);

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECTX);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void COption_BBS::Uninit()
{
	// レンダーテクスチャの破棄
	SAFE_REF_RELEASE(m_pRenderScene);


	// オブジェクトXの終了
	CObjectX::Uninit();
}

//==========================================================================
// 削除処理
//==========================================================================
void COption_BBS::Kill()
{
	
	// オブジェクトXの終了
	CObjectX::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void COption_BBS::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// 掲示板の更新
	assert(m_pOptionMenu != nullptr);
#ifndef EDIT
	m_pOptionMenu->Update(fDeltaTime, fDeltaRate, fSlowRate);
#endif

	// オブジェクトXの更新
	CObjectX::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// 描画処理
//==========================================================================
void COption_BBS::Draw()
{
	// オブジェクトXの描画
	CObjectX::Draw();
}

//==========================================================================
// レンダーテクスチャ生成処理
//==========================================================================
HRESULT COption_BBS::CreateRenderTexture()
{
	// シーンレンダーテクスチャの生成
	m_pRenderScene = CRenderTexture::Create
	( // 引数
		CRenderTextureManager::LAYER_BBS,						// 描画順レイヤー
		std::bind(&COption_BBS::CreateTexture, this),			// テクスチャ作成関数ポインタ
		std::bind(&CCamera::SetCamera, GET_MANAGER->GetCamera())	// カメラ設定関数ポインタ
	);
	if (m_pRenderScene == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// テクスチャ用オブジェクト生成処理
//==========================================================================
HRESULT COption_BBS::CreateTextureObject()
{
	// オプションメニュー生成
	m_pOptionMenu = COptionMenu::Create();
#ifdef EDIT
	m_pOptionMenu->SetType(CObject::TYPE::TYPE_OBJECT2D);
#else
	m_pOptionMenu->SetType(CObject::TYPE::TYPE_NONE);
#endif // EDIT

	return S_OK;
}

//==========================================================================
// テクスチャ作成処理
//==========================================================================
void COption_BBS::CreateTexture()
{
	// オプションメニューの描画
	assert(m_pOptionMenu != nullptr);
	m_pOptionMenu->Draw();

}
