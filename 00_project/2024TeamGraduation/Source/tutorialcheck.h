//=============================================================================
//
//  チュートリアル確認画面ヘッダー [tutorialcheck.h]
//  Author：相馬靜雅
//
//=============================================================================
//==========================================================================
// 二重インクルード防止
//==========================================================================
#ifndef _TUTORIAL_CHECK_H_
#define _TUTORIAL_CHECK_H_

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object2D.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CTitle_ControllWait;	// 操作待機シーン

//==========================================================================
// クラス定義
//==========================================================================
// チュートリアル確認画面クラス
class CTutorialCheck : public CObject2D
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// 状態
	enum EState
	{
		STATE_NONE = 0,	// なにもなし
		STATE_FADEIN,	// フェードイン
		STATE_FADEOUT,	// フェードアウト
		STATE_MAX
	};

	// 選択肢
	enum ESelect
	{
		SELECT_YES = 0,	// チュートリアル見る
		SELECT_NO,		// 見ない
		SELECT_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CTutorialCheck(int controllIdx, int nPriority = mylib_const::PRIORITY_DEF2D);
	~CTutorialCheck() override;

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Kill() override;		// 削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新

	// メンバ関数
	void SetState(EState state);	// 状態設定

	//=============================
	// 静的メンバ関数
	//=============================
	static CTutorialCheck* Create(int controllIdx, CTitle_ControllWait* pControllWait);	// 生成

private:

	//=============================
	// 関数ポインタ
	//=============================
	typedef void(CTutorialCheck::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	// 状態
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 状態更新
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 何もなし
	void StateFadeIn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// フェードイン
	void StateFadeOut(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// フェードアウト

	// 生成
	HRESULT CreateBG();		// 背景生成
	HRESULT CreateSelect();	// 選択肢生成

	// その他
	void UpdateColor();	// 色の更新
	void Decide();		// 決定
	void Cancel();		// キャンセル

	//=============================
	// メンバ変数
	//=============================
	// 状態
	EState m_state;		// 状態
	float m_fStateTime;	// 状態タイマー

	// オブジェクト
	CObject2D* m_pSelect[ESelect::SELECT_MAX];	// 選択肢の情報
	CTitle_ControllWait* m_pControllWait;		// 操作待機情報

	// その他
	ESelect m_select;			// 選択肢
	const int m_nControllIdx;	// 操作インデックス
};

#endif	// _ARROW_UI_H_
