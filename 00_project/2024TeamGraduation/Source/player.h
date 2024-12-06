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
#include "playerPosAdj.h"
#include "ball.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerMarker;	// マーカー
class CShadow;			// 影
class CPlayerBase;		// ベースプレイヤー
class CPlayerAction;	// アクション
class CPlayerStatus;	// ステータス
class CBall;			// ボール
class CSpecialEffect;	// スペシャル演出エフェクト
class CBindKey;			// 割当キー基底クラス
class CEffekseerObj;	// エフェクシアオブジェクト
class CCatchSpecial;	// キャッチスペシャル
class CDressup;			// ドレスアップ(髪)

//==========================================================================
// クラス定義
//==========================================================================
// プレイヤークラス定義
class CPlayer : public CObjectChara
{
public:

	//=============================
	// 定数
	//=============================
	static constexpr int ID_HAIR = 15;			// 髪のインデックス番号
	static constexpr int ID_ACCESSORY = 16;		// アクセのインデックス番号
	static constexpr int ID_FACE = 2;			// 顔のインデックス番号

	//=============================
	// 列挙型定義
	//=============================
	// モーション列挙
	enum EMotion
	{
		MOTION_DEF = 0,					// ニュートラルモーション
		MOTION_DEF_BALL,				// ニュートラルモーション(ボール所持)
		MOTION_WALK,					// 移動
		MOTION_WALK_BALL,				// 移動(ボール所持)
		MOTION_WALK_SPAWN,				// 移動(入場)
		MOTION_CRAB_FRONT,				// カニ歩き(前)
		MOTION_CRAB_BACK,				// カニ歩き(後)
		MOTION_CRAB_LEFT,				// カニ歩き(左)
		MOTION_CRAB_RIGHT,				// カニ歩き(右)
		MOTION_RUN,						// 走り
		MOTION_RUN_BALL,				// 走り(ボール所持)
		MOTION_BLINK,					// ブリンク
		MOTION_DODGE,					// 回避成功時
		MOTION_JUMP,					// ジャンプ
		MOTION_JUMP_BALL,				// ジャンプ(ボール所持)
		MOTION_LAND,					// 着地
		MOTION_CATCH_STANCE,			// キャッチの構え
		MOTION_CATCH_STANCE_JUMP,		// キャッチの構え(ジャンプ)
		MOTION_CATCH_NORMAL,			// キャッチ(通常)
		MOTION_CATCH_JUMP,				// キャッチ(ジャンプ)
		MOTION_JUSTCATCH_NORMAL,		// ジャストキャッチ(通常)
		MOTION_JUSTCATCH_JUMP,			// ジャストキャッチ(ジャンプ)
		MOTION_CATCHSPECIAL_CAPTURE,	// キャッチスペシャル(捕獲)
		MOTION_CATCHSPECIAL_BRAKE,		// キャッチスペシャル(耐え)
		MOTION_CATCHSPECIAL_SUCC,		// キャッチスペシャル(成功)
		MOTION_CATCHSPECIAL_FAIL,		// キャッチスペシャル(失敗)
		MOTION_DROPCATCH_WALK,			// 落ちてるのキャッチ(歩き)
		MOTION_THROW,					// 投げ
		MOTION_THROW_RUN,				// 投げ(走り)
		MOTION_THROW_JUST,				// 投げ(ジャストキャッチ)
		MOTION_THROW_JUST_JUMP,			// 投げ(ジャストキャッチ)(ジャンプ)
		MOTION_THROW_JUMP,				// 投げ(ジャンプ)
		MOTION_THROW_DROP,				// 投げ(ドロップボール)
		MOTION_THROW_PASS,				// 投げ(パス)
		MOTION_TOSS,					// トス
		MOTION_HYPE,					// 盛り上げ
		MOTION_SPECIAL,					// スペシャル
		MOTION_DAMAGE,					// ダメージ
		MOTION_DAMAGE_LEFT,				// 左ダメージ
		MOTION_DAMAGE_RIGHT,			// 右ダメージ
		MOTION_DAMAGE_BACK,				// 背後ダメージ
		MOTION_DEAD,					// 死亡
		MOTION_DEAD_AFTER,				// 死亡後
		MOTION_GRIP_DEF,				// デフォグリップ
		MOTION_GRIP_FRONT,				// 前グリップ
		MOTION_UNSTABLE,				// おっとっと
		MOTION_BOW,						// 礼
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
		STATE_DEAD_CARRY,		// 死後運搬
		STATE_DODGE,			// 回避
		STATE_CATCH_NORMAL,		// 通常キャッチ
		STATE_CATCH_JUST,		// ジャストキャッチ
		STATE_CATCH_SPECIAL,	// スペシャルキャッチ
		STATE_SPECIAL,			// スペシャル
		STATE_OUTCOURT,			// コート越え(ノックバック)
		STATE_OUTCOURT_RETURN,	// コート越えから戻る(ノックバック)
		STATE_INVADE_TOSS,		// 相手コートに侵入トス
		STATE_INVADE_RETURN,	// 相手コートに侵入から戻る
		STATE_MAX
	};

