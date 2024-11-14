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

	CObject3D(int nPriority, const LAYER layer = LAYER::LAYER_DEFAULT);
	virtual ~CObject3D();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;	// 動的削除処理

	virtual void SetVtx();
	virtual void BindTexture(int nIdx);

	void SetWorldMtx(const MyLib::Matrix& mtx) { m_mtxWorld = mtx; }		// マトリックス設定
	MyLib::Matrix GetWorldMtx() const { return m_mtxWorld; }				// マトリックス取得
	void SetColor(const D3DXCOLOR& col) { m_col = col; }				// 色設定
	D3DXCOLOR GetColor() const { return m_col; }						// 色取得
	void SetColorOrigin(const D3DXCOLOR& col) { m_colOrigin = col; }				// 色設定
	D3DXCOLOR GetColorOrigin() const { return m_colOrigin; }						// 色取得
	void SetAlpha(float a) { m_col.a = a; }			// 透明度設定
	float GetAlpha() { return m_col.a; }			// 透明度取得

	void SetSize(const MyLib::Vector3& size) { m_size = size; }		// サイズ設定
	MyLib::Vector3 GetSize() const { return m_size; }					// サイズ取得
	void SetSizeOrigin(const MyLib::Vector3& size) { m_sizeOrigin = size; }	// 元のサイズ設定
	MyLib::Vector3 GetSizeOrigin() const { return m_sizeOrigin; }			// 元のサイズ取得
	

	//--------------------------
	// 頂点情報
	//--------------------------
	// UV
	void SetUV(const std::vector<D3DXVECTOR2>& uv) { m_vecUV = uv; }		// テクスチャ座標設定
	std::vector<D3DXVECTOR2> GetUV() { return m_vecUV; }					// テクスチャ座標取得

	static CObject3D *Create(int nPriority = 5);
	static CObject3D *Create(const MyLib::Vector3& pos, const MyLib::Vector3& rot);
	CObject3D* GetObject3D() { return this; }
	virtual CObject3DMesh* GetObject3DMesh() { return nullptr; }

protected:

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ

	// 仮想関数
	virtual void CalWorldMtx();	// ワールドマトリックスの計算処理

private:

	//=============================
	// メンバ変数
	//=============================
	MyLib::Matrix m_mtxWorld;	// ワールドマトリックス
	std::vector<D3DXVECTOR2> m_vecUV;		// テクスチャ座標
	D3DXCOLOR m_col;				// 色
	D3DXCOLOR m_colOrigin;			// 元の色
	MyLib::Vector3 m_size;			// サイズ
	MyLib::Vector3 m_sizeOrigin;	// 元のサイズ
	int m_nTexIdx;					// テクスチャのインデックス番号
};



#endif