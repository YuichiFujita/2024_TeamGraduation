//==========================================================================
// 
//  観客_アニメーション3Dヘッダー [audienceAnim.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _AUDIENCE_ANIM_H_
#define _AUDIENCE_ANIM_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "audience.h"
#include "gamemanager.h"
#include "objectCharaAnim.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CObjectX;

//==========================================================================
// クラス定義
//==========================================================================
// 観客_アニメーション3Dクラス定義
class CAudienceAnim : public CAudience
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// モーション列挙
	enum EMotion
	{
		MOTION_IDOL_U = 0,	// 上待機モーション
		MOTION_IDOL_D,		// 下待機モーション
		MOTION_JUMP_U,		// 上ジャンプモーション
		MOTION_JUMP_D,		// 下ジャンプモーション
		MOTION_MOVE_L,		// 左移動モーションz 
		MOTION_MOVE_R,		// 右移動モーション
		MOTION_MAX			// この列挙型の総数
	};

	// 観戦エリア列挙
	enum EArea
	{
		AREA_FAR = 0,	// 奥
		AREA_UP,		// 上
		AREA_NEAR,		// 手前
		AREA_MAX		// この列挙型の総数
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CAudienceAnim(EObjType type, CGameManager::ETeamSide team);
	~CAudienceAnim();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	void Kill() override;	// 削除処理
	void SetPosition(const MyLib::Vector3& pos) override;	// 位置設定
	void SetMove(const MyLib::Vector3& move) override;		// 移動量設定
	void SetRotation(const MyLib::Vector3& rot) override;	// 向き設定
	inline MyLib::Vector3 GetPosition() const override		{ return m_pFrontAnimChara->GetPosition(); }	// 位置取得
	inline MyLib::Vector3 GetMove() const override			{ return m_pFrontAnimChara->GetMove(); }		// 移動量取得
	inline MyLib::Vector3 GetRotation() const override		{ return m_pFrontAnimChara->GetRotation(); }	// 向き取得

protected:
	//=============================
	// オーバーライド関数
	//=============================
	// 状態関数
	int UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 入場状態の更新
	int UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 通常状態の更新
	int UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// 盛り上がり状態の更新
	int UpdateSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// スペシャル状態の更新
	int UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// 退場状態の更新

	// ゲッター/セッター
	void SetMotion(const int nMotion) override;	// モーション設定
	void EndSettingSpawn() override {}			// スポーン終了時の設定
	void SetIdolMotion(const EMotion motion) { m_idolMotion = motion; }		// 待機モーション
	void SetJumpMotion(const EMotion motion) { m_jumpMotion = motion; }		// ジャンプモーション
	void SetMoveMotion(const EMotion motion) { m_moveMotion = motion; }		// 移動モーション
	virtual bool SetNTR(CGameManager::ETeamSide team) override;									// NTR設定

private:

	//=============================
	// 関数リスト
	//=============================
	// 観戦位置計算リスト
	typedef void(CAudienceAnim::*WATCH_POS_FUNC)();
	static WATCH_POS_FUNC m_CalcWatchPositionFunc[];

	//=============================
	// メンバ関数
	//=============================
	HRESULT CreateAnimCharacter(const MyLib::Vector3& rPos);	// キャラクター生成
	HRESULT CreatePenLight();		// ペンライト生成
	virtual void CalcWatchPositionFar();	// 観戦位置計算 (奥)
	virtual void CalcWatchPositionUp();		// 観戦位置計算 (上)
	virtual void CalcWatchPositionNear();	// 観戦位置計算 (手前)

	//=============================
	// メンバ変数
	//=============================
	CObjectCharaAnim* m_pFrontAnimChara;	// 表面キャラクター情報
	CObjectCharaAnim* m_pBackAnimChara;		// 裏面キャラクター情報
	CObjectX* m_pLight;		// ペンライト情報
	EMotion m_idolMotion;	// 待機モーション
	EMotion m_jumpMotion;	// ジャンプモーション
	EMotion m_moveMotion;	// 移動モーション
};

#endif
