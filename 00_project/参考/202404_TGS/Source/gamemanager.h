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

class CRequestPeople;
class CReceiverPeople;
class CSkip_UI;
class CGuide;
class CSpawnEnvironment;

//==========================================================================
// クラス定義
//==========================================================================
// ゲームマネージャクラス定義
class CGameManager
{
public:

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

	enum AttackType
	{
		ATTACK_NORMAL = 0,	// 通常攻撃
		ATTACK_STRONG,		// 強攻撃
		ATTACK_COUNTER,		// 反撃攻撃
		ATTACK_MAX
	};

	CGameManager();
	~CGameManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	void SetType(SceneType type);	// シーンの種類設定
	SceneType GetType();		// シーンの種類取得
	SceneType GetOldType() { return m_OldSceneType; }		// 前回のシーンの種類取得
	void SetEnableRush() { m_bEndRush = true; }		// ラッシュの終了フラグを有効にする
	bool IsRushEnd() { return m_bEndRush; }			// 終了のフラグ取得
	void SetEnableControll(bool bControll) { m_bControll = bControll; }		// 操作フラグを切り替えする
	bool IsControll() { return m_bControll; }				// 操作のフラグ取得
	bool IsEndNormalStage() { return m_bEndNormalStage; }	// 通常ステージのア終了判定
	void AddNowStage();	// ステージの加算
	int GetNowStage();	// 現在のステージ取得
	void SetNumStage(int nStage);				// ステージの総数設定
	int GetNumStage() { return m_nNumStage; }	// ステージの総数取得
	int GetJudgeRank() { return m_nJudgeRank; }

	void StartSetting();		// スタート時の設定
	void GameClearSettings();	// ゲームクリア時の設定
	void GameResultSettings();	// ゲームリザルトの設定

	// 評価関連
	void AddEvaluationPoint(int point) { m_nEvaluationPoint += point; }	// ポイント加算
	void SetEvaluationPoint(int point) { m_nEvaluationPoint = point; }	// ポイント設定
	int GetEvalutionPoint() { return m_nEvaluationPoint; }	// ポイント取得
	void LoadEvaluationPoint() { m_nEvaluationPoint = m_nSaveEvaluationPoint; }	// 保存したポイントに変更
	void SaveEvaluationPoint() { m_nSaveEvaluationPoint = m_nEvaluationPoint; }	// ポイントを保存

	void SetReceiverPeople(CReceiverPeople* pPeople) { m_pReceiverPeople = pPeople; }

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
	SceneType m_SceneType;	// シーンの種類
	SceneType m_OldSceneType;	// シーンの種類
	bool m_bEndRush;		// ラッシュが終了したか
	bool m_bControll;		// 操作できるか
	bool m_bEndNormalStage;	// 通常ステージが終了したか
	bool m_bGameStart;		// ゲーム開始時のフラグ
	int m_nNowStage;		// 現在のステージ
	int m_nNumStage;		// ステージの総数
	int m_nEvaluationPoint;	// 評価ポイント
	int m_nSaveEvaluationPoint;	// 保存した評価ポイント
	int m_nGuideTimer;		//ガイドのタイマー
	float m_fCameraLengthOld;	// 前のカメラの距離
	float m_fPosRY;			// プレイヤーと荷物を画面内に収めるときに使うposRのY座標
	float m_fSceneTimer;	// シーンタイマー
	int m_nJudgeRank;		// クリアランク
	CGuide* m_pGuide;
	CRequestPeople* m_pRequestPeople;	// 依頼人のポインタ
	CReceiverPeople* m_pReceiverPeople;	// 届け先のポインタ
	CSkip_UI* m_pSkipUI;				// スキップUIのポインタ

private:
	
	//=============================
	// メンバ関数
	//=============================

	//=============================
	// メンバ変数
	//=============================
	CSpawnEnvironment* m_pSpawn_Air;		// 空気生成
	CSpawnEnvironment* m_pSpawn_LeafFlow;	// 流れる葉生成
	CSpawnEnvironment* m_pSpawn_Leaf;		// 降る葉生成
};


#endif