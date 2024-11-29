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

	CCatchSpecial(CPlayer* pPlayer, EState state);
	~CCatchSpecial();

	//=============================
	// メンバ関数
	//=============================
	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetState(EState state);			// 状態設定
	EState GetState() { return m_state; }	// 状態取得

	void SetEnableSuccess(bool bSuccess) { m_bSuccess = bSuccess; }		// 成功フラグ設定
	bool IsState() { return m_bSuccess; }								// 成功フラグ取得

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

	static std::vector<std::function<EState(const CPlayer*, const bool)>> s_CheckFunc;

	//=============================
	// メンバ関数
	//=============================
	//-----------------------------
	// スペシャルキャッチ状態関数
	//-----------------------------
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);				// なし
	void State_Kamehame_Succ(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// かめはめ波？キャッチ成功
	void State_Kamehame_Fail(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// かめはめ波？キャッチ失敗

	void Success();		// 成功時共通
	void Failure();		// 失敗時共通

	//=============================
	// メンバ変数
	//=============================
	EState m_state;				// 状態
	float m_fStateTime;			// 状態時間
	CPlayer* m_pPlayer;			// プレイヤー
	bool m_bSuccess;			// 成功フラグ
};

#endif
