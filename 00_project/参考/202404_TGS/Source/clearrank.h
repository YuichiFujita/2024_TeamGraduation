//=============================================================================
// 
//  クリアランクヘッダー [clearrank.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _CLEARRANK_H_
#define _CLEARRANK_H_	// 二重インクルード防止

#include "object2d.h"
#include "judge.h"

//==========================================================================
// クラス定義
//==========================================================================
// クリアランククラス
class CClearRank : public CObject2D
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_SCROLL_TEXT = 0,	// 文字送り
		STATE_SCROLL_VOID,		// 空間送り
		STATE_SCROLL_RANK,		// ランク送り
		STATE_FINISH,			// 終了
		STATE_EMPHASIZE,		// 強調
		STATE_NONE,				// なにもなし
		STATE_MAX
	};

	CClearRank(int nPriority = 3);
	~CClearRank();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetVtx() override;

	void SetState(State state);				// 状態設定
	State GetState() { return m_state; }	// 状態取得

	bool IsFinish() { return m_bFinish; }	// 終了

	//=============================
	// 静的関数
	//=============================
	static CClearRank* Create(CJudge::JUDGE rank);

private:

	//=============================
	// 関数リスト
	//=============================
	// 状態リスト
	typedef void(CClearRank::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void UpdateState();
	void StateScrollText();	// 文字送り
	void StateSrollVoid();	// 空間送り
	void StateScrollRank();	// ランク送り
	void StateFinish();		// 終了
	void StateEmphasize();	// 強調
	void StateNone() {}		// なにもなし

	// その他関数
	void CreateText();	// 文字生成
	void CreateRank();	// ランク生成
	void Skip();		// スキップ

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	float m_fStateTime;		// 状態カウンター
	State m_state;			// 状態

	// その他
	CJudge::JUDGE m_Rank;	// ランク
	CObject2D* m_pText;		// 文字
	float m_fMoveTextLen;	// テキストの移動距離
	float m_fMoveRankLen;	// ランクの移動距離
	bool m_bFinish;			// 終了
};


#endif