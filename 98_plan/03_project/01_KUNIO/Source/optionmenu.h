//=============================================================================
// 
//  オプションメニューヘッダー [optionmenu.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OPTIONMENU_H_
#define _OPTIONMENU_H_	// 二重インクルード防止

#include "object.h"
#include "title_optionselect.h"

//==========================================================================
// クラス定義
//==========================================================================
// オプションメニュークラス
class COptionMenu : public CObject
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum STATE
	{
		STATE_NONE = 0,	// なにもない
		STATE_EDIT,		// 編集
		STATE_FADEIN,	// フェードイン
		STATE_FADEOUT,	// フェードアウト
		STATE_MAX
	};

	COptionMenu(int nPriority = 7);
	~COptionMenu();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	virtual void Kill();						// 削除
	void SetState(const STATE& state);			// 状態設定
	STATE GetState() { return m_state; }		// 状態取得
	bool IsNowChange() { return m_bNowChange; }	// 変更中か否か

	//=============================
	// 静的関数
	//=============================
	static COptionMenu* Create(CTitle_OptionSelect::Select selectType);	// 生成処理

protected:

	//=============================
	// 関数リスト
	//=============================
	// 状態リスト
	typedef void(COptionMenu::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void UpdateState();	// 状態更新
	void StateNone();		// なにもなし
	virtual void StateEdit();		// 編集
	void StateFadeIn();		// フェードイン
	void StateFadeOut();	// フェードアウト

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	STATE m_state;			// 状態
	float m_fStateTimer;	// 状態タイマー
	bool m_bNowChange;		// 変更中か否か
};


#endif