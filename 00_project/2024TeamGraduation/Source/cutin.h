//==========================================================================
// 
//  カットインヘッダー [cutin.h]
//  Author : 藤田勇一
// 
//==========================================================================

#ifndef _CUT_IN_H_
#define _CUT_IN_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object2D.h"

//==========================================================================
// クラス定義
//==========================================================================
// カットインクラス
class CCutIn : public CObject2D
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum EState
	{
		STATE_NONE = 0,	// なにもない状態
		STATE_TIME,		// 時間経過状態
		STATE_END,		// 終了状態
		STATE_MAX
	};

	CCutIn(int nPriority = 7);
	~CCutIn();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Kill() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

	//=============================
	// 静的メンバ関数
	//=============================
	static CCutIn* Create();

	//=============================
	// メンバ関数
	//=============================
	bool IsEnd() const { return (m_state == STATE_END); }	// 終了フラグ取得

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CCutIn::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFuncList[];	// 関数リスト

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	void UpdateTime(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 時間経過状態の更新

	//=============================
	// メンバ変数
	//=============================
	float m_fStateTime;	// 状態カウンター
	EState m_state;		// 状態
};


#endif