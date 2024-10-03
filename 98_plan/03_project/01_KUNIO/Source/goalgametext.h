//=============================================================================
// 
// ゴールゲームテキスト処理 [goalgametext.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _GOALGAMETEXT_H_
#define _GOALGAMETEXT_H_		// 二重インクルード防止のマクロを定義する

#include "object2D.h"

//==========================================================================
// クラス定義
//==========================================================================
// ゴールゲームテキストクラス定義
class CGoalGameText : public CObject2D
{
public:

	CGoalGameText(int nPriority = 8);
	~CGoalGameText();

	
	//  オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	static CGoalGameText* Create();

private:

	//=============================
	// 状態列挙
	//=============================
	enum eState
	{
		STATE_EXPANSION = 0,
		STATE_EXPNONE,
		STATE_FADEOUT,
		STATE_MAX
	};

	//=============================
	// 関数リスト
	//=============================
	typedef void(CGoalGameText::* STATE_FUNC)();	// 状態処理のリスト
	static STATE_FUNC m_StateFuncList[];

	//=============================
	// メンバ関数
	//=============================
	void UpdateState();		// 状態更新
	void StateExpansion();	// 拡大
	void StateExpNone();	// 拡大後何もしない
	void StateFadeOut();	// フェードアウト状態

	//=============================
	// メンバ変数
	//=============================
	eState m_state;		// 状態
	float m_fStateTimer;	// 状態タイマー

};

#endif