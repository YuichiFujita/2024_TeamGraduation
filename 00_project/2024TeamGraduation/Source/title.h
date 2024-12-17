//=============================================================================
// 
//  タイトルヘッダー [title.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TITLE_H_
#define _TITLE_H_	// 二重インクルード防止

#include "manager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CTitleLogo;
class CTitleScene;

//==========================================================================
// クラス定義
//==========================================================================
// タイトルクラス
class CTitle : public CScene
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum ESceneType
	{
		SCENETYPE_NONE = 0,			// なにもなし
		SCENETYPE_CONTROLLWAIT,		// 操作待ち
		SCENETYPE_SUSURU,			// SUSURU
		SCENETYPE_MAX
	};

	enum EState
	{
		STATE_NONE = 0,		// NONE
		STATE_CHANGESCENE,	// シーン切り替え中
		STATE_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CTitle();
	~CTitle();

	//=============================
	// オーバーライドされた関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

	// シーンの種類
	ESceneType GetSceneType() { return m_SceneType; }
	void ChangeScene(ESceneType type);	// シーン切り替え


	//=============================
	// 静的関数
	//=============================
	static CTitle* GetInstance();	// インスタンス取得
	static CTitle* Create();		// 生成処理

private:

	//=============================
	// 関数ポインタ
	//=============================
	typedef void(CTitle::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 状態更新処理
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// なにもなし
	void StateChangeScene(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// シーン切り替え

	//=============================
	// メンバ変数
	//=============================
	EState m_state;				// 状態
	ESceneType m_SceneType;		// シーンの種類
	float m_fSceneTime;			// シーンカウンター
	CTitleLogo* m_pLogo;		// ロゴのポインタ
	CTitleScene* m_pTitleScene;		// タイトルシーン
	static CTitle* m_pThisPtr;	// 自身のポインタ
};



#endif