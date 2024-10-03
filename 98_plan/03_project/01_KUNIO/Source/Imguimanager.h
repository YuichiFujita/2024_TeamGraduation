//=============================================================================
// 
// Imguiのマネージャ [Imguimanager.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _IMGUIMANAGER_H_
#define _IMGUIMANAGER_H_		// 二重インクルード防止

/**
	@brief	Imguiのマネージャ
*/
namespace ImguiMgr
{
	void Init(HWND hwnd, LPDIRECT3DDEVICE9 pd3dDevice);	// 初期化処理
	void Update();	// 更新処理
	void Draw();	// 描画処理
	void Uninit();	// 終了処理

	void Backup();
	void Restore();
}



#endif