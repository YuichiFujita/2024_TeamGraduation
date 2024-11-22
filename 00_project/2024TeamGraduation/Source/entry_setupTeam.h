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
	// その他
	//--------------------------
	virtual void Debug() override;

private:

	//=============================
	// メンバ関数
	//=============================
	void Save();

	//=============================
	// メンバ変数
	//=============================
	std::vector<int> m_vecAddIdx[CGameManager::ETeamSide::SIDE_MAX];	// 追加されたインデックス
	CGameManager::ETeamSide m_TeamSide[mylib_const::MAX_PLAYER];		// チームサイド
	int m_vecPlayerNum[CGameManager::ETeamSide::SIDE_MAX];				// プレイヤーの数
};

#endif