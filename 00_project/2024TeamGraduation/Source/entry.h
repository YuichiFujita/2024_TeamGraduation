//=============================================================================
// 
//  エントリーヘッダー [entry.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _ENTRY_H_
#define _ENTRY_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "manager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CEntryScene;

//==========================================================================
// クラス定義
//==========================================================================
// エントリークラス
class CEntry : public CScene
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum ESceneType
	{
		SCENETYPE_NONE = 0,			// なにもなし
		SCENETYPE_SETUPTEAM,		// チーム等設定
		SCENETYPE_DRESSUP,			// 着せ替え・利き手設定
		SCENETYPE_GAMESETTING,		// 詳細設定
		SCENETYPE_MAX
	};

	CEntry();
	~CEntry();

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
	//--------------------------
	// シーン
	//--------------------------
	void SetSceneType(ESceneType type);					// シーン設定
	ESceneType GetSceneType() { return m_SceneType; }	// シーン取得

	//=============================
	// 静的関数
	//=============================
	static CEntry* GetInstance() { return m_pThisPtr; }	// インスタンス取得
	static CEntry* Create();							// 生成処理

private:

	//=============================
	// 関数ポインタ
	//=============================
	typedef void(CEntry::* SCENE_FUNC)(const float, const float, const float);
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
	float m_fSceneTime;			// シーンカウンター
	ESceneType m_SceneType;		// シーンの種類
	CEntryScene* m_pEntryScene;	// エントリーシーン
	static CEntry* m_pThisPtr;	// 自身のポインタ
};



#endif