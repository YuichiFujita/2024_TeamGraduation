//=============================================================================
// 
//  ゲーム時ポーズヘッダー [pause_game.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PAUSE_GAME_H_
#define _PAUSE_GAME_H_	// 二重インクルード防止

#include "pause.h"

//==========================================================================
// クラス定義
//==========================================================================
// ゲーム時ポーズクラス
class CPause_Game : public CPause
{
public:

	CPause_Game();
	~CPause_Game();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

private:
	
	void DecideByMode() override;	// モード別決定処理

};

#endif