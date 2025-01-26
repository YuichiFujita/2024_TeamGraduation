//============================================================
//
//	ゲーム終了演出マネージャーヘッダー [gameEndManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GAME_END_MANAGER_H_
#define _GAME_END_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	前方宣言
//************************************************************
class CLightPoint;
class CPlayer;

//************************************************************
//	クラス定義
//************************************************************
// ゲーム終了演出マネージャークラス
class CGameEndManager : public CObject
{
public:
	// 定数
	static constexpr int NUM_LIGHT = 4;	// ライト数

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,		// 何もしない状態
		STATE_LOOK_PLAYER,	// プレイヤーカメラ状態
		STATE_DEATH_WAIT,	// プレイヤー死亡待機状態
		STATE_GAME_SET,		// ゲームセット出現状態
		STATE_WAIT,			// 待機状態
		STATE_END,			// 終了状態
		STATE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CGameEndManager(CPlayer* pDeathPlayer);

	// デストラクタ
	~CGameEndManager() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Kill() override;		// 削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw() override;		// 描画

	// メンバ関数
	void SetState(const EState state)	{ m_state = state; }	// 状態設定
	EState GetState(void) const			{ return m_state; }		// 状態取得

	// 静的メンバ関数
	static CGameEndManager *Create(CPlayer* pDeathPlayer);	// 生成

private:
	// エイリアス定義
	typedef void (CGameEndManager::*AFuncUpdateState)(const float, const float, const float);	// 状態更新の関数ポインタ型

	// 静的メンバ変数
	static AFuncUpdateState m_aFuncUpdateState[];	// 状態更新関数
	static CGameEndManager* m_pInstance;			// 自身のインスタンス

	// メンバ関数
	void UpdateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 何もしない更新
	void UpdateLookPlayer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// プレイヤーカメラ更新
	void UpdateDeathWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// プレイヤー死亡待機更新
	void UpdateGameSet(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ゲームセット出現更新
	void UpdateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 待機更新
	void UpdateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 終了更新
	void SetLightPosition();	// ライト位置設定

	// メンバ変数
	CLightPoint* m_apLight[NUM_LIGHT];	// ライト情報
	CPlayer* m_pDeathPlayer;	// 死亡プレイヤー
	EState m_state;		// 状態
	float m_fCurTime;	// 現在の待機時間
};

#endif	// _GAME_END_MANAGER_H_
