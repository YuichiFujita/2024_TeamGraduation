//============================================================
//
//	登場演出チーム名表示UIヘッダー [spawnUI.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SPAWN_UI_H_
#define _SPAWN_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "gamemanager.h"

//************************************************************
// 前方宣言
//************************************************************
class CObject2D_Anim;
class CScrollString2D;

//************************************************************
//	クラス定義
//************************************************************
// 登場演出チーム名表示UIクラス
class CSpawnUI : public CObject
{
public:
	// コンストラクタ
	CSpawnUI(const CGameManager::ETeamSide team);

	// デストラクタ
	~CSpawnUI() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Kill() override;		// 削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新
	void Draw() override;		// 描画
	void SetPosition(const MyLib::Vector3& rPos) override;	// 位置設定
	void SetEnableDisp(const bool bDisp) override;			// 描画状況設定

	// 静的メンバ関数
	static CSpawnUI* Create(const CGameManager::ETeamSide team);	// 生成

	// メンバ関数
	inline void SetSpawn() { m_state = STATE_SPAWN; }	// 生成開始設定

private:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_SPAWN,	// 生成状態
		STATE_HUTATUNA,	// 二つ名状態
		STATE_TEAMNAME,	// チーム名状態
		STATE_DESPAWN,	// 消失状態
		STATE_MAX		// この列挙型の総数
	};

	// 状態関数リスト
	typedef void(CSpawnUI::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFuncList[];	// 関数のリスト

	// メンバ関数
	void UpdateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 何もしないの更新
	void UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 生成の更新
	void UpdateHutatuna(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 二つ名の更新
	void UpdateTeamname(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// チーム名の更新
	void UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 消失の更新
	void SetPositionRelative();	// 相対位置設定
	HRESULT CreateUI();	// UI生成

	// メンバ変数
	CObject2D_Anim* m_pBG;			// 背景情報
	CScrollString2D* m_pHutatuna;	// 二つ名情報
	CScrollString2D* m_pTeamName;	// チーム名情報
	const CGameManager::ETeamSide m_team;	// チーム
	EState m_state;		// 状態
	float m_fCurTime;	// 経過時間
};

#endif	// _SPAWN_UI_H_
