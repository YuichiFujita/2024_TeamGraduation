//==========================================================================
// 
//  ボールヘッダー [ball.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _BALL_H_
#define _BALL_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "objectX.h"
#include "gamemanager.h"
#include "listmanager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayer;

//==========================================================================
// クラス定義
//==========================================================================
// ボールクラス
class CBall : public CObjectX
{
public:
	
	//=============================
	// 列挙型定義
	//=============================
	// 状態
	enum EState
	{
		STATE_SPAWN = 0,	// 生成状態 (開始時のフリーボール)
		STATE_CATCH,		// キャッチ状態 (プレイヤーが保持)
		STATE_HOM_NOR,		// 通常ホーミング状態 (ターゲット追従)
		STATE_HOM_JUMP,		// ジャンプホーミング状態 (ターゲット追従)
		STATE_MOVE,			// 移動状態 (慣性移動)
		STATE_REBOUND,		// リバウンド状態 (ぶつかった時の落下)
		STATE_LAND,			// 着地状態 (地面に転がっている)
		STATE_MAX			// この列挙型の総数
	};

	// 攻撃種類
	enum EAttack
	{
		ATK_NONE = 0,	// 攻撃判定無し
		ATK_NORMAL,		// 通常攻撃
		ATK_JUMP,		// ジャンプ攻撃
		ATK_SPECIAL,	// スペシャリスト攻撃
		ATK_MAX			// この列挙型の総数
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CBall(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CBall();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;

	//=============================
	// メンバ関数
	//=============================
	void Kill();	// 削除
	void CatchAttack(CPlayer* pPlayer);		// 攻撃キャッチ
	void CatchLand(CPlayer* pPlayer);		// 着地キャッチ
	void ThrowNormal(CPlayer* pPlayer);		// 通常投げ
	void ThrowJump(CPlayer* pPlayer);		// ジャンプ投げ
	void ThrowSpecial(CPlayer* pPlayer);	// スペシャル投げ

	CGameManager::TeamSide GetTypeTeam() const { return m_typeTeam; }	// チームサイド取得
	EAttack GetTypeAtk() const	{ return m_typeAtk; }	// 攻撃種類取得
	EState GetState() const		{ return m_state; }		// 状態取得
	float GetRadius() const;	// 半径取得
	bool IsAttack() const;		// 攻撃フラグ取得

	//=============================
	// 静的関数
	//=============================
	/*
		@brief		生成処理
		@details	位置
	*/
	static CBall *Create(const MyLib::Vector3& rPos);
	static CListManager<CBall> GetListObj() { return m_list; }	// リスト取得

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CBall::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFuncList[];	// 関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// オーバーライド関数
	void CalWorldMtx();	// ワールドマトリックスの計算

	// 状態関数
	void UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 生成状態の更新
	void UpdateCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// キャッチ状態の更新
	void UpdateHomingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 通常ホーミング状態の更新
	void UpdateHomingJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ジャンプホーミング状態の更新
	void UpdateMove(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 移動状態の更新
	void UpdateReBound(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// リバウンド状態の更新
	void UpdateLand(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 着地状態の更新

	void UpdateGravity(const float fDeltaRate, const float fSlowRate);	// 重力加算
	void UpdateDecay(const float fDeltaRate, const float fSlowRate);	// 移動量減速
	void UpdateGravityPosition(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// 位置に重力反映
	void UpdateMovePosition(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// 位置に移動量反映
	void UpdateMove(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// 移動
	bool UpdateLanding(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// 地面着地

	CPlayer* CollisionPlayer(MyLib::Vector3* pPos);	// プレイヤーとの当たり判定
	CPlayer* CollisionThrow(void);		// ホーミング対象との当たり判定
	void SetState(const EState state);	// 状態設定
	void Catch(CPlayer* pPlayer);		// キャッチ
	void Throw(CPlayer* pPlayer);		// 投げ
	void Landing(void);					// 着地
	void ReBound(CPlayer* pHitPlayer, MyLib::Vector3* pMove);	// リバウンド

	//=============================
	// 静的メンバ変数
	//=============================
	static CListManager<CBall> m_list;	// リスト

	//=============================
	// メンバ変数
	//=============================
	CPlayer* m_pPlayer;	// プレイヤー情報
	CPlayer* m_pTarget;	// ホーミングターゲット情報
	CPlayer* m_pCover;	// カバー対象プレイヤー情報
	float m_fMoveSpeed;	// 移動速度
	float m_fGravity;	// 重力
	MyLib::Vector3 m_oldOverLine;	// ホーミング終了ライン

	CGameManager::TeamSide m_typeTeam;	// チームサイド
	EAttack m_typeAtk;	// 攻撃種類
	EState m_state;		// 状態
	float m_fStateTime;	// 状態カウンター
};

#endif