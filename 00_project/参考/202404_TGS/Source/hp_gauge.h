//=============================================================================
// 
//  体力ゲージヘッダー [hp_gauge.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _HP_GAUGE_H_
#define _HP_GAUGE_H_	// 二重インクルード防止

#include "objectBillboard.h"

class CHPGaugeTip;

//==========================================================================
// クラス定義
//==========================================================================
// 体力ゲージクラス定義
class CHP_Gauge : public CObject
{
public:

	CHP_Gauge(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CHP_Gauge();

	static CHP_Gauge *Create(float fPosLen, int nMaxLife, float fSizeBuff = 1.0f);

	// オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVtx();
	void SetVtx(int nCntGauge);

	void Kill();
	void UpdatePosition(MyLib::Vector3 pos, int nLife);

private:

	// 列挙型定義
	typedef enum
	{
		VTXTYPE_BLACK = 0,	// 黒ゲージ
		VTXTYPE_PINK,		// ピンクゲージ
		VTXTYPE_MAX
	}VTXTYPE;

	// 構造体定義
	struct SHP_Gauge
	{
		CObjectBillboard *pObjBillboard;	// ビルボードのオブジェクト
		float fMaxWidth;					// 幅の最大値
		float fWidthDest;					// 幅の差分
		float fMaxHeight;					// 高さの最大値
	};

	void ChangeColor(int nCntGauge);		// 色変更
	void GaugeDecrement(int nCntGauge);		// 減少処理

	static int m_nNumAll;					// 総数
	static const char *m_apTextureFile[];	// テクスチャのファイル
	SHP_Gauge m_HPGauge[VTXTYPE_MAX];		// HPゲージの情報
	float m_fPosLength;						// 原点からの長さ
	int m_nLife;							// 体力
	int m_nMaxLife;							// 最大体力
	int m_nTexIdx[VTXTYPE_MAX];				// テクスチャのインデックス番号
	CHPGaugeTip* m_pTip;	// ゲージの先端
};


#endif