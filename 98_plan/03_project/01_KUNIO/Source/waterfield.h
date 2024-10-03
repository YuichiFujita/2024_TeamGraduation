//=============================================================================
// 
//  水フィールドヘッダー [waterfield.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _WATERFIELD_H_
#define _WATERFIELD_H_	// 二重インクルード防止

#include "object3DMesh.h"

//==========================================================================
// クラス定義
//==========================================================================
// 水フィールドクラス
class CWaterField : public CObject3DMesh
{
public:

	enum TYPE
	{
		TYPE_NORMAL = 0,
		TYPE_RIGHT,
		TYPE_LEFT,
		TYPE_MAX
	};

	CWaterField(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CWaterField();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void SetVtx() override;

	static CWaterField *Create(TYPE type);

protected:

	float m_fTexU;	// Uスクロール用
	float m_fTexV;	// Vスクロール用
};



#endif