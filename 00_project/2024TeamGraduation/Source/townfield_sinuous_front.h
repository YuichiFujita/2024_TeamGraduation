//=============================================================================
// 
//  街フィールド(うねり)ヘッダー [townfield_sinuous_front.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TONWFIELD_SINUOUS_FRONT_H_
#define _TONWFIELD_SINUOUS_FRONT_H_	// 二重インクルード防止

#include "townfield_sinuous.h"

//==========================================================================
// クラス定義
//==========================================================================
// 街フィールド(うねり)クラス
class CTownField_Sinuous_Front : public CTownField_Sinuous
{

public:

	CTownField_Sinuous_Front(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CTownField_Sinuous_Front();

	// オーバーライドされた関数
	virtual void BindVtxPosition() override;	// 頂点座標割り当て

private:

	virtual void SetVtxTexUV() override;	// 各頂点UV座標設定
};



#endif