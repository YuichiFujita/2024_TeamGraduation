//=============================================================================
// 
//  プレイヤーヘッダー [player.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "gamemanager.h"
#include "objectChara.h"
#include "listmanager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CShadow;				// 影
class CPlayerControlMove;	// 移動(操作)
class CPlayerControlAction;	// アクション(操作)
class CPlayerAction;		// アクション
class CPlayerStatus;		// ステータス
class CBall;				// ボール

//==========================================================================
// クラス定義
//==========================================================================
// プレイヤークラス定義
class CPlayer : public CObjectChara
{
public:
	//=============================
	// 列挙型定義
	//=============================
	// モーション列挙
	enum MOTION
	{
		MOTION_DEF = 0,		// ニュートラルモーション
		MOTION_WALK,		// 移動
		MOTION_RUN,			// 走り
		MOTION_BLINK,		// ブリンク
		MOTION_DODGE,		// 回避成功時
		MOTION_JUMP,		// ジャンプ
		MOTION_LAND,		// 着地
		MOTION_CATCH,		// キャッチ
		MOTION_THROW,		// 投げ
		MOTION_THROW_JUMP,	// 投げ(ジャンプ)
		MOTION_SPECIAL,		// スペシャル
		MOTION_WIN,			// 勝利
		MOTION_DEAD,		// 死亡
		MOTION_MAX
	};

	// 状態定義
	enum STATE
	{
		STATE_NONE = 0,		// なにもない
		STATE_INVINCIBLE,	// 無敵
		STATE_DMG,			// ダメージ
		STATE_DEAD,			// 死
		STATE_DEADWAIT,		// 死亡待機
		STATE_DODGE,		// 回避
		STATE_MAX
	};

	// アクション
	enum Action
	{
		ACTION_NONE = 0,	// 通常
		ACTION_BLINK,		// ブリンク
		ACTION_DODGE,		// 回避
		ACTION_RUN,			// 走り
		ACTION_JUMP,		// ジャンプ
		ACTION_CATCH,		// キャッチ
		ACTION_THROW,		// 投げ
		ACTION_THROW_JUMP,	// 投げ(ジャンプ)
		ACTION_SPECIAL,		// スペシャル
		ACTION_MAX
	};

	// ダッシュ方向
	enum DashAngle
	{
		ANGLE_UP = 0,
		ANGLE_RIGHTUP,
		ANGLE_RIGHT,
		ANGLE_RIGHTDW,
		ANGLE_DOWN,
		ANGLE_LEFTDW,
		ANGLE_LEFT,
		ANGLE_LEFTUP,
		ANGLE_MAX
	};

	//=============================
	// 構造体
	//=============================
	// ダメージ情報
	struct sDamageInfo
	{
		bool bActiveSuperArmor;	// スーパーアーマー
		bool bReceived;			// ダメージ受け付け判定
		float reciveTime;		// ダメージ受付時間

		sDamageInfo() : bActiveSuperArmor(false), bReceived(false), reciveTime(0.0f) {}
	};

	// モーションの判定
	struct SMotionFrag
	{
		bool bJump;			// ジャンプ中
		bool bATK;			// 攻撃中
		bool bKnockBack;	// ノックバック中
		bool bDead;			// 死亡中
		bool bMove;			// 移動中
		bool bBlink;		// ブリンク

		// コンストラクタ
		SMotionFrag() : bJump(false), bATK(false), bKnockBack(false), bDead(false), bMove(false), bBlink(false) {}
	};

	// ダッシュ情報
	struct SDashInfo
	{
		bool bDash;			// ダッシュ判定
		DashAngle angle;	// ダッシュ方向

