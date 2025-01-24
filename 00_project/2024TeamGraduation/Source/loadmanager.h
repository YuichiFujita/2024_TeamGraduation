//=============================================================================
// 
// ロードマネージャ [loadmanager.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _LOADMANAGER_H_
#define _LOADMANAGER_H_		// 二重インクルード防止

#include "scene.h"

class CLoadScreen;

//==========================================================================
// クラス定義
//==========================================================================
// ロードマネージャクラス
class CLoadManager
{
public:

	CLoadManager();
	~CLoadManager();

	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Draw();

	void LoadScene(CScene::MODE mode);
	void UnLoad();	// ロード解放
	void ResetLoad();	// ロードリセット
	bool IsLoadComplete();	// ロードが完了フラグ取得
	bool IsLoopMoment() const;	// ロード画面繰り返し瞬間取得
	DWORD GetNumLoop() const;	// ロード画面ループ数取得
	void ResetNumLoop();		// ロード画面ループ数リセット
	void SetEnableAutoPlay(const bool bPlay);	// ロード画面自動再生設定

	// 静的関数
	static CLoadManager* Create();

private:

	//=============================
	// メンバ関数
	//=============================
	void LoadInBackground();
	void Load();
	void ResetInternalLoad();	// 新しいシーンをセットする前のリセット

	//=============================
	// メンバ変数
	//=============================
	std::thread m_LoadingThread;
	bool m_bLoadComplete;		// ロード完了フラグ
	CScene::MODE m_ModeNext;	// 次のモード
	CLoadScreen* m_pLoadScreen;	// ロード画面
};


#endif