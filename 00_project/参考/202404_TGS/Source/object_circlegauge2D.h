//=============================================================================
// 
//  オブジェクト円ゲージ(2D)ヘッダー [object_circlegauge2D.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECT_CIRCLEGAUGE2D_H_
#define _OBJECT_CIRCLEGAUGE2D_H_	// 二重インクルード防止

#include "object.h"

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクト円ゲージ(2D)クラス定義
class CObjectCircleGauge2D : public CObject
{
public:

	CObjectCircleGauge2D(int nPriority = 7, const LAYER layer = LAYER::LAYER_2D);
	~CObjectCircleGauge2D();

	// オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	virtual void SetVtx();

	void SetColor(const D3DXCOLOR col);		// 色設定
	D3DXCOLOR GetColor() const;			// 色取得
	void SetRate(const float fRate);		// 割合設定
	float GetRate() const;				// 割合取得
	void SetRateDest(const float fRate);	// 目標の割合設定
	float GetRateDest() const;			// 目標の割合取得
	void SetSize(float fSize) { m_fSize = fSize; }	// サイズ設定
	float GetSize() { return m_fSize; }

	void BindTexture(int nIdx);	// テクスチャのインデックス番号割り当て
	static CObjectCircleGauge2D *Create(int nPolygon, float fSize);
protected:

private:

	D3DXCOLOR m_col;	// 色
	float m_fSize;		// サイズ
	float m_fRate;		// 割合
	float m_fRateDest;	// 目標の割合
	int m_nNumVertex;	// 頂点数
	int m_nTexIdx;		// テクスチャのインデックス番号
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
};

#endif