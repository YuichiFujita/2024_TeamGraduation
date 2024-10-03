//=============================================================================
// 
//  水しぶき背景ヘッダー [splashwater_bg.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SPLASHWATER_BG_H_
#define _SPLASHWATER_BG_H_	// 二重インクルード防止

#include "object2d.h"

//==========================================================================
// クラス定義
//==========================================================================
// 水しぶきクラス
class CSplashwater_BG : public CObject2D
{
public:
	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_NONE = 0,		// なにもない
		STATE_APPEARANCE,	// 出現
		STATE_MAX
	};

	CSplashwater_BG(int nPriority = 7);
	~CSplashwater_BG();

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
	static CSplashwater_BG* Create();

private:
	//=============================
	// メンバ関数
	//=============================
	// 状態系
	void StateAppearance();	// 出現状態

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	float m_fLife;				// 寿命
	float m_fOriginLife;		// 寿命
	State m_state;				// 状態
	float m_fStateTime;			// 状態タイマー
};


#endif