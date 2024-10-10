//=============================================================================
// 
//  ビルボードヘッダー [objectBillboard.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECTBILLBOARD_H_
#define _OBJECTBILLBOARD_H_	// 二重インクルード防止

#include "main.h"
#include "object.h"
#include "constans.h"

//==========================================================================
// クラス定義
//==========================================================================
// ビルボードクラス定義
class CObjectBillboard : public CObject
{
public:

	CObjectBillboard(int nPriority = mylib_const::PRIORITY_ZSORT);
	~CObjectBillboard();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	virtual void SetVtx();

	//--------------------------
	// 変形パラメータ
	//--------------------------
	virtual void SetWorldMtx(const MyLib::Matrix& mtx) { m_mtxWorld = mtx; }	// マトリックス設定
	virtual MyLib::Matrix GetWorldMtx() const { return m_mtxWorld; }			// マトリックス取得

	//--------------------------
	// 色
	//--------------------------
	virtual void SetColor(const D3DXCOLOR& col) { m_col = col; }					// 色設定
	virtual D3DXCOLOR GetColor() const { return m_col; }							// 色取得
	virtual void SetOriginColor(const D3DXCOLOR& col) { m_colOrigin = col; }		// 元の色設定
	virtual D3DXCOLOR GetOriginColor() const { return m_colOrigin; }				// 元の色取得
	virtual void SetAlpha(const float alpha) { m_col.a = alpha; }					// 不透明度設定
	virtual float GetAlpha() const { return m_col.a; }								// 不透明度取得
	
	//--------------------------
	// サイズ関連
	//--------------------------
	virtual void SetSize(const D3DXVECTOR2& size);									// サイズの設定
	virtual D3DXVECTOR2 GetSize() const { return m_size; }							// サイズの取得
	virtual void SetSizeOrigin(const D3DXVECTOR2& size) { m_sizeOrigin = size; }	// 元のサイズの設定
	virtual D3DXVECTOR2 GetSizeOrigin() const { return m_sizeOrigin; }				// 元のサイズの取得

	// 対角線
	void SetLength(const float fLength) { m_fLength = fLength; }					// 対角線の長さ設定
	float GetLength() const { return m_fLength; }									// 対角線の長さ取得
	void SetAngle(const float fAngle) { m_fAngle = fAngle; }						// 対角線の向き設定
	float GetAngle() const { return m_fAngle; }										// 対角線の向き取得

	//--------------------------
	// 頂点情報
	//--------------------------
	// UV
	void SetUV(const std::vector<D3DXVECTOR2>& uv) { m_vecUV = uv; }		// テクスチャ座標設定
	std::vector<D3DXVECTOR2> GetUV() { return m_vecUV; }					// テクスチャ座標取得

	void BindTexture(int nIdx) { m_nTexIdx = nIdx; }	// テクスチャ割り当て
	static CObjectBillboard *Create(int nPriority = mylib_const::PRIORITY_ZSORT);
	static CObjectBillboard *Create(const MyLib::Vector3& pos, const MyLib::Vector3& rot);
	CObjectBillboard* GetObjectBillBoard() { return this; }				// オブジェクトビルボード取得
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff() const { return m_pVtxBuff; }	// 頂点バッファ取得

private:

	//=============================
	// メンバ変数
	//=============================
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	MyLib::Matrix m_mtxWorld;			// ワールドマトリックス
	D3DXCOLOR m_col;					// 色
	D3DXCOLOR m_colOrigin;				// 元の色
	D3DXVECTOR2 m_size;					// サイズ
	D3DXVECTOR2 m_sizeOrigin;			// 元のサイズ
	float m_fLength;					// 対角線の長さ
	float m_fAngle;						// 対角線の向き
	std::vector<D3DXVECTOR2> m_vecUV;	// テクスチャ座標
	int m_nTexIdx;						// テクスチャのインデックス番号
};



#endif