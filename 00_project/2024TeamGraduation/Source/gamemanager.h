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
#include "specialValueManager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CBall;
class CCollisionLine_Box;
class CTeamStatus;
class CGymWallManager;
class CTimerUI;

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
		SCENE_SPECIAL_STAG,		// スペシャル演出
		SCENE_END,				// 終了
		SCENE_DEBUG,			// デバッグ
		SCENE_MAX
	};

	// チームサイド列挙
	enum ETeamSide
	{
		SIDE_NONE = -1,	// コート不明
		SIDE_LEFT,		// 左コート
		SIDE_RIGHT,		// 右コート
		SIDE_MAX		// この列挙型の総数
	};

	CGameManager();
	~CGameManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetSceneType(ESceneType type);						// シーンの種類設定
	ESceneType GetType() { return m_SceneType; }		// シーンの種類取得
	ESceneType GetOldType() { return m_OldSceneType; }	// 前回のシーンの種類取得
	void SetEnableControll(bool bControll) { m_bControll = bControll; }	// 操作フラグを切り替えする
	bool IsControll() { return m_bControll; }							// 操作のフラグ取得
	MyLib::Vector3 GetCourtSize() { return m_courtSize; }					// コートサイズ取得
	MyLib::Vector3 GetCourtSize(const ETeamSide team, MyLib::Vector3& pos);	// コートサイズ取得(チーム)
	bool SetPosLimit(MyLib::Vector3& pos);									// コート移動制限
	CGymWallManager* GetGymWallManager() { return m_pGymWallManager; }

	void Debug();			// デバッグ
	void StartSetting();	// スタート時の設定
	CBall* GetBall();		// ボール取得
	CTeamStatus* GetTeamStatus(const ETeamSide team) { return m_pTeamStatus[team]; }	// チームステータス取得
	void AddCharmValue(ETeamSide side, CCharmManager::ETypeAdd charmType);			// モテ加算
	void SubCharmValue(ETeamSide side, CCharmManager::ETypeSub charmType);			// モテ減算
	void AddSpecialValue(ETeamSide side, CSpecialValueManager::ETypeAdd charmType);	// スペシャル加算

	static CGameManager* Create(CScene::MODE mode);				// 生成処理
	static CGameManager* GetInstance() { return m_pThisPtr; }	// インスタンス取得

private:
	
	//=============================
	// 関数リスト
	//=============================
	typedef void(CGameManager::*SCENE_FUNC)();
	static SCENE_FUNC m_SceneFunc[];	// 状態関数

	//=============================
	// メンバ関数
	//=============================
	// シーン
	void SceneMain();			// メイン
	void SceneStart();			// 開始演出
	void SceneSpecial_Stag();	// スペシャル演出
	void SceneEnd();			// 終了演出
	void SceneDebug();			// デバッグ

	// その他
	void UpdateLimitTimer();		// 制限時間更新
	void UpdateAudience();			// 観客更新
	void UpdateSpecialStag();		// スペシャル演出更新
	void UpdateTeamStatus();		// チームステータス更新
	void CreateTeamStatus();		// チームステータス生成

	//=============================
	// メンバ変数
	//=============================
	ESceneType m_SceneType;		// シーンの種類
	ESceneType m_OldSceneType;	// シーンの種類
	bool m_bControll;			// 操作できるか
	float m_fSceneTimer;		// シーンタイマー
	MyLib::Vector3 m_courtSize;						// コートのサイズ
	CTeamStatus* m_pTeamStatus[ETeamSide::SIDE_MAX];	// チームステータス

	//--------------------------
	// 生成したオブジェクト
	//--------------------------
	CGymWallManager* m_pGymWallManager;					// 体育館の壁
	CCharmManager* m_pCharmManager;						// モテマネージャ
	CSpecialValueManager* m_pSpecialValueManager;		// スぺ値マネージャ
	CTimerUI* m_pTimerUI;								// タイマーUI

#if _DEBUG
	CCollisionLine_Box* m_pCourtSizeBox = nullptr;	// コートサイズのボックス
#endif
	static CGameManager* m_pThisPtr;				// 自身のポインタ
};


#endif