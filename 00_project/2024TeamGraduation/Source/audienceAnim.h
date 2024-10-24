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

private:

	//=============================
	// メンバ変数
	//=============================
	CObjectCharaAnim* m_pAnimChara;	// キャラクターアニメーション情報
};

#endif
