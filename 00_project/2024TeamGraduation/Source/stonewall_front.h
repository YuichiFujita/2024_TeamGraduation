//=============================================================================
// 
//  石垣ヘッダー [stonewall_front.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _STONEWALL_FRONT_H_
#define _STONEWALL_FRONT_H_	// 二重インクルード防止

#include "stonewall.h"

//==========================================================================
// クラス定義
//==========================================================================
// 石垣クラス
class CStoneWall_Front : public CStoneWall
{

public:

	CStoneWall_Front(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CStoneWall_Front();

	// オーバーライドされた関数
	virtual void BindVtxPosition() override;	// 頂点座標割り当て

	// メンバ関数
	virtual void DispPoint() override;	// 点表示

	static CStoneWall_Front *Create();	// 生成

private:

	//=============================
	// メンバ関数
	//=============================
	virtual void SetVtxTexUV() override;	// 各頂点UV座標設定

};



#endif