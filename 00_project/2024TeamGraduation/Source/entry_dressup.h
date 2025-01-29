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
#include "playerManager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CString2D;
class CDressupUI;
class CSelectUI;
class CObject2D;
class CObject2D_Anim;
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

	// 遷移先
	enum ETrans
	{
		TRANS_BACK = 0,	// 戻る
		TRANS_NEXT,		// 進む
		TRANS_MAX		// この列挙型の総数
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
	bool TransSetting();													// 着せ替え遷移
	void SetState(const EState state);										// 状態設定
	inline EState GetState() const { return m_state; }						// 状態取得
	void SetDressUIReady(const int nPlayerIdx, const bool bReady);			// 着せ替えUI準備完了フラグの設定
	bool IsDressUIReady(const int nPlayerIdx);								// 着せ替えUI準備完了済みかの確認
	void SetDressUIControl(const int nPadIdx, const int nPlayerIdx);		// 着せ替えUI操作権の設定
	bool IsDressSelectOK(const int nPadIdx, const int nPlayerIdx) const;	// 着せ替えUI選択可能かの確認
	void SetSelectUISelect(const int nPadIdx, const bool bSelect);			// 選択UI選択操作フラグの設定
	MyLib::Vector3 GetDressUIPosition(const int nPlayerIdx) const;			// 着せ替えUI位置取得
	MyLib::Vector3 GetNameUIPosition(const CGameManager::ETeamSide team);	// 名前UI位置取得
	MyLib::Vector2 GetNameUISize(const CGameManager::ETeamSide team);		// 名前UI大きさ取得
	MyLib::Vector3 GetNameBGPosition(const CGameManager::ETeamSide team);	// 名前BG位置取得
	MyLib::Vector2 GetNameBGSize(const CGameManager::ETeamSide team);		// 名前BG大きさ取得
	MyLib::Vector3 GetAreaUIPosition(const CPlayer::EFieldArea area);		// ポジション変更UI位置取得
	MyLib::Vector2 GetAreaUISize(const CPlayer::EFieldArea area);			// ポジション変更UI大きさ取得
	MyLib::Vector3 GetTransUIPosition(const ETrans trans);					// 遷移UI位置取得
	MyLib::Vector2 GetTransUISize(const ETrans trans);						// 遷移UI大きさ取得
	int GetNumDressUI() const;												// 表示着せ替えUI数取得 (全着せ替えUI)
	int GetNumDressUI(const CGameManager::ETeamSide team) const;			// 表示着せ替えUI数取得 (チーム別着せ替えUI)
	void ChangeDressUIArea(const CGameManager::ETeamSide team);				// 着せ替えUIのポジション変更
	int GetTransUINumSelect(const int nSelectX) const;						// 遷移UI選択数取得
	int GetAreaUINumSelect(const int nSelectX) const;						// ポジション変更UI選択数取得
	bool IsNameSelectOK(const CGameManager::ETeamSide team) const;			// 名前選択可能かの確認
	bool IsAreaChangeOK(const CGameManager::ETeamSide team) const;			// ポジション変更可能かの確認
	void SetSelectObjectColor(const D3DXCOLOR& rCol, const MyLib::PosGrid2& rSelect);		// 選択オブジェクト色設定
	bool IsTeamReady(const CPlayer::EFieldArea area, const CGameManager::ETeamSide team);	// チームの準備全完了確認
	inline CString2D* GetNameString2D(const CGameManager::ETeamSide team) { return m_apTeamName[team]; }	// 名前文字列ポインタ取得
	void Save();	// セーブ

private:

	// 読込情報構造体
	struct SLoad
	{
		CPlayerManager::LoadInfo aInUser[mylib_const::MAX_PLAYER];	// 内野User着せ替え
		std::vector<CPlayerManager::LoadInfo> vecInAI;				// 内野AI着せ替え
		std::vector<CPlayerManager::LoadInfo> aVecOut[CGameManager::ETeamSide::SIDE_MAX];	// 外野着せ替え
	};

	//=============================
	// メンバ関数
	//=============================
	CDressupUI* GetPtrDressUI(const int nIdx) const;	// 着せ替えUI取得
	int GetIdxDressUI(const CDressupUI* pUI) const;		// X選択インデックス取得
	bool IsAllReady();	// 準備全完了フラグ取得
	SLoad Load();		// ロード
	std::vector<CPlayerManager::LoadInfo> LoadPlayer(std::ifstream* pFile, const int nTeam, const char* pEndKey);	// ロードプレイヤー

	//=============================
	// メンバ変数
	//=============================
	CObject2D* m_apNameBG[CGameManager::SIDE_MAX];		// チーム名背景情報
	CString2D* m_apTeamName[CGameManager::SIDE_MAX];	// チーム名情報
	CDressupUI* m_apDressInfo[CPlayerManager::OUT_MAX];	// 外野着せ替え情報
	CObject2D_Anim* m_apAreaUI[CGameManager::SIDE_MAX];	// ポジション変更UI情報
	CObject2D* m_apTransUI[TRANS_MAX];			// 遷移UI情報
	std::vector<CDressupUI*> m_vecDressInfo;	// 内野着せ替え情報
	std::vector<CSelectUI*> m_vecSelect;		// 選択情報
	CEntryRuleManager* m_pRuleManager;			// ルールマネージャー
	EState m_state;								// 状態
};

#endif