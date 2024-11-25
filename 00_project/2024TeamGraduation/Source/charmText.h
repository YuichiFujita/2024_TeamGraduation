//==========================================================================
// 
//  モテ文字ヘッダー [charmText.h]
//  Author : 相馬靜雅
// 
//==========================================================================

#ifndef _CHARMTEXT_H_
#define _CHARMTEXT_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// モテ文字クラス
class CCharmText : public CObject
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum EState
	{
		STATE_FADEIN = 0,	// フェードイン
		STATE_WAIT,			// 待機(モテチェイン可能)
		STATE_FADEOUT,		// フェードアウト
		STATE_MAX
	};

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CCharmText(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_2D);
	~CCharmText();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;						// 削除
	virtual void SetEnableDisp(bool bDisp) override;	// 描画状況設定

	//=============================
	// メンバ関数
	//=============================
	void SetState(EState state);	// 状態設定

	//=============================
	// 静的メンバ関数
	//=============================
	static CCharmText* Create();	// 生成

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CCharmText::*STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// 状態関数

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// 状態更新
	void StateFadeIn();		// フェードイン
	void StateWait();		// 待機
	void StateFadeOut();	// フェードアウト
	
	// 生成
	HRESULT CreateFace();	// 顔アイコン生成
	HRESULT CreateText();	// 文字生成

	//=============================
	// メンバ変数
	//=============================
	CObject2D* m_pFace;		// 顔アイコン
	CObject2D* m_pText;		// 文字
	EState m_state;			// 状態
	float m_fStateTime;		// 状態タイマー
};

#endif
