//=============================================================================
// 
// キャッチ結果処理 [catchresult.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _CATCHRESULT_H_
#define _CATCHRESULT_H_		// 二重インクルード防止のマクロを定義する

#include "object2D.h"

//==========================================================================
// クラス定義
//==========================================================================
// キャッチ結果
class CCatchResult : public CObject2D
{
public:

	enum TYPE
	{
		TYPE_OK = 0,
		TYPE_FAIL,
		TYPE_MAX
	};

	CCatchResult(int nPriority = 8);
	~CCatchResult();

	//  オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	static CCatchResult* Create(const MyLib::Vector3& pos, TYPE resultType);

protected:

	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_EXPANSION = 0,
		STATE_EXPNONE,
		STATE_FADEOUT,
		STATE_MAX
	};

	//=============================
	// メンバ関数
	//=============================
	virtual void StateExpansion() = 0;	// 拡大
	virtual void StateExpNone() = 0;	// 拡大後何もしない
	virtual void StateFadeOut() = 0;	// フェードアウト状態

	//=============================
	// メンバ変数
	//=============================
	State m_state;			// 状態
	float m_fStateTimer;	// 状態タイマー

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CCatchResult::* STATE_FUNC)();	// 状態処理のリスト
	static STATE_FUNC m_StateFuncList[];
};

#endif