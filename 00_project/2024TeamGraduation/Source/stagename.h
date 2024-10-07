//=============================================================================
// 
// ステージ名ヘッダー [stagename.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _STAGENAME_H_
#define _STAGENAME_H_		// 二重インクルード防止のマクロを定義する

#include "object2D.h"

//==========================================================================
// クラス定義
//==========================================================================
// ステージ名クラス
class CStageName : public CObject2D
{
public:

	CStageName(int nPriority = 10);
	~CStageName();
	
	//  オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CStageName* Create();

private:

	//=============================
	// メンバ関数
	//=============================

	//=============================
	// メンバ変数
	//=============================
	float m_fAlphaTime;	// 不透明度タイマー
	static CStageName* m_pThisPtr;	// 自身のポインタ
};

#endif