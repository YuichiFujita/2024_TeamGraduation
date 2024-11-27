//============================================================
//
//	プレイヤー登場演出ヘッダー [playerSpawnManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYER_SPAWN_MANAGER_H_
#define _PLAYER_SPAWN_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	前方宣言
//************************************************************
class CPlayer;

//************************************************************
//	クラス定義
//************************************************************
// プレイヤー登場演出クラス
class CPlayerSpawnManager : public CObject
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,		// 何もしない状態
		STATE_CUTIN,		// カットイン状態
		STATE_END,			// 終了状態
		STATE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CPlayerSpawnManager();

	// デストラクタ
	~CPlayerSpawnManager() override;

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
	static CPlayerSpawnManager* Create();		// 生成
	static CPlayerSpawnManager* GetInstance();	// 取得

private:
	// エイリアス定義
	typedef void (CPlayerSpawnManager::*AFuncUpdateState)(const float, const float, const float);	// 状態更新の関数ポインタ型

	// 静的メンバ変数
	static AFuncUpdateState m_aFuncUpdateState[];	// 状態更新関数
	static CPlayerSpawnManager* m_pInstance;		// 自身のインスタンス

	// メンバ関数
	void UpdateCutIn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// カットイン更新

	// メンバ変数
	EState m_state;		// 状態
	float m_fCurTime;	// 現在の待機時間
};

#endif	// _PLAYER_SPAWN_MANAGER_H_
