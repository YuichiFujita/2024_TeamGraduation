//=============================================================================
// 
// タイトルエンター [title_pressenter.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TITLE_PRESSENTER_H_
#define _TITLE_PRESSENTER_H_		// 二重インクルード防止のマクロを定義する

#include "object2D.h"

class CTitle_Select;

//==========================================================================
// クラス定義
//==========================================================================
// タイトルエンター
class CTitle_PressEnter : public CObject2D
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum STATE
	{
		STATE_NONE = 0,			// なにもなし
		STATE_FADEIN,			// フェードイン
		STATE_FADEOUT,		// フェードアウト
		STATE_NOACTIVE,			// 反応しない
		STATE_MAX
	};

	CTitle_PressEnter(float fadetime, int nPriority = 8);
	~CTitle_PressEnter() {}
	
	//  オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	void SetState(STATE state);
	STATE GetState() { return m_state; }
	CTitle_Select* GetSelect() { return m_pSelect; }

	static CTitle_PressEnter* Create(float fadetime);	// 生成処理

private:

	//=============================
	// 関数ポインタ
	//=============================
	typedef void(CTitle_PressEnter::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	void StateNone();		// なにもなし
	void StateFadeIn();		// フェードイン
	void StateFadeOut();		// フェードアウト
	void StateNoActive();		// 反応しない

	//=============================
	// メンバ変数
	//=============================
	STATE m_state;		// 状態
	float m_fStateTime;	// 状態カウンター
	const float m_fFadeOutTime;	// フェードにかかる時間
	CTitle_Select* m_pSelect;
};

#endif