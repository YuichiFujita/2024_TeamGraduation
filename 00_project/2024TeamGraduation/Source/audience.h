//==========================================================================
// 
//  観客ヘッダー [audience.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _AUDIENCE_H_
#define _AUDIENCE_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object.h"
#include "listmanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// 観客クラス
class CAudience : public CObject
{
public:

	//=============================
	// 定数
	//=============================
	static constexpr float MAX_LEFT_LINE = -1000.0f;	// 左の生成位置上限
	static constexpr float MAX_RIGHT_LINE = 1000.0f;	// 右の生成位置上限

	//=============================
	// 列挙型定義
	//=============================
	// オブジェクト種類
	enum EObjType
	{
		OBJTYPE_ANIM = 0,	// 3Dポリゴン
		OBJTYPE_LOWPOLY,	// ローポリキャラクター
		OBJTYPE_HIGHPOLY,	// ハイポリキャラクター
		OBJTYPE_MAX			// この列挙型の総数
	};

	// 状態
	enum EState
	{
		STATE_SPAWN = 0,	// 入場状態
		STATE_NORMAL,		// 通常状態
		STATE_JUMP,			// 盛り上がり状態
		STATE_DESPAWN,		// 退場状態
		STATE_MAX			// この列挙型の総数
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CAudience(EObjType type, int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CAudience();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill();	// 削除

	//=============================
	// 純粋仮想関数
	//=============================
	virtual void SetPosition(const MyLib::Vector3& pos) override = 0;	// 位置設定
	virtual MyLib::Vector3 GetPosition() const override = 0;			// 位置取得
	virtual void SetMove(const MyLib::Vector3& move) override = 0;		// 移動量設定
	virtual MyLib::Vector3 GetMove() const override = 0;				// 移動量取得
	virtual void SetRotation(const MyLib::Vector3& rot) override = 0;	// 向き設定
	virtual MyLib::Vector3 GetRotation() const override = 0;			// 向き取得

	//=============================
	// 仮想関数
	//=============================


	//=============================
	// メンバ関数
	//=============================


	//=============================
	// 静的メンバ関数
	//=============================
	static CAudience* Create(EObjType type);

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CAudience::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFuncList[];	// 関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	void UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 入場状態の更新
	void UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 通常状態の更新
	void UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 盛り上がり状態の更新
	void UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 退場状態の更新

	//=============================
	// 静的メンバ変数
	//=============================
	static CListManager<CAudience> m_list;	// リスト

	//=============================
	// メンバ変数
	//=============================
	const EObjType m_type;		// オブジェクト種類
	const float m_fJumpLevel;	// ジャンプ量
	EState m_state;	// 状態
};

#endif
