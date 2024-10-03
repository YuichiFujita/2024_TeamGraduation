//=============================================================================
// 
//  水フィールドヘッダー [waterfield_right.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _WATERFIELD_RIGHT_H_
#define _WATERFIELD_RIGHT_H_	// 二重インクルード防止

#include "waterfield.h"

//==========================================================================
// クラス定義
//==========================================================================
// 水フィールドクラス
class CWaterField_Right : public CWaterField
{
public:

	CWaterField_Right();
	~CWaterField_Right();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;

};



#endif