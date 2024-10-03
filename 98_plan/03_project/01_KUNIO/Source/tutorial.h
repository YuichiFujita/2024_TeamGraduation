//=============================================================================
// 
//  タイトルヘッダー [tutorial.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_	// 二重インクルード防止

#include "manager.h"

class CTutorialLogo;

//==========================================================================
// クラス定義
//==========================================================================
// タイトルクラス
class CTutorial : public CScene
{
public:

	CTutorial();
	~CTutorial();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// 静的関数
	static CTutorial* GetInstance();	// インスタンス取得
	static CTutorial* Create();		// 生成処理

private:

	CTutorialLogo* m_pLogo;		// ロゴのポインタ
	static CTutorial* m_pThisPtr;	// 自身のポインタ
};



#endif