//=============================================================================
// 
// ステージクリアテキスト処理 [stagecleartext.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _STAGECLEARTEXT_H_
#define _STAGECLEARTEXT_H_		// 二重インクルード防止のマクロを定義する

#include "main.h"
#include "object2D.h"

//==========================================================================
// クラス定義
//==========================================================================
// ステージクリアテキストクラス定義
class CStageClearText : public CObject2D
{
public:

	CStageClearText(int nPriority = 8);
	~CStageClearText();

	static CStageClearText *Create(const MyLib::Vector3 pos);
	
	//  オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVtx();

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
	typedef void(CStageClearText::* STATE_FUNC)();	// 状態処理のリスト
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
	bool m_bCreateResultWindow;	// リザルト画面の呼び出しフラグ

};

#endif