	// アクション
	enum EAction
	{
		ACTION_NONE = 0,	// 通常
		ACTION_UNSTABLE,	// おっとっと
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

	// カニ歩き方向
	enum CRAB_DIRECTION
	{
		CRAB_NONE = -1,
		CRAB_UP,
		CRAB_DOWN,
		CRAB_LEFT,
		CRAB_RIGHT,
		CRAB_MAX,
	};

	// ベース種類列挙
	enum EBaseType
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

	// 動作状態
	enum EHit
	{
		HIT_NONE = 0,	// 通常
		HIT_CATCH,		// キャッチ
		HIT_DODGE,		// 回避
		HIT_MAX
	};

	// 体系列挙
	enum EBody
	{
		BODY_NORMAL = 0,	// 標準体系
		BODY_DEBU,			// デブ
		BODY_GARI,			// ガリ
		BODY_MAX			// この列挙型の総数
	};

	// ポジション列挙
	enum EFieldArea
	{
		FIELD_NONE = -1,	// 指定なし
		FIELD_IN,			// 内野
		FIELD_OUT,			// 外野
		FIELD_MAX			// この列挙型の総数
	};

	// 人間列挙
	enum EHuman
	{
		HUMAN_NONE = -1,	// 指定なし
		HUMAN_ENTRY,		// エントリー
		HUMAN_SPAWN,		// 登場演出
		HUMAN_REFEREE,		// 体育教師
		HUMAN_RESULT,		// リザルト
		HUMAN_MAX			// この列挙型の総数
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

	// 判定情報
	struct SHitInfo
	{
		bool bHit;	// 当たったか
		EHit eHit;	// 動作状態

