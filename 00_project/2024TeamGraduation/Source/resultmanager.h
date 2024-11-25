//=============================================================================
// 
//  リザルトマネージャヘッダー [resultmanager.h]
//  Author : Kai Takada
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
// 名前空間
//==========================================================================
namespace ResultManager
{
	const std::string TEXTFILE = "data\\TEXT\\result\\teamStatus.txt";	// チームステータスtxt
}

//==========================================================================
// 前方宣言
//==========================================================================
class CTeamStatus;

//==========================================================================
// クラス定義
//==========================================================================
// リザルトマネージャクラス定義
class CResultManager
{
public:
	
	CResultManager();
	~CResultManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void CreatePrelude();							// 前座勝敗生成
	void CreateCharmContest();						// モテ勝敗生成

	void Debug();		// デバッグ
	void Load();		// チームステータス読み込み

	static CResultManager* Create();							// 生成処理
	static CResultManager* GetInstance() { return m_pThisPtr; }	// インスタンス取得

private:
	
	//=============================
	// メンバ変数
	//=============================
	float m_fTension;											// テンション
	CGameManager::ETeamSide m_teamWin;							// 勝利チーム
	float m_fCharmValue[CGameManager::ETeamSide::SIDE_MAX];		// モテ値
	float m_fTime;												// 時間経過

	static CResultManager* m_pThisPtr;							// 自身のポインタ
};

#endif