//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playercontrol.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYERCONTROL_H_
#define _PLAYERCONTROL_H_	// 二重インクルード防止

#define BRESSRANGE (0)

#include "player.h"
#include "baggage.h"

#include "command_group.h"

#if BRESSRANGE
#include "debug_bressrange.h"
#endif

class CSuffocation;
class CEffekseerObj;

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerControlMove
{
public:

	// コンストラクタ
	CPlayerControlMove() : m_nCntWaterRipple(0), m_nIntervalWaterRipple(21), m_nAddRippleCounter(0), m_nIntervalAddRippleCounter(0){}

	virtual void Move(CPlayer* player);	// 移動

private:
	int m_nCntWaterRipple;		// 水波紋カウンター
	int m_nIntervalWaterRipple;	// 水波紋カウンター間隔
	int m_nAddRippleCounter;			// 水波紋カウンター加算
	int m_nIntervalAddRippleCounter;	// 水波紋カウンター加算間隔


};

//=============================
// 荷物操作
//=============================
class CPlayerControlBaggage
{
public:

	enum STATE
	{
		STATE_NONE = 0,	// 何もない
		STATE_WAIT,		// 入力待機
		STATE_PRESS,	// 入力中
		STATE_RELEASE,	// 離した
		STATE_MAX
	};

	// コンストラクタ
	CPlayerControlBaggage() : m_pGuide(nullptr)
	{
		m_pEffekseerObj = nullptr;
		m_bFall = true;
		m_state = STATE::STATE_NONE;
		m_nGoalTimer = 0;
		m_pSuffocation = nullptr;
	}

	~CPlayerControlBaggage();

	virtual void Action(CPlayer* player, CBaggage* pBaggage);	// アクション
	void EffectStop();	// エフェクト停止
	void Reset(CPlayer* player, CBaggage* pBaggage);		// リセット
	void GoalSetting();	// ゴール時の設定
	STATE GetState() { return m_state; }

private:

	// メンバ関数
	bool CollisionObstacle(CPlayer* player, CBaggage* pBaggage);	// 障害物判定
	bool EndCheck(CBaggage* pBaggage);
	void GoalAction(CPlayer* player, CBaggage* pBaggage);
	void BressEffect(CPlayer* player, CBaggage* pBaggage);		// 息エフェクト
	void Suffocation(CPlayer* player, CBaggage* pBaggage);

	// メンバ変数
	CEffekseerObj* m_pEffekseerObj = nullptr;

#if BRESSRANGE
	CDebugBressRange* m_pBressRange = nullptr;		// 息範囲
	CDebugBressRange* m_pBressHeight = nullptr;		// 息の届く高さ範囲
	CCollisionLine_Box* m_pBox = nullptr;	// デバッグ用当たり判定
#endif

	float m_fHeight = 0.0f;				// 現在の息高さ
	float m_fMaxHeight = 1000.0f;		// 息の届く最大の高さ
	float m_fTimeDownHeight = 0.0f;		// 高さの降下時間
	float m_fHeightVelocity = 10.0f;	// 息の加算量
	float m_fBressTimer = 0.0f;			// 息のタイマー
	float m_fIntervalBress = 0.0f;		// 息のタイマー
	float m_fGoalWaitTimer = 0.0f;		// ゴール時の待ち時間
	bool m_bLandOld;					// 前回の着地情報
	bool m_bFall;						// 落下しているかどうか
	int m_nGoalTimer;					// タイマー
	STATE m_state;
	MyLib::Vector3 m_GoalMove;			// ゴール時の移動量
	CSuffocation* m_pSuffocation;
	CGuide* m_pGuide;					// ガイド
};

//=============================
// 浮上操作
//=============================
class CPlayerControlSurfacing
{
public:

	// コンストラクタ
	CPlayerControlSurfacing() {}

	virtual float Surfacing(CPlayer* player);	// 移動

private:

	float m_fHeight = 0.0f;	// 浮上高さ
};

//=============================
// トリック入力
//=============================
class CPlayerControlTrick
{
public:

	// コンストラクタ
	CPlayerControlTrick() {
		m_pCommandPad = nullptr;
		m_pCommandPad = CCommandGroup::Create("data\\TEXT\\command\\gamepad.txt");
		m_fHeightTimer = 0.0f;
		m_fHeight = 0.0f;
	}

	virtual void Uninit();
	virtual void Trick(CPlayer* player, int& nIdx, bool& bValue);	// トリック
	float GetHeight() { return m_fHeight; }

private:

	// メンバ変数
	CCommandGroup* m_pCommandPad;	// ゲームパッドコマンド
	float m_fHeightTimer;	// 追加高さ
	float m_fHeight;		// 高さ
};

#endif