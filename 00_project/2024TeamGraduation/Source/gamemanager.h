//=============================================================================
// 
//  ゲームマネージャヘッダー [gamemanager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_	// 二重インクルード防止

#include "constans.h"
#include "instantfade.h"
#include "scene.h"

class CBall;

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
	enum SceneType
	{
		SCENE_MAIN = 0,			// メイン
		SCENE_START,			// 開始演出
		SCENE_SKIP,				// スキップ
		SCENE_COUNTDOWN,		// カウントダウン
		SCENE_MAINRESULT,		// メインリザルト
		SCENE_DURING_MAINRESULT,		// メインリザルト中
		SCENE_MAINCLEAR,		// メインクリア
		SCENE_BEFOREBATTLE,		// 戦闘準備
		SCENE_BATTLESTART,		// 戦闘開始
		SCENE_SKILLTREE,		// スキルツリー
		SCENE_BOSS,				// ボス
		SCENE_TRANSITIONWAIT,	// 遷移待機
		SCENE_TRANSITION,		// 転移
		SCENE_RESULT,			// リザルト
		SCENE_WAIT_AIRPUSH,		// 空気送り待ち
		SCENE_GOAL,				// ゴール
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

	CGameManager();
	~CGameManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetType(SceneType type);	// シーンの種類設定
	SceneType GetType();			// シーンの種類取得
	SceneType GetOldType() { return m_OldSceneType; }		// 前回のシーンの種類取得
	void SetEnableControll(bool bControll) { m_bControll = bControll; }		// 操作フラグを切り替えする
	bool IsControll() { return m_bControll; }				// 操作のフラグ取得
	MyLib::Vector3 GetCourtSize() { return m_courtSize; }	// コートサイズ取得
	void PosLimit(MyLib::Vector3& pos);	// コート移動制限

	void StartSetting();		// スタート時の設定
	void GameClearSettings();	// ゲームクリア時の設定
	void GameResultSettings();	// ゲームリザルトの設定

	CBall* GetBall();	// ボール取得

	static CGameManager *Create(CScene::MODE mode);	// 生成処理

protected:

	//=============================
	// メンバ関数
	//=============================
	virtual void SceneTransition();	// 遷移中
	virtual void SceneWaitAirPush();
	virtual void SceneStart();			// 開始演出
	virtual void SceneSkip();			// スキップ
	virtual void SceneGoal();
	virtual void ContainPlayerBaggage();
	virtual void TurnAway();
	virtual void CheckJudgeZone();
	virtual void SetEnemy();
	virtual void SetBoss();

	//=============================
	// メンバ変数
	//=============================
	SceneType m_SceneType;		// シーンの種類
	SceneType m_OldSceneType;	// シーンの種類
	bool m_bControll;		// 操作できるか
	float m_fSceneTimer;	// シーンタイマー

private:
	
	//=============================
	// メンバ関数
	//=============================

	//=============================
	// メンバ変数
	//=============================
	MyLib::Vector3 m_courtSize;		//コートのサイズ
};


#endif