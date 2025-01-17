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
class CString2D;
class CDressupUI;
class CSelectUI;
class CObject2D;
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
	inline CEntry_Dressup* GetDressupTeam() override { return this; }	// ドレスアップ設定シーン取得

	//-----------------------------
	// その他
	//-----------------------------
	virtual void Debug() override;

	//=============================
	// メンバ関数
	//=============================
	void SetState(const EState state);					// 状態設定
	inline EState GetState() const	{ return m_state; }	// 状態取得
	void SetDressUIReady(const int nPlayerIdx, const bool bReady);			// 着せ替えUI準備完了フラグの設定
	bool IsDressUIReady(const int nPlayerIdx);								// 着せ替えUI準備完了済みかの確認
	void SetDressUIControl(const int nPadIdx, const int nPlayerIdx);		// 着せ替えUI操作権の設定
	bool IsSelectOK(const int nPadIdx, const int nPlayerIdx) const;			// 選択可能かの確認
	void SetSelectUISelect(const int nPadIdx, const bool bSelect);			// 選択UI選択操作フラグの設定
	MyLib::Vector3 GetDressUIPosition(const int nPlayerIdx) const;			// 着せ替えUI位置取得
	inline int GetNumPlayer() const	{ return (int)m_vecDressInfo.size(); }	// プレイヤー数取得
	MyLib::Vector3 GetNameUIPosition(const CGameManager::ETeamSide team);	// 名前UI位置取得
	MyLib::Vector2 GetNameUISize(const CGameManager::ETeamSide team);		// 名前UI大きさ取得
	MyLib::Vector3 GetBackUIPosition();	// 戻るUI位置取得
	MyLib::Vector2 GetBackUISize();		// 戻るUI大きさ取得
	inline CString2D* GetNameString2D(const CGameManager::ETeamSide team) { return m_apTeamName[team]; }	// 名前文字列ポインタ取得

private:

	//=============================
	// メンバ関数
	//=============================
	void TransSetting();	// 着せ替え遷移
	bool IsAllReady();		// 準備全完了フラグ取得
	void Save();			// セーブ
	void Load();			// ロード

	//=============================
	// メンバ変数
	//=============================
	CString2D* m_apTeamName[CGameManager::SIDE_MAX];	// チーム名情報
	std::vector<CDressupUI*> m_vecDressInfo;			// 着せ替え情報
	std::vector<CSelectUI*> m_vecSelect;				// 選択情報
	CEntryRuleManager* m_pRuleManager;					// ルールマネージャー
	CObject2D* m_pBack;		// 戻る情報
	CObject2D* m_pEnter;	// 決定情報
	EState m_state;			// 状態
};

#endif