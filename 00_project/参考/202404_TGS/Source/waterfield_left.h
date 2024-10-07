//=============================================================================
// 
//  水フィールドヘッダー [waterfield_left.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _WATERFIELD_LEFT_H_
#define _WATERFIELD_LEFT_H_	// 二重インクルード防止

#include "waterfield.h"

//==========================================================================
// クラス定義
//==========================================================================
// 水フィールドクラス
class CWaterField_Left : public CWaterField
{
public:

	CWaterField_Left();
	~CWaterField_Left();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;

};



#endif