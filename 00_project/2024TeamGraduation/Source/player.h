//=============================================================================
// 
//  プレイヤーヘッダー [player.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_	// 二重インクルード防止

#include "gamemanager.h"
#include "objectChara.h"
#include "listmanager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CShadow;
class CHP_GaugePlayer;
class CEnemy;
class CBaggage;
class CRetry_Ui;

class CPlayerControlMove;		// 移動
class CPlayerControlBaggage;	// 荷物操作
class CPlayerControlSurfacing;	// 浮上操作
class CPlayerControlTrick;		// トリック操作

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
		MOTION_DEAD,		// 死亡
		MOTION_START,		// スタート演出
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
		STATE_RETURN,		// チェックポイントに戻る
		STATE_RESTART,		// スタートに戻る
		STATE_RESPAWN,		// 復活
		STATE_MAX
	};

	// 向き
	enum ANGLE
	{
		RIGHT = 0,	// 右
		LEFT,		// 左
		NONE,		// 通常
		MAX
	};

	//=============================
	// 構造体
	//=============================
	// プレイヤーステータス
	struct sPlayerStatus
	{
		float respawnHeal;				// リスポーン時の回復量
		float guardStaminaSubValue;		// ガード時のスタミナ減少量
		float counterStaminaSubValue;	// カウンター時のスタミナ減少量
		float counterStaminaHealValue;	// カウンター時のスタミナ回復量
		float attackMultiply;			// 攻撃の倍率
		float chargeTime;				// チャージ時間
		float counterExtensionFrame;	// カウンター猶予フレーム
		float damageMitigation;			// ダメージ軽減率
		float addDownTime;				// ダウン時間付与
		bool bChargeFlinch;				// 怯みフラグ

		sPlayerStatus() : respawnHeal(0.0f), guardStaminaSubValue(0.0f), counterStaminaSubValue(0.0f),
			counterStaminaHealValue(0.0f), attackMultiply(0.0f), chargeTime(0.0f),
			counterExtensionFrame(0.0f), damageMitigation(0.0f), addDownTime(0.0f), bChargeFlinch(false) {}

		sPlayerStatus(
			float resheal, float guardvalue, float countervalue, 
			float counterheal, float attackmultiply, float chargetime, 
			float countertime, float damagemitigation, float downtime, bool bFlinsh) : 
			respawnHeal(resheal), guardStaminaSubValue(guardvalue), counterStaminaSubValue(countervalue),
			counterStaminaHealValue(counterheal), attackMultiply(attackmultiply), chargeTime(chargetime),
			counterExtensionFrame(countertime), damageMitigation(damagemitigation), addDownTime(downtime), bChargeFlinch(bFlinsh) {}
	};

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
		bool bGuard;		// ガード
		bool bCounter;		// カウンター中
		bool bKnockBack;	// ノックバック中
		bool bDead;			// 死亡中
		bool bMove;			// 移動中
	};

	CPlayer(int nPriority = 2);
	~CPlayer();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	MyLib::HitResult_Character Hit(const int nValue);	// ヒット処理

	STATE GetState();		// 状態取得
	void SetState(STATE state, int nCntState = 0);	// 状態設定
	virtual void Kill();			// 死亡処理
	void SwitchRockOnTarget();		// ロック対象切り替え

	// モーション
	void SetMotion(int motionIdx);									// モーションの設定
	void SetEnableDash(bool bDash)	{ m_bDash = bDash; }			// ダッシュ状況設定
	bool IsDash()					{ return m_bDash; }				// ダッシュ判定
	void SetEnableJump(bool bJump)	{ m_bJump = bJump; }			// ジャンプ状況設定
	bool IsJump()					{ return m_bJump; }				// ジャンプ判定
	float GetDashTime()				{ return m_fDashTime; }			// ダッシュ時間
	void SetMotionFrag(SMotionFrag frag) { m_sMotionFrag = frag; }	// モーションのフラグ設定
	SMotionFrag GetMotionFrag() { return m_sMotionFrag; }			// モーションのフラグ取得
	void SetDamageInfo(sDamageInfo info) { m_sDamageInfo = info; }	// ダメージ情報設定
	sDamageInfo GetDamageInfo() { return m_sDamageInfo; }			// ダメージ情報取得

	//=============================
	// 操作
	//=============================
	void ChangeMoveControl(CPlayerControlMove* control);		// 移動の操作変更
	void ChangeBaggageControl(CPlayerControlBaggage* control);	// 荷物の操作変更
	void ChangeSurfacingControl(CPlayerControlSurfacing* control);	// 浮上の操作変更
	void ChangeTrickControl(CPlayerControlTrick* control);	// 浮上の操作変更

	//=============================
	// その他
	//=============================
	void SetMyPlayerIdx(int idx) { m_nMyPlayerIdx = idx; }	// 自分のインデックス設定
	int GetMyPlayerIdx() { return m_nMyPlayerIdx; }			// 自分のインデックス取得
	MyLib::Vector3 GetCylinderPosition() { return m_posCylinder; };		// 筒の位置取得
	void SetMoveLength(float len) { m_fMoveLength = len; }				// 移動距離設定
	float GetMoveLength() { return m_fMoveLength; }						// 移動距離取得
	void SetMoveAngle(const ANGLE& angle) { m_moveAngle = angle; }		// 移動の向き設定
	ANGLE GetMoveAngle() { return m_moveAngle; }						// 移動の向き取得

	CBaggage* GetBaggage() { return m_pBaggage; }			// 自分の荷物取得
	CRetry_Ui* GetRetryUI() { return m_pRetryUI; }			// リトライUI取得
	void CreateRetryUI();										// リトライUI設定

	// 設定系
	void DeadSetting(MyLib::HitResult_Character* result);
	void GoalSetting();	// ゴール時の設定

	static CPlayer* Create(int nIdx);	// 生成
	static CListManager<CPlayer> GetListObj() { return m_List; }	// リスト取得

