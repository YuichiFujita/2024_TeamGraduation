//=============================================================================
// 
//  街フィールド(うねり)ヘッダー [townfield_sinuous.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TONWFIELD_SINUOUS_H_
#define _TONWFIELD_SINUOUS_H_	// 二重インクルード防止

#include "mapmesh.h"

//==========================================================================
// クラス定義
//==========================================================================
// 街フィールド(うねり)クラス
class CTownField_Sinuous : public CMapMesh
{

public:

	CTownField_Sinuous(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CTownField_Sinuous();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void BindVtxPosition() override;	// 頂点座標割り当て

private:

	virtual void SetVtxTexUV() override;	// 各頂点UV座標設定
};



#endif