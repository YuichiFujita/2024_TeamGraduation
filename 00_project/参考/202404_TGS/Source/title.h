//=============================================================================
// 
//  タイトルヘッダー [title.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TITLE_H_
#define _TITLE_H_	// 二重インクルード防止

#include "manager.h"

class CTitleLogo;
class CTitle_PressEnter;
class CKeyConfigSetting;
class CPeopleManager;

//==========================================================================
// 前方宣言
//==========================================================================
class CSpawnEnvironment;

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
	enum SCENETYPE
	{
		SCENETYPE_NONE = 0,			// なにもなし
		SCENETYPE_FADEIN,			// フェードイン
		SCENETYPE_FADEOUT_LOGO,		// ロゴフェードアウト
		SCENETYPE_SOUNDSETTING,		// サウンド設定
		SCENETYPE_OTHERSETTING,		// その他設定
		SCENETYPE_MAX
	};

	CTitle();
	~CTitle();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// シーンの種類
	void SetSceneType(SCENETYPE type) { m_SceneType = type; }
	SCENETYPE GetSceneType() { return m_SceneType; }

	// その他
	CTitle_PressEnter* GetTitlePressEnter() { return m_pPressEnter; }
	CKeyConfigSetting* GetSetting() { return m_pConfigSetting; }
	void SetSetting(CKeyConfigSetting* pSetting) { m_pConfigSetting = pSetting; }

	// 静的関数
	static CTitle* GetInstance();	// インスタンス取得
	static CTitle* Create();		// 生成処理

private:

	//=============================
	// 関数ポインタ
	//=============================
	typedef void(CTitle::* SCENE_FUNC)();
	static SCENE_FUNC m_SceneFunc[];

	//=============================
	// メンバ関数
	//=============================
	void SceneNone();			// なにもなし
	void SceneFadeInLogo();		// ロゴフェードイン
	void SceneFadeOutLoGo();		// ロゴフェードアウト

	//=============================
	// メンバ変数
	//=============================
	float m_fSceneTime;			// シーンカウンター
	SCENETYPE m_SceneType;		// シーンの種類
	CTitleLogo* m_pLogo;		// ロゴのポインタ
	CKeyConfigSetting* m_pConfigSetting;
	CTitle_PressEnter* m_pPressEnter;	// プレスエンター
	CSpawnEnvironment* m_pSpawn_Leaf;	// 降る葉生成
	CPeopleManager* m_pPeopleManager;	// 人マネージャ

	static CTitle* m_pThisPtr;	// 自身のポインタ
};



#endif