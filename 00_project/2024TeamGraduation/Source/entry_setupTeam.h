//=============================================================================
//
// チーム等設定処理 [entry_setupTeam.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _ENTRY_SETUPTEAM_H_		// このマクロ定義がされていなかったら
#define _ENTRY_SETUPTEAM_H_		// 二重インクルード防止のマクロを定義する

//==========================================================================
// インクルードファイル
//==========================================================================
#include "entryscene.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CObjectChara;

//==========================================================================
// クラス定義
//==========================================================================
// チーム等設定クラス
class CEntry_SetUpTeam : public CEntryScene
{
public:

	CEntry_SetUpTeam();
	~CEntry_SetUpTeam();

	//=============================
	// メンバ関数
	//=============================
	virtual HRESULT Init() override;		// 初期化
	virtual void Uninit() override;		// 終了
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新

	//--------------------------
	// チーム情報
	//--------------------------
	int GetPlayerNum(CGameManager::ETeamSide side) { return m_nPlayerNum[side]; }	// プレイヤーの数取得
	int GetAllPlayerNum() { return (m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT] + m_nPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT]); }	// 全てのプレイヤーの数取得
	int GetEntryIdx(int i);									// エントリーのインデックス
	std::vector<int> GetIdxByTeam(CGameManager::ETeamSide side) { m_vecAddIdx[side]; }	// チームごとの追加されたインデックス取得
	CGameManager::ETeamSide GetTeamSide(int i) { return m_TeamSide[i]; }		// チームサイド

	//--------------------------
	// その他
	//--------------------------
	virtual CEntry_SetUpTeam* GetSetupTeam() { return this; }	// チーム等の設定取得
	virtual void Debug() override;

private:

	//=============================
	// メンバ関数
	//=============================
	void SelectTeam();		// チーム選択
	void ChangeMaxPlayer();	// プレイヤー最大数変更
	void Save();			// セーブ
	void Load();			// ロード

	//=============================
	// メンバ変数
	//=============================
	std::vector<int> m_vecAddIdx[CGameManager::ETeamSide::SIDE_MAX];	// 追加されたインデックス
	CGameManager::ETeamSide m_TeamSide[CGameManager::MAX_PLAYER];		// チームサイド
	int m_nMaxChangeIdx[CGameManager::ETeamSide::SIDE_MAX];				// 最大数変更するインデックス
	int m_nPlayerNum[CGameManager::ETeamSide::SIDE_MAX];				// プレイヤーの数
	int m_nEntryIdx[mylib_const::MAX_PLAYER];							// エントリーのインデックス
};

#endif