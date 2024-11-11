//=============================================================================
// 
//  ゲームマネージャヘッダー [gamemanager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "constans.h"
#include "instantfade.h"
#include "scene.h"
#include "charmManager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CBall;
class CCollisionLine_Box;
class CTeamStatus;
class CGymWallManager;

//==========================================================================
// クラス定義
//==========================================================================
// ゲームマネージャクラス定義
class CGameManager
{
public:

	// 定数
	static constexpr float FIELD_LIMIT = 0.0f;	// 地面Y座標
	static constexpr float CENTER_LINE = 0.0f;	// センターラインX座標

	// シーンの種類
	enum ESceneType
	{
		SCENE_MAIN = 0,			// メイン
		SCENE_START,			// 開始演出
		SCENE_BEFOREBATTLE,		// 戦闘準備
		SCENE_BATTLESTART,		// 戦闘開始
		SCENE_SPECIAL_STAG,		// スペシャル演出
		SCENE_DEBUG,			// デバッグ
		SCENE_MAX
	};

	// チームサイド列挙
	enum TeamSide
	{
		SIDE_NONE = 0,	// コート不明
		SIDE_LEFT,		// 左コート
		SIDE_RIGHT,		// 右コート
		SIDE_MAX		// この列挙型の総数
	};

	// チーム種類列挙
	enum TeamType
	{
		TYPE_LEFT = 1,	// 左コート
		TYPE_RIGHT,		// 右コート
		TYPE_MAX = 2	// この列挙型の総数
	};

	CGameManager();
	~CGameManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetType(ESceneType type);						// シーンの種類設定
	ESceneType GetType() { return m_SceneType; }		// シーンの種類取得
	ESceneType GetOldType() { return m_OldSceneType; }	// 前回のシーンの種類取得
	void SetEnableControll(bool bControll) { m_bControll = bControll; }	// 操作フラグを切り替えする
	bool IsControll() { return m_bControll; }							// 操作のフラグ取得
	MyLib::Vector3 GetCourtSize() { return m_courtSize; }					// コートサイズ取得
	MyLib::Vector3 GetCourtSize(const TeamSide team, MyLib::Vector3& pos);	// コートサイズ取得(チーム)
	bool SetPosLimit(MyLib::Vector3& pos);									// コート移動制限
	CGymWallManager* GetGymWallManager() { return m_pGymWallManager; }

	void Debug();			// デバッグ
	void StartSetting();	// スタート時の設定
	CBall* GetBall();		// ボール取得
	CTeamStatus* GetTeamStatus(const int nTeam) { return m_pTeamStatus[nTeam]; }	// チームステータス取得
	void AddCharmValue(TeamSide side, CCharmManager::EType charmType);	// モテ加算

	static CGameManager* Create(CScene::MODE mode);				// 生成処理
	static CGameManager* GetInstance() { return m_pThisPtr; }	// インスタンス取得

private:
	
	//=============================
	// メンバ関数
	//=============================
	virtual void SceneStart();		// 開始演出
	void UpdateAudience();			// 観客更新
	void UpdateSpecialStag();		// スペシャル演出更新
	void CreateTeamStatus();		// チームステータス生成

	//=============================
	// メンバ変数
	//=============================
	ESceneType m_SceneType;		// シーンの種類
	ESceneType m_OldSceneType;	// シーンの種類
	bool m_bControll;			// 操作できるか
	float m_fSceneTimer;		// シーンタイマー

	CGymWallManager* m_pGymWallManager;

	MyLib::Vector3 m_courtSize;						// コートのサイズ
	CTeamStatus* m_pTeamStatus[TeamType::TYPE_MAX];	// チームステータス
	static CGameManager* m_pThisPtr;				// 自身のポインタ

#if _DEBUG
	CCollisionLine_Box* m_pCourtSizeBox = nullptr;	// コートサイズのボックス
#endif
};


#endif