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
#include "ball.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CShadow;			// 影
class CPlayerControlAction;	// 操作(アクション)
class CPlayerControlMove;	// 操作(移動)
class CPlayerAction;	// アクション
class CPlayerStatus;	// ステータス
class CBall;			// ボール
class CDressup;			// 着せ替え

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
	enum EMotion
	{
		MOTION_DEF = 0,				// ニュートラルモーション
		MOTION_WALK,				// 移動
		MOTION_CRAB_FRONT,			// カニ歩き(前)
		MOTION_CRAB_BACK,			// カニ歩き(後)
		MOTION_CRAB_LEFT,			// カニ歩き(左)
		MOTION_CRAB_RIGHT,			// カニ歩き(右)
		MOTION_RUN,					// 走り
		MOTION_BLINK,				// ブリンク
		MOTION_DODGE,				// 回避成功時
		MOTION_JUMP,				// ジャンプ
		MOTION_JUMP_BALL,			// ジャンプ(ボール所持)
		MOTION_LAND,				// 着地
		MOTION_CATCH_STANCE,		// キャッチの構え
		MOTION_CATCH_STANCE_JUMP,	// キャッチの構え(ジャンプ)
		MOTION_CATCH_NORMAL,		// キャッチ(通常)
		MOTION_CATCH_JUMP,			// キャッチ(ジャンプ)
		MOTION_JUSTCATCH_NORMAL,	// ジャストキャッチ(通常)
		MOTION_JUSTCATCH_JUMP,		// ジャストキャッチ(ジャンプ)
		MOTION_THROW,				// 投げ
		MOTION_THROW_RUN,			// 投げ(走り)
		MOTION_THROW_JUMP,			// 投げ(ジャンプ)
		MOTION_HYPE,				// 盛り上げ
		MOTION_SPECIAL,				// スペシャル
		MOTION_WIN,					// 勝利
		MOTION_DAMAGE,				// ダメージ
		MOTION_DEAD,				// 死亡
		MOTION_DEAD_AFTER,			// 死亡後
		MOTION_GRIP_DEF,			// デフォグリップ
		MOTION_GRIP_FRONT,			// 前グリップ
		MOTION_MAX
	};

	// 状態定義
	enum EState
	{
		STATE_NONE = 0,			// なにもない
		STATE_INVINCIBLE,		// 無敵
		STATE_DMG,				// ダメージ
		STATE_DEAD,				// 死
		STATE_DEAD_AFTER,		// 死後
		STATE_DODGE,			// 回避
		STATE_CATCH_NORMAL,		// 通常キャッチ
		STATE_CATCH_JUST,		// ジャストキャッチ
		STATE_SPECIAL,			// スペシャル
		STATE_OUTCOURT,			// コート越え
		STATE_OUTCOURT_RETURN,	// コート越えから戻る
		STATE_MAX
	};

	// アクション
	enum EAction
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
	enum EDashAngle
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

	// ユーザーの種類列挙
	enum EUserType
	{
		TYPE_USER = 0,
		TYPE_AI,
		TYPE_MAX
	};

	// 利き手列挙
	enum EHandedness
	{
		HAND_R,	// 右利き
		HAND_L,	// 左利き
		HAND_MAX
	};

	//=============================
	// 構造体
	//=============================
	// ダメージ情報
	struct SDamageInfo
	{
		bool bActiveSuperArmor;			// スーパーアーマー
		bool bReceived;					// ダメージ受け付け判定
		float fReceiveTime;				// ダメージ受付時間
		CBall::EAttack eReiceiveType;	// 受けた種類

		SDamageInfo() : bActiveSuperArmor(false), bReceived(false), fReceiveTime(0.0f), eReiceiveType(CBall::EAttack::ATK_NONE) {}
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
		bool bCatch;		// キャッチ
		bool bCatchJust;	// ジャストキャッチ

		// コンストラクタ
		SMotionFrag() : bJump(false), bATK(false), bKnockBack(false), bDead(false), bMove(false), bBlink(false), bCatch(false), bCatchJust(false) {}
	};

	// ダッシュ情報
	struct SDashInfo
	{
		bool bDash;			// ダッシュ判定
		EDashAngle eAngle;	// ダッシュ方向

		SDashInfo() : bDash(false), eAngle(ANGLE_LEFT) {}
	};

	// ノックバック情報
	struct SKnockbackInfo
	{
		MyLib::Vector3 posStart;	// 始点
		MyLib::Vector3 posEnd;		// 終点

		SKnockbackInfo() : posStart(MyLib::Vector3()), posEnd(MyLib::Vector3()) {}
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
	virtual void Kill() override;	// 動的削除処理

	//=============================
	// モーション
	//=============================
	void SetMotion(int motionIdx, int startKey = 0, bool bBlend = true) const;				// モーションの設定
	void SetEnableMove(bool bPossible) { m_bPossibleMove = bPossible; }	// 移動可能フラグ設定
	bool IsPossibleMove()			{ return m_bPossibleMove; }			// 移動可能フラグ取得
	void SetEnableDash(bool bDash)	{ m_bDash = bDash; }				// ダッシュ状況設定
	bool IsDash()					{ return m_bDash; }					// ダッシュ判定
	void SetEnableJump(bool bJump)	{ m_bJump = bJump; }				// ジャンプ状況設定
	bool IsJump()					{ return m_bJump; }					// ジャンプ判定
	void SetFootLR(bool bFootLR) { m_bFootLR = bFootLR; }				// 足左右判定設定
	bool IsFootLR() { return m_bFootLR; }								// 足左右判定取得
	void InverseFootLR() { m_bFootLR = !m_bFootLR; }					// 足左右判定反転
	void SetMotionFrag(SMotionFrag frag)	{ m_sMotionFrag = frag; }	// モーションのフラグ設定
	SMotionFrag GetMotionFrag()				{ return m_sMotionFrag; }	// モーションのフラグ取得
	void SetDamageInfo(SDamageInfo info)	{ m_sDamageInfo = info; }	// ダメージ情報設定
	SDamageInfo GetDamageInfo()				{ return m_sDamageInfo; }	// ダメージ情報取得

	//=============================
	// パターン
	//=============================
	CPlayerAction* GetActionPattern()	{ return m_pActionPattern; }	// アクション取得
	CPlayerStatus* GetStatus() const	{ return m_pStatus; }			// ステータス取得

	//=============================
	// 着せ替え
	//=============================
	void UpdateDressUP(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ドレスアップの更新

	//=============================
	// その他
	//=============================
	bool Hit(CBall* pBall);			// ヒット処理
	void SetSpecialAttack();		// スペシャル攻撃設定
	void SetState(EState state);	// 状態設定
	EState GetState() { return m_state; }					// 状態取得
	void SetMyPlayerIdx(int idx) { m_nMyPlayerIdx = idx; }	// 自分のインデックス設定
	int GetMyPlayerIdx() { return m_nMyPlayerIdx; }			// 自分のインデックス取得
	void SetBall(CBall* pBall) { m_pBall = pBall; }			// ボール情報設定
	CBall* GetBall() const { return m_pBall; }				// ボール情報取得
	bool IsCrab();											// カニ歩き判定
	void DeadSetting(MyLib::HitResult_Character* result, CBall* pBall);	// 死亡設定
	void DamageSetting(CBall* pBall);									// ダメージ発生時設定
	void CatchSetting(CBall* pBall);									// キャッチ時処理
	void OutCourtSetting();												// コート越え処理
	static CListManager<CPlayer> GetList() { return m_List; }			// リスト取得

	//=============================
	// 定数
	//=============================
	float GetDodgeDistance();	// 回避範囲取得

	//=============================
	// 静的関数
	//=============================
	/*
		@brief	生成処理
		@param	type	[in]	ユーザーの種類
		@param	team	[in]	チームサイド
		@param	rPos	[in]	初期位置
	*/
	static CPlayer* Create(EUserType type, const CGameManager::TeamSide team, const MyLib::Vector3& rPos, EHandedness handtype = EHandedness::HAND_R);

protected:
	//=============================
	// メンバ関数
	//=============================
	virtual void Operate(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// 操作
	virtual void DeleteControl();	// 操作削除
	virtual void Debug();			// デバッグ処理
	void UpdateFootLR();			// 足左右の更新
	CPlayerControlMove* GetPlayerControlMove();		// 操作取得(移動)
	CPlayerControlAction* GetPlayerControlAction();	// 操作取得(アクション)
	void SetPlayerControlMove(CPlayerControlMove* pControlMove) { m_pControlMove = pControlMove; }				// 操作設定(移動)
	void SetPlayerControlAction(CPlayerControlAction* pControlAction) { m_pControlAction = pControlAction; }	// 操作設定(アクション)

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
	void StateNone();				// なし
	void StateInvincible();			// 無敵
	void StateDamage();				// ダメージ
	void StateDead();				// 死亡
	void StateDeadAfter();			// 死亡後
	void StateDodge();				// 回避
	void StateCatch_Normal();		// 通常キャッチ
	void StateCatch_Just();			// ジャストキャッチ
	void StateSpecial();			// スペシャル
	void StateOutCourt();			// コート越え
	void StateOutCourt_Return();	// コート越えから戻る

	//-----------------------------
	// その他関数
	//-----------------------------
	void UpdateDamageReciveTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ダメージ受付時間更新
	void Controll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 操作
	void LimitPos();	// 位置制限
	void ResetFrag();	// フラグリセット

	//-----------------------------
	// モーション系関数
	//-----------------------------
	void MotionSet(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// モーションの設定
	void DefaultMotionSet(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// デフォルトモーションの設定
	void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// 攻撃時処理
	void AttackInDicision(CMotion::AttackInfo ATKInfo, int nCntATK) override;	// 攻撃判定中処理

	void CatchSettingLandNormal(CBall::EAttack atkBall);	// キャッチ時処理(地上・通常)
	void CatchSettingLandJust(CBall::EAttack atkBall);		// キャッチ時処理(地上・ジャスト)
	//TOKODO: 空中キャッチモーション出来たら実装
	//void CatchSettingFlyNormal(CBall::EAttack atkBall);		// キャッチ時処理(空中・通常)
	//void CatchSettingFlyJust(CBall::EAttack atkBall);		// キャッチ時処理(空中・ジャスト)
	void MotionCrab(int nStartKey);		// キャッチ時処理(地上・ジャスト)

	//=============================
	// メンバ変数
	//=============================
	//-----------------------------
	// 状態
	//-----------------------------
	EState m_Oldstate;	// 前回の状態
	EState m_state;		// 状態
	float m_fStateTime;	// 状態時間

	//-----------------------------
	// オブジェクトのパラメータ
	//-----------------------------
	MyLib::Color m_mMatcol;			// マテリアルの色
	SKnockbackInfo m_sKnockback;		// ノックバックの位置
	
	//-----------------------------
	// 行動フラグ
	//-----------------------------
	bool m_bPossibleMove;		// 移動可能フラグ
	bool m_bJump;				// ジャンプ中かどうか
	bool m_bDash;				// ダッシュ判定
	bool m_bFootLR;				// 足左右判定 (t: 右 / f: 左)
	bool m_bAlign;				// 揃え
	SMotionFrag m_sMotionFrag;	// モーションのフラグ

	//-----------------------------
	// パターン用インスタンス
	//-----------------------------
	CPlayerAction* m_pActionPattern;	// アクションパターン
	CPlayerStatus* m_pStatus;			// ステータス
	CPlayerControlMove* m_pControlMove;		// 移動操作
	CPlayerControlAction* m_pControlAction;	// アクション操作

	//-----------------------------
	// 着せ替え
	//-----------------------------
	CDressup* m_pDressup_Hair;		// 髪着せ替え
	CDressup* m_pDressup_Accessory;	// アクセ着せ替え

	//-----------------------------
	// その他変数
	//-----------------------------
	int m_nMyPlayerIdx;	// プレイヤーインデックス番号
	CShadow* m_pShadow;	// 影の情報
	CBall* m_pBall;		// ボールの情報
	SDamageInfo m_sDamageInfo;	// ダメージ情報
	EHandedness m_Handress;		// 利き手
	static CListManager<CPlayer> m_List;	// リスト
};

#endif
