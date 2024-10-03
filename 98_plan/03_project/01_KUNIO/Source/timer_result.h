//=============================================================================
// 
//  タイマーヘッダー [timer_result.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TIMER_RESULT_H_
#define _TIMER_RESULT_H_	// 二重インクルード防止

#include "timer.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// タイマークラス
class CTimer_Result : public CTimer
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_SCROLL_TEXT = 0,	// 文字送り
		STATE_SCROLL_VOID,		// 空間送り
		STATE_SCROLL_TIME,		// タイム送り
		STATE_FINISH,			// 終了
		STATE_EMPHASIZE,		// 強調
		STATE_NONE,				// なにもなし
		STATE_MAX
	};

	CTimer_Result(int nPriority = 6);
	~CTimer_Result();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	virtual void ApplyTimer() override;	// タイマー反映

	void SetState(State state);				// 状態設定
	State GetState() { return m_state; }	// 状態取得


	bool IsFinish() { return m_bFinish; }	// 終了

private:

	//=============================
	// 関数リスト
	//=============================
	// 状態リスト
	typedef void(CTimer_Result::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void UpdateState();
	void StateScrollText();	// 文字送り
	void StateSrollVoid();	// 空間送り
	void StateScrollTime();	// タイム送り
	void StateFinish();		// 終了
	void StateEmphasize();	// 強調
	void StateNone() {}		// なにもなし

	// その他関数
	void CreateText();	// 文字生成
	void Skip();		// スキップ

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	float m_fStateTime;		// 状態カウンター
	State m_state;			// 状態

	// その他
	CObject2D* m_pText;		// 文字
	float m_fMoveTextLen;	// テキストの移動距離
	float m_fMoveTimeLen;	// タイムの移動距離
	bool m_bFinish;			// 終了

};



#endif