		SHitInfo() : bHit(false), eHit(EHit::HIT_NONE) {}
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayer(const CGameManager::ETeamSide typeTeam, const EFieldArea typeArea, const CPlayer::EBaseType typeBase, int nPriority = mylib_const::PRIORITY_DEFAULT);
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
	void SetMotion(int motionIdx, int startKey = 0, bool bBlend = true) const;			// モーションの設定
	void SetEnableMove(bool bPossible)			{ m_bPossibleMove = bPossible; }		// 行動可能フラグ設定
	bool IsPossibleMove()						{ return m_bPossibleMove; }				// 行動可能フラグ取得
	void SetEnableAction(bool bPossible)		{ m_bPossibleAction = bPossible; }		// 移動可能フラグ設定
	bool IsPossibleAction()						{ return m_bPossibleAction; }			// 移動可能フラグ取得
	void SetEnableDash(bool bDash)				{ m_bDash = bDash; }					// ダッシュ状況設定
	bool IsDash()								{ return m_bDash; }						// ダッシュ判定
	void SetEnableJump(bool bJump)				{ m_bJump = bJump; }					// ジャンプ状況設定
	bool IsJump()								{ return m_bJump; }						// ジャンプ判定
	void SetFootLR(bool bFootLR)				{ m_bFootLR = bFootLR; }				// 足左右判定設定
	bool IsFootLR()								{ return m_bFootLR; }					// 足左右判定取得
	void InverseFootLR()						{ m_bFootLR = !m_bFootLR; }				// 足左右判定反転
	void SetMotionFrag(SMotionFrag& frag)		{ m_sMotionFrag = frag; }				// モーションのフラグ設定
	SMotionFrag GetMotionFrag()					{ return m_sMotionFrag; }				// モーションのフラグ取得
	void SetDamageInfo(SDamageInfo& info)		{ m_sDamageInfo = info; }				// ダメージ情報設定
	SDamageInfo GetDamageInfo()					{ return m_sDamageInfo; }				// ダメージ情報取得
	void SetKnockBackInfo(SKnockbackInfo& info)	{ m_sKnockback = info; }				// ノックバック情報設定
	SKnockbackInfo GetKnockBackInfo()			{ return m_sKnockback; }				// ノックバック情報取得
	bool IsAutoMotionSet()						{ return m_bAutoMotionSet; }			// オートモーション情報取得
	void SetAutoMotionSet(bool bAutoMotionSet)	{ m_bAutoMotionSet = bAutoMotionSet; }	// オートモーション情報設定
	bool IsBrake()								{ return m_bBrake; }					// ブレーキ取得
	void SetEnableBrake(bool bBrake)			{ m_bBrake = bBrake; }					// ブレーキ設定

	//=============================
	// パターン
	//=============================
	void ChangePosAdjuster(EBaseType base, CGameManager::ETeamSide team, EFieldArea area);	// プレイヤー位置補正変更
	CPlayerPosAdj* GetPosAdjuster() const	{ return m_pPosAdj; }			// プレイヤー位置補正取得
	CPlayerAction* GetActionPattern() const	{ return m_pActionPattern; }	// アクション取得
	CPlayerStatus* GetStatus() const		{ return m_pStatus; }			// ステータス取得
	CPlayerBase* GetBase() const			{ return m_pBase; }				// ベース取得

	//=============================
	// スペシャル用
	//=============================
	CSpecialEffect* GetSpecialEffect() { return m_pSpecialEffect; }	// スぺシャルエフェクト取得

	//=============================
	// スペシャルキャッチ用
	//=============================
	void SetCatchSpecial(CCatchSpecial* pCatch) { m_pCatchSpecial = pCatch; }	// スぺシャルキャッチ情報設定
	CCatchSpecial* GetCatchSpecial() { return m_pCatchSpecial; }				// スぺシャルキャッチ情報取得

	//=============================
	// ドレスアップ用
	//=============================
	void BindDressUp(int nHair, int nAccessory, int nFace);	// ドレスアップ割り当て

	//=============================
	// その他
	//=============================
	SHitInfo Hit(CBall* pBall);													// ヒット処理
	void SetSpecialAttack();													// スペシャル攻撃設定
	void SetState(EState state);												// 状態設定
	void ChangeBase(EBaseType type);											// ベース変更
	EBaseType GetBaseType() const;												// ベース取得
	EFieldArea GetAreaType() const { return m_typeArea; }						// ポジション取得
	CGameManager::ETeamSide GetTeam() const { return m_typeTeam; }				// チームサイド取得
	EState GetState() { return m_state; }										// 状態取得
	EBody GetBodyType() { return m_BodyType; }									// 体型取得
	EHandedness GetHandedness() { return m_Handedness; }						// 利き手取得
	void SetMyPlayerIdx(int idx) { m_nMyPlayerIdx = idx; }						// 自分のインデックス設定
	int GetMyPlayerIdx() const { return m_nMyPlayerIdx; }						// 自分のインデックス取得
	int GetPositionIdx() const { return m_nPosIdx; }							// 自分のポジション別インデックス取得
	void SetBall(CBall* pBall) { m_pBall = pBall; }								// ボール情報設定
	CBall* GetBall() const { return m_pBall; }									// ボール情報取得
	void DeadSetting(MyLib::HitResult_Character* result, CBall* pBall);			// 死亡設定
	void DamageSetting(CBall* pBall);											// ダメージ発生時設定
	void CatchSetting(CBall* pBall);											// キャッチ時処理
	void CoverCatchSetting(CBall* pBall);										// カバーキャッチ時処理
	void OutCourtSetting();														// コート越え処理
	void SetHaveTime(float time) { m_fHaveTime = time; }						// ボール所持タイマー
	float GetHaveTime() { return m_fHaveTime; }									// ボール所持タイマー
	void SetEscapeTime(float time) { m_fEscapeTime = time; }					// 端逃げタイマー
	float GetEscapeTime() { return m_fEscapeTime; }								// 端逃げタイマー
	MyLib::Vector3 CalcFuturePosition(const int nFutureFrame);					// 未来位置計算
	MyLib::Vector3 GetLookOffset() const;										// 未来位置オフセット取得

