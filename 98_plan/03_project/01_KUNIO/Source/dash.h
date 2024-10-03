//=============================================================================
// 
//  ダッシュヘッダー [dash.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _DASH_H_
#define _DASH_H_	// 二重インクルード防止

#include "object2d.h"

//==========================================================================
// クラス定義
//==========================================================================
// ダッシュクラス
class CDash : public CObject2D
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_NONE = 0,		// なにもない
		STATE_DASH,			// ダッシュ
		STATE_MAX
	};

	enum Angle
	{
		ANGLE_UP = 0,
		ANGLE_RIGHTUP,
		ANGLE_RIGHT,
		ANGLE_RIGHTDW,
		ANGLE_DOWN,
		ANGLE_LEFTDW,
		ANGLE_LEFT,
		ANGLE_LEFTUP,
		ANGLE_MAX
	};

	struct MyStruct
	{
		bool bDash;
		Angle angle;

		MyStruct() : bDash(false), angle(ANGLE_LEFT) {}
	};

	CDash(int nPriority = 7);
	~CDash();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//=============================
	// 静的関数
	//=============================
	static CDash* Create();

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CDash::* STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// 関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void UpdateState();		// 状態更新
	void StateNone();		// なし
	void StateDash();		// ダッシュ

	// その他関数
	MyStruct Trigger(Angle angle);

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	float m_fStateTime;		// 状態カウンター
	State m_state;			// 状態

	// その他変数
	int m_nCntTrigger[Angle::ANGLE_MAX];		// トリガーのカウント
	float m_fDashInterval;		// ダッシュのインターバル

};


#endif