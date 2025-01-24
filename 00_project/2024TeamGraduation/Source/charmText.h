//==========================================================================
// 
//  モテ文字ヘッダー [charmText.h]
//  Author : 相馬靜雅
// 
//==========================================================================

#ifndef _CHARMTEXT_H_
#define _CHARMTEXT_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object.h"
#include "gameManager.h"
#include "charmManager.h"
#include "thoughtBalloon.h"

//==========================================================================
// クラス定義
//==========================================================================
// モテ文字クラス
class CCharmText : public CObject
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum EState
	{
		STATE_FADEIN = 0,	// フェードイン(モテチェイン可能)
		STATE_WAIT,			// 待機(モテチェイン可能)
		STATE_FADEOUT,		// フェードアウト
		STATE_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CCharmText(int nPriority = mylib_const::PRIORITY_ZSORT, const LAYER layer = LAYER::LAYER_2D);
	~CCharmText();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;						// 削除
	virtual void SetEnableDisp(bool bDisp) override;	// 描画状況設定

	//=============================
	// メンバ関数
	//=============================
	void SetEnableTextDisp(bool bDisp)					{ m_pThoughtBalloon->SetEnableTextDisp(bDisp); }	// テキスト表示設定
	void SetCountUP(int nCnt)							{ m_nCntUp = nCnt; }					// 上昇カウント設定
	void AddCountUP(int nCnt)							{ m_nCntUp += nCnt; }					// 上昇カウント加算
	void SetEnablePossibleChain(bool bChain)			{ m_bPossibleChain = bChain; }			// チェイン可能フラグ設定
	bool IsPossibleChain()								{ return m_bPossibleChain; }			// チェイン可能フラグ取得
	void SetMyChainIdx(int idx)							{ m_nMyChainIdx = idx; }				// 自分のチェインインデックス
	int GetMyChainIdx()									{ return m_nMyChainIdx; }				// 自分のチェインインデックス
	void SetChainIdx(const std::vector<int>& vecIdx)	{ m_nVecChainIdx = vecIdx; }			// チェインしているインデックス番号設定
	std::vector<int> GetChainIdx()						{ return m_nVecChainIdx; }				// チェインしているインデックス番号取得
	void SetState(EState state);			// 状態設定
	EState GetState() { return m_state; }	// 状態取得

	//=============================
	// 静的メンバ関数
	//=============================
	static CCharmText* Create(CGameManager::ETeamSide side);	// 生成

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CCharmText::*STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// 状態関数

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 状態更新

	//--------------------------
	// チェイン
	//--------------------------
	virtual void SetChain(int nChainIdx);	// チェイン設定
	virtual void CheckChain() = 0;			// チェインの確認
	//virtual void EffectChain();				// チェインエフェクト

	// 生成
	HRESULT CreateText();	// 文字生成

protected:

	//=============================
	// 定数
	//=============================
	static constexpr float STATETIME_FADEIN = 0.3f;
	static constexpr float STATETIME_WAIT = CCharmManager::TIME_CHAIN - STATETIME_FADEIN;
	static constexpr float STATETIME_FADEOUT = 0.5f;
	static constexpr float DISTANCE_UP = 20.0f;	// 上昇間隔
	static constexpr float DISTANCE_XZ = 80.0f;	// 上昇間隔

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	virtual void StateFadeIn();		// フェードイン
	virtual void StateWait();		// 待機
	virtual void StateFadeOut();	// フェードアウト

	//=============================
	// メンバ変数
	//=============================
	CThoughtBalloon* m_pThoughtBalloon;	// 実況吹き出し
	EState m_state;			// 状態
	float m_fStateTime;		// 状態タイマー
	int m_nCntUp;			// 上昇カウント
	bool m_bPossibleChain;	// チェイン可能フラグ
	int m_nMyChainIdx;		// 自分のチェインインデックス
	CGameManager::ETeamSide m_TeamSide;		// チームサイド
	std::vector<int> m_nVecChainIdx;	// チェインしているインデックス番号
};

#endif
