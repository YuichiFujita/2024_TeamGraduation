//============================================================
//
//	遷移UIヘッダー [transUI.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TRANS_UI_H_
#define _TRANS_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
// 前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// 遷移UIクラス
class CTransUI : public CObject
{
public:
	// コンストラクタ
	CTransUI();

	// デストラクタ
	~CTransUI() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Kill() override;		// 削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw() override;		// 描画
	void SetEnableDisp(const bool bDisp) override;	// 描画状況設定

	// 静的メンバ関数
	static CTransUI* Create();	// 生成

	// メンバ関数
	bool UpdateDecide(const bool bAutoUninit = false);	// 決定操作更新
	void SetEnableDispUI(const bool bDisp);				// UI表示状況設定

private:
	// 状態列挙
	enum EState
	{
		STATE_DISP_OFF = 0,	// 表示OFF状態
		STATE_SPAWN_BG,		// 背景生成状態
		STATE_SPAWN_STRING,	// 文字生成状態
		STATE_DISP_ON,		// 表示ON状態
		STATE_MAX			// この列挙型の総数
	};

	// 状態関数リスト
	typedef void(CTransUI::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFuncList[];	// 関数のリスト

	// メンバ関数
	void UpdateDispOFF(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 表示OFFの更新
	void UpdateSpawnBG(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 背景生成の更新
	void UpdateSpawnString(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 文字生成の更新
	void UpdateDispON(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 表示ONの更新
	void SetDisp(const bool bDisp);	// 表示状況設定
	bool IsDispTransState();		// 表示遷移状態かの確認
	HRESULT CreateUI();				// UI生成

	// メンバ変数
	CObject2D* m_pBG;		// 背景情報
	CObject2D* m_pString;	// 文字情報
	EState m_state;			// 状態
	float m_fCurTime;		// 経過時間
};

#endif	// _TRANS_UI_H_
