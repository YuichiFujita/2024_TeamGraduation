//==========================================================================
// 
//  レンダラーヘッダー [renderer.h]
//  Author : 相馬靜雅
//  Adder  : 藤田勇一
// 
//==========================================================================

#ifndef _RENDERER_H_
#define _RENDERER_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "renderTexture.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CScreen;

//==========================================================================
// クラス定義
//==========================================================================
// レンダラークラス定義
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Draw();
	HRESULT CreateRenderTexture();							// レンダーテクスチャー生成
	void DrawRenderTexture(LPDIRECT3DSURFACE9* pSurface);	// レンダーテクスチャ描画
	void SetEnableShader(const bool bShader);				// スクリーンシェーダーフラグ設定
	bool IsShader() const;									// スクリーンシェーダーフラグ取得
	inline LPDIRECT3DDEVICE9 GetDevice() const	{ return m_pD3DDevice; }
	inline LPDIRECT3D9 GetD3D() const			{ return m_pD3D; }
	inline int GetRenderTextureIndex() const	{ return m_pRenderScene->GetTextureIndex(); }	// レンダーテクスチャインデックス取得

private:
	// メンバ変数
	CRenderTexture*		m_pRenderScene;		// シーンレンダーテクスチャ
	CScreen*			m_pDrawScreen;		// スクリーン描画ポリゴン
	LPDIRECT3DSURFACE9	m_pDefSurScreen;	// 元の描画サーフェイス保存用
	LPDIRECT3D9			m_pD3D;				// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9	m_pD3DDevice;		// Direct3Dデバイス
};


#endif