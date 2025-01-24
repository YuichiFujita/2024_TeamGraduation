//==========================================================================
// 
//  ロード画面ヘッダー [loadscreen.h]
//  Author : 相馬靜雅
//  Adder  : 藤田勇一
// 
//==========================================================================

#ifndef _LOADSCREEN_H_
#define _LOADSCREEN_H_	// 二重インクルード防止

//==========================================================================
// 前方宣言
//==========================================================================
#include "object2D_Anim.h"

//==========================================================================
// クラス定義
//==========================================================================
// ロード画面クラス定義
class CLoadScreen
{
public:

	//=============================
	// コンストラクタ/デストラクタ
	//=============================
	CLoadScreen();
	~CLoadScreen();

	//=============================
	// メンバ関数
	//=============================
	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Draw();
	void Kill();
	inline DWORD GetNumLoop() const	{ return m_pLoad->GetNumLoop(); }
	inline void ResetNumLoop()		{ m_pLoad->ResetNumLoop(); }
	inline void SetEnableAutoPlay(const bool bPlay) { m_pLoad->SetEnableAutoPlay(bPlay); }

	//=============================
	// 静的メンバ関数
	//=============================
	static CLoadScreen *Create();

private:

	//=============================
	// メンバ関数
	//=============================


	//=============================
	// メンバ変数
	//=============================
	CObject2D_Anim* m_pLoad;	// ロード画面情報

};

#endif