//=============================================================================
// 
//  ランキングマネージャヘッダー [rankingmanager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _RANKINGMANAGER_H_
#define _RANKINGMANAGER_H_	// 二重インクルード防止

#include "main.h"
#include "constans.h"

// 前方宣言

//==========================================================================
// クラス定義
//==========================================================================
// ランキングマネージャクラス定義
class CRankingManager
{
public:

	CRankingManager();
	~CRankingManager();

	HRESULT Init();
	void Uninit();
	void Update();

	void Reset();				// リセット
	void SetNowScore(int nScore);	// 今回のスコア設定
	int GetNowScore();			// 今回のスコア取得
	static CRankingManager *Create();	// 生成処理
	
private:
	int m_nNowScore;	// 今回のスコア
};



#endif