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
// 前方宣言
//==========================================================================
class CRenderTextureManager;
class CRenderTexture;
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

	// エイリアス定義
	using ACameraFunc = std::function<void()>;	// カメラ設定関数型
	using ADrawFunc = std::function<void()>;	// 描画関数型

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Draw();
	void DrawRenderTexture(LPDIRECT3DSURFACE9* pSurface, ADrawFunc pDrawFunc, ACameraFunc pSetCameraFunc);	// レンダーテクスチャ描画
	HRESULT CreateRenderTexture();				// レンダーテクスチャー生成
	void SetEnableShader(const bool bShader, const float fTime = 0.0f);	// スクリーンシェーダーフラグ設定
	bool IsShader() const;						// スクリーンシェーダーフラグ取得
	int GetRenderTextureIndex() const;			// レンダーテクスチャインデックス取得
	inline LPDIRECT3DDEVICE9 GetDevice() const	{ return m_pD3DDevice; }
	inline LPDIRECT3D9 GetD3D() const			{ return m_pD3D; }

private:
	// メンバ変数
	CRenderTextureManager*	m_pRenderTextureManager;	// レンダーテクスチャマネージャー
	CRenderTexture*			m_pRenderScene;		// シーンレンダーテクスチャ
	CScreen*				m_pDrawScreen;		// スクリーン描画ポリゴン
	LPDIRECT3DSURFACE9		m_pDefSurScreen;	// 元の描画サーフェイス保存用
	LPDIRECT3D9				m_pD3D;				// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice;		// Direct3Dデバイス
};

#endif
