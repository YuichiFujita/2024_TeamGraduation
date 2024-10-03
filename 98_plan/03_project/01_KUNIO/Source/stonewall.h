//=============================================================================
// 
//  石垣ヘッダー [stonewall.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _STONEWALL_H_
#define _STONEWALL_H_	// 二重インクルード防止

#include "mapmesh.h"

//==========================================================================
// クラス定義
//==========================================================================
// 石垣クラス
class CStoneWall : public CMapMesh
{

public:

	CStoneWall(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CStoneWall();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void BindVtxPosition() override;	// 頂点座標割り当て

	std::vector<MyLib::Vector3> GetVecTopPosition() { return m_vecTopPosition; }	// 頂上の位置取得
	virtual void DispPoint();	// 点表示

	static CStoneWall *Create();	// 生成

protected:

	std::vector<MyLib::Vector3> m_vecTopPosition;	// 頂上の位置

private:

	//=============================
	// メンバ関数
	//=============================
	virtual void SetVtxTexUV() override;	// 各頂点UV座標設定


};



#endif