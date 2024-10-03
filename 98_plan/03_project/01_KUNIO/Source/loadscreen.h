//=============================================================================
// 
//  フェードヘッダー [loadscreen.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _LOADSCREEN_H_
#define _LOADSCREEN_H_	// 二重インクルード防止

#include "main.h"
#include "object2D_Anim.h"
#include "scene.h"

//==========================================================================
// クラス定義
//==========================================================================
// フェードクラス定義
class CLoadScreen
{
public:

	CLoadScreen();
	~CLoadScreen();

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Kill();

	static CLoadScreen *Create();

private:

	//=============================
	// メンバ変数
	//=============================
	void CreateCylinder();	// 筒生成
	void MoveCylinder();	// 筒の動き
	void CollisionText();	// 文字との判定

	//=============================
	// メンバ変数
	//=============================
	CObject2D* m_apObj2D[10];	// 文字列
	CObject2D* m_pCylinder;		// 筒
	float m_fBobbingTime;		// ぷかぷかタイマー
};

#endif