//=============================================================================
// 
//  メッシュシリンダーヘッダー [meshcylinder.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_	// 二重インクルード防止

#include "main.h"
#include "object3DMesh.h"

//==========================================================================
// クラス定義
//==========================================================================
// メッシュシリンダークラス定義
class CMeshCylinder : public CObject3DMesh
{
public:
	CMeshCylinder(int nPriority = 2);
	~CMeshCylinder();

	// オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVtx();

	static CMeshCylinder *Create();
	static CMeshCylinder *Create(const char *aFileName = nullptr);
	CMeshCylinder *GetMyObject();

private:

};



#endif