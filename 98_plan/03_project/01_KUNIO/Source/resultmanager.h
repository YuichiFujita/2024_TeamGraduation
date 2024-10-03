//=============================================================================
// 
//  リザルトマネージャヘッダー [resultmanager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _RESULTMANAGER_H_
#define _RESULTMANAGER_H_	// 二重インクルード防止

#include "main.h"
#include "constans.h"
#include "judge.h"

// 前方宣言
class CScroll;
class CTimer_Result;
class CClearRank;
class CToatalRank;

//==========================================================================
// クラス定義
//==========================================================================
// リザルトマネージャクラス定義
class CResultManager
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_SCORE = 0,	// スコア表示
		STATE_PRESSENTER,	// 押下待ち
		STATE_MAX
	};

	CResultManager();
	~CResultManager();

	HRESULT Init();
	void Uninit();
	void Update();
	void Reset();	// リセット

	void SetJudgeRank(CJudge::JUDGE rank) { m_JudgeRank = rank; }	// 最終評価設定
	CJudge::JUDGE GetJudgeRank() { return m_JudgeRank; }			// 最終評価取得
	static void SetClearTime(float time) { m_fClearTime = time; }			// クリア時間設定
	static float GetClearTime() { return m_fClearTime; }					// クリア時間取得

	void CreateResultScreen();	// リザルト画面生成

	State GetState() { return m_state; }	// 状態

	static CResultManager *Create();	// 生成処理
	static CResultManager* GetInstance() { return m_pThisPtr; }
	
private:


	//=============================
	// メンバ変数
	//=============================
	State m_state;				// 状態
	CJudge::JUDGE m_JudgeRank;	// 最終評価
	static float m_fClearTime;			// クリア時間
	static CResultManager* m_pThisPtr;	// 自身のポインタ

	CScroll* m_pScroll;			// 巻き物のオブジェクト
	CTimer_Result* m_pTimer;	// タイマーのオブジェクト
	CClearRank* m_pClearRank;	// クリア時のランク
	CToatalRank* m_pToatalRank;	// 総合評価
	bool m_bScoreFinish;		// スコアの表示終了
};



#endif