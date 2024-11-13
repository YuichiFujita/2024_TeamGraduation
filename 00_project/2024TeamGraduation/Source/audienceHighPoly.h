//==========================================================================
// 
//  観客_ハイポリキャラヘッダー [audienceHighPoly.h]
//  Author : 相馬靜雅
// 
//==========================================================================

#ifndef _AUDIENCE_HIGHPOLY_H_
#define _AUDIENCE_HIGHPOLY_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "audience.h"
#include "gamemanager.h"
#include "objectChara.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CObjectX;

//==========================================================================
// クラス定義
//==========================================================================
// 観客_ハイポリキャラクラス定義
class CAudienceHighPoly : public CAudience
{
public:

	//=============================
	// 定数
	//=============================
	static constexpr float NEAR_LINE = 1000.0f;	// 手前の生成位置上限
	static constexpr float FAR_LINE = 1100.0f; 	// 奥の生成位置上限

	//=============================
	// 列挙型定義
	//=============================
	// モーション列挙
	enum EMotion
	{
		MOTION_DEF = 0,	// 通常状態
		MOTION_SPAWN,	// 入場状態
		MOTION_JUMP,	// 盛り上がり状態
		MOTION_SPECIAL,	// スペシャル状態
		MOTION_DESPAWN,	// 退場状態
		MOTION_MAX		// この列挙型の総数
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CAudienceHighPoly(EObjType type, CGameManager::ETeamSide team);
	~CAudienceHighPoly();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	void Kill() override;	// 削除処理
	inline void SetPosition(const MyLib::Vector3& pos) override	{ m_pChara->SetPosition(pos); }		// 位置設定
	inline MyLib::Vector3 GetPosition() const override			{ return m_pChara->GetPosition(); }	// 位置取得
	inline void SetMove(const MyLib::Vector3& move) override	{ m_pChara->SetMove(move); }		// 移動量設定
	inline MyLib::Vector3 GetMove() const override				{ return m_pChara->GetMove(); }		// 移動量取得
	inline void SetRotation(const MyLib::Vector3& rot) override	{ m_pChara->SetRotation(rot); }		// 向き設定
	inline MyLib::Vector3 GetRotation() const override			{ return m_pChara->GetRotation(); }	// 向き取得

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
	void EndSettingSpawn() override;			// スポーン終了時の設定
	virtual bool SetDespawn(EObjType type = OBJTYPE_NONE) override;	// 退場設定

private:

	//=============================
	// メンバ関数
	//=============================
	HRESULT CreateCharacter(const MyLib::Vector3& pos);	// キャラクター生成

	//=============================
	// メンバ変数
	//=============================
	CObjectChara* m_pChara;	// キャラクター情報
	CObjectX* m_pLight;		// ペンライト情報
};

#endif
