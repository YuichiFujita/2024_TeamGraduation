//==========================================================================
// 
//  ドレスアップ設定処理 [entry_dressup.h]
//  Author : 相馬靜雅
//  Adder  : 藤田勇一
// 
//==========================================================================

#ifndef _ENTRY_DRESSUP_H_
#define _ENTRY_DRESSUP_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "entryscene.h"
#include "player.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CDressupUI;
class CEntryRuleManager;

//==========================================================================
// クラス定義
//==========================================================================
// ドレスアップ設定クラス
class CEntry_Dressup : public CEntryScene
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// 状態
	enum EState
	{
		STATE_DRESSUP = 0,	// 着せ替え状態
		STATE_SETTING,		// ゲーム設定状態
		STATE_END,			// 終了状態
		STATE_MAX			// この列挙型の総数
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CEntry_Dressup();
	~CEntry_Dressup();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;	// 初期化
	virtual void Uninit() override;		// 終了
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新

	//-----------------------------
	// その他
	//-----------------------------
	virtual void Debug() override;

	//=============================
	// メンバ関数
	//=============================
	void SetState(const EState state);					// 状態設定
	inline EState GetState() const { return m_state; }	// 状態取得

private:

	//=============================
	// メンバ関数
	//=============================
	bool TransSetupTeam();	// チーム設定遷移
	void TransSetting();	// 着せ替え遷移
	bool IsAllReady();		// 準備全完了フラグ取得
	void Save();			// セーブ
	void Load();			// ロード

	//=============================
	// メンバ変数
	//=============================
	std::vector<CDressupUI*> m_vecDressInfo;	// 着せ替え情報
	CEntryRuleManager* m_pRuleManager;	// ルールマネージャー
	EState m_state;	// 状態
};

#endif