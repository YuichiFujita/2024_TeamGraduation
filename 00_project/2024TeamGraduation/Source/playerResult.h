//=============================================================================
// 
//  リザルト用プレイヤーヘッダー [playerResult.h]
//  Author : Kai Takada
// 
//=============================================================================

#ifndef _PLAYER_RESULT_H_
#define _PLAYER_RESULT_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "player.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// プレイヤークラス定義
class CPlayerResult : public CPlayer
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// モーション列挙
	enum EMotion
	{
		MOTION_DEF = 0,	// ニュートラルモーション
		MOTION_WIN,		// 勝利
		MOTION_LOSE,	// 敗北
		MOTION_MAX
	};

	// 状態
	enum EState
	{
		STATE_NONE = 0,		// なにもない
		STATE_WIN,			// 勝利
		STATE_LOSE,			// 敗北
		STATE_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerResult(
		const CGameManager::ETeamSide typeTeam = CGameManager::ETeamSide::SIDE_NONE,
		const CPlayer::EFieldArea typeArea = CPlayer::EFieldArea::FIELD_NONE,
		const CPlayer::EBaseType typeBase = CPlayer::EBaseType::TYPE_USER,
		int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CPlayerResult();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;	// 動的削除

	//=============================
	// メンバ関数
	//=============================
	void SetState(EState state);		// 状態設定

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayerResult::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// 状態関数

	//=============================
	// オーバーライド関数
	//=============================
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 状態更新

	//=============================
	// メンバ関数
	//=============================
	//-----------------------------
	// 状態関数
	//-----------------------------
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// なし
	void StateWin(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 勝利
	void StateLose(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 敗北

	//-----------------------------
	// 状態
	//-----------------------------
	EState m_state;		// 状態
	float m_fStateTime;	// 状態時間

};

#endif
