//=============================================================================
// 
//  カウントダウンヘッダー [countdown_start.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _COUNTDOWN_START_H_
#define _COUNTDOWN_START_H_	// 二重インクルード防止

#include "object2d.h"

//==========================================================================
// クラス定義
//==========================================================================
// カウントダウンクラス
class CCountdown_Start : public CObject2D
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_NONE = 0,		// なにもない
		STATE_DROP_UP,		// 落下(上)
		STATE_DROP_WAIT,	// 落下(待機)
		STATE_DROP_DOWN,	// 落下(下)
		STATE_COMPLETE,		// 完了！
		STATE_FADEOUT,		// フェードアウト
		STATE_MAX
	};

	CCountdown_Start(int nPriority = 7);
	~CCountdown_Start();

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
	static CCountdown_Start* Create();	// 生成処理

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CCountdown_Start::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// 関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void UpdateState();		// 状態更新
	void StateNone();		// なし
	void StateDrop_UP();	// 落下(上)
	void StateDrop_Wait();	// 落下(待機)
	void StateDrop_DOWN();	// 落下(下)
	void StateComplete();	// 完了
	void StateFadeout();	// フェードアウト

	// その他関数

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	float m_fStateTime;		// 状態カウンター
	State m_state;			// 状態

	// その他変数
	int m_nCount;	// カウント

};


#endif