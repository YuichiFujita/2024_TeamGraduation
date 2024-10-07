//=============================================================================
// 
//  オブジェクト3Dヘッダー [object3D.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_	// 二重インクルード防止

#include "object.h"

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクト3Dクラス定義
class CObject3D : public CObject
{
public:

	CObject3D(int nPriority = 5, const LAYER layer = LAYER::LAYER_DEFAULT);
	virtual ~CObject3D();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	virtual void SetVtx();
	virtual void BindTexture(int nIdx);

	void SetWorldMtx(const MyLib::Matrix mtx);		// マトリックス設定
	MyLib::Matrix GetWorldMtx() const;				// マトリックス取得
	void SetColor(const D3DXCOLOR col);				// 色設定
	D3DXCOLOR GetColor() const;						// 色取得
	void SetColorOrigin(const D3DXCOLOR& col) { m_colOrigin = col; }				// 色設定
	D3DXCOLOR GetColorOrigin() const { return m_colOrigin; }						// 色取得
	void SetAlpha(float a) { m_col.a = a; }			// 透明度設定
	void SetSize(const MyLib::Vector3& size);		// サイズの設定
	MyLib::Vector3 GetSize() const;					// サイズの取得
	void SetSizeOrigin(const MyLib::Vector3& size);	// 元のサイズの設定
	MyLib::Vector3 GetSizeOrigin() const;			// 元のサイズの取得
	virtual void SetTex(D3DXVECTOR2 *tex);			// テクスチャ座標の設定
	virtual D3DXVECTOR2 *GetTex();					// テクスチャ座標の取得

	static CObject3D *Create(int nPriority);
	static CObject3D *Create(MyLib::Vector3 pos, MyLib::Vector3 rot);
	CObject3D *GetObject3D();
	virtual CObject3DMesh *GetObject3DMesh();

protected:

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ

private:

	//=============================
	// メンバ変数
	//=============================
	MyLib::Matrix m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR2 m_fTex[32];		// テクスチャ座標
	D3DXCOLOR m_col;			// 色
	D3DXCOLOR m_colOrigin;			// 色
	MyLib::Vector3 m_fSize;		// サイズ
	int m_nTexIdx;				// テクスチャのインデックス番号
};



#endif