//==========================================================================
// 
//  レンダーテクスチャマネージャー処理 [renderTextureManager.cpp]
//  Author : 藤田勇一
// 
//==========================================================================
#include "renderTextureManager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// 静的メンバ変数
//==========================================================================
CRenderTextureManager* CRenderTextureManager::m_pInstance = nullptr;	// 自身のインスタンス

//==========================================================================
// コンストラクタ
//==========================================================================
CRenderTextureManager::CRenderTextureManager()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CRenderTextureManager::~CRenderTextureManager()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CRenderTextureManager* CRenderTextureManager::Create()
{
	// 既に生成済みの場合抜ける
	if (m_pInstance != nullptr) { return m_pInstance; }

	// インスタンスの生成
	m_pInstance = DEBUG_NEW CRenderTextureManager;
	if (m_pInstance != nullptr)
	{ // メモリの確保が出来た場合

		// インスタンスの初期化
		if (FAILED(m_pInstance->Init()))
		{ // 初期化に失敗した場合

			return nullptr;
		}
	}

	// インスタンスを返す
	return m_pInstance;
}

//==========================================================================
// インスタンス取得処理
//==========================================================================
CRenderTextureManager* CRenderTextureManager::GetInstance()
{
	// インスタンスを返す
	return m_pInstance;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CRenderTextureManager::Init()
{
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CRenderTextureManager::Uninit()
{
	// 自身のインスタンスの破棄
	SAFE_DELETE(m_pInstance);
}

//==========================================================================
// 描画処理
//==========================================================================
void CRenderTextureManager::Draw()
{
	for (int i = 0; i < ELayer::LAYER_MAX; i++)
	{ // レイヤーの総数分繰り返す

		int nIdxLoop = 0;	// ループ数
		for (auto& rItrRenTex : m_vecRenderTexture[i])
		{ // 配列の要素数分繰り返す

			// レンダーテクスチャの描画
			rItrRenTex->Draw();
		}
	}
}

//==========================================================================
// レンダーテクスチャ登録処理
//==========================================================================
void CRenderTextureManager::RegistRenderTexture(CRenderTexture* pRenderTexture, const ELayer layer)
{
	// 引数レイヤーの最後尾にレンダーテクスチャを追加
	m_vecRenderTexture[layer].push_back(pRenderTexture);
}

//==========================================================================
// レンダーテクスチャ削除処理
//==========================================================================
HRESULT CRenderTextureManager::DeleteRenderTexture(CRenderTexture* pRenderTexture)
{
	for (int i = 0; i < ELayer::LAYER_MAX; i++)
	{ // レイヤーの総数分繰り返す

		int nIdxLoop = 0;	// ループ数
		for (auto& rItrRenTex : m_vecRenderTexture[i])
		{ // 配列の要素数分繰り返す

			// 引数レンダーテクスチャと違う場合次へ
			if (rItrRenTex != pRenderTexture) { nIdxLoop++; continue; }

			// レンダーテクスチャを動的配列から削除
			m_vecRenderTexture[i].erase(m_vecRenderTexture[i].begin() + nIdxLoop);

			return S_OK;
		}
	}

	// ここに来たら無いレンダーテクスチャを削除しようとしてるよ
	assert(false);
	return E_FAIL;
}
