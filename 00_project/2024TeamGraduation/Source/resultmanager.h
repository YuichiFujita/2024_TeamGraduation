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
#include "gameManager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerReferee_Result;
class CWinTeamResult;

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
class CObject3D;
class CResultCrown;
class CEffekseerObj;
class CSkip;

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
		STATE_NONE = 0,				// なにもない
		STATE_PRELUDE_READY,		// 前座勝敗準備
		STATE_PRELUDE,				// 前座勝敗
		STATE_CONTEST_READY,		// モテ勝敗準備
		STATE_CONTEST_DRUMROLL,		// モテ勝敗ドラムロール
		STATE_CONTEST_PRESENTS,		// モテ勝敗発表
		STATE_CONTEST,				// モテ勝敗
		STATE_MAX
	};

	CResultManager();
	~CResultManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Kill();	// 動的削除処理
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//=============================
	// メンバ関数
	//=============================
	MyLib::Vector3 GetPosMid(CGameManager::ETeamSide team);		// コートの中心位置取得
	EState GetState() { return m_state; }		// 状態取得
	CGameManager::ETeamSide GetTeamPreludeWin() { return m_teamPreludeWin; }		// 勝利チーム(前座)
	CGameManager::ETeamSide GetTeamContestWin() { return m_teamContestWin; }		// 勝利チーム(モテ)
	bool IsSceneTrans() { return m_bSceneTrans; }									// シーン遷移可能フラグ
	
	static CResultManager* Create();							// 生成処理
	static CResultManager* GetInstance() { return m_pThisPtr; }	// インスタンス取得

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CResultManager::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];			// 状態関数

	typedef void(CResultManager::* STATE_START_FUNC)();
	static STATE_START_FUNC m_StateStartFunc[];	// 状態開始関数

	typedef void(CResultManager::* STATE_END_FUNC)();
	static STATE_END_FUNC m_StateEndFunc[];		// 状態終了関数

	typedef void(CResultManager::* STATE_SKIP_FUNC)();
	static STATE_SKIP_FUNC m_StateSkipFunc[];	// 状態スキップ関数

	//=============================
	// メンバ関数
	//=============================
	//-----------------------------
	// 状態関数
	//-----------------------------
	void SetState(EState state);		// 状態設定
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);				// 状態更新
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);					// なし
	void StatePreludeReady(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 前座勝敗準備
	void StatePrelude(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);				// 前座勝敗
	void StateCharmContestReady(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// モテ勝敗準備
	void StateCharmContestDrumroll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// モテ勝敗ドラムロール
	void StateCharmContestPresents(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// モテ勝敗発表
	void StateCharmContest(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// モテ勝敗
	
	// 開始
	void StateStartPreludeReady();			// [開始]前座勝敗準備
	void StateStartPrelude();				// [開始]前座勝敗
	void StateStartCharmContestReady();		// [開始]モテ勝敗準備
	void StateStartCharmContestDrumroll();	// [開始]モテ勝敗ドラムロール
	void StateStartCharmContestPresents();	// [開始]モテ勝敗発表
	void StateStartCharmContest();			// [開始]モテ勝敗

	// 終了
	void StateEndPrelude();				// [終了]前座勝敗
	void StateEndCharmContest();		// [終了]モテ勝敗

	void SkipState();	// [スキップ]状態進行
		
	//-----------------------------
	// その他
	//-----------------------------
	void CreateAudience();								// 観客生成
	void CreateCrown(CGameManager::ETeamSide team);		// 王冠モデル生成
	void CreatePolygon(EState state);					// 勝敗ポリゴン生成
	void CreateEffect();								// エフェクト生成

	void Debug();		// デバッグ
	void Load();		// ゲーム情報読み込み
	void LoadTeam(std::ifstream* File, std::string line, int nTeam);		// チーム情報読み込み

	//=============================
	// メンバ変数
	//=============================
	float m_fTension;											// テンション
	CGameManager::ETeamSide m_teamPreludeWin;					// 勝利チーム(前座)
	CGameManager::ETeamSide m_teamContestWin;					// 勝利チーム(モテ)
	float m_fCharmValue[CGameManager::ETeamSide::SIDE_MAX];		// モテ値
	CObject3D* m_pText;											// 勝利チーム用3Dポリゴン
	CResultCrown* m_pCrown;										// 王冠モデル
	CPlayerReferee_Result* m_pReferee;							// 審判
	CWinTeamResult* m_pWinTeam;									// 勝利チーム
	float m_fSpotLightTime;										// スポットライトのタイマー

	//-----------------------------
	// エフェクト
	//-----------------------------
	CEffekseerObj* m_pEfkConfetti;									// 紙吹雪エフェクシア
	CEffekseerObj* m_pEfkLight[CGameManager::ETeamSide::SIDE_MAX];	// スポットエフェクシア

	//-----------------------------
	// 状態
	//-----------------------------
	EState m_state;		// 状態
	float m_fStateTime;	// 状態時間
	bool m_bStateTrans;	// 状態遷移可能フラグ(t:不可/f:可)
	bool m_bSceneTrans;	// シーン遷移可能フラグ

	static CResultManager* m_pThisPtr;							// 自身のポインタ
};

#endif