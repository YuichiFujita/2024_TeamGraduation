//=============================================================================
// 
//  選択肢の筆ヘッダー [selectdraw.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SELECTDRAW_H_
#define _SELECTDRAW_H_	// 二重インクルード防止

#include "object2d.h"

//==========================================================================
// クラス定義
//==========================================================================
// 選択肢の筆クラス
class CSelectDraw : public CObject2D
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
		STATE_DRAWING,		// 書く
		STATE_MAX
	};

	CSelectDraw(int nPriority = 2);
	~CSelectDraw();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetState(State state);	// 状態設定
	virtual void SetPosition(const MyLib::Vector3& pos) override;		// 位置設定
	virtual void SetOriginPosition(const MyLib::Vector3& pos) override;	// 元の位置設定

	//=============================
	// 静的関数
	//=============================
	static CSelectDraw* Create(const MyLib::Vector3& pos);	// 生成処理

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CSelectDraw::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// 関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void UpdateState();		// 状態更新
	void StateNone();		// なし
	void StateFadeIn();		// フェードイン
	void StateFadeout();	// フェードアウト
	void StateDrawing();	// 書く

	// その他関数

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	State m_state;			// 状態
	float m_fStateTime;		// 状態カウンター

};


#endif