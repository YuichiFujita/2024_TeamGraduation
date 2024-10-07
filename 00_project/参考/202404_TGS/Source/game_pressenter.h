//=============================================================================
// 
// タイトルエンター [game_pressenter.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _GAME_PRESSENTER_H_
#define _GAME_PRESSENTER_H_		// 二重インクルード防止のマクロを定義する

#include "object2D.h"

//==========================================================================
// クラス定義
//==========================================================================
// タイトルエンター
class CGame_PressEnter : public CObject2D
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum STATE
	{
		STATE_NONE = 0,			// なにもなし
		STATE_FADEIN,			// フェードイン
		STATE_TUTORIAL_FADEOUT,	// チュートリアル確認のフェードアウト
		STATE_MAX
	};

	CGame_PressEnter(float fadetime, int nPriority = 8);
	~CGame_PressEnter() {}
	
	//  オーバーライドされた関数
	HRESULT Init() override;
	void Update() override;

	void SetState(STATE state);
	STATE GetState() { return m_state; }

	static CGame_PressEnter* Create(float fadetime);	// 生成処理

private:

	//=============================
	// 関数ポインタ
	//=============================
	typedef void(CGame_PressEnter::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	void StateNone();		// なにもなし
	void StateFadeIn();		// フェードイン
	void StateTutorial_FadeOut();		// チュートリアル確認のフェードアウト

	//=============================
	// メンバ変数
	//=============================
	STATE m_state;		// 状態
	float m_fStateTime;	// 状態カウンター
	const float m_fFadeOutTime;	// フェードにかかる時間
};

#endif