protected:

	bool Collision(MyLib::Vector3 &pos, MyLib::Vector3 &move);	// 当たり判定
	void MotionSet();	// モーションの設定

	bool m_bJump;				// ジャンプ中かどうか
	bool m_bLandOld;			// 過去の着地情報
	bool m_bHitStage;			// ステージの当たり判定
	bool m_bLandField;			// フィールドの着地判定
	bool m_bHitWall;			// 壁の当たり判定
	int m_nMyPlayerIdx;			// プレイヤーインデックス番号
	int m_nCntWalk;				// 歩行カウンター
	STATE m_state;				// 状態
	CShadow *m_pShadow;			// 影の情報
	SMotionFrag m_sMotionFrag;	// モーションのフラグ
private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayer::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	void UpdateState();		// 状態更新
	void StateNone();		// なし
	void StateInvincible();	// 無敵
	void StateDamage();		// ダメージ
	void StateDead();		// 死亡
	void StateDeadWait();	// 死亡待機
	void StateReturn();		// フェードアウト
	void StateRestart();	// リスタート
	void StateRespawn();	// リスポーン

	// その他関数
	virtual void Controll();		// 操作
	virtual void DeleteControl();	// 操作削除
	void LimitPos();				// 位置制限
	void ReaspawnCheckPoint();		// チェックポイント復活
	void RespawnStart();			// スタート復活
	void CollisionMapObject();		// マップオブジェクトとの当たり判定
	void MotionBySetState();		// モーション別の状態設定
	void ResetFrag();				// フラグリセット
	void UpdateDamageReciveTimer();	// ダメージ受付時間更新
	void ReaspawnSetting();			// リスポーン設定
	void Bobbing();					// ぷかぷか
	void ScreenReset();				// 画面状態リセット
	void MovingWaterLine();			// 移動時の水搔き分け線

	// モーション系関数
	void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// 攻撃時処理
	void AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK) override;	// 攻撃判定中処理

	//=============================
	// メンバ変数
	//=============================
	STATE m_Oldstate;				// 前回の状態
	D3DXCOLOR m_mMatcol;			// マテリアルの色
	MyLib::Vector3 m_posKnokBack;	// ノックバックの位置
	MyLib::Vector3 m_KnokBackMove;	// ノックバックの移動量
	MyLib::Vector3 m_posCylinder;	// 筒の位置
	int m_nCntState;				// 状態遷移カウンター
	ANGLE m_moveAngle;				// 移動の向き
	bool m_bDash;					// ダッシュ判定
	float m_fDashTime;				// ダッシュ時間
	float m_fMoveLength;			// 移動距離
	int m_nCntWaterLine;			// 搔き分け線のカウンター
	int m_bMotionAutoSet;			// モーションの自動設定
	Effekseer::Handle m_WeaponHandle;

	sPlayerStatus m_PlayerStatus;	// プレイヤーステータス
	sDamageInfo m_sDamageInfo;		// ダメージ情報
	CHP_GaugePlayer* m_pHPGauge;	// HPゲージのポインタ
	CBaggage* m_pBaggage;			// 荷物のポインタ
	CRetry_Ui* m_pRetryUI;			// リトライUIのポインタ
	
	// パターン用インスタンス
	CPlayerControlMove* m_pControlMove;			// 移動操作
	CPlayerControlBaggage* m_pControlBaggage;	// 荷物操作
	CPlayerControlSurfacing* m_pControlSurfacing;	// 浮上操作
	CPlayerControlTrick* m_pControlTrick;		// トリック操作

	static CListManager<CPlayer> m_List;	// リスト
};


#endif