	//=============================
	// 静的メンバ関数
	//=============================
	static CListManager<CPlayer> GetList() { return m_List; }	// リスト取得

	//=============================
	// 定数
	//=============================
	float GetDodgeDistance();	// 回避範囲取得

	//=============================
	// 静的関数
	//=============================
	/*
		@brief	ゲームプレイヤーの生成処理
		@param	rPos	 [in]	初期位置 (内野のみ)
		@param	typeTeam [in]	左/右
		@param	typeArea [in]	内野/外野
		@param	typeBase [in]	User/AI
		@param	typeBody [in]	標準/デブ/ガリ
		@param	typeHand [in]	右利き/左利き
	*/
	static CPlayer* Create
	(
		const MyLib::Vector3&	rPos,					// 位置
		CGameManager::ETeamSide	typeTeam,				// チームサイド
		EFieldArea	typeArea = EFieldArea::FIELD_IN,	// ポジション
		EBaseType	typeBase = EBaseType::TYPE_USER,	// ベースタイプ
		EBody		typeBody = EBody::BODY_NORMAL,		// 体型
		EHandedness	typeHand = EHandedness::HAND_R		// 利き手
	);

	/*
		@brief	仮想プレイヤーの生成処理
		@param	rPos	  [in]	初期位置
		@param	typeTeam  [in]	左/右
		@param	typeHuman [in]	人種類
		@param	typeBody  [in]	標準/デブ/ガリ
		@param	typeHand  [in]	右利き/左利き
	*/
	static CPlayer* Create
	(
		const MyLib::Vector3& rPos,			// 位置
		CGameManager::ETeamSide typeTeam,	// チームサイド
		EHuman typeHuman,					// 人
		EBody typeBody = EBody::BODY_NORMAL,		// 体型
		EHandedness typeHand = EHandedness::HAND_R	// 利き手
	);

protected:

	//=============================
	// メンバ関数
	//=============================
	HRESULT CreateShadow();	// 影生成
	void Debug();			// デバッグ処理
	void UpdateFootLR();	// 足左右の更新
	void CreateDressUp();	// ドレスアップ生成

private:
	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayer::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];							// 状態関数

	typedef void(CPlayer::*ACTION_FUNC)();
	static ACTION_FUNC m_ActionFunc[];							// 行動関数

