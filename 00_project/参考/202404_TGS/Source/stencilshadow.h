//=============================================================================
// 
//  ステンシルバッファ使用影[stencilshadow.h]
//  Author : 石原颯馬
// 
//=============================================================================

#ifndef _STENCIL_SHADOW_H_
#define _STENCIL_SHADOW_H_		// 二重インクルード防止

#include "object2D.h"

//==========================================================================
// クラス定義
//==========================================================================
//チェックポイントクラス
class CStencilShadow : public CObject2D
{
public:

	CStencilShadow(int nPriority = 6);
	~CStencilShadow();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	// メンバ関数
	void Kill();		// 削除

	//=============================
	// 静的関数
	//=============================
	static CStencilShadow* Create();

private:

};


#endif