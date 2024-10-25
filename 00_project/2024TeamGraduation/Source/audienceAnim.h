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
// クラス定義
//==========================================================================
// 観客_アニメーション3Dクラス定義
class CAudienceAnim : public CAudience
{
public:

	//=============================
	// 定数
	//=============================
	static constexpr float NEAR_LINE = 1350.0f;	// 手前の生成位置上限
	static constexpr float FAR_LINE = 1750.0f; 	// 奥の生成位置上限

	//=============================
	// 列挙型定義
	//=============================
	// モーション列挙
	enum EMotion
	{
		MOTION_IDOL = 0,	// 待機モーション
		MOTION_JUMP,		// ジャンプモーション
		MOTION_MOVE_L,		// 左移動モーション
		MOTION_MOVE_R,		// 右移動モーション
		MOTION_MAX			// この列挙型の総数
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CAudienceAnim(EObjType type, CGameManager::TeamSide team);
	~CAudienceAnim();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	void Kill() override;	// 削除処理
	inline void SetPosition(const MyLib::Vector3& pos) override	{ m_pAnimChara->SetPosition(pos); }		// 位置設定
	inline MyLib::Vector3 GetPosition() const override			{ return m_pAnimChara->GetPosition(); }	// 位置取得
	inline void SetMove(const MyLib::Vector3& move) override	{ m_pAnimChara->SetMove(move); }		// 移動量設定
	inline MyLib::Vector3 GetMove() const override				{ return m_pAnimChara->GetMove(); }		// 移動量取得
	inline void SetRotation(const MyLib::Vector3& rot) override	{ m_pAnimChara->SetRotation(rot); }		// 向き設定
	inline MyLib::Vector3 GetRotation() const override			{ return m_pAnimChara->GetRotation(); }	// 向き取得

protected:
	//=============================
	// オーバーライド関数
	//=============================
	// 状態関数
	int UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 入場状態の更新
	int UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 通常状態の更新
	int UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 盛り上がり状態の更新
	int UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 退場状態の更新

	// ゲッター/セッター
	void SetMotion(const int nMotion) override;	// モーション設定

private:

	//=============================
	// メンバ変数
	//=============================
	CObjectCharaAnim* m_pAnimChara;	// キャラクターアニメーション情報
	EMotion m_moveMotion;	// 移動モーション
};

#endif
