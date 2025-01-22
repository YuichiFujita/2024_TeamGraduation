//==========================================================================
//
//	勝利チームヘッダー [winteamResult.h]
//	Author：相馬靜雅
//
//==========================================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _WINTEAM_RESULT_H_
#define _WINTEAM_RESULT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object2D_Anim.h"
#include "arrowUI.h"
#include "gameManager.h"

//************************************************************
// クラス定義
//************************************************************
// 勝利チームクラス
class CWinTeamResult : public CObject2D_Anim
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum EState
	{
		STATE_NONE = 0,	// なにもなし
		STATE_SPAWN,	// 登場
		STATE_LOOP,		// ループ
		STATE_FADEOUT,	// フェードアウト
		STATE_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CWinTeamResult(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CWinTeamResult() override;

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Kill() override;		// 削除
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 更新

	//=============================
	// メンバ関数
	//=============================
	void SetState(EState state);			// 状態設定
	EState GetState() { return m_state; }	// 状態取得
	void SetSizeByWidth(const float width);	// 横幅からサイズ設定

	//=============================
	// 静的メンバ関数
	//=============================
	static CWinTeamResult* Create(CGameManager::ETeamSide winSIde);	// 生成

private:

	//=============================
	// 関数リスト
	//=============================
	// 状態関数
	typedef void(CWinTeamResult::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// 状態関数

	//=============================
	// メンバ関数
	//=============================
	// 状態
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 状態更新
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {}	// なし
	void StateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 登場
	void StateLoop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ループ
	void StateFadeOut(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// フェードアウト

	// 生成
	HRESULT CreateAudience();	// 観客の生成

	//=============================
	// メンバ変数
	//=============================
	// 状態
	EState m_state;		// 状態
	float m_fStateTime;	// 状態タイマー

	// その他
	CGameManager::ETeamSide m_winTeam;	// 勝利チーム
	CObject2D_Anim* m_pAudience;	// 観客
};

#endif	// _ARROW_UI_H_
