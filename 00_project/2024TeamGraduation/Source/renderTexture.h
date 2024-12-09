//============================================================
//
//	レンダーテクスチャヘッダー [renderTexture.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "renderTextureManager.h"
#include "renderer.h"

//************************************************************
//	クラス定義
//************************************************************
// レンダーテクスチャクラス
class CRenderTexture
{
public:
	// コンストラクタ
	CRenderTexture(const CRenderTextureManager::ELayer layer);

	// デストラクタ
	~CRenderTexture();

	// メンバ関数
	HRESULT Init();	// 初期化
	void Uninit();	// 終了
	void Draw();	// 描画
	inline void SetDrawFunc(CRenderer::ADrawFunc pDrawFunc) { m_pDrawFunc = pDrawFunc; }	// 描画関数設定
	inline int GetTextureIndex() const { return m_nTextureIdx; }	// テクスチャインデックス取得

	// 静的メンバ関数
	static CRenderTexture *Create(const CRenderTextureManager::ELayer layer, CRenderer::ADrawFunc pDrawFunc);	// 生成
	static void Release(CRenderTexture*& prRenderTexture);	// 破棄

private:
	// メンバ変数
	const CRenderTextureManager::ELayer m_layer;	// レンダーテクスチャレイヤー
	LPDIRECT3DSURFACE9 m_pSurTexture;	// テクスチャサーフェイスへのポインタ
	CRenderer::ADrawFunc m_pDrawFunc;	// オブジェクト描画関数
	int m_nTextureIdx;		// レンダーテクスチャインデックス
};

#endif	// _RENDER_TEXTURE_H_
