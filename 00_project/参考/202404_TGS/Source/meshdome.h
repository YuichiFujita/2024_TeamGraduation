//=============================================================================
// 
//  メッシュドームヘッダー [meshdome.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MESHDOME_H_
#define _MESHDOME_H_	// 二重インクルード防止

#include "main.h"
#include "object3DMesh.h"

//==========================================================================
// クラス定義
//==========================================================================
// メッシュドームクラス定義
class CMeshDome : public CObject3DMesh
{
public:
	CMeshDome(int nPriority = 2, const LAYER layer = LAYER::LAYER_MAP);
	~CMeshDome();

	// オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVtx();

	static CMeshDome *Create();
	static CMeshDome *Create(const float fMove, const char *aFileName = nullptr, const int nWidth = 16, const int nHeight = 8, int nPriority = mylib_const::PRIORITY_DEFAULT);
	CMeshDome *GetMyObject();

private:
	float m_fMove;	// 回転移動量
	int m_nTexIdx;	// テクスチャのインデックス番号
};



#endif