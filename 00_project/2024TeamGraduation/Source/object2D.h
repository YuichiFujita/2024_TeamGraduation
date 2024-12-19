//=============================================================================
// 
//  オブジェクト2Dヘッダー [object2D.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_	// 二重インクルード防止

#include "object.h"

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクト2Dクラス定義
class CObject2D : public CObject
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum AnchorPoint
	{
		LEFT = 0,		// 左
		TOP_LEFT,		// 左上
		TOP_CENTER,		// 上
		TOP_RIGHT,		// 右上
		RIGHT,			// 右
		UNDER_RIGHT,	// 右下
		UNDER_CENTER,	// 下
		UNDER_LEFT,		// 左下
		CENTER,			// 中央
		MAX
	};

	CObject2D(int nPriority, const LAYER layer = LAYER::LAYER_2D);
	virtual ~CObject2D();

	// オーバーライドされた関数
	HRESULT Init() override;
	HRESULT Init(int nNumVertex);
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	virtual void Kill() override;	// 動的削除処理


	virtual void SetVtx();
	virtual void SetVtx(int nNumVertex);
	void BindTexture(int nIdx) { m_nTexIdx = nIdx; }	// テクスチャインデックス設定
	int GetIdxTexture() { return m_nTexIdx; }			// テクスチャインデックス取得
	
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
	void SetTex(D3DXVECTOR2* tex) { memcpy(&m_vecUV[0], tex, sizeof(m_vecUV)); }	// テクスチャ座標の設定
	void SetTexUV(const std::vector<D3DXVECTOR2>& uv);								// テクスチャ座標の設定
	D3DXVECTOR2* GetTex() { return &m_vecUV[0]; }									// テクスチャ座標のポインタ取得
	std::vector<D3DXVECTOR2> GetVecTexUV() { return m_vecUV; }						// テクスチャ座標の取得
	
	// 頂点座標
	void SetVecVtxPos(const std::vector<MyLib::Vector3>& vecpos) { m_vecVtxPos = vecpos; }	// 頂点座標設定
	MyLib::Vector3* GetVtxPos() { return &m_vecVtxPos[0]; }									// 頂点座標のポインタ取得
	std::vector<MyLib::Vector3> GetVecVtxPos() { return m_vecVtxPos; }						// 頂点座標取得
	int GetNumVertex() { return m_nNumVertex; }							// 頂点数

	// 頂点バッファ
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff() const { return m_pVtxBuff; }	// 頂点バッファ取得

	//--------------------------
	// アンカーポイント
	//--------------------------
	void SetAnchorType(const AnchorPoint& type) { m_AnchorType = type; }	// アンカーポイントの種類設定
	AnchorPoint GetAnchorType() { return m_AnchorType; }					// アンカーポイントの種類取得

	static CObject2D *Create(int nPriority = mylib_const::PRIORITY_DEF2D, int nNumVtx = 4);	// 生成処理

private:

	//=============================
	// メンバ関数
	//=============================
	MyLib::Vector3 RotateVtx(const MyLib::Vector3& vtx, const MyLib::Vector3& center);	// 頂点回転処理

	//=============================
	// メンバ変数
	//=============================
	D3DXCOLOR m_col;							// 色
	D3DXCOLOR m_colOrigin;						// 元の色
	D3DXVECTOR2 m_size;							// サイズ
	D3DXVECTOR2 m_sizeOrigin;					// 元のサイズ
	AnchorPoint m_AnchorType;					// アンカーポイントの種類
	float m_fLength;							// 対角線の長さ
	float m_fAngle;								// 対角線の向き
	int m_nTexIdx;								// テクスチャのインデックス番号
	int m_nNumVertex;							// 頂点数
	std::vector<D3DXVECTOR2> m_vecUV;			// テクスチャ座標
	std::vector<MyLib::Vector3> m_vecVtxPos;	// 頂点座標
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファ
};

#endif