		SDashInfo() : bDash(false), angle(ANGLE_LEFT) {}
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayer(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CPlayer();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Release() override;	// 死亡処理

	//=============================
	// モーション
	//=============================
	void SetMotion(int motionIdx);										// モーションの設定
	void SetEnableMove(bool bPossible) { m_bPossibleMove = bPossible; }	// 移動可能フラグ設定
	bool IsPossibleMove()			{ return m_bPossibleMove; }			// 移動可能フラグ取得
	void SetEnableDash(bool bDash)	{ m_bDash = bDash; }				// ダッシュ状況設定
	bool IsDash()					{ return m_bDash; }					// ダッシュ判定
	void SetEnableJump(bool bJump)	{ m_bJump = bJump; }				// ジャンプ状況設定
	bool IsJump()					{ return m_bJump; }					// ジャンプ判定
	void SetMotionFrag(SMotionFrag frag)	{ m_sMotionFrag = frag; }	// モーションのフラグ設定
	SMotionFrag GetMotionFrag()				{ return m_sMotionFrag; }	// モーションのフラグ取得
	void SetDamageInfo(sDamageInfo info)	{ m_sDamageInfo = info; }	// ダメージ情報設定
	sDamageInfo GetDamageInfo()				{ return m_sDamageInfo; }	// ダメージ情報取得

	//=============================
	// パターン
	//=============================
	CPlayerAction* GetActionPattern() { return m_pActionPattern; }	// アクション取得
	CPlayerStatus* GetStatus() { return m_pStatus; }				// ステータス取得

	//=============================
	// その他
	//=============================
	void Hit(CBall* pBall);		// ヒット処理
	void SetState(STATE state);	// 状態設定
	STATE GetState() { return m_state; }						// 状態取得
	void SetMyPlayerIdx(int idx) { m_nMyPlayerIdx = idx; }		// 自分のインデックス設定
	int GetMyPlayerIdx() { return m_nMyPlayerIdx; }				// 自分のインデックス取得
	void SetBall(CBall* pBall) { m_pBall = pBall; }				// ボール情報設定
	CBall* GetBall() { return m_pBall; }						// ボール情報取得
	void DeadSetting(MyLib::HitResult_Character* result);		// 死亡設定
	static CListManager<CPlayer> GetList() { return m_List; }	// リスト取得

protected:
	//=============================
	// メンバ関数
	//=============================
	virtual void Move(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// 移動
	virtual void DeleteControl();	// 操作削除

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayer::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// 状態関数
	typedef void(CPlayer::*ACTION_FUNC)();
	static ACTION_FUNC m_ActionFunc[];	// 行動関数

	//=============================
	// メンバ関数
	//=============================
	//-----------------------------
	// 状態関数
	//-----------------------------
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 状態更新
	void StateNone();		// なし
	void StateInvincible();	// 無敵
	void StateDamage();		// ダメージ
	void StateDead();		// 死亡
	void StateDeadWait();	// 死亡待機

	//-----------------------------
	// その他関数
	//-----------------------------
	void UpdateDamageReciveTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ダメージ受付時間更新
	void Controll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 操作
	void LimitPos();	// 位置制限
	void ResetFrag();	// フラグリセット
	void MotionSet();	// モーションの設定

	//-----------------------------
	// モーション系関数
	//-----------------------------
	void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// 攻撃時処理
	void AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK) override;	// 攻撃判定中処理

	//=============================
	// メンバ変数
	//=============================
	//-----------------------------
	// 状態
	//-----------------------------
	STATE m_Oldstate;	// 前回の状態
	STATE m_state;		// 状態
	float m_fStateTime;	// 状態時間

	//-----------------------------
	// オブジェクトのパラメータ
	//-----------------------------
	MyLib::Color m_mMatcol;			// マテリアルの色
	MyLib::Vector3 m_posKnokBack;	// ノックバックの位置
	
	//-----------------------------
	// 行動フラグ
	//-----------------------------
	bool m_bPossibleMove;		// 移動可能フラグ
	bool m_bJump;				// ジャンプ中かどうか
	bool m_bDash;				// ダッシュ判定
	SMotionFrag m_sMotionFrag;	// モーションのフラグ

	//-----------------------------
	// パターン用インスタンス
	//-----------------------------
	CPlayerAction* m_pActionPattern;	// アクションパターン
	CPlayerStatus* m_pStatus;			// ステータス

	//-----------------------------
	// その他変数
	//-----------------------------
	int m_nMyPlayerIdx;	// プレイヤーインデックス番号
	CShadow* m_pShadow;	// 影の情報
	CBall* m_pBall;		// ボールの情報
	sDamageInfo m_sDamageInfo;	// ダメージ情報
	static CListManager<CPlayer> m_List;	// リスト
};

#endif
