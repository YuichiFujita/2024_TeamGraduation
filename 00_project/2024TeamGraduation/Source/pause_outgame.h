//=============================================================================
// 
//  アウトゲーム時ポーズヘッダー [pause_outgame.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PAUSE_OUTGAME_H_
#define _PAUSE_OUTGAME_H_	// 二重インクルード防止

#include "pause.h"

//==========================================================================
// クラス定義
//==========================================================================
// アウトゲーム時ポーズクラス
class CPause_OutGame : public CPause
{
public:

	CPause_OutGame();
	~CPause_OutGame();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

private:
	
	void DecideByMode() override;	// モード別決定処理

};

#endif