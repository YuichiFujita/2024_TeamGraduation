//=============================================================================
//
//  チュートリアル画面ヘッダー [tutorialscreen.h]
//  Author：相馬靜雅
//
//=============================================================================
//==========================================================================
// 二重インクルード防止
//==========================================================================
#ifndef _TUTORIAL_SCREEN_H_
#define _TUTORIAL_SCREEN_H_

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object2D.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// チュートリアル画面クラス
class CTutorialScreen : public CObject2D
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// 説明の種類
	enum EManualType
	{
		TYPE_MOVE = 0,		// 移動
		TYPE_BLINK,			// ブリンク
		TYPE_JUMP,			// ジャンプ
		TYPE_THROW,			// 投げ
		TYPE_PASS,			// パス
		TYPE_CATCH,			// キャッチ
		TYPE_SP,			// スペシャル
		TYPE_THROW_JUMP,	// ジャンプ投げ
		TYPE_THROW_DASH,	// ダッシュ投げ
		TYPE_JUSTCATCH,		// ジャストキャッチ
		TYPE_MAX
	};

	// 状態
	enum EState
	{
		STATE_NONE = 0,	// なにもなし
		STATE_SPAWN,	// 登場
		STATE_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CTutorialScreen(int nPriority = mylib_const::PRIORITY_DEF2D);
	~CTutorialScreen() override;

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
	static CTutorialScreen* Create();	// 生成

private:

	//=============================
	// 関数ポインタ
	//=============================
	typedef void(CTutorialScreen::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	// 状態
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 状態更新
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 何もなし
	void StateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 登場

	// 生成
	HRESULT CreateBG();		// 背景生成
	HRESULT CreateFade();	// フェード生成
	HRESULT CreateManual();	// 説明生成

	// 更新
	void UpdateBG(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 背景更新
	void ChangeManual();	// 説明の変更処理

	//=============================
	// メンバ変数
	//=============================
	// 状態
	EState m_state;		// 状態
	float m_fStateTime;	// 状態タイマー

	// オブジェクト
	CObject2D* m_pFade;		// フェードの情報
	CObject2D* m_pManual;	// 説明の情報

	// その他
	EManualType m_ManualType;	// 説明の種類
};

#endif	// _ARROW_UI_H_
