//=============================================================================
// 
//  ガイドヘッダー [guide.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _GUIDE_H_
#define _GUIDE_H_	// 二重インクルード防止

#include "object2d.h"

// 前方宣言
class CControlKeyDisp;

//==========================================================================
// クラス定義
//==========================================================================
// 字幕クラス
class CGuide : public CObject2D
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_NONE = 0,		// なにもない
		STATE_FADEIN,		// フェードイン
		STATE_FADEOUT,		// フェードアウト
		STATE_MAX
	};

	enum Type
	{
		START = 0,	// 開始時
		GOAL,		// ゴール時
		MAX
	};

	CGuide(int nPriority = 7);
	~CGuide();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();

	void SetState(State state);	// 状態設定

	//=============================
	// 静的関数
	//=============================
	static CGuide* Create(Type type);	// 生成処理

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CGuide::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// 関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void UpdateState();		// 状態更新
	void StateNone();		// なし
	void StateFadeIn();		// フェードイン
	void StateFadeout();	// フェードアウト

	// その他関数

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	float m_fLifeTimer;		// 生存時間
	float m_fStateTime;		// 状態カウンター
	State m_state;			// 状態
	CControlKeyDisp* m_pKeyDisp;	// 開始ボタン

};


#endif