//=============================================================================
// 
//  タイトルのオプション選択肢ヘッダー [title_optionselect.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TITLE_OPTIONSELECT_H_
#define _TITLE_OPTIONSELECT_H_	// 二重インクルード防止

#include "object.h"

class CObject2D;
class CScroll;
class COptionMenu;

//==========================================================================
// クラス定義
//==========================================================================
// タイトルのオプション選択肢クラス
class CTitle_OptionSelect : public CObject
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum STATE
	{
		STATE_NONE = 0,	// なにもない
		STATE_SCROLLWAIT,	// 巻き物待ち
		STATE_SCROLLWAIT_CLOSE,	// 巻き物待ち(閉じ)
		STATE_SELECT,	// 選択
		STATE_EDIT,		// 編集
		STATE_FADEIN,	// フェードイン
		STATE_FADEOUT,	// フェードアウト
		STATE_MAX
	};

	enum Select
	{
		SELECT_KEYKONFIG = 0,	// キーコンフィグ
		SELECT_SOUND,			// サウンド
		SELECT_OTHER,			// その他
		SELECT_MAX
	};

	CTitle_OptionSelect(int nPriority = 7);
	~CTitle_OptionSelect();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();						// 削除
	void SetState(const STATE& state);		// 状態設定
	STATE GetState() { return m_state; }	// 状態取得

	CScroll* GetScroll() { return m_pScroll; }	// 巻き物取得

	void SetBackSelect();	// 選択肢に戻る設定

	//=============================
	// 静的関数
	//=============================
	static CTitle_OptionSelect* Create();	// 生成処理

private:

	//=============================
	// 関数リスト
	//=============================
	// 状態リスト
	typedef void(CTitle_OptionSelect::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void UpdateState();	// 状態更新
	void StateNone();		// なにもなし
	void StateScrollWait();	// 巻き物待ち
	void StateScrollWait_Close();	// 巻き物待ち
	void StateSelect();		// 選択
	void StateEdit();		// 編集
	void StateFadeIn();		// フェードイン
	void StateFadeOut();	// フェードアウト

	// その他
	void CreateSelect();		// 選択肢生成
	void ChangeOptionMenu();	// オプションメニュー切り替え
	void ResetSelect();			// 選択肢リセット

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	STATE m_state;			// 状態
	float m_fStateTimer;	// 状態タイマー

	// その他
	Select m_select;		// 選択肢
	Select m_Oldselect;		// 前回の選択肢
	CObject2D* m_pSelect[Select::SELECT_MAX];	// 選択肢のオブジェクト
	CScroll* m_pScroll;		// 巻き物
	COptionMenu* m_pOptionMenu;	// オプションメニュー
};


#endif