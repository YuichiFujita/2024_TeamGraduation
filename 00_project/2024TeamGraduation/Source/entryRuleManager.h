//============================================================
//
//	エントリールールマネージャーヘッダー [entryRuleManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENTRYRULE_MANAGER_H_
#define _ENTRYRULE_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_RULE_ARROW	(2)	// 矢印の総数

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;		// オブジェクト2Dクラス
class CObject2D_Anim;	// オブジェクト2Dアニメクラス
class CTimeUI;			// タイムUIクラス
class CEntry_Dressup;	// ドレスアップ設定クラス

//************************************************************
//	クラス定義
//************************************************************
// エントリールールマネージャークラス
class CEntryRuleManager
{
public:
	// テクスチャ列挙
	enum ETexture
	{
		TEXTURE_RULE_TITLE = 0,	// ルールタイトルテクスチャ
		TEXTURE_LIFE,			// 体力テクスチャ
		TEXTURE_START,			// 開始ボタンテクスチャ
		TEXTURE_CONTROL,		// 操作表示テクスチャ
		TEXTURE_ARROW,			// 矢印テクスチャ
		TEXTURE_SELECT,			// 選択テクスチャ
		TEXTURE_MAX				// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_INIT = 0,		// 初期化状態
		STATE_FADEIN,		// フェードイン状態
		STATE_RULESET,		// ルール設定状態
		STATE_FADEOUT,		// フェードアウト状態
		STATE_ENTRYBACK,	// エントリー戻し状態
		STATE_MAX			// この列挙型の総数
	};

	// ルール列挙
	enum ERule
	{
		RULE_TIME = 0,	// 時間
		RULE_LIFE,		// 体力
		RULE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CEntryRuleManager(CEntry_Dressup* pParent);

	// デストラクタ
	~CEntryRuleManager();

	// メンバ関数
	HRESULT Init();		// 初期化
	HRESULT Uninit();	// 終了
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 更新

	// 静的メンバ関数
	static CEntryRuleManager* Create(CEntry_Dressup* pParent);		// 生成
	static void Release(CEntryRuleManager*& prEntryRuleManager);	// 破棄

private:
	// メンバ関数
	void UpdateFadeIn();	// フェードイン更新
	void UpdateFadeOut();	// フェードアウト更新
	void UpdateControl();	// 操作更新
	void UpdateArrow();		// 矢印更新
	void Select();			// 選択操作
	void Decide();			// 決定操作
	void ChangeRule();		// ルール変更操作
	void SetRule();			// ルール変更反映
	void SetEnableDispUI(const bool bDisp);	// UI描画設定
	void SetAlphaUI(const float fAlpha, const bool bFadeOut);	// UI透明度設定

	// メンバ変数
	CObject2D_Anim*	m_apRuleTitle[RULE_MAX];	// ルールタイトルの情報
	CObject2D_Anim*	m_apArrow[MAX_RULE_ARROW];	// 矢印の情報
	CObject2D_Anim*	m_pLife;	// 体力の情報
	CTimeUI*	m_pTime;		// 時間の情報
	CObject2D*	m_pSelect;		// 選択の情報
	CObject2D*	m_pStart;		// 開始ボタンの情報
	CObject2D*	m_pControl;		// 操作表示の情報
	CObject2D*	m_pFade;		// フェードの情報
	CEntry_Dressup* m_pParent;	// 親の情報
	EState	m_state;			// 状態
	int		m_nSelect;			// 現在の選択
	int		m_nOldSelect;		// 前回の選択
	float	m_fSinControlAlpha;	// 操作表示の透明向き
	float	m_fSinArrowAlpha;	// 矢印表示の透明向き
};

#endif	// _ENTRYRULE_MANAGER_H_
