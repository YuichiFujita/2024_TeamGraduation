//=============================================================================
// 
//  フェードヘッダー [fade.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _FADE_H_
#define _FADE_H_	// 二重インクルード防止

#include "main.h"
#include "object2D.h"
#include "scene.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// フェードクラス定義
class CFade
{
public:

	// 列挙型定義
	enum STATE
	{
		STATE_NONE = 0,	// 何もしていない
		STATE_FADEOUT,	// フェードアウト
		STATE_FADEIN,	// フェードイン
		STATE_FADECOMPLETION,	// フェード完了
		STATE_MAX
	};

	CFade();
	~CFade();

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Draw();

	CObject2D *GetMyObject();
	static CFade *Create();
	bool SetFade(CScene::MODE mode);	// 次のモード設定
	STATE GetState();		// 状態取得
	bool IsFade() const;	// フェード中かの確認

private:

	CObject2D *m_aObject2D;		// オブジェクト2Dのオブジェクト
	CScene::MODE m_ModeNext;	// 次のモード
	STATE m_state;				// 状態
};

#endif