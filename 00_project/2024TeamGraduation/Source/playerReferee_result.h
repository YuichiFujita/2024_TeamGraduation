//=============================================================================
// 
//  審判用プレイヤーヘッダー [playerReferee_result.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYER_REFEREE_RESULT_H_
#define _PLAYER_REFEREE_RESULT_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "playerReferee.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CStartText;

//==========================================================================
// クラス定義
//==========================================================================
// プレイヤークラス定義
class CPlayerReferee_Result : public CPlayerReferee
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// モーション列挙
	enum EMotion
	{
		MOTION_DEF = 0,	// ニュートラルモーション
		MOTION_WAIT,	// 待機
		MOTION_TALK,	// しゃべる
		MOTION_WIN_L,	// 左勝ち
		MOTION_WIN_R,	// 右勝ち
		MOTION_MAX
	};

	// 状態
	enum EState
	{
		STATE_NONE = 0,	// なにもない
		STATE_WAIT,		// 待機
		STATE_TALK,		// しゃべって焦らし
		STATE_WIN,		// 勝ち
		STATE_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerReferee_Result(
		const CGameManager::ETeamSide typeTeam = CGameManager::ETeamSide::SIDE_NONE,
		const CPlayer::EFieldArea typeArea = CPlayer::EFieldArea::FIELD_NONE,
		const CPlayer::EBaseType typeBase = CPlayer::EBaseType::TYPE_USER,
		int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CPlayerReferee_Result();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Kill() override;	// 動的削除

	//=============================
	// メンバ関数
	//=============================
	void SetState(EState state);		// 状態設定
	void SetWinTeam(CGameManager::ETeamSide team) { m_winTeam = team; }	// 勝利チーム

	//=============================
	// 静的メンバ関数
	//=============================
	static CPlayerReferee_Result* Create();	// 生成処理

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayerReferee_Result::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// 状態関数

	//=============================
	// オーバーライド関数
	//=============================
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 状態更新
	void AttackAction(CMotionManager::AttackInfo ATKInfo, int nCntATK) override;		// 攻撃時処理

	//=============================
	// メンバ関数
	//=============================
	//-----------------------------
	// 状態関数
	//-----------------------------
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// なし
	void StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 待ち
	void StateTalk(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 喋り
	void StateWin(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 勝ち

	//=============================
	// メンバ変数
	//=============================
	//-----------------------------
	// 状態
	//-----------------------------
	EState m_state;				// 状態
	float m_fStateTime;			// 状態時間
	CGameManager::ETeamSide m_winTeam;	// 勝利チーム

	//-----------------------------
	// エフェクト用
	//-----------------------------
	CEffekseerObj* m_pWin;	// 勝利
};

#endif
