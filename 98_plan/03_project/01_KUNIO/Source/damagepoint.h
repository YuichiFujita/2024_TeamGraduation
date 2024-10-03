//=============================================================================
// 
//  ダメージポイントヘッダー [damagepoint.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _DMGPOINT_H_
#define _DMGPOINT_H_	// 二重インクルード防止

#include "object.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CMultiNumber;

//==========================================================================
// クラス定義
//==========================================================================
// ダメージポイントクラス
class CDamagePoint : CObject
{
public:

	CDamagePoint(int nPriority = 8);
	~CDamagePoint();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();

	static CDamagePoint* Create(MyLib::Vector3 pos, int nDamage);

private:
	
	//=============================
	// メンバ関数
	//=============================
	void SetValue();

	//=============================
	// メンバ変数
	//=============================
	float m_fLife;	// 寿命
	CMultiNumber *m_apNumber;	// 数字のオブジェクト
};



#endif