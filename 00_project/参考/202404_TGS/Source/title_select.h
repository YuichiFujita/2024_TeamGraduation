//=============================================================================
// 
// タイトル選択 [title_select.h]
// Author : Ibuki Okusada
// 
//=============================================================================
#ifndef _TITLE_SELECT_H_
#define _TITLE_SELECT_H_		// 二重インクルード防止のマクロを定義する

#include "object.h"
#include "object2D.h"

class CObject2D;
class CTitle_OptionSelect;

//==========================================================================
// クラス定義
//==========================================================================
// タイトルエンター
class CTitle_Select : public CObject
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum STATE
	{
		STATE_NONE = 0,			// なにもなし
		STATE_FADEIN,			// フェードイン
		STATE_FADEOUT,			// フェードアウト
		STATE_TUTORIAL_FADEOUT,	// チュートリアル確認のフェードアウト
		STATE_NOACTIVE,			// 反応しない
		STATE_SETTING,			// 設定中
		STATE_BACK,				// 戻る
		STATE_MAX
	};

	enum SELECT
	{
		SELECT_START,	// 開始
		SELECT_OPTION,	// 設定
		SELECT_MAX
	};

	CTitle_Select(float fadetime);
	~CTitle_Select() {}

	//  オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();	// 削除

	void SetState(STATE state);
	STATE GetState() { return m_state; }

	CTitle_OptionSelect* GetOptionSelect() { return m_pOptionSelect; }	// オプションの選択肢

	static CTitle_Select* Create(float fadetime);	// 生成処理

private:

	//=============================
	// 関数ポインタ
	//=============================
	typedef void(CTitle_Select::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	void StateNone();			// なにもなし
	void StateFadeIn();			// フェードイン
	void StateFadeOut();			// フェードアウト
	void StateTutorial_FadeOut();		// チュートリアル確認のフェードアウト
	void StateNoActive();		// 反応しない
	void StateSetting();		// 反応しない
	void StateBack();			// 戻る

	// その他
	void DecideOptionSetting();	// オプション決定時の設定
	void DrawSelect();	// 選択肢を書く

	//=============================
	// メンバ変数
	//=============================
	STATE m_state;				// 状態
	float m_fStateTime;			// 状態カウンター
	float m_fSelectDrawTime;	// 選択肢の書く時間
	const float m_fFadeOutTime;	// フェードにかかる時間
	CObject2D* m_ap2D[SELECT_MAX];	// 選択肢
	CObject2D* m_pSelect;			// 背景筆
	CTitle_OptionSelect* m_pOptionSelect;	// オプションの選択肢
	int m_nSelect;				// 
	bool m_bPress;
};

#endif