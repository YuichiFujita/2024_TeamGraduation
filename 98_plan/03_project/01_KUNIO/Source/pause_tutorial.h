//=============================================================================
// 
//  チュートリアル時ポーズヘッダー [pause_tutorial.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PAUSE_TUTORIAL_H_
#define _PAUSE_TUTORIAL_H_	// 二重インクルード防止

#include "pause.h"

//==========================================================================
// クラス定義
//==========================================================================
// チュートリアル時ポーズクラス
class CPause_Tutorial : public CPause
{
public:

	CPause_Tutorial();
	~CPause_Tutorial();

	// オーバーライドされた関数
	HRESULT Init() override;

private:
	
	void DecideByMode() override;	// モード別決定処理

};

#endif