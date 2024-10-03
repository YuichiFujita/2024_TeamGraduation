//=============================================================================
// 
// キャッチ結果処理 [catchresult_FAIL.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _CATCHRESULT_FAIL_H_
#define _CATCHRESULT_FAIL_H_		// 二重インクルード防止のマクロを定義する

#include "catchresult.h"

//==========================================================================
// クラス定義
//==========================================================================
// キャッチ結果
class CCatchResult_FAIL : public CCatchResult
{
public:

	CCatchResult_FAIL(int nPriority = 8);
	~CCatchResult_FAIL();

	//  オーバーライドされた関数
	HRESULT Init() override;

private:

	//=============================
	// メンバ関数
	//=============================
	void StateExpansion() override;	// 拡大
	void StateExpNone() override;	// 拡大後何もしない
	void StateFadeOut() override;	// フェードアウト状態

};

#endif