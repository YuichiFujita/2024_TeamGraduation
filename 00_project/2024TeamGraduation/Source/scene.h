//=============================================================================
// 
//  シーンヘッダー [scene.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SCENE_H_
#define _SCENE_H_	// 二重インクルード防止


//==========================================================================
// 前方宣言
//==========================================================================
class CCamera;
class CEdit_Map;

//==========================================================================
// クラス定義
//==========================================================================
// シーンクラス定義
class CScene
{
public:
	// モードの列挙型定義
	enum MODE
	{
		MODE_NONE = 0,		// なし
		MODE_TITLE,			// タイトル
		MODE_TUTORIAL,		// チュートリアル
		MODE_GAME,			// ゲーム
		MODE_GAMETUTORIAL,	// ゲーム
		MODE_RESULT,		// リザルト
		MODE_RANKING,		// ランキング
		MODE_MAX
	};

	CScene();
	~CScene();

	// オーバーライドする関数
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime);
	virtual void Draw();

	virtual void Kill();

	static CScene *Create(MODE mode);
	MODE GetMode();						// 現在のモード取得

protected:

private:

	MODE m_mode;				// 現在のモード
	CEdit_Map* m_pEditMap;
};



#endif