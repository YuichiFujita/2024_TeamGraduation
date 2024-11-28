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
	
	//=============================
	// 列挙定義
	//=============================
	// 状態
	enum EState
	{
		STATE_NONE = 0,		// なにもない
		STATE_PRELUDE,		// 前座勝敗
		STATE_CONTEST,		// モテ勝敗
		STATE_MAX
	};

	CResultManager();
	~CResultManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//=============================
	// 関数リスト
	//=============================
	typedef void(CResultManager::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// 状態関数

	//=============================
	// メンバ関数
	//=============================
	EState GetState() { return m_state; }		// 状態取得
	CGameManager::ETeamSide GetTeamPreludeWin() { return m_teamPreludeWin; }		// 勝利チーム(前座)
	CGameManager::ETeamSide GetTeamContestWin() { return m_teamContestWin; }		// 勝利チーム(モテ)

	static CResultManager* Create();							// 生成処理
	static CResultManager* GetInstance() { return m_pThisPtr; }	// インスタンス取得

private:

	//=============================
	// メンバ関数
	//=============================
	//-----------------------------
	// 状態関数
	//-----------------------------
	void SetState(EState state);		// 状態設定
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 状態更新
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);				// なし
	void StatePrelude(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 前座勝敗
	void StateCharmContest(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// モテ勝敗

	void CreatePrelude();				// 前座勝敗生成
	void CreateCharmContest();			// モテ勝敗生成
	void CreateAudience();				// 観客生成

	void Debug();		// デバッグ
	void Load();		// ゲーム情報読み込み
	void LoadTeam(std::ifstream* File, std::string line, int nTeam);						// チーム情報読み込み
	void LoadPlayer(std::ifstream* File, std::string line, int nTeam, int nIdxPlayer);		// プレイヤー情報読み込み

	//=============================
	// メンバ変数
	//=============================
	float m_fTension;											// テンション
	CGameManager::ETeamSide m_teamPreludeWin;					// 勝利チーム(前座)
	CGameManager::ETeamSide m_teamContestWin;					// 勝利チーム(モテ)
	float m_fCharmValue[CGameManager::ETeamSide::SIDE_MAX];		// モテ値

	//-----------------------------
	// 状態
	//-----------------------------
	EState m_state;		// 状態
	float m_fStateTime;	// 状態時間

	static CResultManager* m_pThisPtr;							// 自身のポインタ
};

#endif