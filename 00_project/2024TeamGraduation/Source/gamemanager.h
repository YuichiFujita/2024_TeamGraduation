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
#include "charmValueManager.h"
#include "specialValueManager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CBall;
class CCollisionLine_Box;
class CTeamStatus;
class CGymDoor;
class CGymWallManager;
class CTimerUI;
class CCharmManager;

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
	static constexpr int MAX_SIDEPLAYER = 3;	// サイド毎のプレイヤー最大数

	// シーンの種類
	enum ESceneType
	{
		SCENE_MAIN = 0,		// メイン
		SCENE_SPAWN,		// 登場演出
		SCENE_START,		// 開始演出
		SCENE_SPECIAL_STAG,	// スペシャル演出
		SCENE_END,			// 終了
		SCENE_DEBUG,		// デバッグ
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

	// ドア列挙
	enum EDoor
	{
		DOOR_LEFT = 0,	// 左ドア
		DOOR_RIGHT,		// 右ドア
		DOOR_MAX		// この列挙型の総数
	};

	// 試合終了時情報
	struct SEndInfo
	{
		ETeamSide m_winteam;	// 勝利チーム
		float m_fTension;		// 盛り上がり度
	
		SEndInfo() : m_winteam(ETeamSide::SIDE_NONE), m_fTension(0.0f) {};
	};

	CGameManager();
	~CGameManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetSceneType(ESceneType type);					// シーンの種類設定
	ESceneType GetType() { return m_SceneType; }		// シーンの種類取得
	ESceneType GetOldType() { return m_OldSceneType; }	// 前回のシーンの種類取得
	void SetEnableControll(bool bControll) { m_bControll = bControll; }		// 操作フラグを切り替えする
	bool IsControll() { return m_bControll; }								// 操作のフラグ取得
	MyLib::Vector3 GetCourtSize() { return m_courtSize; }					// コートサイズ取得
	MyLib::Vector3 GetCourtSize(const ETeamSide team, MyLib::Vector3& pos);	// コートサイズ取得(チーム)
	float GetHalfCourtDiagonal();											// チームコートの対角線取得
	bool SetPosLimit(MyLib::Vector3& pos, const float fPlusRadius = 0.0f);	// コート移動制限
	CGymWallManager* GetGymWallManager() { return m_pGymWallManager; }		// 体育館の壁情報取得

	void Debug();			// デバッグ
	void StartSetting();	// スタート時の設定
	CBall* GetBall();		// ボール取得
	CTeamStatus* GetTeamStatus(const ETeamSide team) { return m_pTeamStatus[team]; }	// チームステータス取得
	void AddCharmValue(ETeamSide side, CCharmValueManager::ETypeAdd charmType);			// モテ加算
	void SubCharmValue(ETeamSide side, CCharmValueManager::ETypeSub charmType);			// モテ減算
	void AddSpecialValue(ETeamSide side, CSpecialValueManager::ETypeAdd charmType);		// スペシャル加算
	
	void EndGame();			// 試合終了
	void CheckVictory();	// 勝利チーム決定

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
	void SceneSpawn();			// 登場演出
	void SceneStart();			// 開始演出
	void SceneSpecialStag();	// スペシャル演出
	void SceneEnd();			// 終了演出
	void SceneDebug();			// デバッグ

	// その他
	void UpdateLimitTimer();	// 制限時間更新
	void UpdateAudience();		// 観客更新
	void UpdateSpecialStag();	// スペシャル演出更新
	void UpdateTeamStatus();	// チームステータス更新
	void CreateTeamStatus();	// チームステータス生成
	void Save();				// チームステータス保存

	//=============================
	// メンバ変数
	//=============================
	ESceneType m_SceneType;		// シーンの種類
	ESceneType m_OldSceneType;	// シーンの種類
	bool m_bControll;			// 操作できるか
	float m_fSceneTimer;		// シーンタイマー
	MyLib::Vector3 m_courtSize;							// コートのサイズ
	CTeamStatus* m_pTeamStatus[ETeamSide::SIDE_MAX];	// チームステータス
	SEndInfo m_endInfo;									// 終了時情報

	//--------------------------
	// 生成したオブジェクト
	//--------------------------
	CGymDoor* m_apGymDoor[EDoor::DOOR_MAX];			// 体育館のドア
	CGymWallManager* m_pGymWallManager;				// 体育館の壁
	CCharmManager* m_pCharmManager;					// モテマネージャ
	CCharmValueManager* m_pCharmValueManager;		// モテ値マネージャ
	CSpecialValueManager* m_pSpecialValueManager;	// スぺ値マネージャ
	CTimerUI* m_pTimerUI;							// タイマーUI

#if _DEBUG
	CCollisionLine_Box* m_pCourtSizeBox = nullptr;	// コートサイズのボックス
#endif
	static CGameManager* m_pThisPtr;				// 自身のポインタ
};


#endif