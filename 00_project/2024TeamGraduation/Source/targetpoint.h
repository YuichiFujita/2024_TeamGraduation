//=============================================================================
// 
//  目標地点ヘッダー [targetpoint.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TARGETPOINT_H_
#define _TARGETPOINT_H_	// 二重インクルード防止

#include "main.h"
#include "object3DMesh.h"

//==========================================================================
// クラス定義
//==========================================================================
// 目標地点クラス定義
class CTargetPoint : public CObject3DMesh
{
public:
	CTargetPoint(int nPriority = 6);
	~CTargetPoint();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	void SetVtx() override;

	void Control();	// 操作
	static CTargetPoint *Create(MyLib::Vector3 pos, float fWidthLen, float fHeightLen);
	CTargetPoint *GetObject3DMesh();

private:
	int m_nTexIdx;	// テクスチャのインデックス番号
};



#endif