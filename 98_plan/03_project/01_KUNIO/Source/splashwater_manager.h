//=============================================================================
// 
//  水しぶきマネージャヘッダー [splashwater_manager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SPLASHWATER_MANAGER_H_
#define _SPLASHWATER_MANAGER_H_	// 二重インクルード防止

#include "object2d.h"

//==========================================================================
// クラス定義
//==========================================================================
// 水しぶきクラス
class CSplashwater_Manager : public CObject
{
public:

	CSplashwater_Manager(int nPriority = 7);
	~CSplashwater_Manager();

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
	static CSplashwater_Manager* Create();

};


#endif