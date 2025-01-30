//==========================================================================
// 
//  ゲームスタートの文字ヘッダー [startText.h]
//  Author : 相馬靜雅
// 
//==========================================================================

#ifndef _STARTTEXT_H_
#define _STARTTEXT_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object2D.h"

//==========================================================================
// クラス定義
//==========================================================================
// ゲームスタートの文字クラス
class CStartText : public CObject2D
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum EState
	{
		STATE_NONE = 0,	// なにもない
		STATE_FADEIN,	// フェードイン
		STATE_WAIT,		// 待機
		STATE_FADEOUT,	// フェードアウト
		STATE_MAX
	};

	CStartText(int nPriority = 6);
	~CStartText();

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
	static CStartText* Create();

	//=============================
	// メンバ関数
	//=============================
	void SetState(EState state);	// 状態設定

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CStartText::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// 関数リスト

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// 状態更新
	void StateNone() {}		// なにもない
	void StateFadeIn();		// フェードイン
	void StateWait();		// 待機
	void StateFadeOut();	// フェードアウト

	//=============================
	// メンバ変数
	//=============================
	float m_fStateTime;	// 状態カウンター
	EState m_state;		// 状態
};


#endif