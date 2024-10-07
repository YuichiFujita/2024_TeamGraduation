//=============================================================================
// 
// 当たり判定ボックス処理 [collisionLine_Box.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _COLLISIONLINE_BOX_H_
#define _COLLISIONLINE_BOX_H_		// 二重インクルード防止のマクロを定義する

#include "objectLine.h"

//==========================================================================
// クラス定義
//==========================================================================
// 当たり判定ボックスクラス
class CCollisionLine_Box : public CObject
{
public:

	CCollisionLine_Box(int nPriority = mylib_const::PRIORITY_DEF2D, const LAYER layer = LAYER::LAYER_LINE);
	~CCollisionLine_Box();

	//  オーバーライド関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetPosition(const MyLib::Vector3& pos) override;	// 位置設定

	void SetColor(const D3DXCOLOR& col);			// 色設定

	void SetAABB(MyLib::AABB aabb, float scale = 1.0f);	// AAbB情報設定
	MyLib::AABB GetAABB() { return m_AABB; }
	void Kill();

	static CCollisionLine_Box* Create(const MyLib::AABB& aabb, const D3DXCOLOR& col);

private:

	//=============================
	// メンバ変数
	//=============================
	MyLib::AABB m_AABB;
	D3DXCOLOR m_LineColor;
	CObjectLine* m_pLine[12];	// ラインのオブジェクト
};
#endif