	//=============================
	// メンバ関数
	//=============================
	//-----------------------------
	// 状態関数
	//-----------------------------
	virtual void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 状態更新
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);				// なし
	void StateInvincible(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 無敵
	void StateDamage(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ダメージ
	void StateDead(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);				// 死亡
	void StateDeadAfter(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 死亡後
	void StateDeadCarry(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// 死亡運搬
	void StateDodge(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);				// 回避
	void StateCatch_Normal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 通常キャッチ
	void StateCatch_Just(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ジャストキャッチ
	void StateCatch_Special(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// スペシャルキャッチ
	void StateSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// スペシャル
	void StateOutCourt(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// コート越え
	void StateOutCourt_Return(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// コート越えから戻る
	void StateInvade_Toss(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 相手コートに侵入トス
	void StateInvade_Return(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 相手コート侵入から戻る

	//-----------------------------
	// スペシャルキャッチ状態関数
	//-----------------------------
	void CheckSuccessStateCatchSpecial(const bool& bJust, const CBall::ESpecial& typeSpecial);	// スペシャルキャッチ成功判定

	//-----------------------------
	// その他関数
	//-----------------------------
	void UpdateDamageReciveTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ダメージ受付時間更新
	void Controll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 操作
	void InitBase(EBaseType type);	// ベース初期化
	void ResetFrag();	// フラグリセット
	int GetNumUser();	// ユーザーベースのプレイヤー総数取得

	//-----------------------------
	// モーション系関数
	//-----------------------------
	void MotionSet(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// モーションの設定
	void DefaultMotionSet(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// デフォルトモーションの設定
	void UpdateByMotion(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// モーション別更新処理
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// 攻撃時処理
	virtual void AttackInDicision(CMotion::AttackInfo ATKInfo, int nCntATK) override;	// 攻撃判定中処理

	void CatchSettingSpecial(const bool& bJust, const CBall::ESpecial& typeSpecial);					// キャッチ時処理(スペシャル)

	void CatchSettingLandNormal(CBall::EAttack atkBall);	// キャッチ時処理(地上・通常)
	void CatchSettingLandJust(CBall::EAttack atkBall);		// キャッチ時処理(地上・ジャスト)

	// TAKADA : 空中キャッチモーション出来たら実装
	//void CatchSettingFlyNormal(CBall::EAttack atkBall);	// キャッチ時処理(空中・通常)
	//void CatchSettingFlyJust(CBall::EAttack atkBall);		// キャッチ時処理(空中・ジャスト)

	void SetMoveMotion(bool bNowDrop);	// 移動モーション設定
	
	//-----------------------------
	// ドレスアップ
	//-----------------------------
	void DeleteDressUp();	// ドレスアップ削除

	//=============================
	// メンバ変数
	//=============================
	//-----------------------------
	// 状態
	//-----------------------------
	EState m_Oldstate;							// 前回の状態
	EState m_state;								// 状態
	float m_fStateTime;							// 状態時間

	//-----------------------------
	// オブジェクトのパラメータ
	//-----------------------------
	MyLib::Color m_mMatcol;			// マテリアルの色
	SKnockbackInfo m_sKnockback;	// ノックバックの位置
	
	//-----------------------------
	// 行動フラグ
	//-----------------------------
	bool m_bPossibleMove;		// 行動可能フラグ
	bool m_bPossibleAction;		// アクション可能フラグ
	bool m_bAutoMotionSet;		// オートモーション設定
	bool m_bJump;				// ジャンプ中かどうか
	bool m_bDash;				// ダッシュ判定
	bool m_bFootLR;				// 足左右判定 (t: 右 / f: 左)
	bool m_bAlign;				// 揃え
	bool m_bBrake;				// ブレーキ
	SMotionFrag m_sMotionFrag;	// モーションのフラグ

	//-----------------------------
	// パターン用インスタンス
	//-----------------------------
	CPlayerAction* m_pActionPattern;	// アクションパターン
	CPlayerPosAdj* m_pPosAdj;			// プレイヤー位置補正
	CPlayerStatus* m_pStatus;			// ステータス
	CPlayerBase*   m_pBase;				// ベース

	//-----------------------------
	// スペシャル用
	//-----------------------------
	CSpecialEffect* m_pSpecialEffect;	// スぺシャルエフェクト
	
	//-----------------------------
	// キャッチスペシャル用
	//-----------------------------
	CCatchSpecial* m_pCatchSpecial;				// キャッチスペシャル

	//-----------------------------
	// エフェクト用
	//-----------------------------
	CEffekseerObj* m_pEfkCatchStance;	// キャッチの構え
	CEffekseerObj* m_pEfkCatchNormal;	// 通常キャッチ
	CEffekseerObj* m_pEfkCatchJust;		// ジャストキャッチ

	//-----------------------------
	// ドレスアップ
	//-----------------------------
	CDressup* m_pDressUp_Hair;		// ドレスアップ(髪)
	CDressup* m_pDressUp_Accessory;	// ドレスアップ(アクセ)
	CDressup* m_pDressUp_Face;		// ドレスアップ(顔)

	//-----------------------------
	// その他変数
	//-----------------------------
	float m_fHaveTime;			// ボール所持タイマー
	float m_fEscapeTime;		// 端逃げタイマー
	int m_nMyPlayerIdx;			// プレイヤーインデックス番号
	int m_nPosIdx;				// ポジション別インデックス
	CPlayerMarker* m_pMarker;	// マーカーの情報
	CShadow* m_pShadow;			// 影の情報
	CBall* m_pBall;				// ボールの情報
	SDamageInfo m_sDamageInfo;	// ダメージ情報
	EHandedness m_Handedness;	// 利き手
	EBody m_BodyType;			// 体型
	const EFieldArea m_typeArea;				// ポジション
	const CGameManager::ETeamSide m_typeTeam;	// チームサイド
	static CListManager<CPlayer> m_List;		// リスト

#if  _DEBUG
	static int m_nDebugID;	// デバッグ用ID
	int m_nThisDebugID;		// 自分のID
#endif //  _DEBUG


};


//==========================================================================
// 名前空間
//==========================================================================
namespace Crab	// カニ歩き
{
	const float RANGE = D3DX_PI * 0.25f;	// 判定角度の半分

	const float RANGE_MIN_MAX[] =	// 判定角度列挙
	{
		D3DX_PI * 1.0f + RANGE,		// 下Max
		D3DX_PI * 1.0f - RANGE,		// 下Min
		D3DX_PI * 0.0f + RANGE,		// 上Max
		D3DX_PI * 0.0f - RANGE,		// 上Min
		D3DX_PI * -0.5f + RANGE,	// 左Max
		D3DX_PI * -0.5f - RANGE,	// 左Min
		D3DX_PI * 0.5f + RANGE,		// 右Max
		D3DX_PI * 0.5f - RANGE,		// 右Min
	};

	// カニ歩きモーション設定列挙
	const CPlayer::EMotion MOTION_WALK[CPlayer::CRAB_DIRECTION::CRAB_MAX][CPlayer::CRAB_DIRECTION::CRAB_MAX] =
	{
		// [ プレイヤー向き ][ インプット向き ]

		{// UP(プレイヤーが上向き)
			CPlayer::EMotion::MOTION_CRAB_FRONT,	// UP 
			CPlayer::EMotion::MOTION_CRAB_BACK,		// DOWN
			CPlayer::EMotion::MOTION_CRAB_LEFT,		// LEFT
			CPlayer::EMotion::MOTION_CRAB_RIGHT		// RIGHT
		},
		{// DOWN(プレイヤーが下向き)
			CPlayer::EMotion::MOTION_CRAB_BACK,		// UP 
			CPlayer::EMotion::MOTION_CRAB_FRONT,	// DOWN
			CPlayer::EMotion::MOTION_CRAB_RIGHT,	// LEFT
			CPlayer::EMotion::MOTION_CRAB_LEFT 		// RIGHT
		},
		{// LEFT(プレイヤーが左向き)
			CPlayer::EMotion::MOTION_CRAB_RIGHT,	// UP 
			CPlayer::EMotion::MOTION_CRAB_LEFT,		// DOWN
			CPlayer::EMotion::MOTION_CRAB_FRONT,	// LEFT
			CPlayer::EMotion::MOTION_CRAB_BACK 		// RIGHT
		},
		{// RIGHT(プレイヤーが右向き)
			CPlayer::EMotion::MOTION_CRAB_LEFT,		// UP 
			CPlayer::EMotion::MOTION_CRAB_RIGHT,	// DOWN
			CPlayer::EMotion::MOTION_CRAB_BACK,		// LEFT
			CPlayer::EMotion::MOTION_CRAB_FRONT 	// RIGHT
		},
	};
}

#endif
