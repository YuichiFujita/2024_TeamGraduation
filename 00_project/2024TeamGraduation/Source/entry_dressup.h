//=============================================================================
//
// ドレスアップ設定処理 [entry_dressup.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _ENTRY_DRESSUP_H_		// このマクロ定義がされていなかったら
#define _ENTRY_DRESSUP_H_		// 二重インクルード防止のマクロを定義する

//==========================================================================
// インクルードファイル
//==========================================================================
#include "entryscene.h"
#include "player.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CDressup;

//==========================================================================
// クラス定義
//==========================================================================
// ドレスアップ設定クラス
class CEntry_Dressup : public CEntryScene
{
public:

	//=============================
	// 構造体定義
	//=============================
	struct SEntryDressup
	{
		CPlayer* pPlayer;		// プレイヤー
		CDressup* pHair;		// 髪着せ替え
		CDressup* pAccessory;	// アクセ着せ替え
	};

	CEntry_Dressup();
	~CEntry_Dressup();

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
	void Save();					// セーブ
	void Load();					// ロード
	void ChangeBodyType(int i);		// 体型変更
	void ChangeHandedness(int i);	// 利き手変更
	void ReCreatePlayer(int i, CPlayer::EHandedness handedness, CPlayer::EBody body);		// プレイヤー再生成

	//=============================
	// メンバ変数
	//=============================
	int m_nPlayerNum[CGameManager::ETeamSide::SIDE_MAX];	// プレイヤーの数
	std::vector<SEntryDressup> m_vecDressupInfo;			// 着せ替え情報
};

#endif