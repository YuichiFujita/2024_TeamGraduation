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
#include "object3D_Anim.h"

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
	static constexpr float START_LINE = 300.0f;
	static constexpr float END_LINE = 600.0f;

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CAudienceAnim();
	~CAudienceAnim();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	void Kill() override;	// 削除処理
	inline void SetPosition(const MyLib::Vector3& pos) override	{ m_pAnim3D->SetPosition(pos); }		// 位置設定
	inline MyLib::Vector3 GetPosition() const override			{ return m_pAnim3D->GetPosition(); }	// 位置取得
	inline void SetMove(const MyLib::Vector3& move) override	{ m_pAnim3D->SetMove(move); }			// 移動量設定
	inline MyLib::Vector3 GetMove() const override				{ return m_pAnim3D->GetMove(); }		// 移動量取得
	inline void SetRotation(const MyLib::Vector3& rot) override	{ m_pAnim3D->SetRotation(rot); }		// 向き設定
	inline MyLib::Vector3 GetRotation() const override			{ return m_pAnim3D->GetRotation(); }	// 向き取得

private:

	//=============================
	// メンバ変数
	//=============================
	CObject3DAnim* m_pAnim3D;	// アニメーション3D情報
};

#endif
