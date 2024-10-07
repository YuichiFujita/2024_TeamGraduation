//=============================================================================
// 
//  ガラス割れるヘッダー [glassclush.h]
//  Author : 日野澤匠泉様
// 
//=============================================================================

#ifndef _GLASSCLUSH_H_
#define _GLASSCLUSH_H_	// 二重インクルード防止

#include "object2d.h"

//==========================================================================
// クラス定義
//==========================================================================
// サンプル_オブジェクト2Dクラス
class CGlassclush : public CObject2D
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_NONE = 0,		// なにもない
		STATE_MAX
	};

	CGlassclush(int nPriority = 3);
	~CGlassclush();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//=============================
	// 静的関数
	//=============================
	static CGlassclush* Create();
	static CGlassclush* GetInstance() { return m_ThisPtr; }	// インスタンス取得
	static void Kill();

private:

	//=============================
	// メンバ関数
	//=============================
	// 状態系

	// その他関数

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	float m_fStateTime;		// 状態カウンター
	State m_state;			// 状態

	// その他変数
	static CGlassclush* m_ThisPtr;		// 自身のポインタ
};


#endif