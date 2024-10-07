//=============================================================================
// 
//  オブジェクトラインヘッダー [objectLine.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECT3D_ANIM_H_
#define _OBJECT3D_ANIM_H_	// 二重インクルード防止

#include "object.h"

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクトラインクラス
class CObjectLine : public CObject
{
public:

	//=============================
	// 構造体定義
	//=============================
	struct SLineInfo
	{
		MyLib::Vector3 left;
		MyLib::Vector3 right;
		D3DXCOLOR col;

		SLineInfo() : left(0.0f), right(0.0f), col(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)) {}
	};

	CObjectLine(int nPriority = 8, const LAYER layer = LAYER::LAYER_LINE);
	virtual ~CObjectLine();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void SetVtx() override;

	void SetLeftPosition(const MyLib::Vector3& pos) { m_Info.left = pos; }
	void SetRightPosition(const MyLib::Vector3& pos) { m_Info.right = pos; }
	void SetColor(const D3DXCOLOR& col) { m_Info.col = col; }			// 色設定
	void SetWorldMtx(const MyLib::Matrix mtx) { m_mtxWorld = mtx; }		// マトリックス設定
	MyLib::Matrix GetWorldMtx() const { return m_mtxWorld; }			// マトリックス取得

	void SetEnableZSort(bool bSort) { m_bZSort = bSort; }

	static CObjectLine *Create(MyLib::Vector3 left, MyLib::Vector3 right, D3DXCOLOR col);

private:

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	MyLib::Matrix m_mtxWorld;			// ワールドマトリックス
	SLineInfo m_Info;
	bool m_bZSort;				// Zソートのフラグ
};



#endif