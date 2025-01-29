//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playercontrol_action.h]
//  Author : Takeru Ogasawara
// 
//=============================================================================

#ifndef _PLAYERAICONTROL_ATTACK_H_
#define _PLAYERAICONTROL_ATTACK_H_	// 二重インクルード防止

#include "player.h"
#include "playerAIcontrol_mode.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayerAIControlMove;		// 移動(AI)
class CPlayerAIControlAction;	// アクション(AI)
class CPlayer;

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
class CPlayerAIControlAttack : public CPlayerAIControlMode
{
public:

	enum EAttackMode			// 攻撃モード
	{
		ATTACKMODE_PREPARATION = 0,		// 準備
		ATTACKMODE_ATTACK,				// 攻撃
		ATTACKMODE_MAX
	};

	enum EAttackPrepatarion		// 準備
	{
		ATTACKPREPATARION_NONE = 0,		// なし
		ATTACKPREPATARION_WAIT,			// 待つ
		ATTACKPREPATARION_GO,			// 行く
		ATTACKPREPATARION_LEAVE,		// 離れる
		ATTACKPREPATARION_MAX
	};

	enum  EThrowType			// 投げタイプ
	{
		THROWTYPE_NONE = 0,		// なし
		THROWTYPE_NORMAL,		// 通常
		THROWTYPE_JUMP,			// ジャンプ
		THROWTYPE_SPECIAL,		// スペシャル
		THROWTYPE_PASS,			// パス
		THROWTYPE_MAX
	};

	enum EAttackNormal
	{
		ATTACK_NONE = 0,
		ATTACK_THROW,
		ATTACK_DASH,
		ATTACK_MAX
	};

	enum EThrowFlag				// 投げフラグ
	{
		THROW_NONE = 0,			// なし
		THROW_NORMAL,			// 投げ
		THROW_PASS,				// パス
		THROW_SPECIAL,			// スペシャル
	};

private:

	struct STiming
	{
		float fTimer;
		float fPosY;
		bool bSet;
	};

public:

	// コンストラクタ
	CPlayerAIControlAttack();

	static CPlayerAIControlAttack* Create(CPlayer* player);

	HRESULT Init() override;
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;

	// 設定・取得
	void SetThrowFlag(EThrowFlag Throw) { m_eThrowFlag = Throw; }			// 投げ設定
	EThrowFlag GetThrowFlag() { return m_eThrowFlag; }						// 取得
	void SetAttackMode(EAttackMode mode) { m_eAttackMode = mode; }			// 攻撃モード設定
	EAttackMode GetAttackMode() { return m_eAttackMode; }					// 取得

protected:
	//=============================
	// 仮想・純粋関数
	//=============================
	virtual void Preparation() = 0;

	//=============================
	// メンバ関数
	//=============================
	bool Leave(MyLib::Vector3 targetPos, float distance);		// 離れる
	bool Approatch(MyLib::Vector3 targetPos, float distance);	// 近づく
	void SeeTarget(MyLib::Vector3 pos);							// ターゲットをみる

	//=============================
	// 設定・取得関数
	//=============================
	CPlayer* GetBallOwner();				// ボール持ち主
	CPlayer* GetThrowTarget();				// 投げるターゲット

private:
	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayerAIControlAttack::* ATTACKMODE_FUNC)();
	static ATTACKMODE_FUNC m_AttackModeFunc[];		// 攻撃モード

	typedef void(CPlayerAIControlAttack::* PREPARATION_FUNC)();
	static PREPARATION_FUNC m_PreparationFunc[];		// 準備

	typedef void(CPlayerAIControlAttack::* THROWTYPE_FUNC)();
	static THROWTYPE_FUNC m_ThrowTypeFunc[];		// 投げるタイプ関数

	typedef void(CPlayerAIControlAttack::* THROWFLAG_FUNC)();
	static THROWFLAG_FUNC m_ThrowFlagFunc[];		// 投げフラグ関数

	//-----------------------------
	// 状態関数
	//-----------------------------
	// 攻撃モード
	void AttackModePreparation();
	void AttackModeAttack();

	// 準備
	void PreparationNone() {};
	void PreparationWait();
	void PreparationGo();
	virtual void PreparationLeave() = 0;

	// 投げタイプ
	void ThrowTypeNone() {};		// なし
	void ThrowTypeNormal();			// 通常
	void ThrowTypeJump();			// ジャンプ
	void ThrowTypeSpecial();		// スペシャル
	void ThrowTypePass();			// パス
	
	// 投げフラグ
	void ThrowFlagNone() {};		// なし
	void ThrowFlag();				// 投げ
	void ThrowFlagPass();			// パス
	void ThrowFlagSpecial();		// スペシャル

	//=============================
	// メンバ関数
	//=============================
	void UpdateAttack();						// 攻め更新
	void UpdateThrow();							// 投げ更新
	bool IsCancelJumpAttack();					// キャンセル

	// 投げ
	void AttackNormal(CPlayer* pTarget);		// 通常投げ
	void AttackDash(CPlayer* pTarget, float fTargetDis, float fLineDis);			// 走り投げ

	// ジャンプ
	void AttackJump(CPlayer* pTarget);			// ジャンプ投げ
	void AttackDashJump(CPlayer* pTarget, float fTargetDistanse, float LineDistanse);		// 走りジャンプ投げ

	void AttackFeint();							// フェイント

	void UpdateAttackTimer(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void SetAttackTimer(int nMin, int nMax);	// 行動タイマー

	bool IsStop();			// 止める判断

	//=============================
	// メンバ変数
	//=============================
	CPlayer* m_pTarget;
	bool m_bSet;
	int m_nLevel;

	//-----------------------------
	// 列挙
	//-----------------------------
	EAttackMode m_eAttackMode;			// 攻撃モード
	EAttackPrepatarion m_ePreparation;	// 準備
	EThrowType m_eThrowType;			// 投げ種類
	EThrowFlag m_eThrowFlag;			// 投げフラグ
	EAttackNormal m_eAttackNormal;

	//-----------------------------
	// 構造体
	//-----------------------------
	STiming m_sTimig;
};

#endif
