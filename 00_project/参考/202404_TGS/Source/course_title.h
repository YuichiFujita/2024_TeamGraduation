//=============================================================================
// 
//  コースヘッダー [course_title.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _COURSE_RESULT_H_
#define _COURSE_RESULT_H_	// 二重インクルード防止

#include "course.h"

//==========================================================================
// クラス定義
//==========================================================================
// コースクラス
class CCourse_Title : public CCourse
{

public:

	CCourse_Title(int nPriority = 2, const LAYER layer = LAYER::LAYER_MAP);
	~CCourse_Title();

	// オーバーライドされた関数
	virtual HRESULT Init();
	virtual void Uninit();

	
};



#endif