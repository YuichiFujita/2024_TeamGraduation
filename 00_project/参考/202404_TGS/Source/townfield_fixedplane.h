//=============================================================================
// 
//  街フィールド(固定平面)ヘッダー [townfield_fixedplane.h]
//  Author : 相馬 靜雅
// 
//=============================================================================

#ifndef _TONWFIELD_FIXED_PLANE_H_
#define _TONWFIELD_FIXED_PLANE_H_	// 二重インクルード防止

#include "mapmesh.h"

//==========================================================================
// クラス定義
//==========================================================================
// 街フィールド(固定平面)クラス
class CTownField_FixedPlane : public CMapMesh
{

public:

	CTownField_FixedPlane(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CTownField_FixedPlane();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void BindVtxPosition() override;	// 頂点座標割り当て

private:

	virtual void SetVtxTexUV() override;	// 各頂点UV座標設定
};


// 街フィールド(固定平面/リザルト)クラス
class CTownField_FixedPlane_Result : public CTownField_FixedPlane
{
public:

	CTownField_FixedPlane_Result();
	~CTownField_FixedPlane_Result() {}

	// オーバーライドされた関数
	virtual HRESULT Init() override;
};


// 街フィールド(固定平面/ランキング)クラス
class CTownField_FixedPlane_Ranking : public CTownField_FixedPlane
{
public:

	CTownField_FixedPlane_Ranking();
	~CTownField_FixedPlane_Ranking() {}

	// オーバーライドされた関数
	virtual HRESULT Init() override;
};

#endif