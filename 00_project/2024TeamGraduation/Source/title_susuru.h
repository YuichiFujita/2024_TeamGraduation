//=============================================================================
//
// SUSURUシーン処理 [title_susuru.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _TITLE_SUSURU_H_		// このマクロ定義がされていなかったら
#define _TITLE_SUSURU_H_		// 二重インクルード防止のマクロを定義する

//==========================================================================
// インクルードファイル
//==========================================================================
#include "titlescene.h"
#include "player.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CDressup;
class CTitleStudent;	// タイトルの生徒

//==========================================================================
// クラス定義
//==========================================================================
// SUSURUシーンクラス
class CTitle_SUSURU : public CTitleScene
{
public:

	//=============================
	// 定数
	//=============================
	static constexpr int NUM_TEXT = 10;		// テキストの数

	//=============================
	// 列挙型定義
	//=============================
	enum EState
	{
		STATE_WAIT = 0,	// 待機
		STATE_SCROLL,	// 文字送り
		STATE_END,		// 終了
		STATE_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CTitle_SUSURU();
	~CTitle_SUSURU();

	//=============================
	// メンバ関数
	//=============================
	virtual HRESULT Init() override;	// 初期化
	virtual void Uninit() override;		// 終了
	virtual void Kill() override;		// 削除
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新

	//--------------------------
	// その他
	//--------------------------

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CTitle_SUSURU::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// 状態関数

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	//--------------------------
	virtual void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 状態更新
	void StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 待機
	void StateScroll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 文字送り
	void StateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 終了

	//--------------------------
	// その他
	//--------------------------
	HRESULT CreateText();	// テキスト生成
	HRESULT CreateSUSURU();	// SUSURUさん生成

	//=============================
	// メンバ変数
	//=============================
	//--------------------------
	// 状態
	//--------------------------
	EState m_state;			// 状態
	float m_fStateTime;		// 状態時間

	//--------------------------
	// その他
	//--------------------------
	int m_nIdxScroll;	// 送る文字インデックス
	CObject2D* m_apText[NUM_TEXT];	// 文字
	CTitleStudent* m_pSUSURU;	// SUSURUさん
};

#endif