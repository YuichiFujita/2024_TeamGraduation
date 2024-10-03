//=============================================================================
// 
//  オブジェクト2Dゲージヘッダー [object2D_gauge.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECT2D_GAUGE_H_
#define _OBJECT2D_GAUGE_H_	// 二重インクルード防止

#include "object2D.h"

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクト2Dゲージクラス定義
class CObject2D_Gauge : public CObject2D
{
public:

	CObject2D_Gauge(int nPriority = 6);
	virtual ~CObject2D_Gauge();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual HRESULT Init(float width, float height, int maxvalue, std::string texturepath = "");
	virtual void Update() override;
	virtual void SetVtx() override;


	// 強化関数
	MyLib::Vector3 UpgradeMaxValue(int addvalue, bool bChangePos);	// 最大値のアップグレード

	// その他関数
	void SetValue(int value);				// 値設定
	int GetValue() { return m_nValue; }	// 値取得
	void SetMaxValue(int value) { m_nMaxValue = value; }	// 値の最大値設定
	int GetMaxValue() { return m_nMaxValue; }			// 値の最大値取得
	void SetMaxWidth(float width) { m_fMaxWidth = width; }	// 幅の最大値設定
	float GetMaxWidth() { return m_fMaxWidth; }			// 幅の最大値取得
	void SetMoveFactor(float factor) { m_fMoveFactor = factor; }	// 移動の係数設定

	// 静的関数
	static CObject2D_Gauge *Create(float width, float height, int maxvalue, std::string texturepath = "", int nPriority = 6);	// 生成処理
	CObject2D_Gauge *GetObject2Dgauge() { return this; }
protected:

private:
	
	//=============================
	// メンバ関数
	//=============================
	void GaugeDecrement();

	//=============================
	// メンバ変数
	//=============================
	float m_fOriginWidth;	// 幅の最大値
	float m_fMaxWidth;		// 幅の最大値
	float m_fWidthDest;		// 目標の幅
	float m_fMaxHeight;		// 高さの最大値
	float m_fMoveFactor;	// 移動の係数
	int m_nValue;			// 現在の値
	int m_nOriginValue;		// 初期値
	int m_nMaxValue;		// 最大値
};



#endif