//=============================================================================
// 
//  チュートリアルヘッダー [tutorial.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "manager.h"

//==========================================================================
// クラス定義
//==========================================================================
// チュートリアルクラス
class CTutorial : public CScene
{
public:

	//=============================
	// コンストラクタ,デストラクタ
	//=============================
	CTutorial();
	~CTutorial();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

	//=============================
	// メンバ関数
	//=============================
	

	//=============================
	// 静的関数
	//=============================
	static CTutorial* GetInstance() { return m_pThisPtr; }	// インスタンス取得
	static CTutorial* Create();							// 生成処理

private:

	//=============================
	// 関数ポインタ
	//=============================
	typedef void(CTutorial::* SCENE_FUNC)(const float, const float, const float);
	static SCENE_FUNC m_SceneFunc[];

	//=============================
	// メンバ関数
	//=============================
	void SceneNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// なにもなし
	void SceneFadeInLogo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ロゴフェードイン
	void SceneFadeOutLoGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ロゴフェードアウト

	//=============================
	// メンバ変数
	//=============================
	static CTutorial* m_pThisPtr;		// 自身のポインタ
};



#endif