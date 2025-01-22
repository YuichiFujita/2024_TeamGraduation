//=============================================================================
// 
//  キャッチスペシャル処理 [catchSpecial.h]
//  Author : Kai Takada
// 
//=============================================================================

#ifndef _CATCHSPECIAL_H_
#define _CATCHSPECIAL_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "ball.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayer;

//==========================================================================
// クラス定義
//==========================================================================
// キャッチスペシャルクラス定義
class CCatchSpecial
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// 状態定義
	enum EState
	{
		STATE_NONE = 0,			// なにもない
		STATE_BEAM_SUCC,		// スペシャルキャッチ(ビーム)(成功)
		STATE_BEAM_FAIL,		// スペシャルキャッチ(ビーム)(失敗)
		STATE_MAX
	};

	// 状態内状態定義(勢い)
	enum EMomentumState
	{
		MOMENTUM_NONE = 0,		// なし
		MOMENTUM_SLIDE,			// ズザザー
		MOMENTUM_BRAKE,			// 耐える
		MOMENTUM_RESULT,		// 結果(成功)(失敗)
		MOMENTUM_END,			// 終了
		MOMENTUM_MAX
	};

	CCatchSpecial(CPlayer* pPlayer, EState state);
	~CCatchSpecial();

	//=============================
	// メンバ関数
	//=============================
	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetState(EState state);									// 状態設定
	EState GetState() { return m_state; }							// 状態取得
	void SetMomentumState(EMomentumState state);					// 勢い状態設定
	EMomentumState GetMomentumState() { return m_momentumState; }	// 勢い状態設定

	void SetEnableSuccess(bool bSuccess) { m_bSuccess = bSuccess; }		// 成功フラグ設定
	bool IsState() { return m_bSuccess; }								// 成功フラグ取得

	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }		// プレイヤー設定
	CPlayer* GetPlayer() { return m_pPlayer; }						// プレイヤー取得

	void Debug();		// 成功フラグ取得

	//-----------------------------
	// 判定関数
	//-----------------------------
	static EState Check_Kamehameha(const CPlayer* pPlayer, const bool bJust);

	static CCatchSpecial::EState Check(CPlayer* pPlayer, bool bJust, CBall::ESpecial state);		// チェック
	static CCatchSpecial* Create(CPlayer* pPlayer, EState state);			// 生成

private:
	//=============================
	// 関数リスト
	//=============================
	// 状態関数
	typedef void(CCatchSpecial::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// 状態関数

	// 勢い状態関数
	typedef void(CCatchSpecial::* MOMENTUM_FUNC)(const float, const float, const float);
	static MOMENTUM_FUNC m_MomentumFunc[];	// 状態内状態関数

	// 状態開始関数
	typedef void(CCatchSpecial::* START_FUNC)();
	static START_FUNC m_StartFunc[];	// 状態開始関数

	// 勢い状態開始関数
	typedef void(CCatchSpecial::* MOMENTUM_START_FUNC)();
	static MOMENTUM_START_FUNC m_MomentumStartFunc[];	// 状態開始関数

	// 判定関数
	static std::vector<std::function<EState(const CPlayer*, const bool)>> s_CheckFunc;

	//=============================
	// メンバ関数
	//=============================
	//-----------------------------
	// 状態関数
	//-----------------------------
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);				// なし
	void StateKamehameSucc(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// かめはめ波？キャッチ成功
	void StateKamehameFail(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// かめはめ波？キャッチ失敗

	//-----------------------------
	// 状態関数
	//-----------------------------
	void MomentumStateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// なし
	void MomentumStateSlide(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ズザザー
	void MomentumStateBrake(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 耐える
	void MomentumStateResult(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 結果(成功)(失敗)
	void MomentumStateEnd(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 終了
	
	//-----------------------------
	// 状態開始関数
	//-----------------------------
	void StateStartNone();		// なし

	//-----------------------------
	// 勢い状態開始関数
	//-----------------------------
	void MomentumStartSlide();		// ズザザー
	void MomentumStartResult();		// 結果

	void Success();		// 成功時共通
	void Failure();		// 失敗時共通

	//=============================
	// メンバ変数
	//=============================
	EState m_state;					// 状態
	float m_fStateTime;				// 状態時間
	EMomentumState m_momentumState;	// 状態内状態
	float m_fMomentumStateTime;		// 状態内状態時間
	CPlayer* m_pPlayer;				// プレイヤー
	bool m_bSuccess;				// 成功フラグ
};

#endif
