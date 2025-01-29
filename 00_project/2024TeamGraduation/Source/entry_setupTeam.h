//==========================================================================
// 
//  チーム等設定処理 [entry_setupTeam.h]
//  Author : 相馬靜雅
//  Adder  : 藤田勇
// 
//==========================================================================

#ifndef _ENTRY_SETUPTEAM_H_
#define _ENTRY_SETUPTEAM_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "entryscene.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CObjectChara;
class CObject2D;
class CObject2D_Anim;
class CNumber;
class CTransUI;
class CPadUI;

//==========================================================================
// クラス定義
//==========================================================================
// チーム等設定クラス
class CEntry_SetUpTeam : public CEntryScene
{
public:

	//=============================
	// 構造体定義
	//=============================
	// チーム保存構造体
	struct STeam
	{
		CGameManager::ETeamSide team;	// チームサイド
		int nPadIdx;	// 操作権インデックス
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CEntry_SetUpTeam();
	~CEntry_SetUpTeam();

	//=============================
	// 仮想関数
	//=============================
	virtual HRESULT Init() override;	// 初期化
	virtual void Uninit() override;		// 終了
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新

	//=============================
	// メンバ関数
	//=============================
	//-----------------------------
	// チーム情報
	//-----------------------------
	int PadIdxToEntryIdx(int nPadIdx);		// パッドインデックスのエントリーインデックス変換
	int PlayerIdxToPadIdx(int nPlayerIdx);	// プレイヤーインデックスのパッドインデックス変換
	CGameManager::ETeamSide GetTeamSide(int nPlayerIdx);	// チームサイド取得
	CGameManager::ETeamSide GetTeamSideAI(int nOccurIdx);	// チームサイド取得 (AI)
	inline int GetAllPlayerNum() { return (m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT] + m_nPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT]); }	// 全てのプレイヤーの数取得
	inline int GetPlayerNum(CGameManager::ETeamSide side)				{ return m_nPlayerNum[side]; }	// プレイヤーの数取得
	inline std::vector<int> GetIdxByTeam(CGameManager::ETeamSide side)	{ m_vecAddIdx[side]; }			// チームごとの追加されたインデックス取得

	//-----------------------------
	// インデックス
	//-----------------------------
	std::vector<int> GetEntryPlayerIdx();	// エントリー中プレイヤーインデックス取得

	//-----------------------------
	// その他
	//-----------------------------
	virtual inline CEntry_SetUpTeam* GetSetupTeam() { return this; }	// チーム等の設定取得
	virtual void Debug() override;

private:

	//=============================
	// メンバ関数
	//=============================
	//-----------------------------
	// 生成
	//-----------------------------
	HRESULT CreateNumInTeam();	// チーム人数の生成
	HRESULT CreatePadUI();		// コントローラーUIの生成
	HRESULT CreateTransUI();	// 遷移UIの生成
	HRESULT CreateBG();			// 背景の生成
	HRESULT CreateTeamSideUI();	// チームサイドUIの生成

	//-----------------------------
	// 削除
	//-----------------------------
	void FillAI();	// 空きメンバーのAI埋め
	void KillUI();	// UI削除

	//-----------------------------
	// 位置補正
	//-----------------------------
	void PosAdjUI(const bool bAllReady, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// UI位置補正
	void PosAdjPadUI();	// コントローラーUI位置補正
	void PosAdjPadUIToNumIn(const std::vector<CPadUI*>& vecPadUI, CGameManager::ETeamSide side);	// 内側にいる人数をもとにコントローラーUI位置補正

	//-----------------------------
	// プレイヤー操作
	//-----------------------------
	bool SelectTeam();		// チーム選択
	void ChangeMaxPlayer();	// プレイヤー最大数変更
	void TransDressUp(const bool bAllReady);	// 着せ替え遷移
	void DeleteEntry(int* pEntryIdx);			// エントリー解除

	//-----------------------------
	// 更新
	//-----------------------------
	void UpdateNumInUI(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// チーム人数の更新
	void UpdatePadUI(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// パッドUIの更新

	//-----------------------------
	// プレイヤーフラグ
	//-----------------------------
	bool IsUserReady(const int nUserIdx, std::vector<int>::iterator* pItr = nullptr);	// 準備完了フラグ取得
	bool IsUserTeamSelect(const int nUserIdx) const;	// チーム選択中フラグ取得
	bool IsUserMaxChange(const int nUserIdx) const;		// 最大数変更フラグ取得

	//-----------------------------
	// 外部ファイル操作
	//-----------------------------
	void Save();	// セーブ
	void Load();	// ロード
	void LoadIdx(std::ifstream* pFile, const CGameManager::ETeamSide team);	// ロードインデックス

	//=============================
	// メンバ変数
	//=============================
	//-----------------------------
	// UI情報
	//-----------------------------
	CPadUI* m_apPadUI[mylib_const::MAX_PLAYER];		// コントローラーUI情報
	CNumber* m_apNumInTeam[CGameManager::ETeamSide::SIDE_MAX];	// チーム人数情報
	CTransUI* m_pTransUI;	// 遷移UI情報
	CObject2D* m_pBG;		// 背景情報
	CObject2D_Anim* m_apTeamSideUI[CGameManager::ETeamSide::SIDE_MAX];	// チームサイドUI情報

	//-----------------------------
	// セットアップ情報
	//-----------------------------
	std::vector<int> m_vecAddIdx[CGameManager::ETeamSide::SIDE_MAX];	// 追加されたインデックス
	STeam m_TeamSide[CGameManager::MAX_PLAYER];				// チームサイド
	int m_nMaxChangeIdx[CGameManager::ETeamSide::SIDE_MAX];	// 最大数変更するインデックス
	int m_nPlayerNum[CGameManager::ETeamSide::SIDE_MAX];	// プレイヤーの数
	int m_nEntryIdx[mylib_const::MAX_PLAYER];				// エントリーのインデックス

	//-----------------------------
	// その他
	//-----------------------------
	float m_fTimeNumInTeam[CGameManager::ETeamSide::SIDE_MAX];	// チーム人数のタイマー
};

#endif