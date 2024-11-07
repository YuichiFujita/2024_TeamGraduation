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
//	クラス定義
//************************************************************
// レンダーテクスチャクラス
class CRenderTexture
{
public:
	// コンストラクタ
	CRenderTexture();

	// デストラクタ
	~CRenderTexture();

	// メンバ関数
	HRESULT Init();	// 初期化
	void Uninit();	// 終了
	void Draw();	// 描画
	inline int GetTextureIndex() const { return m_nTextureIdx; }	// テクスチャインデックス取得

	// 静的メンバ関数
	static CRenderTexture *Create();	// 生成
	static void Release(CRenderTexture*& prRenderTexture);	// 破棄

private:
	// メンバ変数
	LPDIRECT3DSURFACE9 m_pSurTexture;	// テクスチャサーフェイスへのポインタ
	int m_nTextureIdx;					// レンダーテクスチャインデックス
};

#endif	// _RENDER_TEXTURE_H_
