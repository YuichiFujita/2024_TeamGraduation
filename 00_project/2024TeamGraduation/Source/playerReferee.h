//=============================================================================
// 
//  審判用プレイヤーヘッダー [playerReferee.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYER_REFEREE_H_
#define _PLAYER_REFEREE_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "player.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CStartText;

//==========================================================================
// クラス定義
//==========================================================================
// プレイヤークラス定義
class CPlayerReferee : public CPlayer
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
		MOTION_TOSS,	// トス
		MOTION_RETUN,	// 戻る
		MOTION_MAX
	};

	// 状態
	enum EState
	{
		STATE_NONE = 0,		// なにもない
		STATE_TOSSWAIT,		// トス待機
		STATE_TOSS,			// トス
		STATE_RETURN,		// 戻る
		STATE_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CPlayerReferee(
		const CGameManager::ETeamSide typeTeam = CGameManager::ETeamSide::SIDE_NONE,
		const CPlayer::EFieldArea typeArea = CPlayer::EFieldArea::FIELD_NONE,
		const CPlayer::EBaseType typeBase = CPlayer::EBaseType::TYPE_USER,
		int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CPlayerReferee();

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
	typedef void(CPlayerReferee::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// 状態関数

protected:

	//=============================
	// オーバーライド関数
	//=============================
	virtual void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 状態更新
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// 攻撃時処理

private:
	//=============================
	// メンバ関数
	//=============================
	//-----------------------------
	// 状態関数
	//-----------------------------
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// なし
	void StateTossWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// トス待ち
	void StateToss(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// トス
	void StateReturn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 戻る

	//-----------------------------
	// 状態
	//-----------------------------
	EState m_state;				// 状態
	float m_fStateTime;			// 状態時間
	CStartText* m_pStartText;	// スタート文字
};

#endif
