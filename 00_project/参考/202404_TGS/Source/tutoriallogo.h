//=============================================================================
// 
// タイトルロゴ処理 [tutoriallogo.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TUTORIALLOGO_H_
#define _TUTORIALLOGO_H_		// 二重インクルード防止のマクロを定義する

#include "object2D.h"


//==========================================================================
// クラス定義
//==========================================================================
// タイトルロゴ
class CTutorialLogo : public CObject2D
{
public:

	CTutorialLogo(int nPriority = 8);
	~CTutorialLogo();
	
	//  オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CTutorialLogo* Create();	// 生成処理

private:


};

#endif