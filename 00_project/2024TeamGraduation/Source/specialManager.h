//============================================================
//
//	スペシャル演出マネージャーヘッダー [specialManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SPECIAL_MANAGER_H_
#define _SPECIAL_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	前方宣言
//************************************************************
class CPlayer;
class CLightPoint;

//************************************************************
//	クラス定義
//************************************************************
// スペシャル演出マネージャークラス
class CSpecialManager : public CObject
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_FADEOUT,	// フェードアウト状態
		STATE_END,		// 終了状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CSpecialManager(const CPlayer* pAttack, const CPlayer* pTarget);

	// デストラクタ
	~CSpecialManager() override;

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
	static CSpecialManager *Create(const CPlayer* pAttack, const CPlayer* pTarget);	// 生成

private:
	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CSpecialManager::*AFuncUpdateState)(const float fDeltaTime);

	// 静的メンバ変数
	static AFuncUpdateState m_aFuncUpdateState[];	// 状態更新関数
	static CSpecialManager* m_pThisClass;			// 自身のインスタンス

	// メンバ関数
	void UpdateFadeOut(const float fDeltaTime);	// フェードアウト更新
	void UpdateEnd(const float fDeltaTime);		// 終了更新

	void SetLightPosition();	// ライト位置設定

	// メンバ変数
	const CPlayer* m_pAttackPlayer;	// 攻撃プレイヤー
	const CPlayer* m_pTargetPlayer;	// 標的プレイヤー
	CLightPoint* m_pAttackLight;	// 攻撃プレイヤーを照らすライト
	CLightPoint* m_pTargetLight;	// 標的プレイヤーを照らすライト
	EState m_state;		// 状態
	float m_fCurTime;	// 現在の待機時間
};

#endif	// _SPECIAL_MANAGER_H_
