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
class CShadow;
class CPlayer;
class CBallHolderMarker;
class CBallLandingMarker;
class CEffekseerObj;

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
		STATE_SPAWN = 0,		// 生成状態 (開始時のフリーボール)
		STATE_CATCH,			// キャッチ状態 (プレイヤーが保持)
		STATE_HOM_NOR,			// 通常ホーミング状態
		STATE_HOM_JUMP,			// ジャンプホーミング状態
		STATE_MOVE,				// 移動状態
		STATE_SPECIAL_STAG,		// スペシャル演出状態
		STATE_SPECIAL_THROW,	// スペシャル投げ状態
		STATE_HOM_PASS,			// パスホーミング状態
		STATE_PASS,				// パス状態
		STATE_CENTER_RETURN,	// 中央投げ状態
		STATE_REBOUND,			// リバウンド状態 (ぶつかった時の落下)
		STATE_FREE,				// フリー状態 (触れただけで取れる)
		STATE_LAND,				// 着地状態 (地面に転がっている)
		STATE_MAX				// この列挙型の総数
	};

	// スペシャル種類		//Caution: 追加したらcatchSpecialのCHECK_FUNCも追加して
	enum ESpecial
	{
		SPECIAL_NONE = 0,	// スペシャル無し
		SPECIAL_KAMEHAMEHA,	// かめはめ波
		SPECIAL_MAX			// この列挙型の総数
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
	virtual void Kill() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;

	//=============================
	// メンバ関数
	//=============================
	void CatchAttack(CPlayer* pPlayer);			// 攻撃キャッチ
	void CatchLand(CPlayer* pPlayer);			// 着地キャッチ
	void Spawn(CPlayer* pPlayer);				// スポーン
	void ThrowNormal(CPlayer* pPlayer);			// 通常投げ
	void ThrowJump(CPlayer* pPlayer);			// ジャンプ投げ
	void Special(CPlayer* pPlayer);				// スペシャル発動
	bool Pass(CPlayer* pPlayer);				// パス
	void CenterReturn(CPlayer* pPlayer);		// 中央投げ
	void Toss(CPlayer* pPlayer);				// トス
	void SpecialEndSetting();					// スペシャル終了
	MyLib::Vector3 GetPosPassStart() const		{ return m_posPassStart; }	// パス開始位置取得
	MyLib::Vector3 GetPosPassEnd() const		{ return m_posPassEnd; }	// パス終了位置取得
	CGameManager::ETeamSide GetTypeTeam() const	{ return m_typeTeam; }		// チームサイド取得
	ESpecial GetTypeSpecial() const	{ return m_typeSpecial; }	// スペシャル種類取得
	EAttack GetTypeAtk() const		{ return m_typeAtk; }		// 攻撃種類取得
	EState GetState() const			{ return m_state; }			// 状態取得
	CPlayer* GetPlayer() const		{ return m_pPlayer; }		// 所持プレイヤー取得
	CPlayer* GetTarget() const		{ return m_pTarget; }		// ターゲット取得
	CPlayer* GetCover() const		{ return m_pCover; }		// カバー対象プレイヤー取得
	int GetDamage() const			{ return m_nDamage; }		// ダメージ取得
	int GetCoverHeal() const		{ return m_nCoverHeal; }	// カバー回復取得
	float GetKnockback() const		{ return m_fKnockback; }	// ノックバック取得
	bool IsLanding() const			{ return m_bLanding; }		// 着地フラグ取得
	float GetRadius() const;		// 半径取得
	bool IsAttack() const;			// 攻撃フラグ取得
	bool IsPass() const;			// パスフラグ取得
	bool IsSpecial() const;			// スペシャルフラグ取得

	//=============================
	// 静的メンバ関数
	//=============================
	/*
		@brief		生成処理
		@details	位置
	*/
	static CBall *Create(const MyLib::Vector3& rPos);
	static CListManager<CBall> GetListObj() { return m_list; }	// リスト取得

protected:
	//=============================
	// メンバ関数
	//=============================
	void Throw(CPlayer* pPlayer);	// 投げ

private:

	//=============================
	// 構造体定義
	//=============================
	// ヒットタイミング情報
	struct SHitTimingInfo
	{
		int nHitFrame;	// ヒットまでにかかるフレーム数
		float fHitTime;	// ヒットまでにかかる時間
		bool bHit;		// ヒットしたか
	};

	//=============================
	// 関数リスト
	//=============================
	// 状態関数
	typedef void(CBall::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFuncList[];	// 関数のリスト

	// スペシャル関数
	typedef void(CBall::*SPECIAL_FUNC)(const float, const float, const float);
	static SPECIAL_FUNC m_SpecialFuncList[];	// 関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// オーバーライド関数
	void CalWorldMtx() override;	// ワールドマトリックスの計算

	//--------------------------
	// 状態関数
	//--------------------------
	void UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 生成状態の更新
	void UpdateCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// キャッチ状態の更新
	void UpdateHomingNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 通常ホーミング状態の更新
	void UpdateHomingJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ジャンプホーミング状態の更新
	void UpdateMove(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 移動状態の更新
	void UpdateSpecialStag(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// スペシャル演出状態の更新
	void UpdateSpecialThrow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// スペシャル投げ状態の更新
	void UpdateHomingPass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// パスホーミング状態の更新
	void UpdatePass(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// パス状態の更新
	void UpdateCenterReturn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 中央投げ状態の更新
	void UpdateReBound(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// リバウンド状態の更新
	void UpdateFree(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// フリー状態の更新
	void UpdateLand(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 着地状態の更新

	//--------------------------
	// スペシャル関数
	//--------------------------
	void UpdateKamehameha(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// かめはめ波の更新

	//--------------------------
	// 汎用関数
	//--------------------------
	void UpdateGravity(const float fDeltaRate, const float fSlowRate);	// 重力加算
	void UpdateDecay(const float fDeltaRate, const float fSlowRate);	// 移動量減速
	void UpdateUpOnlyGravityPosition(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// 位置に重力反映 (上昇のみ)
	void UpdateGravityPosition(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// 位置に重力反映
	void UpdateMovePosition(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// 位置に移動量反映
	void UpdateMove(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// 移動
	bool UpdateLanding(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaRate, const float fSlowRate);	// 地面着地
	CPlayer* CollisionPlayer(MyLib::Vector3* pPos);				// プレイヤーとの当たり判定
	CPlayer* CollisionThrowTarget(const bool bAbsLock = false);	// 投げる対象との当たり判定
	CPlayer* CollisionPassTarget();		// パスする対象との当たり判定
	float GetCollRadius();				// 判定半径取得
	void SetState(const EState state);	// 状態設定
	void Catch(CPlayer* pPlayer);		// キャッチ
	void ThrowSpecial();				// スペシャル投げ
	void Landing();						// 着地
	void UpdateTypeTeam();				// チームサイド更新
	void UpdateTypeAtk();				// 攻撃種類更新
	void UpdateTypeSpecial();			// スペシャル種類更新
	void OutcomeThrowJust();			// ジャスト投げ効果
	void OutcomeThrowDrop();			// ドロップ投げ効果
	void CalcSetInitialSpeed(const float fMove);	// 初速計算
	void ReBound(CPlayer* pHitPlayer, MyLib::Vector3* pMove);	// リバウンド

	MyLib::Vector3 CalcVecMove(CPlayer* pTarget, CPlayer* pPlayer);	// 移動ベクトル計算
	SHitTimingInfo CalcHitSpeedTime	// ボールが敵に到達するまでの時間計算
	( // 引数
		const MyLib::Vector3& rPosTarget,	// ターゲット位置
		const MyLib::Vector3& rPosBall,		// ボール位置
		const float fRadiusTarget,			// ターゲット半径
		const float fInitSpeed,				// 初速
		const float fMoveSpeed				// 移動量
	);

	//--------------------------
	// エフェクト
	//--------------------------
	void UpdateThrowLine();	// 投げた線の更新

	//=============================
	// 静的メンバ変数
	//=============================
	static CListManager<CBall> m_list;	// リスト

	//=============================
	// メンバ変数
	//=============================
	CBallHolderMarker* m_pHoldMarker;	// ボール所持マーカー情報
	CBallLandingMarker* m_pLandMarker;	// ボール着地点マーカー情報
	CShadow* m_pShadow;		// 影情報
	CPlayer* m_pPlayer;		// プレイヤー情報
	CPlayer* m_pTarget;		// ホーミングターゲット情報
	CPlayer* m_pCover;		// カバー対象プレイヤー情報
	float m_fMoveSpeed;		// 移動速度
	float m_fInitialSpeed;	// 初速
	float m_fGravity;		// 重力
	float m_fBouncy;		// 跳力
	float m_fHomingTime;	// ホーミング時間
	bool m_bLanding;		// 着地フラグ
	MyLib::Vector3 m_posPassStart;	// パス開始位置
	MyLib::Vector3 m_posPassEnd;	// パス終了位置

	CGameManager::ETeamSide m_typeTeam;	// チームサイド
	ESpecial m_typeSpecial;	// スペシャル種類
	EAttack m_typeAtk;		// 攻撃種類
	EState m_state;			// 状態
	float m_fStateTime;		// 状態カウンター
	int m_nDamage;			// ダメージ
	int m_nCoverHeal;		// カバー回復
	float m_fKnockback;		// ノックバック
	float m_fBallAngle;		// 回転角度
	CEffekseerObj* m_pThrowLine;	// 投げのライン
	CEffekseerObj* m_pAura;			// オーラ
	CEffekseerObj* m_pSPBallet;		// スペシャルの弾
};

#endif