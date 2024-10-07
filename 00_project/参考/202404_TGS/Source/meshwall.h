//=============================================================================
// 
//  メッシュウォールヘッダー [meshwall.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MESHWALL_H_
#define _MESHWALL_H_	// 二重インクルード防止

#include "object3DMesh.h"

//==========================================================================
// クラス定義
//==========================================================================
// メッシュフィールドクラス定義
class CMeshWall : public CObject3DMesh
{
public:

	CMeshWall(int nPriority = 1);
	~CMeshWall();

	// オーバーライドされた関数
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void SetVtx();

	static CMeshWall *Create(MyLib::Vector3 pos, MyLib::Vector3 rot, float fWidthLen, float fHeightLen, int nWidth = 1, int nHeight = 1, int nPriority = 1, const char *aFileName = nullptr);
	virtual CMeshWall *GetMyObject();
protected:

private:
	int m_nTexIdx;	// テクスチャのインデックス番号
};



#endif