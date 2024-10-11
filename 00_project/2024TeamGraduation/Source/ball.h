//=============================================================================
// 
//  ボールヘッダー [ball.h]
//  Author : 藤田勇一
// 
//=============================================================================

#ifndef _BALL_H_
#define _BALL_H_	// 二重インクルード防止

#include "objectX.h"
#include "listmanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// ボールクラス
class CBall : public CObjectX
{
public:
	
	//=============================
	// 列挙型定義
	//=============================
	enum EState
	{
		STATE_SPAWN = 0,	// 生成状態 (開始時のフリーボール)
		STATE_CATCH,		// 所持状態 (プレイヤーが保持している)
		STATE_ATTACK,		// 攻撃状態 (攻撃判定を持っている)
		STATE_FALL,			// 落下状態 (地面に転がっている)
		STATE_MAX			// この列挙型の総数
	};

	CBall(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CBall();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;

	//=============================
	// メンバ関数
	//=============================
	void Kill();	// 削除

	//=============================
	// 静的関数
	//=============================
	/*
		@brief		生成処理
		@details	必要があれば引数追加
	*/
	static CBall *Create();
	static CListManager<CBall> GetListObj() { return m_List; }	// リスト取得

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CBall::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_SampleFuncList[];	// 関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	void UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 生成状態の更新
	void UpdateCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 所持状態の更新
	void UpdateAttack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 攻撃状態の更新
	void UpdateFall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 落下状態の更新

	//=============================
	// 静的メンバ変数
	//=============================
	static CListManager<CBall> m_List;	// リスト

	//=============================
	// メンバ変数
	//=============================
	float m_fStateTime;	// 状態カウンター
	EState m_state;		// 状態

};


#endif