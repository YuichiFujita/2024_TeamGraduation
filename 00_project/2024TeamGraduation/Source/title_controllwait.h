//=============================================================================
//
// 操作待機処理 [title_controllwait.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _TITLE_CONTROLLWAIT_H_		// このマクロ定義がされていなかったら
#define _TITLE_CONTROLLWAIT_H_		// 二重インクルード防止のマクロを定義する

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
class COption_BBS;

//==========================================================================
// クラス定義
//==========================================================================
// 操作待機クラス
class CTitle_ControllWait : public CTitleScene
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum EState
	{
		STATE_WAIT = 0,	// 待機
		STATE_CONTROLL,	// 操作
		STATE_TRANSITION_MORE_1st,	// 色々に遷移
		STATE_TRANSITION_WAIT,		// 遷移待機
		STATE_TRANSITION_MORE_2nd,	// 色々に遷移
		STATE_TRANSITION_MORE_LAST,	// 色々に遷移(ラスト)
		STATE_TRANSITION_BACK,		// 戻る
		STATE_MAX
	};

	enum EStateBG	// 背景の状態
	{
		STATEBG_SPAWN = 0,	// 出現
		STATEBG_LOOP,		// ループ
		STATEBG_MAX
	};

	enum ESelect
	{
		SELECT_BATTLE = 0,	// 開始
		SELECT_MORE,		// 色々
		SELECT_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CTitle_ControllWait();
	~CTitle_ControllWait();

	//=============================
	// メンバ関数
	//=============================
	virtual HRESULT Init() override;	// 初期化
	virtual void Uninit() override;		// 終了
	virtual void Kill() override;		// 削除
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新

	void SetState(EState state);		// 状態設定
	void SetStateBG(EStateBG state);		// 状態設定

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CTitle_ControllWait::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// 状態関数

	typedef void(CTitle_ControllWait::* STATEBG_FUNC)(const float, const float, const float);
	static STATEBG_FUNC m_StateBGFunc[];	// 状態関数

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	//--------------------------
	virtual void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 状態更新
	void StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 待機
	void StateControll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 操作
	void StateTransitionMoreFirst(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 色々に遷移
	void StateTransitionWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 遷移待機
	void StateTransitionMoreSecond(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 色々に遷移
	void StateTransitionMoreLast(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 色々に遷移(ラスト)
	void StateTansitionBack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 戻る

	// 背景状態
	void StateBGSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 出現
	void StateBGLoop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ループ

	//--------------------------
	// 更新
	//--------------------------
	void UpdateSelect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 選択肢更新
	void UpdateColor(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 色更新

	//--------------------------
	// 生成
	//--------------------------
	HRESULT CreateSelect();	// 選択肢生成
	HRESULT CreateBG();		// 背景生成

	// その他
	void Decide();	// 決定

	//=============================
	// メンバ変数
	//=============================
	//--------------------------
	// 状態
	//--------------------------
	EState m_state;			// 状態
	float m_fStateTime;		// 状態時間

	// 背景状態
	EStateBG m_stateBG;		// 状態
	float m_fStateTimeBG;	// 状態時間

	// 選択肢
	ESelect m_select;		// 選択肢
	CObject2D* m_pSelectUI[ESelect::SELECT_MAX];	// 選択肢のUI
	CObject2D* m_pBG[ESelect::SELECT_MAX];			// 背景

	//--------------------------
	// その他
	//--------------------------
	CTitleLogo* m_pLogo;		// ロゴ
	COption_BBS* m_pOptionBBS;	// 掲示板
	float m_fMarkerTime;		// マーカーのタイマー
	std::vector<CTitleStudent*> m_vecTitleStudent;	// タイトルの生徒
};

#endif