//=============================================================================
// 
//  リザルトマネージャヘッダー [resultmanager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _RESULTMANAGER_H_
#define _RESULTMANAGER_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
//#include "constans.h"
//#include "instantfade.h"
//#include "scene.h"

#include "gameManager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CBall;
class CCollisionLine_Box;
class CTeamStatus;
class CGymWallManager;
class CTimerUI;
class CCharmManager;

//==========================================================================
// クラス定義
//==========================================================================
// ゲームマネージャクラス定義
class CResultManager
{
public:

	CResultManager();
	~CResultManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void Debug();			// デバッグ

	static CResultManager* Create(CScene::MODE mode);				// 生成処理
	static CResultManager* GetInstance() { return m_pThisPtr; }	// インスタンス取得

private:
	
	//=============================
	// メンバ関数
	//=============================


	//=============================
	// メンバ変数
	//=============================


	//--------------------------
	// 生成したオブジェクト
	//--------------------------
	
	float f[CGameManager::ETeamSide::SIDE_MAX];		// モテ値
	CGameManager::ETeamSide m_teamWin;				// 勝利チーム

	static CResultManager* m_pThisPtr;				// 自身のポインタ
};


#endif