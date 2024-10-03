//=============================================================================
// 
//  依頼人セリフエフェクト [serifeffect.h]
//  Author : 石原颯馬
// 
//=============================================================================

#ifndef _SERIFEFFECT_H_
#define _SERIFEFFECT_H_	// 二重インクルード防止

#include "object3D.h"

//==========================================================================
// クラス定義
//==========================================================================
class CSerifEffect : public CObject3D
{
public:

	CSerifEffect(int nPriority = 11);
	~CSerifEffect();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Kill();	// 削除
	static CSerifEffect* Create(MyLib::Vector3 pos, MyLib::Vector3 rot, int life);	// 生成処理

private:

	//=============================
	// メンバ変数
	//=============================
	int m_Life;
	int m_MaxLife;
};


#endif