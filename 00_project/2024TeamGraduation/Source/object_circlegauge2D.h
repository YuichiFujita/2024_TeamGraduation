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

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CObjectCircleGauge2D(int nPriority = 7, const LAYER layer = LAYER::LAYER_2D);
	~CObjectCircleGauge2D();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	virtual void Kill() override;	// 動的削除処理
	virtual void SetVtx() override;

	//=============================
	// メンバ関数
	//=============================
	void BindTexture(int nIdx) { m_nTexIdx = nIdx; }				// テクスチャのインデックス番号割り当て
	void SetColor(const D3DXCOLOR& col) { m_col = col; }			// 色設定
	D3DXCOLOR GetColor() const { return m_col; }					// 色取得
	void SetRate(const float fRate) { m_fRate = fRate; }			// 割合設定
	float GetRate() const { return m_fRate; }						// 割合取得
	void SetRateDest(const float fRate) { m_fRateDest = fRate; }	// 目標の割合設定
	float GetRateDest() const { return m_fRateDest; }				// 目標の割合取得
	void SetSize(float fSize) { m_fSize = fSize; }					// サイズ設定
	float GetSize() { return m_fSize; }								// サイズ取得

	//=============================
	// 静的メンバ関数
	//=============================
	static CObjectCircleGauge2D *Create(int nPolygon, float fSize);

private:

	//=============================
	// メンバ変数
	//=============================
	D3DXCOLOR m_col;	// 色
	float m_fSize;		// サイズ
	float m_fRate;		// 割合
	float m_fRateDest;	// 目標の割合
	int m_nNumVertex;	// 頂点数
	int m_nTexIdx;		// テクスチャのインデックス番号
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
};

#endif